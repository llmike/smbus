ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

include $(MKFILES_ROOT)/qphabtarg.mk

EXTRA_INCVPATH+=../../../
EXTRA_INCVPATH+=../../../../smb-ec/
