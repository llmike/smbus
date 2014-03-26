ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

USEFILE=../../dump_spd.use

EXTRA_INCVPATH+=../../../smb-ec/

include $(MKFILES_ROOT)/qtargets.mk
