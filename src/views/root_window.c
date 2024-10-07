/**
 * @brief The implementation
 */
#include "root_window.h"
#include "interfaces/app_interface.h"
#include "utils/sys_interface.h"
#include "utils/logging.h"
#include "composites/two_button_popup_overlay.h"
#include "composites/one_button_acknowledge.h"
#include "composites/validated_entry.h"
#include "composites/screen_cleaner.h"
#include "composites/asynchronous_scanner.h"

#define DO_TWO 0

static const char *MSG_OUT_CURSOR_NAME = "msgOutCursor";
static GtkTextMark *msgOutCursor;
static char timestamp[20];  // not sure why it felt better to allocate the memory once

void on_main_wnd_close_clicked(__attribute__((unused)) GtkWidget *srcWidget,
                                __attribute__((unused)) gpointer uData) {
   gtk_main_quit();
}

gboolean on_main_wnd_delete_event(__attribute__((unused)) GtkWidget *srcWidget,
                                    __attribute__((unused)) GdkEvent *event,
                                    __attribute__((unused)) gpointer uData) {
   gtk_main_quit();
   return FALSE;
}

void temp_callback(GtkResponseType prompt_response)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);

   if (prompt_response == GTK_RESPONSE_ACCEPT)
   {
      logging_llprintf(LOGLEVEL_DEBUG, "%s: USER ACCEPTED", __func__);
   }
   else if (prompt_response == GTK_RESPONSE_REJECT)
   {
      logging_llprintf(LOGLEVEL_DEBUG, "%s: USER REJECTED", __func__);
   }
   else
   {
      logging_llprintf(LOGLEVEL_DEBUG, "%s: UNKNOWN RESPONSE", __func__);
   }
}

void on_do_something_button_clicked(__attribute__((unused)) GtkButton *button, gpointer *user_data)
{
   app_widget_ref_struct *wdgts = (app_widget_ref_struct *) user_data;
   if (gtk_entry_buffer_get_length(gtk_entry_get_buffer(GTK_ENTRY(wdgts->w_say_something_entry))) > 0){
      print_log_level_msgout(LOGLEVEL_INFO, "%s", gtk_entry_get_text(GTK_ENTRY(wdgts->w_say_something_entry)));
   } else {
      print_log_level_msgout(LOGLEVEL_INFO, "nothing to say?");
   }

#if DO_TWO
   GtkWidget *popup = two_button_popup_new("This is a test", "This is a test of the two button popup and flavoring it on "
                                                             "creation", "You Rock!", "You Still Rock!", (TwoButtonResponseCallback_T)temp_callback);
   gtk_overlay_add_overlay(GTK_OVERLAY(wdgts->w_home_page_overlay), popup);
   gtk_widget_show_all(popup);
#endif
}

void on_btn_info_ack_clicked(__attribute__((unused)) GtkButton *button, gpointer *user_data)
{
   app_widget_ref_struct *wdgts = (app_widget_ref_struct *) user_data;
   GtkWidget *popup = one_button_acknowledge_new("Acknowledge Info",
                                                 "This is a one button acknowledgement that can do something "
                                                 "special when acknowledged, like setting a step complete",
                                                 "OK",
                                                 (OneButtonResponseCallback_T)temp_callback);
   gtk_overlay_add_overlay(GTK_OVERLAY(wdgts->w_home_page_overlay), popup);
   gtk_widget_show_all(popup);
}

void on_btn_get_config_type_bool_clicked(__attribute__((unused)) GtkButton *button, __attribute__((unused)) gpointer *user_data)
{
   app_widget_ref_struct *wdgts = (app_widget_ref_struct *) user_data;
   gchar text_entered_by_user[2048];
   gchar build_a_msg_out[2048];

   gboolean use_entered_text = (gtk_entry_buffer_get_length(gtk_entry_get_buffer(GTK_ENTRY(wdgts->w_say_something_entry))) > 0);

   if (use_entered_text){
      snprintf(text_entered_by_user, sizeof(text_entered_by_user), "%s",
               gtk_entry_get_text(GTK_ENTRY(wdgts->w_say_something_entry)));
      print_log_level_msgout(LOGLEVEL_INFO, "%s", gtk_entry_get_text(GTK_ENTRY(wdgts->w_say_something_entry)));
   } else {
      print_log_level_msgout(LOGLEVEL_INFO, "nothing to say?");
   }


   snprintf(build_a_msg_out, sizeof(build_a_msg_out), "%s%s%s\n%s%s",
            "This is a build-a-message popup, taking some 'inputs' from the parent, such as:\n",
            "The user entered ", use_entered_text ? text_entered_by_user : "nothing in the text input\n",
            "And this popup was enabled because the Enable Config Menu is ", gtk_switch_get_active(GTK_SWITCH(wdgts->w_config_option_on_off)) ? "enabled": "somehow disabled");
   GtkWidget *popup = two_button_popup_new("Turn it up to 11!", build_a_msg_out,
                                           "Rock on!", "That's a nope!",
                                           (TwoButtonResponseCallback_T)temp_callback);
   gtk_overlay_add_overlay(GTK_OVERLAY(wdgts->w_home_page_overlay), popup);
   gtk_widget_show_all(popup);
}


