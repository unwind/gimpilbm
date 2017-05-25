
#include <string.h>

#include "hamehb.h"

/**** EHB stuff ****/

grayval* reallocEhbCmap(grayval *cmap, gint *p_ncols)
{
	gint	i;

	if(VERBOSE)
		printf("Generating EHB (%d->%d) colormap entries...\n", *p_ncols, 2 * *p_ncols);
	cmap = g_realloc(cmap, byteppRGB * 2 * *p_ncols);
	if(!cmap)
		g_error("Out of memory.");
	else
	{
		for(i = byteppRGB * *p_ncols - 1; i >= 0; --i)
			cmap[byteppRGB * *p_ncols + i] = cmap[i] >> 1;
		*p_ncols *= 2;
	}
	return cmap;
}

/**** HAM stuff ****/

/* Pre-optimized (?) constant palette for HAM, 16 colors. */
const guint8 hamPal[16 * byteppRGB] =
{
	0x02, 0x02, 0x24,  0x02, 0xFE, 0x44,
	0xFE, 0x02, 0x59,  0x02, 0x7E, 0x3A,
	0xFE, 0xFE, 0x49,  0x7E, 0x02, 0x42,
	0x7E, 0xFE, 0x3E,  0xFE, 0x7E, 0x85,
	0x7E, 0x7E, 0x77,  0x02, 0x02, 0xFC,
	0x02, 0xFE, 0xFC,  0xFE, 0x02, 0xFC,
	0x02, 0x7E, 0xFC,  0xFE, 0xFE, 0xFC,
	0x7E, 0x02, 0xFC,  0x7E, 0xFE, 0xFC
};

#define	hamPalCols (sizeof hamPal / byteppRGB)

/* Compute and return distance between the two colors. The distance is simply the squared Euclidian 3D distance. */
static guint rgbDist(const guint8 *rgbA, const guint8 *rgbB)
{
	const guint dr = (rgbB[0] - rgbA[0]) * (rgbB[0] - rgbA[0]);
	const guint dg = (rgbB[1] - rgbA[1]) * (rgbB[1] - rgbA[1]);
	const guint db = (rgbB[2] - rgbA[2]) * (rgbB[2] - rgbA[2]);
	const guint dist = dr + dg + db;
/*	printf("comparing 0x%02x%02x%02x vs 0x%02x%02x%02x -> dr=%u, dg=%u, db=%u -> dist=%u\n",
		rgbA[0], rgbA[1], rgbA[2],
		rgbB[0], rgbB[1], rgbB[2],
		dr, dg, db, dist);
*/	return dist;
}

/* Is r, g, or b the smallest difference? */
static int judgeDiff(int r1, int g1, int b1, int r2, int g2, int b2)
{
	const int dr = abs(r1 - r2) * abs(r1 - r2);
	const int dg = abs(g1 - g2) * abs(g1 - g2);
	const int db = abs(b1 - b2) * abs(b1 - b2);
	int	pts;

	return 3 * 255 * 255 - (dr + dg + db);
/*
	printf(" dr=%d, dg=%d, db=%d", dr, dg, db);
	if(dr < dg)
	{
		if(dr < db)
			pts = (dr >> 1) + dg + db;
		else
			pts = (db >> 1) + dr + dg;
	}
	else
	{
		if(dg < db)
			pts = (dg >> 1) + dr + db;
		else
			pts = (db >> 1) + dr + dg;
	}
	printf(" -> pts=%d\n", pts);
	return pts;
*/
}

static int judgeDiffs(guint8 *offsPtr, int r2, int g2, int b2)
{
	int	bestPts = 3 * 255 * 255;
	int	bestOffs = 0;             /* Make gcc happy */
	const guchar *palPtr = hamPal + sizeof hamPal;
	const guint8 current[] = { r2 & 0xff, g2 & 0xff, b2 & 0xff };

	g_assert(offsPtr != NULL);
	for(gint i = hamPalCols - 1; i >= 0; --i)
	{
		palPtr -= 3;
		const int aPts = judgeDiff(palPtr[0], palPtr[1], palPtr[2], r2, g2, b2);
		rgbDist(palPtr, current);
		if(aPts < bestPts)
		{
			bestOffs = i;
			bestPts = aPts;
			if(!aPts)
				break;
		}
	}
	*offsPtr = bestOffs;
	return bestPts;
}

/* Pick the color from the palette which is closest to the target, returning its distance and index. */
static guint rgbBestFromPalette(guint *index, const guint8 *target)
{
	guint bestDist = G_MAXUINT;
	for(gint i = hamPalCols - 1; i >= 0; --i)
	{
		const guint8 * const rgbPal = hamPal + 3 * i;
		const guint dist = rgbDist(rgbPal, target);
		if(dist < bestDist)
		{
			*index = (guint) i;
			bestDist = dist;
			if(bestDist == 0)
				break;
		}
	}
	return bestDist;
}

