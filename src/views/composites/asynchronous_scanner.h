/**
 * @file asynchronous_scanner.h
 */

#ifndef ASYNCHRONOUS_SCANNER_H__
#define ASYNCHRONOUS_SCANNER_H__
#include <gtk/gtk.h>
#include "app_globals.h"

G_BEGIN_DECLS

#define ASYNCHRONOUS_TYPE_SCANNER (asynchronous_scanner_get_type())

G_DECLARE_FINAL_TYPE(AsynchronousScanner, asynchronous_scanner, ASYNCHRONOUS, SCANNER, GtkBox)

typedef void (*OnAsyncScannerItemCallback_T)(gchar *button_index);

GtkWidget* asynchronous_scanner_new(OnAsyncScannerItemCallback_T on_async_scanner_item_callback);

G_END_DECLS
#endif  // ASYNCHRONOUS_SCANNER_H__
