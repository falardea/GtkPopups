/**
 * @file asynchronous_scanner.c
 */
#include <gtk/gtk.h>
#include "asynchronous_scanner.h"

#define MAX_SCAN_TIMEOUT 10;

struct _AsynchronousScanner
{
   GtkBox      parent;
   GtkButton   *btn_close_or_cancel;
   gboolean    user_canceled;
   gboolean    running_scan;
   gint        countdown;
   void        (*on_async_scanner_item_callback)(gchar *button_idx);
};

G_DEFINE_TYPE(AsynchronousScanner, asynchronous_scanner, GTK_TYPE_BOX)

static void asynchronous_scanner_finalize(GObject *self);

gboolean asynchronous_scanner_timeout_callback(gpointer user_data);

void on_asynchronous_scanner_shown(__attribute__((unused)) GtkWidget *self)
{
   AsynchronousScanner *scanner = ASYNCHRONOUS_SCANNER(self);

   g_print("%s: %s\n", __func__, "Starting scan process...");

   scanner->running_scan = TRUE;

   gdk_threads_add_timeout_seconds(1, (GSourceFunc)asynchronous_scanner_timeout_callback, (gpointer)scanner);
}

static void on_btn_async_scanner_item_clicked(GtkButton *button, __attribute__((unused)) gpointer user_data)
{
   g_print("%s: Widget name/index = %s\n", __func__, gtk_widget_get_name(GTK_WIDGET(button)));
}

static void on_btn_close_or_cancel_clicked(__attribute__((unused)) GtkButton *button, gpointer user_data)
{
   AsynchronousScanner *scanner = ASYNCHRONOUS_SCANNER(user_data);

   if (scanner->running_scan)
   {
      g_print("%s: %s\n", __func__, "user canceled scan");
      scanner->user_canceled = TRUE;
   }
   else
   {
      g_print("%s: %s\n", __func__, "user closing scan dialog");
      if (GTK_IS_WIDGET(scanner) && gtk_widget_get_parent(GTK_WIDGET(scanner)) != NULL)
      {
         gtk_container_remove(GTK_CONTAINER(gtk_widget_get_parent(GTK_WIDGET(scanner))), GTK_WIDGET(scanner));
      }
      else
      {
         g_print("%s: %s\n", __func__, "Not running, but we also don't exist anymore... how?");
      }
   }

}

static void asynchronous_scanner_class_init(AsynchronousScannerClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass   *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = asynchronous_scanner_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/resources/asynchronous_scanner.glade");
   gtk_widget_class_bind_template_child(widget_class, AsynchronousScanner, btn_close_or_cancel);
   // buttons?
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_async_scanner_item_clicked", (GCallback)on_btn_async_scanner_item_clicked);
   gtk_widget_class_bind_template_callback_full(widget_class, "on_btn_close_or_cancel_clicked", (GCallback)on_btn_close_or_cancel_clicked);
}

static void asynchronous_scanner_init(AsynchronousScanner *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

GtkWidget* asynchronous_scanner_new(OnAsyncScannerItemCallback_T on_async_scanner_item_callback)
{
   AsynchronousScanner *scanner;
   scanner = g_object_new(ASYNCHRONOUS_TYPE_SCANNER, NULL);
   scanner->user_canceled = FALSE;
   scanner->running_scan = FALSE;
   scanner->countdown = MAX_SCAN_TIMEOUT;
   scanner->on_async_scanner_item_callback = on_async_scanner_item_callback;

   g_signal_connect(G_OBJECT(scanner), "realize", G_CALLBACK(on_asynchronous_scanner_shown), scanner);

   return GTK_WIDGET(scanner);
}

//void asynchronous_scanner_set_pump_button_name(char *pumpName)
//{
//
//}

static void asynchronous_scanner_finalize(GObject *self)
{
   g_print("%s\n", __func__);
   g_return_if_fail(self != NULL);
   g_return_if_fail(ASYNCHRONOUS_IS_SCANNER(self));

   G_OBJECT_CLASS (asynchronous_scanner_parent_class)->finalize (self);
}

gboolean asynchronous_scanner_timeout_callback(gpointer user_data)
{
   AsynchronousScanner *scanner = ASYNCHRONOUS_SCANNER(user_data);
   scanner->countdown--;

   if (!scanner->user_canceled && scanner->countdown > 0)
   {
      g_print("%s: %d\n", __func__, scanner->countdown);
      return G_SOURCE_CONTINUE;
   }
   else
   {
      g_print("%s: canceled or timed out\n", __func__);
      scanner->running_scan = FALSE;
      gtk_button_set_label(GTK_BUTTON(scanner->btn_close_or_cancel), "Close");
      return G_SOURCE_REMOVE;
   }
}
