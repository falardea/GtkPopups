/**
 * Created by french on 10/5/24.
 * @brief
 */
#include <gtk/gtk.h>
#include "app_globals.h"
#include "utils/logging.h"
#include "validated_entry.h"

struct _ValidatedEntry
{
   GtkBox         parent;
   GtkLabel       *title_label;
   GtkLabel       *message_label;

   GtkLabel       *entry_label;
   GtkEntry       *entry_to_validate;
   GtkCheckButton *chckbtn_modify_override;

   GtkButton      *btn_validate_and_submit;
   GtkButton      *btn_cancel;
   gboolean       (*validate_and_submit_callback) (const gchar *text_to_validate);
   gchar          *default_text_fallback;
};

G_DEFINE_TYPE(ValidatedEntry, validated_entry, GTK_TYPE_BOX )

static void validated_entry_finalize( GObject *self );

void on_chckbtn_modify_override_toggled(GtkToggleButton *opt_switch, gpointer user_data)
{
   ValidatedEntry *self = VALIDATED_ENTRY(user_data);
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   logging_llprintf(LOGLEVEL_INFO, "%s: %s", __func__, self->chckbtn_modify_override ? "modify-ing/overriding" : "disabled entry");

   gboolean mod_over = gtk_toggle_button_get_active(opt_switch);

   gtk_widget_set_sensitive(GTK_WIDGET(self->entry_to_validate), mod_over);
}

void on_btn_validate_and_submit_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   ValidatedEntry *self = VALIDATED_ENTRY(user_data);
   gboolean valid_input = (* self->validate_and_submit_callback)(gtk_entry_get_text(self->entry_to_validate));

   if (valid_input)
   {
      // What if the "validation_and_submit_callback" destroys "self"?
      gtk_style_context_remove_class(gtk_widget_get_style_context(GTK_WIDGET(self->entry_to_validate)), "error");
      gtk_container_remove(GTK_CONTAINER(gtk_widget_get_parent(GTK_WIDGET(self))), GTK_WIDGET(self));
   }
   else
   {
      gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(self->entry_to_validate)), "error");
      logging_llprintf(LOGLEVEL_DEBUG, "%s: Invalid input", __func__ );
   }
}

void on_btn_cancel_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   ValidatedEntry *self = VALIDATED_ENTRY(user_data);
   /** (* self->validate_and_submit_callback)(GTK_RESPONSE_REJECT); */
   gtk_container_remove(GTK_CONTAINER(gtk_widget_get_parent(GTK_WIDGET(self))), GTK_WIDGET(self));
}

static void validated_entry_class_init(ValidatedEntryClass *klass)
{
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = validated_entry_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/resources/validated_entry.glade");
   gtk_widget_class_bind_template_child(widget_class, ValidatedEntry, title_label);
   gtk_widget_class_bind_template_child(widget_class, ValidatedEntry, message_label);

   // This could be a component/composite interface
   gtk_widget_class_bind_template_child(widget_class, ValidatedEntry, entry_label);
   gtk_widget_class_bind_template_child(widget_class, ValidatedEntry, entry_to_validate);
   gtk_widget_class_bind_template_child(widget_class, ValidatedEntry, chckbtn_modify_override);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_chckbtn_modify_override_toggled", (GCallback)on_chckbtn_modify_override_toggled);

   gtk_widget_class_bind_template_child(widget_class, ValidatedEntry, btn_validate_and_submit);
   gtk_widget_class_bind_template_child(widget_class, ValidatedEntry, btn_cancel);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_validate_and_submit_clicked", (GCallback)on_btn_validate_and_submit_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_reject_clicked", (GCallback)on_btn_cancel_clicked);
}

static void validated_entry_init(ValidatedEntry *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

GtkWidget *validated_entry_new(const gchar *title, const gchar *msg,
                               const gchar *entry_label_text,
                               const gchar *default_entry_text,
                               const gchar *modify_override_text,
                                const gchar *btn_validate_and_submit_text, const gchar *btn_cancel_text,
                               ValidateFormCallback_T return_callback)
{
   ValidatedEntry *self;
   self = g_object_new(VALIDATED_TYPE_ENTRY, NULL);

   gtk_label_set_label(GTK_LABEL(self->title_label), title);
   gtk_label_set_label(GTK_LABEL(self->message_label), msg);

   gtk_label_set_label(GTK_LABEL(self->entry_label), entry_label_text);
   gtk_entry_set_text(GTK_ENTRY(self->entry_to_validate), default_entry_text);
   gtk_button_set_label(GTK_BUTTON(self->chckbtn_modify_override), modify_override_text);

   gtk_button_set_label(GTK_BUTTON(self->btn_validate_and_submit), btn_validate_and_submit_text);
   gtk_button_set_label(GTK_BUTTON(self->btn_cancel), btn_cancel_text);

   if (return_callback != NULL)
   {
      self->validate_and_submit_callback = return_callback;
   }
   return GTK_WIDGET(self);
}

static void validated_entry_finalize( GObject *self )
{
   g_return_if_fail(self != NULL);
   g_return_if_fail(VALIDATED_IS_ENTRY(self));

   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   G_OBJECT_CLASS (validated_entry_parent_class)->finalize (self);
}
