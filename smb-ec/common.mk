ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

INSTALLDIR=sbin

define PINFO
PINFO DESCRIPTION=SMBus driver for ACPI Embedded Controller
endef

USEFILE=../../smb-ec.use

include $(MKFILES_ROOT)/qtargets.mk
-include $(PROJECT_ROOT)/roots.mk
