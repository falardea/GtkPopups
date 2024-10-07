/*
 * Created by rfalardeau on 5/30/2024.
*/
#ifndef TWO_BUTTON_POPUP_OVERLAY_H__
#define TWO_BUTTON_POPUP_OVERLAY_H__
#include "app_globals.h"
G_BEGIN_DECLS

#define TWO_BUTTON_TYPE_POPUP             ( two_button_popup_get_type () )
G_DECLARE_FINAL_TYPE(TwoButtonPopup, two_button_popup, TWO_BUTTON, POPUP, GtkBox )

typedef void (*TwoButtonResponseCallback_T)(GtkResponseType prompt_response);

GtkWidget*        two_button_popup_new       (const gchar *title, const gchar *msg,
                                              const gchar *affirm_btn_text, const gchar *reject_btn_text,
                                              TwoButtonResponseCallback_T return_callback);

G_END_DECLS
#endif  // TWO_BUTTON_POPUP_OVERLAY_H__
