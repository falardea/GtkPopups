/**
 * Created by french on 10/5/24.
 * @brief
 */
#ifndef VALIDATED_ENTRY_H__
#define VALIDATED_ENTRY_H__
#include "app_globals.h"
G_BEGIN_DECLS

#define VALIDATED_TYPE_ENTRY             ( validated_entry_get_type () )
G_DECLARE_FINAL_TYPE(ValidatedEntry, validated_entry, VALIDATED, ENTRY, GtkBox )

typedef gboolean (*ValidateFormCallback_T)(const gchar *text_to_validate);

GtkWidget*        validated_entry_new       (const gchar *title, const gchar *msg,
                                             const gchar *entry_label_text,
                                             const gchar *default_entry_text,
                                             const gchar *modify_override_text,
                                             const gchar *btn_validate_and_submit_text, const gchar *btn_cancel_text,
                                             ValidateFormCallback_T return_callback);

G_END_DECLS
#endif  // VALIDATED_ENTRY_H__
