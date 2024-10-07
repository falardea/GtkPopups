/**
 * @file screen_cleaner.h
 */

#ifndef SCREEN_CLEANER_H__
#define SCREEN_CLEANER_H__
#include <gtk/gtk.h>
#include "app_globals.h"

G_BEGIN_DECLS

#define SCREEN_TYPE_CLEANER (screen_cleaner_get_type())

G_DECLARE_FINAL_TYPE(ScreenCleaner, screen_cleaner, SCREEN, CLEANER, GtkBox)

GtkWidget* screen_cleaner_new(gint countdown);

G_END_DECLS
#endif  // SCREEN_CLEANER_H__
