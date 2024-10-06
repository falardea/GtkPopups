/**
 * @brief The implementation
 */
#include <gtk/gtk.h>
#include "app_globals.h"
#include "ui_builder.h"
#include "utils/logging.h"

static void bind_widgets_src_active_tar_sensitive(GtkWidget *src, GtkWidget *tar)
{
   g_object_bind_property(src, "active", tar, "sensitive", G_BINDING_DEFAULT);
}

app_widget_ref_struct *get_app_widgets_pointer(void)
{
   return g_app_widget_refs;
}

app_widget_ref_struct *app_builder(void) {
   GtkBuilder *builder;

   app_widget_ref_struct *appWidgetsT = g_slice_new(app_widget_ref_struct);

   builder = gtk_builder_new();

   if (gtk_builder_add_from_resource(builder, "/resource_path/resources/popup_samples.glade", NULL) == 0) {
      logging_llprintf(LOGLEVEL_ERROR, "failed to load glade resource");
      return NULL;
   }

   appWidgetsT->w_msg_out_textview = GTK_WIDGET(gtk_builder_get_object(builder, "msg_out_textview"));
   appWidgetsT->w_say_something_entry = GTK_WIDGET(gtk_builder_get_object(builder, "say_something_entry"));
   appWidgetsT->w_home_page_overlay = GTK_WIDGET(gtk_builder_get_object(builder, "home_page_overlay"));
   appWidgetsT->w_btn_acknowledge = GTK_WIDGET(gtk_builder_get_object(builder, "btn_acknowledge"));
   appWidgetsT->w_config_option_on_off = GTK_WIDGET(gtk_builder_get_object(builder, "config_option_on_off"));
   appWidgetsT->w_btn_get_config_type_bool = GTK_WIDGET(gtk_builder_get_object(builder, "btn_get_config_type_bool"));

   appWidgetsT->w_lbl_input_from_entry_popup = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_input_from_entry_popup"));
   bind_widgets_src_active_tar_sensitive(GTK_WIDGET(appWidgetsT->w_config_option_on_off), GTK_WIDGET(appWidgetsT->w_btn_get_config_type_bool));

   gtk_builder_connect_signals(builder, appWidgetsT);

   g_object_unref(builder);
   return appWidgetsT;
}
