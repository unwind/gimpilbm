#ifndef hamehb_h
#define hamehb_h

#include <stdlib.h>
#include <glib.h>
#include "ilbm.h"

extern const guint8	hamPal[16 * byteppRGB];

extern grayval*	reallocEhbCmap(grayval *cmap, gint *p_ncols);
extern void	deHam(grayval *dest, const palidx *src, gint width, guint16 depth, const grayval *cmap, gboolean alpha);

#endif
