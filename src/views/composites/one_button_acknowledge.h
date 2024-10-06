/**
 * Created by french on 10/5/24.
 * @brief
 */
#ifndef ONE_BUTTON_ACKNOWLEDGE_H__
#define ONE_BUTTON_ACKNOWLEDGE_H__
#include "app_globals.h"
G_BEGIN_DECLS

#define ONE_BUTTON_TYPE_ACKNOWLEDGE             ( one_button_acknowledge_get_type () )
G_DECLARE_FINAL_TYPE(OneButtonAcknowledge, one_button_acknowledge, ONE_BUTTON, ACKNOWLEDGE, GtkBox )

typedef void (*OneButtonResponseCallback_T)(GtkResponseType acknowledge_response);

GtkWidget*        one_button_acknowledge_new  (const gchar *title, const gchar *msg,
                                              const gchar *acknowledge_btn_text,
                                              OneButtonResponseCallback_T return_callback);

G_END_DECLS
#endif  // ONE_BUTTON_ACKNOWLEDGE_H__
