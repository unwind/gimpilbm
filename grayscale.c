#include "grayscale.h"

/**** Grayscale ****/

void transGray(grayval *dest, gint width, const palidx *transGrayTab)
{
	g_assert(dest != NULL);
	g_assert(transGrayTab != NULL);
	while(width--)
	{
		*dest = transGrayTab[*dest];
		++dest;
	}
}

void dumpGrayTrans(const palidx *t)
{
	gint	i;

	g_assert(NULL != t);
	printf ("grayTrans colortable:\n");
	/* Crashes if maxGrayshades % 8 > 0 */
	for(i = 0; i < maxGrayshades; i += 8)
		printf("  %02x %02x %02x %02x  %02x %02x %02x %02x\n",
			t[i], t[i + 1], t[i + 2], t[i + 3],
			t[i + 4], t[i + 5], t[i + 6], t[i + 7]);
}

guint8 * allocGrayscale(void)
{
	guint8	*gbuf = g_new(guint8, maxGrayshades * byteppRGB);

	if(gbuf == NULL)
	{
		fputs("Out of memory.\n", stderr);
	}
	else
	{
		guint8	*gp;
		gint	i;
		if(VERBOSE)
			fputs("Composing grayscale colormap...\n", stdout);
		for(gp = gbuf, i = 0; i < maxGrayshades; ++i)
		{
			*gp++ = i;
			*gp++ = i;
			*gp++ = i;
		}
	}
	return gbuf;
}

/*  Returns TRUE if the given colormap consists of only
 *  gray entries
 */
gboolean isGrayscale(const guint8 *cmap, gint ncols)
{
	g_assert(cmap != NULL);
	g_assert(ncols >= 0);
	while(ncols--)
	{
		if(!((*cmap == cmap[1]) && (cmap[1] == cmap[2])))
			return FALSE;
		cmap += byteppRGB;
	}
	return TRUE;
}

/* Allocates a 256 byte array of the values 0..255 */
guint8 * allocGrayKeep(void)
{
	guint8	*gt = g_new(guint8, maxGrayshades);

	if(gt == NULL)
		fputs("Out of memory.\n", stderr);
	else
	{
		gint	i;

		for(i = maxGrayshades - 1; i >= 0; --i)
			gt[i] = i;
	}
	return gt;
}

/*  Returns a ncols wide array
 *  "cmap" has to be grayscale
 */
guint8 * allocGrayTrans(const guint8 *cmap, gint ncols)
{
	guint8	*gt = g_new(guint8, maxGrayshades);

	g_assert(cmap != NULL);
	g_assert(ncols >= 0);
	if(gt == NULL)
		fputs("Out of memory.\n", stderr);
	else
	{
		gint	idx = 0;

		while(ncols--)
		{
			gt[idx] = *cmap;
			++idx;
			cmap += byteppRGB;
		}
	}
	return gt;
}
