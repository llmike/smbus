PROG= acpidump
SRCS= acpidump.c getopt_long.c
CFLAGS := -I../include
CFLAGS += -Wall -Wstrict-prototypes -Wdeclaration-after-statement -Os -s -D_QNX -DDEFINE_ALTERNATE_TYPES

all: acpidump
$(PROG) : $(SRCS)
	$gcc $(CFLAGS) $(SRCS) -o $(PROG)

CLEANFILES= $(PROG)

clean :
	rm -f $(CLEANFILES) $(patsubst %.c,%.o, $(SRCS)) *~

