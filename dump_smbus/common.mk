ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

USEFILE=../../dump_smbus.use

EXTRA_INCVPATH+=../../../smb-ec/

include $(MKFILES_ROOT)/qtargets.mk
