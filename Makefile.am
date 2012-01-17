EXTRA_DIST	= gimpilbm.jcc
gimpplugin_PROGRAMS = ilbm
ilbm_SOURCES	= iff.c iff.h ilbm.c ilbm.h \
		  hamehb.c hamehb.h byterun1.c byterun1.h \
		  plugin.c plugin.h gui.c gui.h
#ilbm_LDADD	= -L/usr/X11/lib -lglib -lgdk -lgtk -lgimp -lX11 -lXext -lXi -lm
#ilbm_LDFLAGS	= $(shell @GIMPTOOL@ --libs ; @GTKCONFIG@ --libs)
INCLUDES	= $(shell @GIMPTOOL@ --cflags) @debugflags@
ilbm_LDFLAGS	= $(shell @GIMPTOOL@ --libs)
