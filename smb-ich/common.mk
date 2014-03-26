ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

INSTALLDIR=sbin

define PINFO
PINFO DESCRIPTION=SMBus driver for Intel ICH/PCH
endef

USEFILE=../../smb-ich.use

include $(MKFILES_ROOT)/qtargets.mk
-include $(PROJECT_ROOT)/roots.mk
