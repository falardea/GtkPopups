/**
 * @file password_prompt.c
 */
#include <gtk/gtk.h>
#include "utils/logging.h"
#include "validated_prompt.h"

struct _ValidatedPrompt
{
   GtkBox         parent;
   GtkBox         *form_content;

   GtkButton      *btn_validate_and_submit;
   GtkButton      *btn_cancel;
   gboolean       (*validate_and_submit_callback) (GtkWidget *content_to_validate);
};

G_DEFINE_TYPE(ValidatedPrompt, validated_prompt, GTK_TYPE_BOX)

static void validated_prompt_finalize( GObject *self );

static void on_btn_validate_and_submit_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   ValidatedPrompt *self = VALIDATED_PROMPT(user_data);
   gboolean valid_input = (* self->validate_and_submit_callback)(NULL);

   if (valid_input)
   {
      // What if the "validation_and_submit_callback" destroys "self"?
      gtk_style_context_remove_class(gtk_widget_get_style_context(GTK_WIDGET(self->form_content)), "error");
      gtk_container_remove(GTK_CONTAINER(gtk_widget_get_parent(GTK_WIDGET(self))), GTK_WIDGET(self));
   }
   else
   {
      gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(self->form_content)), "error");
      logging_llprintf(LOGLEVEL_DEBUG, "%s: Invalid input", __func__ );
   }
}

static void on_btn_cancel_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   ValidatedPrompt *self = VALIDATED_PROMPT(user_data);
   /** (* self->validate_and_submit_callback)(GTK_RESPONSE_REJECT); */
   gtk_container_remove(GTK_CONTAINER(gtk_widget_get_parent(GTK_WIDGET(self))), GTK_WIDGET(self));
}

static void validated_prompt_class_init(ValidatedPromptClass *klass)
{
   GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = validated_prompt_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/resources/validated_entry.glade");

   gtk_widget_class_bind_template_child(widget_class, ValidatedPrompt, btn_validate_and_submit);
   gtk_widget_class_bind_template_child(widget_class, ValidatedPrompt, btn_cancel);

   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_validate_and_submit_clicked", (GCallback)on_btn_validate_and_submit_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_reject_clicked", (GCallback)on_btn_cancel_clicked);
}

static void validated_prompt_init(ValidatedPrompt *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

GtkWidget* validated_prompt_new(GtkWidget *form_content,
                                const gchar *btn_validate_and_submit_text,
                                const gchar *btn_cancel_text,
                                ValidateContentCallback_T return_callback)
{
   ValidatedPrompt *self;
   self = g_object_new(VALIDATED_TYPE_PROMPT, NULL);

   gtk_button_set_label(GTK_BUTTON(self->btn_validate_and_submit), btn_validate_and_submit_text);
   gtk_button_set_label(GTK_BUTTON(self->btn_cancel), btn_cancel_text);

   if (return_callback != NULL)
   {
      self->validate_and_submit_callback = return_callback;
   }
   return GTK_WIDGET(self);
}

static void validated_prompt_finalize( GObject *self )
{
   g_return_if_fail(self != NULL);
   g_return_if_fail(VALIDATED_IS_PROMPT(self));

   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   G_OBJECT_CLASS (validated_prompt_parent_class)->finalize (self);
}

