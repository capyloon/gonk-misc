/* (c) 2020 KAI OS TECHNOLOGIES (HONG KONG) LIMITED All rights reserved. This
 * file or any portion thereof may not be reproduced or used in any manner
 * whatsoever without the express written permission of KAI OS TECHNOLOGIES
 * (HONG KONG) LIMITED. KaiOS is the trademark of KAI OS TECHNOLOGIES (HONG
 * KONG) LIMITED or its affiliate company and may be registered in some
 * jurisdictions. All other trademarks are the property of their respective
 * owners.
 */

#include <IBatteryStats.h>
#include <IProcessInfoService.h>
#include <android/content/pm/BnPackageManagerNative.h>
#include <android/content/pm/IPackageManagerNative.h>
#include <android/gui/BnSurfaceComposer.h>
#include <android/gui/ISurfaceComposer.h>
#include <android/hardware/ISensorPrivacyListener.h>
#include <android/hardware/ISensorPrivacyManager.h>
#include <android/log.h>
#include <binder/BinderService.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>

#define ALOG_TAG "GonkServices"

#define LOG(args...) __android_log_print(ANDROID_LOG_INFO, ALOG_TAG, args)

namespace android {

using namespace android::content;
using namespace android::gui;

using binder::Status;

class BnSensorPrivacyService
    : public BnInterface<hardware::ISensorPrivacyManager> {
public:
  virtual status_t onTransact(uint32_t code, const Parcel &data, Parcel *reply,
                              uint32_t flags = 0) = 0;
};

class FakeSensorPrivacyService : public BinderService<FakeSensorPrivacyService>,
                                 public BnSensorPrivacyService {
public:
  FakeSensorPrivacyService();
  virtual ~FakeSensorPrivacyService();
  static const char *getServiceName() { return "sensor_privacy"; }

  virtual Status supportsSensorToggle(int32_t toggleType, int32_t sensor,
                                      bool *_aidl_return);
  virtual Status addSensorPrivacyListener(
      const sp<hardware::ISensorPrivacyListener> &listener);
  virtual Status addToggleSensorPrivacyListener(
      const sp<hardware::ISensorPrivacyListener> &listener);
  virtual Status removeSensorPrivacyListener(
      const sp<hardware::ISensorPrivacyListener> &listener);
  virtual Status removeToggleSensorPrivacyListener(
      const sp<hardware::ISensorPrivacyListener> &listener);
  virtual Status isSensorPrivacyEnabled(bool *_aidl_return);
  virtual Status isCombinedToggleSensorPrivacyEnabled(int32_t sensor,
                                                      bool *_aidl_return);
  virtual Status isToggleSensorPrivacyEnabled(int32_t toggleType,
                                              int32_t sensor,
                                              bool *_aidl_return);
  virtual Status setSensorPrivacy(bool enable);
  virtual Status setToggleSensorPrivacy(int32_t userId, int32_t source,
                                        int32_t sensor, bool enable);
  virtual Status setToggleSensorPrivacyForProfileGroup(int32_t userId,
                                                       int32_t source,
                                                       int32_t sensor,
                                                       bool enable);

  virtual status_t onTransact(uint32_t code, const Parcel &data, Parcel *reply,
                              uint32_t flags) override;
};

FakeSensorPrivacyService::FakeSensorPrivacyService()
    : BnSensorPrivacyService() {}

FakeSensorPrivacyService::~FakeSensorPrivacyService() {}

Status FakeSensorPrivacyService::supportsSensorToggle(int32_t toggleType,
                                                      int32_t sensor,
                                                      bool *_aidl_return) {
  (void)toggleType;
  (void)sensor;
  *_aidl_return = false;

  return Status::ok();
}

Status FakeSensorPrivacyService::addToggleSensorPrivacyListener(
    const sp<hardware::ISensorPrivacyListener> &listener) {
  (void)listener;

  return Status::ok();
}

Status FakeSensorPrivacyService::addSensorPrivacyListener(
    const sp<hardware::ISensorPrivacyListener> &listener) {
  (void)listener;

  return Status::ok();
}

Status FakeSensorPrivacyService::removeToggleSensorPrivacyListener(
    const sp<hardware::ISensorPrivacyListener> &listener) {
  (void)listener;

  return Status::ok();
}

Status FakeSensorPrivacyService::removeSensorPrivacyListener(
    const sp<hardware::ISensorPrivacyListener> &listener) {
  (void)listener;

  return Status::ok();
}

Status FakeSensorPrivacyService::isSensorPrivacyEnabled(bool *_aidl_return) {
  /* Do not enable sensor privacy for b2g*/
  *_aidl_return = false;

  return Status::ok();
}

Status FakeSensorPrivacyService::setSensorPrivacy(bool enable) {
  (void)enable;

  return Status::ok();
}

Status FakeSensorPrivacyService::isCombinedToggleSensorPrivacyEnabled(
    int32_t sensor, bool *_aidl_return) {
  (void)sensor;

  *_aidl_return = false;

  return Status::ok();
}

Status FakeSensorPrivacyService::isToggleSensorPrivacyEnabled(
    int32_t toggleType, int32_t sensor, bool *_aidl_return) {
  (void)toggleType;
  (void)sensor;

  *_aidl_return = false;

  return Status::ok();
}

Status FakeSensorPrivacyService::setToggleSensorPrivacy(int32_t userId,
                                                        int32_t source,
                                                        int32_t sensor,
                                                        bool enable) {
  (void)userId;
  (void)source;
  (void)sensor;
  (void)enable;

  return Status::ok();
}

Status FakeSensorPrivacyService::setToggleSensorPrivacyForProfileGroup(
    int32_t userId, int32_t source, int32_t sensor, bool enable) {
  (void)userId;
  (void)source;
  (void)sensor;
  (void)enable;

  return Status::ok();
}

status_t FakeSensorPrivacyService::onTransact(uint32_t code, const Parcel &data,
                                              Parcel *reply, uint32_t flags) {
  switch (code) {
  case IBinder::FIRST_CALL_TRANSACTION + 0: /* supportsSensorToggle */ {
    CHECK_INTERFACE(hardware::ISensorPrivacyManager, data, reply);
    bool enabled;
    int32_t toggleType;
    int32_t sensor;
    data.readInt32(&toggleType);
    data.readInt32(&sensor);
    Status status(supportsSensorToggle(toggleType, sensor, &enabled));
    status_t result = status.writeToParcel(reply);
    reply->writeBool(enabled);
    return result;
  }
  case IBinder::FIRST_CALL_TRANSACTION + 1: /* addSensorPrivacyListener */ {
    CHECK_INTERFACE(hardware::ISensorPrivacyManager, data, reply);
    sp<hardware::ISensorPrivacyListener> listener;
    data.readStrongBinder(&listener);
    Status status(addSensorPrivacyListener(listener));
    return status.writeToParcel(reply);
  }
  case IBinder::FIRST_CALL_TRANSACTION +
      2: /* addToggleSensorPrivacyListener */ {
    CHECK_INTERFACE(hardware::ISensorPrivacyManager, data, reply);
    sp<hardware::ISensorPrivacyListener> listener;
    data.readStrongBinder(&listener);
    Status status(addToggleSensorPrivacyListener(listener));
    return status.writeToParcel(reply);
  }
  case IBinder::FIRST_CALL_TRANSACTION + 3: /* removeSensorPrivacyListener */ {
    CHECK_INTERFACE(hardware::ISensorPrivacyManager, data, reply);
    sp<hardware::ISensorPrivacyListener> listener;
    data.readStrongBinder(&listener);
    Status status(removeSensorPrivacyListener(listener));
    return status.writeToParcel(reply);
  }
  case IBinder::FIRST_CALL_TRANSACTION +
      4: /* removeToggleSensorPrivacyListener */ {
    CHECK_INTERFACE(hardware::ISensorPrivacyManager, data, reply);
    sp<hardware::ISensorPrivacyListener> listener;
    data.readStrongBinder(&listener);
    Status status(removeToggleSensorPrivacyListener(listener));
    return status.writeToParcel(reply);
  }
  case IBinder::FIRST_CALL_TRANSACTION + 5: /* isSensorPrivacyEnabled */ {
    CHECK_INTERFACE(hardware::ISensorPrivacyManager, data, reply);
    bool enabled;
    Status status(isSensorPrivacyEnabled(&enabled));
    status_t result = status.writeToParcel(reply);
    reply->writeBool(enabled);
    return result;
  }
  case IBinder::FIRST_CALL_TRANSACTION +
      6: /* isCombinedToggleSensorPrivacyEnabled */ {
    CHECK_INTERFACE(hardware::ISensorPrivacyManager, data, reply);
    bool enabled;
    int32_t sensor;
    data.readInt32(&sensor);
    Status status(isCombinedToggleSensorPrivacyEnabled(sensor, &enabled));
    status_t result = status.writeToParcel(reply);
    reply->writeBool(enabled);
    return result;
  }
  case IBinder::FIRST_CALL_TRANSACTION + 7: /* isToggleSensorPrivacyEnabled */ {
    CHECK_INTERFACE(hardware::ISensorPrivacyManager, data, reply);
    bool enabled;
    int32_t toggleType;
    int32_t sensor;
    data.readInt32(&toggleType);
    data.readInt32(&sensor);
    Status status(isToggleSensorPrivacyEnabled(toggleType, sensor, &enabled));
    status_t result = status.writeToParcel(reply);
    reply->writeBool(enabled);
    return result;
  }
  case IBinder::FIRST_CALL_TRANSACTION + 8: /* setSensorPrivacy */ {
    CHECK_INTERFACE(hardware::ISensorPrivacyManager, data, reply);
    bool enabled;
    data.readBool(&enabled);
    Status status(setSensorPrivacy(enabled));
    return status.writeToParcel(reply);
  }
  case IBinder::FIRST_CALL_TRANSACTION + 9: /* setToggleSensorPrivacy */ {
    CHECK_INTERFACE(hardware::ISensorPrivacyManager, data, reply);
    int32_t userId;
    int32_t source;
    int32_t sensor;
    bool enabled;
    data.readInt32(&userId);
    data.readInt32(&source);
    data.readInt32(&sensor);
    data.readBool(&enabled);
    Status status(setToggleSensorPrivacy(userId, source, sensor, enabled));
    return status.writeToParcel(reply);
  }
  case IBinder::FIRST_CALL_TRANSACTION +
      10: /* setToggleSensorPrivacyForProfileGroup */ {
    CHECK_INTERFACE(hardware::ISensorPrivacyManager, data, reply);
    int32_t userId;
    int32_t source;
    int32_t sensor;
    bool enabled;
    data.readInt32(&userId);
    data.readInt32(&source);
    data.readInt32(&sensor);
    data.readBool(&enabled);
    Status status(
        setToggleSensorPrivacyForProfileGroup(userId, source, sensor, enabled));
    return status.writeToParcel(reply);
  }

  default:
    return BBinder::onTransact(code, data, reply, flags);
  }
}

class BnProcessInfoService : public BnInterface<IProcessInfoService> {
public:
  virtual status_t onTransact(uint32_t code, const Parcel &data, Parcel *reply,
                              uint32_t flags = 0) = 0;
};

class GonkProcessInfoService : public BinderService<GonkProcessInfoService>,
                               public BnProcessInfoService {
public:
  GonkProcessInfoService();
  virtual ~GonkProcessInfoService();
  static const char *getServiceName() { return "processinfo"; }

  virtual status_t getProcessStatesFromPids(size_t length,
                                            /*in*/ int32_t *pids,
                                            /*out*/ int32_t *states) override;

  virtual status_t
  getProcessStatesAndOomScoresFromPids(size_t length,
                                       /*in*/ int32_t *pids,
                                       /*out*/ int32_t *states,
                                       /*out*/ int32_t *scores) override;

  virtual status_t onTransact(uint32_t code, const Parcel &data, Parcel *reply,
                              uint32_t flags) override;
};

GonkProcessInfoService::GonkProcessInfoService() : BnProcessInfoService() {}

GonkProcessInfoService::~GonkProcessInfoService() {}

status_t
GonkProcessInfoService::getProcessStatesFromPids(size_t length,
                                                 /*in*/ int32_t *pids,
                                                 /*out*/ int32_t *states) {
  return getProcessStatesAndOomScoresFromPids(length, pids, states, nullptr);
}

#define PROCESS_STATE_UNKNOWN -1;
#define PROCESS_STATE_TOP_SLEEPING 13;
#define PROCESS_STATE_NONEXISTENT 19;
status_t GonkProcessInfoService::getProcessStatesAndOomScoresFromPids(
    size_t length,
    /*in*/ int32_t *pids,
    /*out*/ int32_t *states,
    /*out*/ int32_t *scores) {
  // sanity check for parameters
  if (length <= 0 || !pids || !states) {
    return -ENOSYS;
  }

  for (int i = 0; i < length; i++) {
    FILE *fp;
    char cmd[80], result[80];
    { /* handle process states */
      sprintf(cmd, "cat proc/%d/stat", pids[i]);
      /* Open the command for reading. */
      fp = popen(cmd, "r");
      if (fp == NULL) {
        return -ENOSYS;
      }

      /* Read the output a argument each time - output it. */
      fscanf(fp, "%s", result);      /* process id, skip */
      fscanf(fp, "%s", result);      /* process name, skip */
      while (!strchr(result, ')')) { /* keep finding ')' for the end of name */
        fscanf(fp, "%s", result);    /* process name, skip */
      }
      fscanf(fp, "%s", result); /* process state */
      states[i] = PROCESS_STATE_NONEXISTENT;
      if (result[0] != '\0') {
        switch (result[0]) {
        case 'S':
          states[i] = PROCESS_STATE_TOP_SLEEPING;
          break;
        default:
          states[i] = PROCESS_STATE_UNKNOWN;
        }
      }
      pclose(fp);
    }

    if (scores) { /* handle process oom_score */
      sprintf(cmd, "cat proc/%d/oom_adj", pids[i]);
      /* Open the command for reading. */
      fp = popen(cmd, "r");
      if (fp == NULL) {
        return -ENOSYS;
      }

      /* Read the output a argument each time - output it. */
      fscanf(fp, "%s", result); /* process oom_score */
      if (result[0] != '\0') {
        scores[i] = atoi(result);
      }
      pclose(fp);
    }
  }

  return NO_ERROR;
}

status_t GonkProcessInfoService::onTransact(uint32_t code, const Parcel &data,
                                            Parcel *reply, uint32_t flags) {
  switch (code) {
  case GET_PROCESS_STATES_FROM_PIDS: {
    CHECK_INTERFACE(IProcessInfoService, data, reply);
    int32_t length = data.readInt32();
    if (length <= 0) {
      return NO_ERROR;
    }
    int32_t pids[length];
    status_t result = data.read((void *)pids, length * sizeof(int32_t));
    if (result != NO_ERROR) {
      return result;
    }
    int32_t statesLength = data.readInt32();
    if (length != statesLength) {
      return -ENOSYS;
    }
    int32_t states[length];
    result = getProcessStatesFromPids(length, pids, states);
    if (result != NO_ERROR) {
      return result;
    }
    reply->writeNoException();
    reply->writeInt32Array(statesLength, states);
    reply->writeInt32(result);
    return NO_ERROR;
    break;
  }
  case GET_PROCESS_STATES_AND_OOM_SCORES_FROM_PIDS: {
    CHECK_INTERFACE(IProcessInfoService, data, reply);
    int32_t length = data.readInt32();
    if (length <= 0) {
      return NO_ERROR;
    }
    int32_t pids[length];
    status_t result = data.read((void *)pids, length * sizeof(int32_t));
    if (result != NO_ERROR) {
      return result;
    }
    int32_t statesLength = data.readInt32();
    int32_t scoresLength = data.readInt32();
    if (length != statesLength || statesLength != scoresLength) {
      return -ENOSYS;
    }
    int32_t states[length], scores[length];
    result = getProcessStatesAndOomScoresFromPids(length, pids, states, scores);
    if (result != NO_ERROR) {
      return result;
    }
    reply->writeNoException();
    reply->writeInt32Array(statesLength, states);
    reply->writeInt32Array(scoresLength, scores);
    reply->writeInt32(result);
    return NO_ERROR;
    break;
  }
  default:
    return BBinder::onTransact(code, data, reply, flags);
  }
}

// Gonk implementation of
// frameworks/native/libs/binder/aidl/android/content/pm/IPackageManagerNative.aidl

class GonkPackageManagerNative
    : public pm::BnPackageManagerNative,
      public BinderService<GonkPackageManagerNative> {
public:
  GonkPackageManagerNative() : pm::BnPackageManagerNative() {}
  virtual ~GonkPackageManagerNative() {}

  static const char *getServiceName() { return "package_native"; }

  // IPackageManagerNative implementation.

  virtual Status getNamesForUids(const std::vector<int32_t> &uids,
                                 std::vector<::std::string> *_aidl_return) {
    LOG("%s", __FUNCTION__);
    *_aidl_return = {};
    for (auto i = 0; i < uids.size(); i++) {
      LOG("uid=%d", uids[i]);
      (*_aidl_return).push_back(""); // All unknown.
    }
    return Status::ok();
  }

  virtual Status getInstallerForPackage(const String16 &packageName,
                                        std::string *_aidl_return) {
    LOG("%s", __FUNCTION__);
    (void)packageName;
    *_aidl_return = "preload";
    return Status::ok();
  }

  virtual Status getVersionCodeForPackage(const String16 &packageName,
                                          int64_t *_aidl_return) {
    LOG("%s", __FUNCTION__);
    (void)packageName;
    *_aidl_return = 0; // Unknown code.
    return Status::ok();
  }

  virtual Status
  isAudioPlaybackCaptureAllowed(const std::vector<::std::string> &packageNames,
                                std::vector<bool> *_aidl_return) {
    LOG("%s", __FUNCTION__);
    *_aidl_return = {};
    for (auto i = 0; i < packageNames.size(); i++) {
      (*_aidl_return).push_back(true);
    }
    return Status::ok();
  }

  virtual Status getLocationFlags(const std::string &packageName,
                                  int32_t *_aidl_return) {
    LOG("%s", __FUNCTION__);
    (void)packageName;
    *_aidl_return = pm::BnPackageManagerNative::LOCATION_SYSTEM;
    return Status::ok();
  }

  virtual Status getTargetSdkVersionForPackage(const String16 &packageName,
                                               int32_t *_aidl_return) {
    LOG("%s", __FUNCTION__);
    (void)packageName;
    // Use the current SDK version instead??
    *_aidl_return = 0;
    return Status::ok();
  }

  virtual Status getModuleMetadataPackageName(::std::string *_aidl_return) {
    LOG("%s", __FUNCTION__);
    *_aidl_return = "b2g";
    return Status::ok();
  }

  virtual Status getAllPackages(std::vector<std::string> *_aidl_return) {
    LOG("%s", __FUNCTION__);
    *_aidl_return = {};
    return Status::ok();
  }

  virtual Status registerPackageChangeObserver(
      const ::android::sp<::android::content::pm::IPackageChangeObserver>
          &observer) {
    LOG("%s", __FUNCTION__);
    (void)observer;
    return Status::ok();
  }

  virtual Status unregisterPackageChangeObserver(
      const sp<pm::IPackageChangeObserver> &observer) {
    LOG("%s", __FUNCTION__);
    (void)observer;
    return Status::ok();
  }

  virtual Status
  hasSha256SigningCertificate(const std::string &packageName,
                              const std::vector<uint8_t> &certificate,
                              bool *_aidl_return) {
    LOG("%s", __FUNCTION__);
    (void)packageName;
    (void)certificate;
    *_aidl_return = false;
    return Status::ok();
  }

  virtual Status isPackageDebuggable(const String16 &packageName,
                                     bool *_aidl_return) {
    LOG("%s", __FUNCTION__);
    (void)packageName;
    *_aidl_return = false;
    return Status::ok();
  }

  virtual Status hasSystemFeature(const String16 &featureName, int32_t version,
                                  bool *_aidl_return) {
    LOG("%s %s version=%d", __FUNCTION__, String8(featureName).c_str(),
        version);
    *_aidl_return = false;
    return Status::ok();
  }

  virtual Status
  registerStagedApexObserver(const sp<pm::IStagedApexObserver> &observer) {
    LOG("%s", __FUNCTION__);
    (void)observer;
    return Status::ok();
  }

  virtual Status
  unregisterStagedApexObserver(const sp<pm::IStagedApexObserver> &observer) {
    LOG("%s", __FUNCTION__);
    (void)observer;
    return Status::ok();
  }

  virtual Status
  getStagedApexModuleNames(std::vector<std::string> *_aidl_return) {
    LOG("%s", __FUNCTION__);
    *_aidl_return = {};
    return Status::ok();
  }

  virtual Status
  getStagedApexInfo(const std::string &moduleName,
                    std::optional<pm::StagedApexInfo> *_aidl_return) {
    LOG("%s", __FUNCTION__);
    (void)moduleName;
    *_aidl_return = std::nullopt;
    return Status::ok();
  }
};

// Fake SurfaceFlinger implementation.
// Relevant AOSP sources:
// frameworks/native/libs/gui/aidl/android/gui/ISurfaceComposer.aidl
// frameworks/native/services/surfaceflinger

class FakeSurfaceFlingerImpl : public gui::BnSurfaceComposer {
public:
  FakeSurfaceFlingerImpl() : gui::BnSurfaceComposer() {}

