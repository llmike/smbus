ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

USEFILE=../../sbread.use

EXTRA_INCVPATH+=../../../
EXTRA_INCVPATH+=../../../../smb-ec/

include $(MKFILES_ROOT)/qtargets.mk
