/* 
 * Ardesia -- a program for painting on the screen
 * with this program you can play, draw, learn and teach
 * This program has been written such as a freedom sonet
 * We believe in the freedom and in the freedom of education
 *
 * Copyright (C) 2009 Pilolli Pietro <pilolli@fbk.eu>
 *
 * Ardesia is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Ardesia is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <preference_dialog.h>
#include <annotation_window.h>


/*
 * Start the dialog that ask to the user
 * the background setting
 */
void start_preference_dialog(GtkToolButton   *toolbutton, GtkWindow *parent)
{
  PreferenceData *preference_data = (PreferenceData *) g_malloc(sizeof(PreferenceData));

  /* 0 no background, 1 background color, 2 png background, */
  preference_data->background = 0;

  GtkWidget *preferenceDialog;

  /* Initialize the main window */
  preference_data->preferenceDialogGtkBuilder = gtk_builder_new();

  /* Load the gtk builder file created with glade */
  gtk_builder_add_from_file(preference_data->preferenceDialogGtkBuilder, PREFERENCE_UI_FILE, NULL);
 
  /* Fill the window by the gtk builder xml */
  preferenceDialog = GTK_WIDGET(gtk_builder_get_object(preference_data->preferenceDialogGtkBuilder,"preferences"));
  gtk_window_set_transient_for(GTK_WINDOW(preferenceDialog), parent);
  gtk_window_set_modal(GTK_WINDOW(preferenceDialog), TRUE);
  
#ifdef _WIN32
  /* 
   * In Windows the parent bar go above the dialog;
   * to avoid this behaviour I put the parent keep above to false
   */
  gtk_window_set_keep_above(GTK_WINDOW(parent), FALSE);
#endif 
   
  GObject* imgObj = gtk_builder_get_object(preference_data->preferenceDialogGtkBuilder, "imageChooserButton");
  GtkFileChooser* chooser = GTK_FILE_CHOOSER(imgObj);


  gtk_file_chooser_set_current_folder(chooser, BACKGROUNDS_FOLDER);
 
  /* Put the file filter for the supported formats */
  GtkFileFilter *filter = gtk_file_filter_new ();
  gtk_file_filter_set_name (filter, "PNG and JPEG");
  gtk_file_filter_add_mime_type (filter, "image/jpeg");
  gtk_file_filter_add_mime_type (filter, "image/png");
  gtk_file_chooser_add_filter (chooser, filter);
 
  preference_data->preview = gtk_image_new ();
  gtk_file_chooser_set_preview_widget (chooser, preference_data->preview);
 
  GtkWidget* color_button = GTK_WIDGET(gtk_builder_get_object(preference_data->preferenceDialogGtkBuilder,"backgroundColorButton"));
  gtk_color_button_set_use_alpha (GTK_COLOR_BUTTON(color_button), TRUE);
 
  /* Connect all signals by reflection */
  gtk_builder_connect_signals (preference_data->preferenceDialogGtkBuilder, (gpointer) preference_data);
   
  if (preference_data->background == 1)
    {
      GObject * colorObj = gtk_builder_get_object(preference_data->preferenceDialogGtkBuilder,"color");
      GtkToggleButton* colorToolButton = GTK_TOGGLE_BUTTON(colorObj);
      gtk_toggle_button_set_active(colorToolButton, TRUE);
    }
  else if (preference_data->background == 2)
    {
      GObject* fileObj = gtk_builder_get_object(preference_data->preferenceDialogGtkBuilder,"file");
      GtkToggleButton* imageToolButton = GTK_TOGGLE_BUTTON(fileObj);
      gtk_toggle_button_set_active(imageToolButton, TRUE);
    }

  gtk_dialog_run(GTK_DIALOG(preferenceDialog));
  
  if (preferenceDialog != NULL)
    {
      gtk_widget_destroy(preferenceDialog);
    }

  g_object_unref (preference_data->preferenceDialogGtkBuilder);
  g_free(preference_data);

#ifdef _WIN32
  /* 
   * Reput the keep above flag at the parent window bar
   */
  gtk_window_set_keep_above(GTK_WINDOW(parent), TRUE);
#endif   
}


