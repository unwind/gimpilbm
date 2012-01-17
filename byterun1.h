#ifndef byterun1_h
#define byterun1_h

#include <stdio.h>
#include <string.h>
#include <glib.h>

#ifdef MIN
#undef MIN
#endif
#define MIN(a,b) (((a)<(b))?(a):(b))

gint32 packRow(guint8 * odest, const guint8 * src, guint32 width);
gboolean unpackRow(FILE * fil, gint8 * bitlinebuf, gint32 bytesNeeded);

#endif
