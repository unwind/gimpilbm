#ifndef plugin_h
#define plugin_h

#include <gtk/gtk.h>

typedef struct {
  gdouble threshold;
  gint32 compress;
  gint32 save_ham;
  gint32 save_chunky;
  gint32 set_backgnd;
} ILBMSaveVals;

typedef struct {
  gint run;
} ILBMSaveInterface;

typedef struct _GimpPlugInInfo PluginStructType;

extern ILBMSaveVals ilbmvals;
extern ILBMSaveInterface ilbmint;

extern const char loadFuncID[];
extern const char saveFuncID[];
extern const char nameExtensions[];

#endif