/* Compute resulting RGB color from holding current and modifying the indicated component using data from target. */
static void rgbHam(guint8 *ham, const guint8 *current, const guint8 *target, guint component)
{
	for(guint i = 0; i < 3; ++i)
	{
		ham[i] = (i == component) ? ((target[i] & 0xf0) | (target[i] >> 4)) : current[i];
	}
}

void lineToHam(guint8 *hamIdxOut, const guint8 *rgbIn, gint bytepp, gint width)
{
	guint8 current[3];

/*	for(gint i = 0; i < width; ++i)
	{
		printf("x=%d: %02x%02x%02x\n", i, rgbIn[i * bytepp + 0], rgbIn[i * bytepp + 1], rgbIn[i * bytepp + 2]);
	}
*/
	g_assert(hamIdxOut != NULL);
	g_assert(rgbIn != NULL);
	bytepp -= 3;                  /* just need 0/1 offsets */
	for(gint x = 0; x < width; ++x)
	{
		guint palIndex;
		const guint palDist = rgbBestFromPalette(&palIndex, rgbIn);
		if(x == 0)
		{
			/* Start each new scanline with an absolute color, from the palette. */
			memcpy(current, hamPal + 3 * palIndex, sizeof current);
//			printf("x=%u: current set to %02x%02x%02x (index %u)\n", x, current[0], current[1], current[2], palIndex);
			*hamIdxOut++ = (guint8) palIndex;
		}
		else
		{
			/* Mid-line, decide if current pixel is best served by a palette reload, or by HAM:ing. */
			guint	hamDist[3], hamDistMin = G_MAXUINT;
//			printf("x=%u, have=%02x%02x%02x, want=%02x%02x%02x\n", x, current[0], current[1], current[2], rgbIn[0], rgbIn[1], rgbIn[2]);
			for(guint i = 0; i < 3; ++i)
			{
				guint8 ham[3];
				rgbHam(ham, current, rgbIn, i);
				hamDist[i] = rgbDist(ham, rgbIn);
				if(hamDist[i] < hamDistMin)
				{
					hamDistMin = hamDist[i];
				}
			}
/*			printf(" hamDist=[%u,%u,%u], hamDistMin=%u\n", hamDist[0], hamDist[1], hamDist[2], hamDistMin);
			printf(" palDist=%u (index %u)\n", palDist, palIndex);
*/			if(palDist < hamDistMin)
			{
				/* Don't HAM, do a full reload from palette. */
				memcpy(current, hamPal + 3 * palIndex, sizeof current);
				*hamIdxOut++ = (guint8) palIndex;
//				printf("  current set to %02x%02x%02x (index %u)\n", current[0], current[1], current[2], palIndex);
			}
			else
			{
				const guint8 hamBits[] = { 0x20, 0x30, 0x10 };
				for(guint i = 0; i < 3; ++i)
				{
					if(hamDistMin == hamDist[i])
					{
						*hamIdxOut++ = hamBits[i] | (rgbIn[i] >> 4);
						current[i] = (rgbIn[i] & 0xf0) | (rgbIn[i] >> 4);
//						printf("  current set to %02x%02x%02x by HAM on component %c\n", current[0], current[1], current[2], "RGB"[i]);
						break;
					}
				}
			}
		}
		rgbIn += 3;
		if(bytepp)	/* Bring along the alpha, if present. */
			*hamIdxOut++ = *rgbIn++;
	}
}

#define	HAM_MODIFY_NONE		0
#define	HAM_MODIFY_RED		2
#define	HAM_MODIFY_GREEN	3	/* Hello? */
#define	HAM_MODIFY_BLUE		1

static grayval hamExpand(grayval data, guint16 depth)
{
	if(depth == 4)
		return (data << 4) | data;
	return (data << 2) | (data >> 4);
}

void deHam(grayval *dest, const palidx *src, gint width, guint16 depth, const grayval *cmap, gboolean alpha)
{
	grayval cr = 0, cg = 0, cb = 0;
	grayval	* const mods[] = { &cb, &cr, &cg };

	g_assert(dest != NULL);
	g_assert(src != NULL);
	g_assert(width > 0);
	g_assert(depth >= 3);
	g_assert(cmap != NULL);

	depth -= 2;
	const grayval dmask = (1 << depth) - 1;

	/* Note: this treats each scanline on its own, doesn't modify color across scanlines. Not sure what the hardware does. */
	while(width--)
	{
		const grayval	idx = *src++;
		const grayval	data = idx & dmask;
		const grayval	control = idx >> depth;

		if(control == HAM_MODIFY_NONE)	/* Replace all components with color from palette. */
		{
			cr = cmap[3 * data + 0];
			cg = cmap[3 * data + 1];
			cb = cmap[3 * data + 2];
		}
		else	/* Use 'control' to index into pointer array, and overwrite proper component. */
			*mods[control - 1] = hamExpand(data, depth);
		/* Write current color into output RGB buffer. */
		*dest++ = cr;
		*dest++ = cg;
		*dest++ = cb;
		/* Leave alpha channel untouched */
		if(alpha)
			++dest;
	}
}
