#ifndef grayscale_h
#define grayscale_h

#include "ilbm.h"

/**** Grayscale ****/

extern void	transGray(grayval *dest, gint width, const palidx *transGrayTab);
extern void	dumpGrayTrans(const palidx *t);

extern guint8 *	allocGrayscale(void);
extern gboolean	isGrayscale(const guint8 *cmap, gint ncols);
extern guint8 *	allocGrayKeep(void);
extern guint8 *	allocGrayTrans(const guint8 *cmap, gint ncols);

#endif
