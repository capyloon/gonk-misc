/*
 * Copyright (C) 2013 Mozilla Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <binder/BinderService.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <binder/IAppOpsService.h>

namespace android {

class FakeAppOpsService :
	public BinderService<FakeAppOpsService>,
	public BnAppOpsService
{
public:
    FakeAppOpsService();
    virtual ~FakeAppOpsService();
    static const char *getServiceName() { return "appops"; }

    virtual int32_t checkOperation(int32_t code, int32_t uid, const String16& packageName);
#if ANDROID_VERSION >= 33
    virtual int32_t noteOperation(int32_t code, int32_t uid, const String16& packageName,
		                  const std::optional<String16>& attributionTag, bool shouldCollectAsyncNotedOp,
				  const String16& message, bool shouldCollectMessage);
    virtual void finishOperation(const sp<IBinder>& token, int32_t code, int32_t uid,
		                 const String16& packageName, const std::optional<String16>& attributionTag);
    virtual bool shouldCollectNotes(int32_t opCode) { (void)opCode; return false; }
    virtual void setCameraAudioRestriction(int32_t mode) { (void)mode; }
#else
    virtual int32_t noteOperation(int32_t code, int32_t uid, const String16& packageName);
    virtual void finishOperation(int32_t code, int32_t uid, const String16& packageName);
#endif
    virtual int32_t startOperation(int32_t code, int32_t uid, const String16& packageName);
#if ANDROID_VERSION >= 29
#if ANDROID_VERSION >= 33
    virtual int32_t startOperation(const sp<IBinder>& token, int32_t code, int32_t uid,
		                   const String16& packageName, const std::optional<String16>& attributionTag,
				   bool startIfModeDefault, bool shouldCollectAsyncNotedOp, const String16& message,
			           bool shouldCollectMessage);
#else
    virtual int32_t startOperation(const sp<IBinder>& token, int32_t code, int32_t uid,
            const String16& packageName, bool startIfModeDefault);
#endif
    virtual int32_t checkAudioOperation(int32_t code, int32_t usage, int32_t uid,
            const String16& packageName);
#else
    virtual int32_t startOperation(const sp<IBinder>& token, int32_t code, int32_t uid,
            const String16& packageName);
#endif
    virtual sp<IBinder> getToken(const sp<IBinder>& clientToken);
    virtual void startWatchingMode(int32_t op, const String16& packageName,
            const sp<IAppOpsCallback>& callback);
    virtual void stopWatchingMode(const sp<IAppOpsCallback>& callback);
#if ANDROID_VERSION >= 23
    virtual int32_t permissionToOpCode(const String16& permission);
#endif

};

FakeAppOpsService::FakeAppOpsService()
  : BnAppOpsService()
{
}

FakeAppOpsService::~FakeAppOpsService()
{
}

int32_t
FakeAppOpsService::checkOperation(int32_t code, int32_t uid, const String16& packageName)
{
    (void)code;
    (void)uid;
    (void)packageName;

    return MODE_ALLOWED;
}

#if ANDROID_VERSION >= 33
int32_t
FakeAppOpsService::noteOperation(int32_t code, int32_t uid, const String16& packageName,
           	                 const std::optional<String16>& attributionTag, bool shouldCollectAsyncNotedOp,
			         const String16& message, bool shouldCollectMessage) {
    (void)code;
    (void)uid;
    (void)packageName;
    (void)attributionTag;
    (void)shouldCollectAsyncNotedOp;
    (void)message;
    (void)shouldCollectMessage;

    return MODE_ALLOWED;
}
void
FakeAppOpsService::finishOperation(const sp<IBinder>& token, int32_t code, int32_t uid,
          		           const String16& packageName, const std::optional<String16>& attributionTag) {
    (void)token;
    (void)code;
    (void)uid;
    (void)packageName;
    (void)attributionTag;
}
#else
int32_t
FakeAppOpsService::noteOperation(int32_t code, int32_t uid, const String16& packageName)
{
    (void)code;
    (void)uid;
    (void)packageName;

    return MODE_ALLOWED;
}

void
FakeAppOpsService::finishOperation(const sp<IBinder>& token, int32_t code, int32_t uid,
                 		   const String16& packageName)
{
    (void)token;
    (void)code;
    (void)uid;
    (void)packageName;
}
#endif

int32_t
FakeAppOpsService::startOperation(int32_t code, int32_t uid, const String16& packageName)
{
    (void)code;
    (void)uid;
    (void)packageName;

    return MODE_ALLOWED;
}

#if ANDROID_VERSION >= 29
#if ANDROID_VERSION >= 33
int32_t
FakeAppOpsService::startOperation(const sp<IBinder>& token, int32_t code, int32_t uid,
		                  const String16& packageName, const std::optional<String16>& attributionTag,
			          bool startIfModeDefault, bool shouldCollectAsyncNotedOp, const String16& message,
		                  bool shouldCollectMessage) {
    (void)token;
    (void)code;
    (void)uid;
    (void)packageName;
    (void)attributionTag;
    (void)startIfModeDefault;
    (void)shouldCollectAsyncNotedOp;
    (void)message;
    (void)shouldCollectMessage;

    return MODE_ALLOWED;
}
#else
int32_t
FakeAppOpsService::startOperation(const sp<IBinder>& token, int32_t code, int32_t uid,
    const String16& packageName, bool startIfModeDefault)
{
    (void)token;
    (void)code;
    (void)uid;
    (void)packageName;
    (void)startIfModeDefault;

    return MODE_ALLOWED;
}
#endif

int32_t
FakeAppOpsService::checkAudioOperation(int32_t code, int32_t usage, int32_t uid,
    const String16& packageName)
{
    (void)code;
    (void)usage;
    (void)uid;
    (void)packageName;

    return MODE_ALLOWED;
}
#else
int32_t
FakeAppOpsService::startOperation(const sp<IBinder>& token, int32_t code, int32_t uid,
    const String16& packageName)
{
    (void)token;
    (void)code;
    (void)uid;
    (void)packageName;

    return MODE_ALLOWED;
}
#endif

sp<IBinder>
FakeAppOpsService::getToken(const sp<IBinder>& clientToken)
{
    (void)clientToken;

    return NULL;
}

void
FakeAppOpsService::startWatchingMode(int32_t op, const String16& packageName, const sp<IAppOpsCallback>& callback)
{
    (void)op;
    (void)packageName;
    (void)callback;
}

void
FakeAppOpsService::stopWatchingMode(const sp<IAppOpsCallback>& callback)
{
    (void)callback;
}

#if ANDROID_VERSION >= 23
int32_t
FakeAppOpsService::permissionToOpCode(const String16& permission)
{
    (void)permission;

    return MODE_ALLOWED;
}
#endif
}; // namespace android

using namespace android;

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    FakeAppOpsService::publishAndJoinThreadPool();
    return 0;
}