  virtual ~FakeSurfaceFlingerImpl() {}

  Status createDisplay(const std::string &displayName, bool secure,
                       sp<IBinder> *outDisplay) {
    LOG("%s", __FUNCTION__);
    (void)displayName;
    (void)secure;
    (void)outDisplay;
    return Status::ok();
  }

  Status destroyDisplay(const sp<IBinder> &display) {
    LOG("%s", __FUNCTION__);
    (void)display;
    return Status::ok();
  }

  Status getPhysicalDisplayIds(std::vector<int64_t> *outDisplayIds) {
    LOG("%s", __FUNCTION__);
    std::vector<int64_t> displayIds;
    displayIds.reserve(1);
    displayIds.push_back(0);
    *outDisplayIds = displayIds;

    return Status::ok();
  }

  Status getPrimaryPhysicalDisplayId(int64_t *outDisplayId) {
    LOG("%s", __FUNCTION__);
    (void)outDisplayId;
    return Status::ok();
  }

  Status getPhysicalDisplayToken(int64_t displayId, sp<IBinder> *outDisplay) {
    LOG("%s %ld", __FUNCTION__, displayId);
    const sp<IBinder> nullToken;
    *outDisplay = nullToken;
    return Status::ok();
  }

  Status setPowerMode(const sp<IBinder> &display, int mode) {
    LOG("%s", __FUNCTION__);
    (void)display;
    (void)mode;
    return Status::ok();
  }

