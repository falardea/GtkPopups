/**
 * @brief A place for globals
 */
#ifndef APP_GLOBALS_H__
#define APP_GLOBALS_H__
#include <gtk/gtk.h>

#define DEFAULT_TIMESTAMP_LOG_FORMAT "%04d-%02d-%02dT%02d:%02d:%02d"

typedef enum {
   RVALUE_SUCCESS,
   RVALUE_ERROR
} RVALUE;

typedef enum {
   LOGLEVEL_DEBUG,
   LOGLEVEL_INFO,
   LOGLEVEL_ERROR
} LOGLEVEL;

extern const char* DEBUG_STR;
extern const char* INFO_STR;
extern const char* ERROR_STR;
extern const int   LOGGING_MAX_MSG_LENGTH;

typedef struct {
   GtkWidget   *w_msg_out_textview;
   GtkWidget   *w_say_something_entry;
   GtkWidget   *w_home_page_overlay;
   GtkWidget   *w_btn_acknowledge;
   GtkWidget   *w_config_option_on_off;
   GtkWidget   *w_btn_get_config_type_bool;
   GtkWidget   *w_lbl_input_from_entry_popup;
   GtkWidget   *w_btn_screen_cleaner;
   GtkWidget   *w_btn_async_scanner;
} app_widget_ref_struct;

extern app_widget_ref_struct *g_app_widget_refs;

app_widget_ref_struct *get_app_widgets_pointer(void);

#endif  /* APP_GLOBALS_H__ */
