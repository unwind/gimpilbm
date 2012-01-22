#include "plugin.h"
#include "gui.h"

#include <gtk/gtk.h>
#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

static void saveCloseCallback(GtkWidget *widget, gpointer data)
{
	gtk_main_quit();
}

static void saveOkCallback(GtkWidget *widget, gpointer data)
{
	ilbmint.run = TRUE;
	gtk_widget_destroy(GTK_WIDGET(data));
}

static void saveScaleUpdate(GtkAdjustment *adjustment, gdouble *scale_val)
{
	*scale_val = adjustment->value;
}


static void saveToggleUpdate(GtkWidget *widget, gint32 *data)
{
	*data = GTK_TOGGLE_BUTTON(widget)->active;
}

#define SCALE_WIDTH	125

static GtkWidget * new_vbx_compr(void)
{
	GtkWidget	*vbx_compr, *rad_compr;
	GSList		*gsl_compr;

	vbx_compr = gtk_vbox_new(FALSE, 0);
	gtk_container_border_width(GTK_CONTAINER(vbx_compr), 10);
	gtk_widget_show(vbx_compr);

	rad_compr = gtk_radio_button_new_with_label(NULL, "None");
	gtk_box_pack_start(GTK_BOX(vbx_compr), rad_compr, FALSE, TRUE, 0);
	gtk_widget_show(rad_compr);

	gsl_compr = gtk_radio_button_group(GTK_RADIO_BUTTON(rad_compr));
	rad_compr = gtk_radio_button_new_with_label(gsl_compr, "ByteRun1");
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(rad_compr), TRUE);
	gtk_box_pack_start(GTK_BOX(vbx_compr), rad_compr, FALSE, TRUE, 0);
	gtk_widget_show(rad_compr);

	return vbx_compr;
}

gint saveDialog(void)
{
	GtkWidget	*dlg;
	GtkWidget	*frame;
	GtkWidget	*table;
	GtkWidget	*vbx_compr;
	GtkWidget	*label;
	GtkObject	*scale_data;
	GtkWidget	*scale;
	GtkWidget	*toggle;

	gimp_ui_init("gimpilbm", FALSE);

	dlg = gimp_dialog_new("Save as IFF", "iff",
				 NULL,
				 0,
				 gimp_standard_help_func, "filters/iff.html",
				 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				 GTK_STOCK_OK, GTK_RESPONSE_OK,
				 NULL);

	/**** "dialogue" part ****/

	/*  parameter settings  */
	frame = gtk_frame_new("Parameter Settings");
	gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_ETCHED_IN);
	gtk_container_border_width(GTK_CONTAINER(frame), 10);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dlg)->vbox), frame, TRUE, TRUE, 0);
	table = gtk_table_new(3, 2, FALSE);
	gtk_container_border_width(GTK_CONTAINER(table), 10);
	gtk_container_add(GTK_CONTAINER(frame), table);

	/**** Alpha treshold ****/
	label = gtk_label_new("Alpha Treshold");
	gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
	gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1, GTK_FILL | GTK_EXPAND, GTK_FILL, 5, 0);
	scale_data = gtk_adjustment_new(ilbmvals.threshold, 0.0, 1.0, 0.01, 0.01, 0.0);
	scale = gtk_hscale_new(GTK_ADJUSTMENT(scale_data));
	gtk_widget_set_usize(scale, SCALE_WIDTH, 0);
	gtk_table_attach(GTK_TABLE(table), scale, 1, 2, 0, 1, GTK_FILL | GTK_EXPAND, GTK_FILL, 0, 0);
	gtk_scale_set_value_pos(GTK_SCALE(scale), GTK_POS_RIGHT);
	gtk_scale_set_digits(GTK_SCALE(scale), 2);
	gtk_range_set_update_policy(GTK_RANGE(scale), GTK_UPDATE_DELAYED);
	g_signal_connect(G_OBJECT(scale_data), "value_changed", G_CALLBACK(saveScaleUpdate), &ilbmvals.threshold);

	/**** Compress ****/
	toggle = gtk_check_button_new_with_label("Compress");
	gtk_table_attach(GTK_TABLE(table), toggle, 0, 2, 2, 3, GTK_FILL, 0, 0, 0);
	g_signal_connect(G_OBJECT(toggle), "toggled", G_CALLBACK(saveToggleUpdate), &ilbmvals.compress);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(toggle), ilbmvals.compress);

	/**** Save as HAM ****/
	toggle = gtk_check_button_new_with_label("Save as HAM");
	gtk_table_attach(GTK_TABLE(table), toggle, 0, 2, 3, 4, GTK_FILL, 0, 0, 0);
	g_signal_connect(G_OBJECT(toggle), "toggled", G_CALLBACK(saveToggleUpdate), &ilbmvals.save_ham);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(toggle), ilbmvals.save_ham);

	/**** Save chunky ****/
	toggle = gtk_check_button_new_with_label("Save chunky (RGB8)");
	gtk_table_attach(GTK_TABLE(table), toggle, 0, 2, 4, 5, GTK_FILL, 0, 0, 0);
	g_signal_connect(G_OBJECT(toggle), "toggled", G_CALLBACK(saveToggleUpdate), &ilbmvals.save_chunky);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(toggle), ilbmvals.save_chunky);