  Status getDisplayStats(const sp<IBinder> &display,
                         gui::DisplayStatInfo *outStatInfo) {
    LOG("%s", __FUNCTION__);
    (void)display;
    (void)outStatInfo;
    return Status::ok();
  }

  Status getDisplayState(const sp<IBinder> &display,
                         gui::DisplayState *outState) {
    LOG("%s", __FUNCTION__);
    (void)display;
    (void)outState;
    return Status::ok();
  }

  Status clearBootDisplayMode(const sp<IBinder> &display) {
    LOG("%s", __FUNCTION__);
    (void)display;
    return Status::ok();
  }

  Status getBootDisplayModeSupport(bool *outMode) {
    LOG("%s", __FUNCTION__);
    (void)outMode;
    return Status::ok();
  }

  Status setAutoLowLatencyMode(const sp<IBinder> &display, bool on) {
    LOG("%s", __FUNCTION__);
    (void)display;
    (void)on;
    return Status::ok();
  }

  Status setGameContentType(const sp<IBinder> &display, bool on) {
    LOG("%s", __FUNCTION__);
    (void)display;
    (void)on;
    return Status::ok();
  }

  Status captureDisplay(const DisplayCaptureArgs &,
                        const sp<IScreenCaptureListener> &) {
    LOG("%s", __FUNCTION__);
    return Status::ok();
  }

