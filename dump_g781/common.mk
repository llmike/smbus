ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

USEFILE=../../dump_g781.use

EXTRA_INCVPATH+=../../../smb-ec/

include $(MKFILES_ROOT)/qtargets.mk