#if 0
	/****  ****/
	vbx_compr = new_vbx_compr();
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dlg)->vbox), vbx_compr, TRUE, TRUE, 0);
#endif

	gtk_widget_show_all(dlg);
	ilbmint.run = gimp_dialog_run(GIMP_DIALOG(dlg)) == GTK_RESPONSE_OK;

	return ilbmint.run;
}

static gint xsaveDialog(void)
{
	GtkWidget *dlg;
	GtkWidget *label;
	GtkWidget *button;
	GtkWidget *scale;
	GtkWidget *frame;
	GtkWidget *table;
	GtkObject *scaleData;

	gint argc = 1;
	gchar **argv = g_new(gchar *, 2);
	*argv = g_strdup(saveFuncID);
	argv[1] = 0;
	gtk_init(&argc, &argv);

	dlg = gtk_dialog_new();
	gtk_window_set_title(GTK_WINDOW(dlg), "Save as IFF-ILBM");
	gtk_window_position(GTK_WINDOW(dlg), GTK_WIN_POS_MOUSE);
	gtk_signal_connect(GTK_OBJECT(dlg), "destroy", (GtkSignalFunc) saveCloseCallback, NULL);
  
  /* Action! */

	button = gtk_button_new_with_label("OK");
	GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
	gtk_signal_connect(GTK_OBJECT(button), "clicked", (GtkSignalFunc) saveOkCallback, dlg);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dlg)->action_area), button, TRUE, TRUE, 0);
	gtk_widget_grab_default(button);
	gtk_widget_show(button);

	button = gtk_button_new_with_label("Cancel");
	GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
	gtk_signal_connect(GTK_OBJECT(button), "clicked", (GtkSignalFunc) gtk_widget_destroy, GTK_OBJECT(dlg));
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dlg)->action_area), button, TRUE, TRUE, 0);
	gtk_widget_show(button);

  /* Params */

	frame = gtk_frame_new("Parameter Settings");
	gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_ETCHED_IN);
	gtk_container_border_width(GTK_CONTAINER(frame), 10);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dlg)->vbox), frame, TRUE, TRUE, 0);

	table = gtk_table_new(1, 2, FALSE);
	gtk_container_border_width(GTK_CONTAINER(table), 10);
	gtk_container_add(GTK_CONTAINER(frame), table);

	label = gtk_label_new("Alpha Threshold");
	gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
	gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1, GTK_FILL | GTK_EXPAND, GTK_FILL, 5, 0);

	scaleData = gtk_adjustment_new(ilbmvals.threshold, 0.0, 1.0, 0.1, 0.01, 0.0);
	scale = gtk_hscale_new(GTK_ADJUSTMENT(scaleData));
	gtk_widget_set_usize(scale, SCALE_WIDTH, 0);
	gtk_table_attach(GTK_TABLE(table), scale, 1, 2, 0, 1, GTK_FILL | GTK_EXPAND, GTK_FILL, 0, 0);
	gtk_scale_set_value_pos(GTK_SCALE(scale), GTK_POS_TOP);
	gtk_scale_set_digits(GTK_SCALE(scale), 2);
	gtk_range_set_update_policy(GTK_RANGE(scale), GTK_UPDATE_DELAYED);
	gtk_signal_connect(GTK_OBJECT(scaleData), "value_changed", (GtkSignalFunc) saveScaleUpdate, &ilbmvals.threshold);

	printf("showing dialog\n");
	gtk_widget_show_all(dlg);

	gtk_main();
	gdk_flush();

	/* Cleanup */
	g_free(*argv);
	g_free(argv);

	return ilbmint.run;
}
