#ifndef oldgimp_h
#define oldgimp_h

#define GIMP_ENABLE_COMPAT_CRUFT

#ifdef OLDGIMP
#  define GimpDrawable GDrawable
#  define GimpPixelRgn GPixelRgn
#  define GimpParam    GParam
#  define GimpParamDef GParamDef
#  define GIMP_RGB     RGB
#  define GIMP_GRAY    GRAY
#  define GIMP_INDEXED INDEXED
#  define GIMP_RGB_IMAGE	RGB_IMAGE
#  define GIMP_RGBA_IMAGE	RGBA_IMAGE
#  define GIMP_GRAY_IMAGE	GRAY_IMAGE
#  define GIMP_GRAYA_IMAGE	GRAYA_IMAGE
#  define GIMP_INDEXED_IMAGE	INDEXED_IMAGE
#  define GIMP_INDEXEDA_IMAGE	INDEXEDA_IMAGE
#  define GIMP_NORMAL_MODE	NORMAL_MODE
#endif

#endif
