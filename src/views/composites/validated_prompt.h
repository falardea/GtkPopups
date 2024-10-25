/**
 * @file validated_prompt.h
 */
#ifndef VALIDATED_PROMPT_H__
#define VALIDATED_PROMPT_H__
#include "app_globals.h"
G_BEGIN_DECLS

#define VALIDATED_TYPE_PROMPT (validated_prompt_get_type ())
G_DECLARE_FINAL_TYPE(ValidatedPrompt, validated_prompt, VALIDATED, PROMPT, GtkBox)

typedef gboolean (*ValidateContentCallback_T)(GtkWidget *content_to_validate);

GtkWidget* validated_prompt_new(GtkWidget *form_content,
                                const gchar *btn_validate_and_submit_text,
                                const gchar *btn_cancel_text,
                                ValidateContentCallback_T return_callback);

G_END_DECLS
#endif  // VALIDATED_PROMPT_H__
