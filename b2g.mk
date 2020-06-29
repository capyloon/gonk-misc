PRODUCT_PACKAGES += \
	b2g-info \
        b2g-ps \
	b2g.sh \
	fakeappops \
	fakeSensorPrivacy \
	fakePackageManagerNative \
	gonkProcessInfo \
	gonksched \
	init.b2g.rc \
	init.bluetooth.rc \
	libcarthage \
	gecko \
	api-daemon \
	api-daemon.sh \
	updater-daemon \
	updater-daemon.sh \
	bluetoothd \
	librecovery \
	gaia \
	sources.xml \
	$(NULL)


# Dhcpcd
DHCPCD_USE_SCRIPT := yes

PRODUCT_PACKAGES += \
    dhcpcd-6.8.2 \
    dhcpcd.conf \
    dhcpcd-run-hooks \
    20-dns.conf \
    95-configured

ifeq ($(ENABLE_RADVD),true)
PRODUCT_PACKAGES += \
        radvd
endif

ifeq ($(ENABLE_DEFAULT_BOOTANIMATION),true)
PRODUCT_COPY_FILES += \
        gonk-misc/bootanimation.zip:system/media/bootanimation.zip \
        gonk-misc/poweron-sound.ogg:system/media/poweron-sound.ogg
endif

# Enabled build librecovery.
ENABLE_LIBRECOVERY := true

$(call inherit-product-if-exists, external/svox/pico/lang/all_pico_languages.mk)
