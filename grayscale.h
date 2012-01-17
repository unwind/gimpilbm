#ifndef grayscale_h
#define grayscale_h

#include "oldgimp.h"
#include "ilbm.h"

/**** Grayscale ****/

void transGray(grayval * dest, gint width, const palidx * transGrayTab);
void dumpGrayTrans(const palidx * t);
guint8 * allocGrayscale(void);
gboolean isGrayscale(const guint8 * cmap, gint ncols);
guint8 * allocGrayKeep(void);
guint8 * allocGrayTrans(const guint8 * cmap, gint ncols);

#endif
