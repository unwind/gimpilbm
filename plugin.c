#include <gtk/gtk.h>
#include <libgimp/gimp.h>

#include "ilbm.h"
#include "gui.h"
#include "plugin.h"

#include <string.h>

/**** Prototypes ****/

static void queryPlugin(void);
static void runPlugin(const gchar *name, gint nparams, const GimpParam * param, gint *nreturn_vals, GimpParam **return_vals);

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

/**** standard settings ****/

const gchar *loadFuncID = "file_ilbm_load";
const gchar *saveFuncID = "file_ilbm_save";
const gchar *nameExtensions = "ilbm,iff,lbm,acb,ham";

/**** main() ****/

MAIN ()

/**** queryPlugin() ****/

static void queryPlugin(void)
{
	static GimpParamDef load_args[] =
	{
		{GIMP_PDB_INT32, "run_mode", "Interactive, non-interactive"},
		{GIMP_PDB_STRING, "filename", "The name of the file to load"},
		{GIMP_PDB_STRING, "raw_filename", "The name entered"},
		{GIMP_PDB_INT32, "gen_gradients", "Build gradients from DRNG chunks"},
		{GIMP_PDB_INT32, "set_backgnd", "Set bgcolor to transparentColor after loading"},
		{GIMP_PDB_INT32, "gen_hampal", "Generate palette when loading HAM pictures"}
	};
	static GimpParamDef load_return_vals[] =
	{
		{GIMP_PDB_IMAGE, "image", "Output image"}
	};
	static const gsize nload_args = sizeof load_args / sizeof *load_args;
	static const gsize nload_return_vals = sizeof load_return_vals / sizeof *load_return_vals;
	static GimpParamDef save_args[] =
	{
		{GIMP_PDB_INT32, "run_mode", "Interactive, non-interactive"},
		{GIMP_PDB_IMAGE, "image", "Input image"},
		{GIMP_PDB_DRAWABLE, "drawable", "Drawable to save"},
		{GIMP_PDB_STRING, "filename", "The name of the file to save"},
		{GIMP_PDB_STRING, "raw_filename", "The name entered"},
		{GIMP_PDB_FLOAT, "alpha_threshold", "Alpha cutoff threshold"},
		{GIMP_PDB_INT32, "compress", "Compress using ByteRun1"},
		{GIMP_PDB_INT32, "save_ham", "Save RGBs in HAM format"},
		{GIMP_PDB_INT32, "set_backgnd", "Make everything in background color transparent"}
	};
	static const gsize nsave_args = sizeof save_args / sizeof *save_args;

	static const gchar *authors = "Johannes Teve" "\xc3\x9f" "en <j.tevessen@gmx.net>,\nEmil Brink <emil@obsession.se>";

	gimp_install_procedure(loadFuncID, "Loads IFF-ILBM (InterLeaved BitMap) files", "Currently loading of masks is disabled",
			 authors, authors,
			 PLUG_IN_VERSION,
			 "<Load>/IFF",
			 NULL,
			 GIMP_PLUGIN,
			 nload_args, nload_return_vals,
			 load_args, load_return_vals);
	gimp_install_procedure(saveFuncID, "Saves IFF-ILBM (InterLeaved BitMap) files", "alpha-alpha",
			 authors, authors,
			 PLUG_IN_VERSION,
			 "<Save>/IFF",
			 "RGB*,GRAY*,INDEXED*",
			 GIMP_PLUGIN,
			 nsave_args, 0,
			 save_args, NULL);
	/* Drop <xxx>/ILBM ? */
	gimp_register_magic_load_handler(loadFuncID, nameExtensions, "" /*"<Load>/IFF" */, "0,string,FORM");
	gimp_register_save_handler(saveFuncID, nameExtensions, "" /*"<Save>/IFF" */);
}

/**** runPlugin() ****/

static void runPlugin(const gchar *name, gint nparams, const GimpParam * param, gint *nreturn_vals, GimpParam **return_vals)
{
	static GimpParam	values[2];
	GimpRunMode		runMode;
	gint32			imageID;
	GimpPDBStatusType	status = GIMP_PDB_SUCCESS;

	if(VERBOSE)
		fputs("gimpilbm " VERSION " ("__DATE__" "__TIME__") running.\n", stdout);

	runMode = (GimpRunMode) param[0].data.d_int32;

	*nreturn_vals = 1;
	*return_vals = values;

	values[0].type = GIMP_PDB_STATUS;
	values[0].data.d_status = GIMP_PDB_CALLING_ERROR;

	if(strcmp(name, loadFuncID) == 0)
	{
		imageID = loadImage(param[1].data.d_string);
		if(-1 != imageID)
		{
			*nreturn_vals = 2;
			values[0].data.d_status = GIMP_PDB_SUCCESS;
			values[1].type = GIMP_PDB_IMAGE;
			values[1].data.d_image = imageID;
		}
		else
		{
			values[0].data.d_status = GIMP_PDB_EXECUTION_ERROR;
		}
	}
	else if(strcmp(name, saveFuncID) == 0)
	{
		switch(runMode)
		{
		case GIMP_RUN_INTERACTIVE:
			gimp_get_data(saveFuncID, &ilbmvals);
			if(!saveDialog())
				return;
			break;
		case GIMP_RUN_NONINTERACTIVE:
			if(nparams != 5)
				status = GIMP_PDB_CALLING_ERROR;
			else if(status == GIMP_PDB_SUCCESS)
				ilbmvals.threshold = param[5].data.d_float;
			if(status == GIMP_PDB_SUCCESS && (0 /*testbounds failed */ ))
				status = GIMP_PDB_CALLING_ERROR;
			break;
		case GIMP_RUN_WITH_LAST_VALS:
			gimp_get_data(saveFuncID, &ilbmvals);
			break;
		}
		*nreturn_vals = 1;
		if(saveImage(param[3].data.d_string, param[1].data.d_int32, param[2].data.d_int32))
		{
			gimp_set_data(saveFuncID, &ilbmvals, sizeof ilbmvals);
			values[0].data.d_status = GIMP_PDB_SUCCESS;
		}
		else
			values[0].data.d_status = GIMP_PDB_EXECUTION_ERROR;
	}
	else
		g_assert(FALSE);
}
