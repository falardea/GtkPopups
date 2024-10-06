/*
 * Copyright (c) 2024 Deka Research and Development. All rights reserved.
 * Created by rfalardeau on 5/30/2024. 
*/
#include <gtk/gtk.h>
#include "app_globals.h"
#include "utils/logging.h"
#include "two_button_popup_overlay.h"

struct _TwoButtonPopup
{
   GtkBox         parent;
   GtkLabel       *title_label;
   GtkLabel       *message_label;
   GtkButton      *btn_accept;
   GtkButton      *btn_reject;
   void           (*user_return_callback) (GtkResponseType prompt_response);
};

G_DEFINE_TYPE(TwoButtonPopup, two_button_popup, GTK_TYPE_BOX )

static void two_button_popup_finalize( GObject *self );

void on_btn_accept_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   TwoButtonPopup *two_pop = (TwoButtonPopup *) user_data;
   (* two_pop->user_return_callback)(GTK_RESPONSE_ACCEPT);
   gtk_container_remove(GTK_CONTAINER(gtk_widget_get_parent(GTK_WIDGET(two_pop))), GTK_WIDGET(two_pop));
}

void on_btn_reject_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   TwoButtonPopup *two_pop = (TwoButtonPopup *) user_data;
   (* two_pop->user_return_callback)(GTK_RESPONSE_REJECT);
   gtk_container_remove(GTK_CONTAINER(gtk_widget_get_parent(GTK_WIDGET(two_pop))), GTK_WIDGET(two_pop));
}

static void two_button_popup_class_init(TwoButtonPopupClass *klass)
{
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = two_button_popup_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/resources/two_button_popup.glade");
   gtk_widget_class_bind_template_child(widget_class, TwoButtonPopup, title_label);
   gtk_widget_class_bind_template_child(widget_class, TwoButtonPopup, message_label);
   gtk_widget_class_bind_template_child(widget_class, TwoButtonPopup, btn_accept);
   gtk_widget_class_bind_template_child(widget_class, TwoButtonPopup, btn_reject);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_accept_clicked", (GCallback)on_btn_accept_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_reject_clicked", (GCallback)on_btn_reject_clicked);
}

static void two_button_popup_init(TwoButtonPopup *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

GtkWidget *two_button_popup_new(const gchar *title, const gchar *msg,
                                const gchar *affirm_btn_text, const gchar *reject_btn_text,
                                TwoButtonResponseCallback_T return_callback)
{
   TwoButtonPopup *pop;
   pop = g_object_new(TWO_BUTTON_TYPE_POPUP, NULL);

   gtk_label_set_label(GTK_LABEL(pop->title_label), title);
   gtk_label_set_label(GTK_LABEL(pop->message_label), msg);

   gtk_button_set_label(GTK_BUTTON(pop->btn_accept), affirm_btn_text);
   gtk_button_set_label(GTK_BUTTON(pop->btn_reject), reject_btn_text);

   if (return_callback != NULL)
   {
      pop->user_return_callback = return_callback;
   }
   return GTK_WIDGET(pop);
}

static void two_button_popup_finalize( GObject *self )
{
   g_return_if_fail(self != NULL);
   g_return_if_fail(TWO_BUTTON_IS_POPUP(self));

   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   G_OBJECT_CLASS (two_button_popup_parent_class)->finalize (self);
}
