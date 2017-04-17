#include "plugin.h"
#include "gui.h"

#include <gtk/gtk.h>
#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

static void saveScaleUpdate(GtkAdjustment *adjustment, gdouble *scale_val)
{
	*scale_val = adjustment->value;
}

static void saveToggleUpdate(GtkWidget *widget, gint32 *data)
{
	*data = GTK_TOGGLE_BUTTON(widget)->active;
}

#define SCALE_WIDTH	125

gint saveDialog(void)
{
	GtkWidget	*dlg;
	GtkWidget	*frame;
	GtkWidget	*table;
	GtkWidget	*label;
	GtkObject	*scale_data;
	GtkWidget	*scale;
	GtkWidget	*toggle;
	gint		ok;

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
	label = gtk_label_new("Alpha Threshold");
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

#if defined DEVMODE && DEVMODE == 1
	/**** Save as HAM ****/
	toggle = gtk_check_button_new_with_label("Save as HAM");
	gtk_table_attach(GTK_TABLE(table), toggle, 0, 2, 3, 4, GTK_FILL, 0, 0, 0);
	g_signal_connect(G_OBJECT(toggle), "toggled", G_CALLBACK(saveToggleUpdate), &ilbmvals.save_ham);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(toggle), ilbmvals.save_ham);
#endif

	/**** Save chunky ****/
	toggle = gtk_check_button_new_with_label("Save chunky (RGB8)");
	gtk_table_attach(GTK_TABLE(table), toggle, 0, 2, 4, 5, GTK_FILL, 0, 0, 0);
	g_signal_connect(G_OBJECT(toggle), "toggled", G_CALLBACK(saveToggleUpdate), &ilbmvals.save_chunky);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(toggle), ilbmvals.save_chunky);

	gtk_widget_show_all(dlg);

	ok = gimp_dialog_run(GIMP_DIALOG(dlg)) == GTK_RESPONSE_OK;

	return ok;
}
