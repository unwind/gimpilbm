#ifndef ilbm_h
#define ilbm_h

#include <libgimp/gimp.h>

#include "iff.h"

#ifndef VERSION
#  define VERSION "0.10.1"
#endif

#define PLUG_IN_VERSION "2017-03-03 (" VERSION ")"

#ifndef DEBUGLEVEL
#  define DEBUGLEVEL	2
#endif

#define VERBOSE		(DEBUGLEVEL > 0)
#define DEBUG		(DEBUGLEVEL > 1)

#define ID_ILBM	MAKE_ID('I','L','B','M')
#define ID_PBM_	MAKE_ID('P','B','M',' ')
#define ID_RGBN	MAKE_ID('R','G','B','N')
#define ID_RGB8	MAKE_ID('R','G','B','8')

#define ID__C__	MAKE_ID('(','C',')',' ')
#define ID_ACBM	MAKE_ID('A','C','B','M')
#define ID_ANNO	MAKE_ID('A','N','N','O')
#define ID_AUTH	MAKE_ID('A','U','T','H')
#define ID_BMHD	MAKE_ID('B','M','H','D')
#define ID_BODY	MAKE_ID('B','O','D','Y')
#define ID_CAMG	MAKE_ID('C','A','M','G')
#define ID_CHRS	MAKE_ID('C','H','R','S')
#define ID_CMAP	MAKE_ID('C','M','A','P')
#define ID_DEST	MAKE_ID('D','E','S','T')
#define ID_DPI_	MAKE_ID('D','P','I',' ')
#define ID_DPPS	MAKE_ID('D','P','P','S')
#define ID_FVER	MAKE_ID('F','V','E','R')
#define ID_GRAB	MAKE_ID('G','R','A','B')
#define ID_IMRT	MAKE_ID('I','M','R','T')
#define ID_NAME	MAKE_ID('N','A','M','E')
#define ID_SPRT	MAKE_ID('S','P','R','T')
#define ID_copy	MAKE_ID('c','o','p','y')

#define ham4bitToGray8(val4)	((((val4) << 4) * (gint16) 17) / 16)

#define gray8ToHam4(g8)		((((guint16)(g8)) * 15) / 255)

/**** Types ****/

typedef guint8	grayval;
typedef guint8	palidx;          /* 8bit!! indexed files are built of it */

/**** Misc ****/

enum ColorLimit {
	maxIndexedBits = 8,
	maxIndexedColors = 1 << maxIndexedBits,
	maxGrayshades = maxIndexedColors
};

enum ByteWidth {
	byteppGray = 1, byteppGrayA = byteppGray + byteppGray,
	byteppRGB = byteppGray * 3, byteppRGBA = byteppRGB + byteppGray
};

enum BitWidth {
	bitperbyte = 8,
	bitppGray = byteppGray * bitperbyte, bitppGrayA = bitppGray + bitppGray,
	bitppRGB = byteppRGB * bitperbyte, bitppRGBA = bitppRGB + bitppGray,
	bitppRGB4 = 12                /* mmh.. */
};

#define BYTEPL(wid)	((((wid)+15)&~15)>>3)

enum {
	transparent = 0x00, opaque = 0xFF
};

typedef struct {
	guint16	w, h;
	gint16	x, y;
	guint8	nPlanes;
	guint8	masking;
	guint8	compression;
	guint8	pad1;
	guint16	transparentColor;
	guint8	xAspect, yAspect;
	gint16	pageWidth, pageHeight;
} ILBMbmhd;

enum MaskingType {
	mskNone, mskHasMask, mskHasTransparentColor, mskLasso
};

enum CompressionType {
	cmpNone, cmpByteRun1, cmpRGBN = 4, cmpRGB8 = 999  /* FIXME! */
};

typedef struct {
	guint32	viewModes;
} ILBMcamg;

enum ViewModeFlags {
	lace = 0x00000004,
	extraHalfbrite = 0x00000080,
	ham = 0x00000800,
	hiRes = 0x00008000
};

#define CAMGMASK	(0x00009EFDL)

typedef struct {
	guint16	dpiX;
	guint16	dpiY;
} ILBMdpi;

typedef struct {
	gint16	grabX;
	gint16	grabY;
} ILBMgrab;

typedef struct {
	guint8	depth;
	guint8	pad;
	guint16	planePick;
	guint16	planeOnOff;
	guint16	planeMask;
} ILBMdest;

typedef struct {
	guint16	preced;
} ILBMsprt;


/**** ILBMAttribs ****/

typedef struct {
	ILBMbmhd	bmhd;
	ILBMcamg	camg;
	guint16		cmapSet;
	guchar		cmap[maxIndexedColors * byteppRGB];
} ILBMAttribs;

extern gint32	loadImage(const gchar *filename);
extern gint	saveImage(const gchar *filename, gint32 imageID, gint32 drawableID);

#endif
