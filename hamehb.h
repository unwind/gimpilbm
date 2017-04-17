#ifndef hamehb_h
#define hamehb_h

#include <stdlib.h>
#include <glib.h>
#include "ilbm.h"

extern const guint8	hamPal[16 * byteppRGB];

void 		lineToHam(guint8 *hamIdxOut, const guint8 *rgbIn, gint bytepp, gint width);
grayval*	reallocEhbCmap(grayval *cmap, gint *p_ncols);
void		deHam(grayval *dest, const palidx *src, gint width, guint16 depth, const grayval *cmap, gboolean alpha);

#endif