gboolean validation_callback(gchar *text_to_validate)
{
   logging_llprintf(LOGLEVEL_DEBUG, "%s", __func__);
   app_widget_ref_struct *wdgts = get_app_widgets_pointer();

   if (strlen(text_to_validate) > 0 && strcmp(text_to_validate, "Abracadabra") == 0)
   {
      gtk_label_set_label(GTK_LABEL(wdgts->w_lbl_input_from_entry_popup), text_to_validate);
      return TRUE;
   }
   return FALSE;
}
void on_show_validated_entry_popup_clicked(__attribute__((unused)) GtkButton *button, __attribute__((unused)) gpointer *user_data)
{
   app_widget_ref_struct *wdgts = (app_widget_ref_struct *) user_data;
   GtkWidget *popup = validated_entry_new("Validated Input Example",
                                          "A general informational area for a form->submit type UI/UX.  This could "
                                          "also be a tree, with configuration items, configured by a boxed-type structure "
                                          "for application-ui state, may a run-state object too...",
                                          "Validate-able Input >>>",
                                          "SOME-DEFAULT-ENTRY-TEXT",
                                          "modify/override from default",
                                          "Validate Input", "Cancel",
                                          (ValidateFormCallback_T)validation_callback);
   gtk_overlay_add_overlay(GTK_OVERLAY(wdgts->w_home_page_overlay), popup);
   gtk_widget_show_all(popup);
}


void on_btn_screen_cleaner_clicked(__attribute__((unused)) GtkButton *button, gpointer *user_data)
{
   app_widget_ref_struct *wdgts = (app_widget_ref_struct *) user_data;
   GtkWidget *cleaner = screen_cleaner_new(12);
   gtk_overlay_add_overlay(GTK_OVERLAY(wdgts->w_home_page_overlay), cleaner);
   gtk_widget_show(cleaner);
}

void on_btn_async_scanner_clicked(__attribute__((unused)) GtkButton *button, gpointer *user_data)
{
   app_widget_ref_struct *wdgts = (app_widget_ref_struct *) user_data;
   GtkWidget *scanner = asynchronous_scanner_new(NULL);
   gtk_overlay_add_overlay(GTK_OVERLAY(wdgts->w_home_page_overlay), scanner);
   gtk_widget_show(scanner);
}



/////////////////////////////////////////////////////////
void set_msgout_buffer(const char *msgout)
{
   if (get_app_state() == APP_STATE_SUCCESS)
   {  /* Let's not use the MsgOut widget before it's been built */
      GtkTextBuffer  *tvBuff  =  gtk_text_view_get_buffer(GTK_TEXT_VIEW(g_app_widget_refs->w_msg_out_textview));
      GtkTextIter    endIter;
      gtk_text_buffer_get_end_iter(tvBuff, &endIter);

      if (!gtk_text_buffer_get_mark(tvBuff, MSG_OUT_CURSOR_NAME))
      {
         msgOutCursor = gtk_text_mark_new(MSG_OUT_CURSOR_NAME, FALSE);
         gtk_text_buffer_add_mark(tvBuff, msgOutCursor, &endIter);
      }

      gtk_text_buffer_insert(tvBuff, &endIter, msgout, -1);

      // This keeps the latest msgout in view, BUT if you were scrolling up and a new msgout was posted, it will
      // autoscroll to the insertion.  It would be better to only auto-scroll the msgout if the user is not looking
      // at previous messages
      gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(g_app_widget_refs->w_msg_out_textview), msgOutCursor, 0.0, TRUE, 0.0, 0.0);
   }
}

void print_log_level_msgout(LOGLEVEL loglevel, const char *_format, ...)
{
   if (loglevel >= get_app_log_level()) {
      bool use_ts = get_app_log_w_timestamp_flag();

      // We're not going to "flag" out the timestamp in the memory sizing here, because... who cares if it's too big?
      char ll_msg_out[LOGGING_MAX_MSG_LENGTH + sizeof(timestamp) + (2*sizeof(':')) + sizeof(get_log_level_str(loglevel))];
      char line_out[LOGGING_MAX_MSG_LENGTH];

      va_list arg;
      va_start(arg, _format);
      vsnprintf(line_out, sizeof (line_out), _format, arg);
      va_end(arg);

      if (use_ts)
         get_timestamp(timestamp, sizeof(timestamp));

      snprintf(ll_msg_out, sizeof (ll_msg_out), "%s%s%s:%s\n",
               use_ts ? timestamp:"", use_ts?":":"", get_log_level_str(loglevel), line_out);

      logging_llprintf(loglevel, "%s", line_out);
      set_msgout_buffer(ll_msg_out);
   }
}
