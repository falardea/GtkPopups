/**
 * @file screen_cleaner.c
 */
#include "screen_cleaner.h"

struct _ScreenCleaner
{
   GtkBox      parent;
   GtkLabel    *countdown_label;
   gint        countdown;
};

G_DEFINE_TYPE(ScreenCleaner, screen_cleaner, GTK_TYPE_BOX)

static void screen_cleaner_finalize(GObject *self);

static gboolean updateTouchscreenPauseLabel(gpointer user_data);

void on_screen_cleaner_shown(GtkWidget *self)
{
   ScreenCleaner *cleaner = SCREEN_CLEANER(self);
   // Start a countdown
   gdk_threads_add_timeout_seconds(1, (GSourceFunc)updateTouchscreenPauseLabel, (gpointer)cleaner);
}

static void screen_cleaner_class_init(ScreenCleanerClass *klass)
{
   GObjectClass   *gobject_class = G_OBJECT_CLASS(klass);
   GtkWidgetClass *widget_class = (GtkWidgetClass *) klass;

   gobject_class->finalize = screen_cleaner_finalize;

   gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(klass), "/resource_path/resources/screen_cleaner.glade");
   gtk_widget_class_bind_template_child(widget_class, ScreenCleaner, countdown_label);
}

static void screen_cleaner_init(ScreenCleaner *self)
{
   gtk_widget_init_template(GTK_WIDGET(self));
}

GtkWidget* screen_cleaner_new(gint countdown)
{
   ScreenCleaner *cleaner;
   cleaner = g_object_new(SCREEN_TYPE_CLEANER, NULL);

   cleaner->countdown = countdown;

   char buf[64];
   snprintf(buf, sizeof(buf), "%d", cleaner->countdown);
   gtk_label_set_text(GTK_LABEL(cleaner->countdown_label), buf);

   g_signal_connect(G_OBJECT(cleaner), "realize", G_CALLBACK(on_screen_cleaner_shown), (gpointer)cleaner);
   return GTK_WIDGET(cleaner);
}

static void screen_cleaner_finalize(GObject *self)
{
   g_return_if_fail(self != NULL);
   g_return_if_fail(SCREEN_IS_CLEANER(self));

   G_OBJECT_CLASS (screen_cleaner_parent_class)->finalize (self);
}


static gboolean updateTouchscreenPauseLabel(gpointer user_data)
{
   ScreenCleaner *self = SCREEN_CLEANER(user_data);
   self->countdown--;

   if (self->countdown > 0)
   {
      // Timeout continuing, update the countdown text
      if (GTK_IS_LABEL(self->countdown_label))
      {
         char buf[64];
         snprintf(buf, sizeof(buf), "%d", self->countdown);
         gtk_label_set_text(GTK_LABEL(self->countdown_label), buf);
      }
      else
      {
         g_print("%s: self->countdown_label is not a label\n", __func__);
      }
      return G_SOURCE_CONTINUE;
   }
   else
   {
      gtk_widget_destroy(GTK_WIDGET(self));
      return G_SOURCE_REMOVE;
   }
}
