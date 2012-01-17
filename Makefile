#
# Initial attempt and hand-writing a Makefile to build "gimpilbm".
#

GIMPTOOL="gimptool-2.0"

CFLAGS=`$(GIMPTOOL) --cflags`
LDFLAGS=`$(GIMPTOOL) --libs`

# --------------------------------------------------------------------------

.PHONY:		clean

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
