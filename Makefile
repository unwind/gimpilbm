#
# Initial attempt and hand-writing a Makefile to build "gimpilbm".
#

GIMPTOOL="gimptool-2.0"

# Trust people named emil to know what works. This is rather heavily heuristical. :)
ifeq "$(shell whoami)" "emil"
DEVMODE=1
else
DEVMODE=0
endif

CFLAGS=`$(GIMPTOOL) --cflags` -std=c99 -Wall -Wextra -pedantic -DDEVMODE=$(DEVMODE) -DDEBUGLEVEL=0
LDFLAGS=`$(GIMPTOOL) --libs`

# --------------------------------------------------------------------------

.PHONY:		clean install

gimpilbm.so:	byterun1.o grayscale.o gui.o hamehb.o iff.o ilbm.o plugin.o
		$(CC) -o $@ $^ $(LDFLAGS)

byterun1.o:	byterun1.c byterun1.h

grayscale.o:	grayscale.c grayscale.h

gui.o:		gui.c gui.h

hamehb.o:	hamehb.c hamehb.h

iff.o:		iff.c iff.h

ilbm.o:		ilbm.c ilbm.h

plugin.o:	plugin.c plugin.h

# --------------------------------------------------------------------------

clean:
		rm -f *.so *.o

install:	gimpilbm.so
		$(GIMPTOOL) --install-bin $^
