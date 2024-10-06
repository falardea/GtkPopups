/**
 * Created by french on 10/5/24.
 * @brief
 */
#include <gtk/gtk.h>
#include "app_globals.h"
#include "one_button_acknowledge.h"
#include "utils/logging.h"

struct _OneButtonAcknowledge
{
   GtkBox         parent;
   GtkLabel       *title_label;
   GtkLabel       *message_label;
   GtkButton      *btn_acknowledge;
   void           (*user_return_callback) (GtkResponseType acknowledge_response);
};

G_DEFINE_TYPE(OneButtonAcknowledge, one_button_acknowledge, GTK_TYPE_BOX )

static void one_button_acknowledge_finalize( GObject *self );

void on_btn_acknowledge_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   OneButtonAcknowledge *ack = (OneButtonAcknowledge *) user_data;
   (* ack->user_return_callback)(GTK_RESPONSE_ACCEPT);
   gtk_container_remove(GTK_CONTAINER(gtk_widget_get_parent(GTK_WIDGET(ack))), GTK_WIDGET(ack));
}

static void one_button_acknowledge_class_init(OneButtonAcknowledgeClass *klass)
{
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = one_button_acknowledge_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/resources/one_button_acknowledge.glade");
   gtk_widget_class_bind_template_child(widget_class, OneButtonAcknowledge, title_label);
   gtk_widget_class_bind_template_child(widget_class, OneButtonAcknowledge, message_label);
   gtk_widget_class_bind_template_child(widget_class, OneButtonAcknowledge, btn_acknowledge);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_acknowledge_clicked", (GCallback)on_btn_acknowledge_clicked);
}

static void one_button_acknowledge_init(OneButtonAcknowledge *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

GtkWidget *one_button_acknowledge_new(const gchar *title, const gchar *msg,
                                      const gchar *acknowledge_btn_text,
                                      OneButtonResponseCallback_T return_callback)
{
   OneButtonAcknowledge *pop;
   pop = g_object_new(ONE_BUTTON_TYPE_ACKNOWLEDGE, NULL);

   gtk_label_set_label(GTK_LABEL(pop->title_label), title);
   gtk_label_set_label(GTK_LABEL(pop->message_label), msg);

   gtk_button_set_label(GTK_BUTTON(pop->btn_acknowledge), acknowledge_btn_text);

   if (return_callback != NULL)
   {
      pop->user_return_callback = return_callback;
   }
   return GTK_WIDGET(pop);
}

static void one_button_acknowledge_finalize( GObject *self )
{
   g_return_if_fail(self != NULL);
   g_return_if_fail(ONE_BUTTON_IS_ACKNOWLEDGE(self));

   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   G_OBJECT_CLASS (one_button_acknowledge_parent_class)->finalize (self);
}