  Status captureDisplayById(int64_t, const sp<IScreenCaptureListener> &) {
    LOG("%s", __FUNCTION__);
    return Status::ok();
  }

  Status captureLayers(const LayerCaptureArgs &,
                       const sp<IScreenCaptureListener> &) {
    LOG("%s", __FUNCTION__);
    return Status::ok();
  }

  Status isWideColorDisplay(const sp<IBinder> &token,
                            bool *outIsWideColorDisplay) {
    LOG("%s", __FUNCTION__);
    (void)token;
    (void)outIsWideColorDisplay;
    return Status::ok();
  }

  Status getDisplayBrightnessSupport(const sp<IBinder> &displayToken,
                                     bool *outSupport) {
    LOG("%s", __FUNCTION__);
    (void)displayToken;
    (void)outSupport;
    return Status::ok();
  }

  Status setDisplayBrightness(const sp<IBinder> &displayToken,
                              const gui::DisplayBrightness &brightness) {
    LOG("%s", __FUNCTION__);
    (void)displayToken;
    (void)brightness;
    return Status::ok();
  }

  Status
  addHdrLayerInfoListener(const sp<IBinder> &displayToken,
                          const sp<gui::IHdrLayerInfoListener> &listener) {
    LOG("%s", __FUNCTION__);
    (void)displayToken;
    (void)listener;
    return Status::ok();
  }

