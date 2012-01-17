#ifndef hamehb_h
#define hamehb_h

#include <glib.h>
#include "ilbm.h"

grayval* reallocEhbCmap (grayval* cmap, gint* p_ncols);

extern const guint8 hamPal[16 * byteppRGB];

void
  deHam(grayval * dest, const palidx * src, gint width, guint16 depth,
        const grayval * cmap, gint alpha);

#endif
