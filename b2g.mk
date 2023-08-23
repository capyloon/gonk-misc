include gonk-misc/sepolicy/sepolicy.mk

PRODUCT_PACKAGES += \
	ads-sdk \
	b2g-info \
	b2g-ps \
	b2g-procrank \
	b2g.sh \
	selinux.sh \
	b2gkillerd \
	gfxdebugger \
	gonksched \
	init.b2g.rc \
	init.bluetooth.rc \
	gecko \
	api-daemon \
	bluetoothd \
	librecovery \
	nutria \
	sources.xml \
	ipfsd \
	fakeappops \
	gonkservices \
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

ifeq ($(BOOTUPANIMATION_RESOLUTION),)
BOOTUPANIMATION_RESOLUTION := qvga
endif

ifeq ($(ENABLE_DEFAULT_BOOTANIMATION),true)

ifneq ($(CUSTOM_BUILD_CONFIG_NAME),)
CUSTOMER := $(CUSTOM_BUILD_CONFIG_NAME)
else
CUSTOMER := default
endif

PRODUCT_COPY_FILES += \
        gonk-misc/animation/$(CUSTOMER)/bootanimation_$(BOOTUPANIMATION_RESOLUTION).zip:system/media/bootanimation.zip \
        gonk-misc/poweron-sound.ogg:system/media/poweron-sound.ogg \
        gonk-misc/fonts.xml:system/etc/fonts.xml
endif

# Enabled build librecovery.
ENABLE_LIBRECOVERY := true

# IME dictionaries from TouchPal
ifeq ($(IME_ENGINE), touchpal)
-include external/libtouchpal/touchpal.mk
endif

$(call inherit-product-if-exists, nutria/nutria.mk)
$(call inherit-product-if-exists, external/svox/pico/lang/all_pico_languages.mk)
$(call inherit-product-if-exists, external/moztt/fonts.mk)