  Status
  removeHdrLayerInfoListener(const sp<IBinder> &displayToken,
                             const sp<gui::IHdrLayerInfoListener> &listener) {
    LOG("%s", __FUNCTION__);
    (void)displayToken;
    (void)listener;
    return Status::ok();
  }

  Status notifyPowerBoost(int boostId) {
    LOG("%s", __FUNCTION__);
    (void)boostId;
    return Status::ok();
  }
};

class FakeSurfaceFlinger1 : public FakeSurfaceFlingerImpl,
                            public BinderService<FakeSurfaceFlinger1> {
public:
  FakeSurfaceFlinger1() : FakeSurfaceFlingerImpl() {}

  virtual ~FakeSurfaceFlinger1() {}

  static const char *getServiceName() { return "SurfaceFlinger"; }
};

class FakeSurfaceFlinger2 : public FakeSurfaceFlingerImpl,
                            public BinderService<FakeSurfaceFlinger2> {
public:
  FakeSurfaceFlinger2() : FakeSurfaceFlingerImpl() {}

  virtual ~FakeSurfaceFlinger2() {}

  static const char *getServiceName() { return "SurfaceFlingerAIDL"; }
};

// Gonk implementation of the batterystats service. See
// frameworks/base/core/java/com/android/internal/app/IBatteryStats.aidl
// frameworks/native/libs/binder/include_batterystats/batterystats/IBatteryStats.h

class GonkBatteryStats : public BnBatteryStats,
                         public BinderService<GonkBatteryStats> {
public:
  GonkBatteryStats() : BnBatteryStats() {}

  virtual ~GonkBatteryStats() {}

  static const char *getServiceName() { return "batterystats"; }

  void noteStartSensor(int uid, int sensor) {
    LOG("%s uid=%d sensor=%d", __FUNCTION__, uid, sensor);
  }

  void noteStopSensor(int uid, int sensor) {
    LOG("%s uid=%d sensor=%d", __FUNCTION__, uid, sensor);
  }

  void noteStartVideo(int uid) {
    LOG("%s uid=%d", __FUNCTION__, uid);
  }

  void noteStopVideo(int uid) {
    LOG("%s uid=%d", __FUNCTION__, uid);
  }

  void noteStartAudio(int uid) {
    LOG("%s uid=%d", __FUNCTION__, uid);
  }

  void noteStopAudio(int uid) {
    LOG("%s uid=%d", __FUNCTION__, uid);
  }

  void noteResetVideo() {
    LOG("%s", __FUNCTION__);
  }

  void noteResetAudio() {
    LOG("%s", __FUNCTION__);
  }

  void noteFlashlightOn(int uid) {
    LOG("%s uid=%d", __FUNCTION__, uid);
  }

  void noteFlashlightOff(int uid) {
    LOG("%s uid=%d", __FUNCTION__, uid);
  }

  void noteStartCamera(int uid) {
    LOG("%s uid=%d", __FUNCTION__, uid);
  }

  void noteStopCamera(int uid) {
    LOG("%s uid=%d", __FUNCTION__, uid);
  }

  void noteResetCamera() {
    LOG("%s", __FUNCTION__);
  }

  void noteResetFlashlight() {
    LOG("%s", __FUNCTION__);
  }
};

}; // namespace android

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  LOG("Starting...");
  android::FakeSensorPrivacyService::instantiate();
  android::GonkProcessInfoService::instantiate();
  android::GonkPackageManagerNative::instantiate();

  android::FakeSurfaceFlinger1::instantiate();
  android::FakeSurfaceFlinger2::instantiate();

  android::GonkBatteryStats::instantiate();

  android::ProcessState::self()->startThreadPool();
  LOG("startThreadPool ok");

  android::IPCThreadState::self()->joinThreadPool();
  return 0;
}
