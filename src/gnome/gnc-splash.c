/********************************************************************\
 * gnc-splash.c -- splash screen for GnuCash                        *
 * Copyright (C) 2001 Gnumatic, Inc.                                *
 *                                                                  *
 * This program is free software; you can redistribute it and/or    *
 * modify it under the terms of the GNU General Public License as   *
 * published by the Free Software Foundation; either version 2 of   *
 * the License, or (at your option) any later version.              *
 *                                                                  *
 * This program is distributed in the hope that it will be useful,  *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    *
 * GNU General Public License for more details.                     *
 *                                                                  *
 * You should have received a copy of the GNU General Public License*
 * along with this program; if not, contact:                        *
 *                                                                  *
 * Free Software Foundation           Voice:  +1-617-542-5942       *
 * 59 Temple Place - Suite 330        Fax:    +1-617-542-2652       *
 * Boston, MA  02111-1307,  USA       gnu@gnu.org                   *
\********************************************************************/

#include "config.h"

#include <gnome.h>

#include "dialog-utils.h"
#include "gnc-splash.h"
#include "gnc-version.h"


static GtkWidget * splash = NULL;


static gint
splash_timeout (gpointer not_used)
{
  if (splash)
  {
    gtk_widget_destroy (splash);
    splash = NULL;
  }

  return FALSE;
}

static void
splash_destroy_cb (GtkObject *object, gpointer user_data)
{
  splash = NULL;
}

void
gnc_show_splash_screen (void)
{
  GtkWidget *pixmap;
  GtkWidget *frame;
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *version;
  gchar ver_string[50];

  if (splash) return;

  splash = gtk_window_new (GTK_WINDOW_DIALOG);

  gtk_signal_connect (GTK_OBJECT (splash), "destroy",
                      GTK_SIGNAL_FUNC (splash_destroy_cb), NULL);

  gtk_window_set_title (GTK_WINDOW (splash), "GnuCash");
  gtk_window_set_position (GTK_WINDOW (splash), GTK_WIN_POS_CENTER);

  pixmap = gnc_get_pixmap ("gnucash_splash.png");

  if (!pixmap)
  {
    g_warning ("can't find splash pixmap");
    gtk_widget_destroy (splash);
    return;
  }

  frame = gtk_frame_new (NULL);
  vbox = gtk_vbox_new (FALSE, 3);
  hbox = gtk_hbox_new (FALSE, 0);
  if (GNUCASH_MINOR_VERSION % 2) {
    sprintf(ver_string, _("Version: Gnucash-cvs (built %s)"),
	    GNUCASH_BUILD_DATE);
  } else {
    sprintf(ver_string, _("Version: Gnucash-%s"), VERSION);
  }
  version = gtk_label_new (ver_string);

  gtk_container_add (GTK_CONTAINER (frame), pixmap);
  gtk_box_pack_end (GTK_BOX (hbox), version, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), frame, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  gtk_container_add (GTK_CONTAINER (splash), vbox);

  gtk_widget_show_all (splash);

  gtk_timeout_add (4000, splash_timeout, NULL); /* 4 seconds */
}
