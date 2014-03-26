ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

USEFILE=../../dump_ec.use

include $(MKFILES_ROOT)/qtargets.mk
