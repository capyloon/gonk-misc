#!/system/bin/sh

# Wait a bit to let services that check that we are in
# SELinux enforcing mode at startup some time to initialize.

sleep 10

# Switch selinux to permissive mode and set
# the android property monitored by init.b2g.rc
# to trigger the start of b2g services that can't
# run in enforcing mode.

setenforce 0 && setprop b2g.permissive 1

