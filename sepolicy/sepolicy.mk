# The external/sepolicy/kaios_sepolicy.mk is a kaios sepolicy enabler

B2G_SEPOLICY_ENABLED=true
ifeq ($(B2G_SEPOLICY_ENABLED),true)
BOARD_PLAT_PUBLIC_SEPOLICY_DIR += gonk-misc/sepolicy/b2gos
#BOARD_PLAT_PUBLIC_SEPOLICY_DIR += gonk-misc/sepolicy/public
#BOARD_PLAT_PRIVATE_SEPOLICY_DIR += gonk-misc/sepolicy/private
#BOARD_SEPOLICY_DIRS += gonk-misc/sepolicy/vendor
endif
