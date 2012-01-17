#include <gtk/gtk.h>
#include <libgimp/gimp.h>

#include "ilbm.h"
#include "gui.h"
#include "plugin.h"

#include <string.h>

/**** Prototypes ****/

static void queryPlugin(void);
static void runPlugin(const gchar *name, gint nparams, const GimpParam * param, gint int *nreturn_vals, GimpParam **return_vals);

/**** Plugin data ****/

const GimpPlugInInfo PLUG_IN_INFO =
{
  NULL,		/* init_proc  */
  NULL,		/* quit_proc  */
  queryPlugin,	/* query_proc */
  runPlugin	/* run_proc   */
};

ILBMSaveVals ilbmvals =
{
  0.50,		/* alpha threshold */
  1,
  0,
  0,
  0
};

ILBMSaveInterface ilbmint =
{
  FALSE		/* run */
};

/**** standard settings ****/

const char loadFuncID[] = "file_ilbm_load";
const char saveFuncID[] = "file_ilbm_save";
const char nameExtensions[] = "ilbm,iff,lbm,acb,ham";

/**** main() ****/

MAIN ()

#if 0
int
main(int argc, char **argv)
{
  return (gimp_main(argc, argv));
}
#endif

/**** queryPlugin() ****/

static void
queryPlugin(void)
{
  static GimpParamDef load_args[] =
  {
    {PARAM_INT32, "run_mode", "Interactive, non-interactive"},
    {PARAM_STRING, "filename", "The name of the file to load"},
    {PARAM_STRING, "raw_filename", "The name entered"},
    {PARAM_INT32, "gen_gradients", "Build gradients from DRNG chunks"},
    {PARAM_INT32, "set_backgnd", "Set bgcolor to transparentColor after loading"},
    {PARAM_INT32, "gen_hampal", "Generate palette when loading HAM pictures"}
  };
  static GimpParamDef load_return_vals[] =
  {
    {PARAM_IMAGE, "image", "Output image"}
  };
  static int nload_args = sizeof(load_args) / sizeof(*load_args);
  static int nload_return_vals = sizeof(load_return_vals) / sizeof(*load_return_vals);
  static GimpParamDef save_args[] =
  {
    {PARAM_INT32, "run_mode", "Interactive, non-interactive"},
    {PARAM_IMAGE, "image", "Input image"},
    {PARAM_DRAWABLE, "drawable", "Drawable to save"},
    {PARAM_STRING, "filename", "The name of the file to save"},
    {PARAM_STRING, "raw_filename", "The name entered"},
    {PARAM_FLOAT, "alpha_threshold", "Alpha cutoff threshold"},
    {PARAM_INT32, "compress", "Compress using ByteRun1"},
    {PARAM_INT32, "save_ham", "Save RGBs in HAM format"},
    {PARAM_INT32, "set_backgnd", "Make everything in background color transparent"}
  };
  static int nsave_args = sizeof(save_args) / sizeof(*save_args);

  gimp_install_procedure((char *) loadFuncID,
                         "Loads IFF-ILBM (InterLeaved BitMap) files",
                         "Currently loading of masks is disabled",
                         "Johannes Teveﬂen <j.tevessen@gmx.net>",
                         "Johannes Teveﬂen <j.tevessen@gmx.net>",
                         PLUG_IN_VERSION,
                         "<Load>/IFF",
                         NULL,
                         PROC_PLUG_IN,
                         nload_args, nload_return_vals,
                         load_args, load_return_vals
    );
  gimp_install_procedure((char *) saveFuncID,
                         "Saves IFF-ILBM (InterLeaved BitMap) files",
                         "alpha-alpha",
                         "Johannes Teveﬂen <j.tevessen@gmx.net>",
                         "Johannes Teveﬂen <j.tevessen@gmx.net>",
                         PLUG_IN_VERSION,
                         "<Save>/IFF",
  /* "RGB*,GRAY*,INDEXED" *//* INDEXED* ? */
                         "RGB*,GRAY*,INDEXED*",
                         PROC_PLUG_IN,
                         nsave_args, 0,
                         save_args, NULL
    );
  /*gimp_register_load_handler ("file_ilbm_load", "iff", "<Load>/IFF"); */
  /* Drop <xxx>/ILBM ? */
  gimp_register_magic_load_handler((char *) loadFuncID, (char *) nameExtensions, "" /*"<Load>/IFF" */ ,
                                   "0,string,FORM");
  gimp_register_save_handler((char *) saveFuncID, (char *) nameExtensions, "" /*"<Save>/IFF" */ );
}

/**** runPlugin() ****/

static void
runPlugin(char *name, int nparams, GParam * param,
          int *nreturn_vals, GParam ** return_vals)
{
  static GParam values[2];
  GRunModeType runMode;
  gint32 imageID;
  GStatusType status = STATUS_SUCCESS;

  if (VERBOSE) {
    fputs(PACKAGE" "VERSION" ("__DATE__" "__TIME__") running.\n", stdout);
  }

  runMode = (GRunModeType) param[0].data.d_int32;

  *nreturn_vals = 1;
  *return_vals = values;

  values[0].type = PARAM_STATUS;
  values[0].data.d_status = STATUS_CALLING_ERROR;

  if (!strcmp(name, loadFuncID)) {
    imageID = loadImage(param[1].data.d_string);
    if (-1 != imageID) {
      *nreturn_vals = 2;
      values[0].data.d_status = STATUS_SUCCESS;
      values[1].type = PARAM_IMAGE;
      values[1].data.d_image = imageID;
    } else {
      values[0].data.d_status = STATUS_EXECUTION_ERROR;
    }
  } else if (!strcmp(name, saveFuncID)) {
    switch (runMode) {
      case RUN_INTERACTIVE:
        gimp_get_data((char *) saveFuncID, &ilbmvals);
        if (!saveDialog())
          return;
        break;
      case RUN_NONINTERACTIVE:
        if (nparams != 5)
          status = STATUS_CALLING_ERROR;
        else if (status == STATUS_SUCCESS)
          ilbmvals.threshold = param[5].data.d_float;
        if (status == STATUS_SUCCESS && (0 /*testbounds failed */ ))
          status = STATUS_CALLING_ERROR;
        break;
      case RUN_WITH_LAST_VALS:
        gimp_get_data((char *) saveFuncID, &ilbmvals);
        break;
      default:
        break;
    }
    *nreturn_vals = 1;
    if (saveImage(param[3].data.d_string,
                  param[1].data.d_int32,
                  param[2].data.d_int32)) {
      gimp_set_data((char *) saveFuncID, &ilbmvals, sizeof(ILBMSaveVals));
      values[0].data.d_status = STATUS_SUCCESS;
    } else
      values[0].data.d_status = STATUS_EXECUTION_ERROR;
  } else {
    g_assert(FALSE);
  }
}
