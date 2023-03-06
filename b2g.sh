#!/system/bin/sh
umask 0027
export TMPDIR=/data/local/tmp
mkdir -p $TMPDIR
chmod 1777 $TMPDIR
ulimit -n 8192

# Pixel 3a temporary fixes
setprop "persist.hwc.powermode" true

# Enable core dumps only on debug builds and if explicitly enabled
DEBUG=`getprop ro.debuggable`
if [ "$DEBUG" == "1" ]; then
  COREDUMP=`getprop persist.debug.coredump`

  if [ -n "$COREDUMP" ]; then
    if [ ! -d /data/core ]; then
      # applications need write/execute to generate core but not read
      mkdir /data/core
      chmod 0733 /data/core
      chown root:root /data/core
    fi
    echo "/data/core/%e.%p.%t.core" > /proc/sys/kernel/core_pattern
    echo "1" > /proc/sys/fs/suid_dumpable
  fi

  if [ "$COREDUMP" == "all" ]; then
    /system/bin/b2g-prlimit 0 core -1 -1
  elif [ "$COREDUMP" == "b2g" ]; then
    ulimit -c -1
  fi
fi

if [ ! -d /system/b2g ]; then

  log -p W "No /system/b2g directory. Attempting recovery."
  if [ -d /system/b2g.bak ]; then
    if ! mount -w -o remount /system; then
      log -p E "Failed to remount /system read-write"
    fi
    if ! mv /system/b2g.bak /system/b2g; then
      log -p E "Failed to rename /system/b2g.bak to /system/b2g"
    fi
    mount -r -o remount /system
    if [ -d /system/b2g ]; then
      log "Recovery successful."
    else
      log -p E "Recovery failed."
    fi
  else
    log -p E "Recovery failed: no /system/b2g.bak directory."
  fi
fi

if [ -z "$B2G_DIR" ]; then
  B2G_DIR="/system/b2g"
fi

EMULATOR=`getprop ro.kernel.qemu`
if [ "$EMULATOR" == "1" -a ! -f "$B2G_DIR/b2g" ]; then
  log -p E "Empty emulator detected not starting b2g."
  setprop sys.boot_completed 1
  exit 1
fi

LD_PRELOAD="$B2G_DIR/libmozglue.so"
if [ -f "$B2G_DIR/libdmd.so" ]; then
  echo "Running with DMD."
  LD_PRELOAD="$B2G_DIR/libdmd.so $LD_PRELOAD"
  export DMD="1"
fi
#if [ -f "$B2G_DIR/libmozsandbox.so" ]; then
#    echo "Running with Sandbox."
#    LD_PRELOAD="$B2G_DIR/libmozsandbox.so $LD_PRELOAD"
#fi
export LD_PRELOAD

LIBDIR="lib"
if [ -d "/system/lib64" ]; then
  LIBDIR="lib64"
fi;

export LD_LIBRARY_PATH=/vendor/$LIBDIR:/system/$LIBDIR:"$B2G_DIR":/apex/com.android.tethering/$LIBDIR:/apex/com.android.i18n/$LIBDIR:/apex/com.android.runtime/$LIBDIR:/system/apex/com.android.runtime.debug/$LIBDIR:/system_ext/apex/com.android.vndk.current/$LIBDIR
export GRE_HOME="$B2G_DIR"

# Run in jar logging mode if needed.
JAR_LOG_ENABLED=`getprop moz.jar.log`
if [ "$JAR_LOG_ENABLED" = "1" ]; then
  export MOZ_JAR_LOG_FILE=/data/local/tmp/jarloader.log
fi

ENABLE_LOG=`getprop kaios.log.enable`
if [ "$ENABLE_LOG" == "1" ]; then
  LOG_FILTER=`getprop kaios.log.filter`
  export NSPR_LOG_MODULES=$LOG_FILTER
fi

if [ -f "/system/bin/fakesurfaceflinger" ]; then
  exec "/system/bin/fakesurfaceflinger" &
  wait
  echo "Setting hwc is done."
fi

# Put the parent process to the /b2g/default cgroup.
echo $$ > /dev/memcg/b2g/default/cgroup.procs

# When app process sandbox is on, we should terminate the app process when some system calls are not allowed.
export MOZ_SANDBOX_CRASH_ON_ERROR="1"
export MOZ_SANDBOX_ERROR="1"

exec $COMMAND_PREFIX "$B2G_DIR/b2g"
