/*
 * id.c
 *
 * Copyright (C) 2020 DirtBagXon
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#define _GNU_SOURCE

#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <ctype.h>
#include <sys/mman.h>
#include "util.h"
#include "id.h"

GtkWidget	*window;
GtkWidget	*outerfixed;
GtkWidget	*listfixed;
GtkWidget	*scroll;
GtkWidget	*imageoverlay;
GtkWidget	*start;
GtkWidget	*logobutton;
GtkBuilder	*builder;
GtkWidget 	* init_tree();

GtkWidget       *doom;
GtkWidget       *doom2;
GtkWidget       *tnt;
GtkWidget       *plutonia;
GtkWidget       *doom3;
GtkWidget       *doom3roe;
GtkWidget       *doom3classic;
GtkWidget       *quake;
GtkWidget       *quakearmagon;
GtkWidget       *quakedoe;
GtkWidget       *quakedop;
GtkWidget       *quake2;
GtkWidget       *quake2reckon;
GtkWidget       *quake2zero;
GtkWidget       *quake2ctf;

static gboolean window_background(GtkWidget *widget, cairo_t *cr, gpointer data);
void	on_destroy();
void	image_overlay();
void	run_game(char *GAME);
char	binary[64] = BINARY_DOOM;
char	home[128] = HOME_DOOM;
char	arguments[128] = ARGS_DOOM;


GtkWidget * init_tree()
{
	GtkListStore *liststore = gtk_list_store_new(1, G_TYPE_STRING);
	GtkTreeIter treeiter;

	gtk_list_store_append(liststore, &treeiter);
	gtk_list_store_set(liststore, &treeiter, 0,NAME_DOOM, -1);
	gtk_list_store_append(liststore, &treeiter);
	gtk_list_store_set(liststore, &treeiter, 0,NAME_DOOM2, -1);
	gtk_list_store_append(liststore, &treeiter);
	gtk_list_store_set(liststore, &treeiter, 0, NAME_DOOMTNT, -1);
	gtk_list_store_append(liststore, &treeiter);
	gtk_list_store_set(liststore, &treeiter, 0, NAME_DOOMPLU, -1);
	gtk_list_store_append(liststore, &treeiter);
	gtk_list_store_set(liststore, &treeiter, 0, NAME_DOOM3, -1);
	gtk_list_store_append(liststore, &treeiter);
	gtk_list_store_set(liststore, &treeiter, 0, NAME_DOOM3ROE, -1);
	gtk_list_store_append(liststore, &treeiter);
	gtk_list_store_set(liststore, &treeiter, 0, NAME_DOOM3C, -1);
	gtk_list_store_append(liststore, &treeiter);
	gtk_list_store_set(liststore, &treeiter, 0, NAME_QUAKE, -1);
	gtk_list_store_append(liststore, &treeiter);
	gtk_list_store_set(liststore, &treeiter, 0, NAME_QUAKEM1, -1);
	gtk_list_store_append(liststore, &treeiter);
	gtk_list_store_set(liststore, &treeiter, 0, NAME_QUAKEM2, -1);
	gtk_list_store_append(liststore, &treeiter);
	gtk_list_store_set(liststore, &treeiter, 0, NAME_QUAKEM3, -1);
	gtk_list_store_append(liststore, &treeiter);
	gtk_list_store_set(liststore, &treeiter, 0, NAME_QUAKE2, -1);
	gtk_list_store_append(liststore, &treeiter);
	gtk_list_store_set(liststore, &treeiter, 0, NAME_QUAKE2M1, -1);
	gtk_list_store_append(liststore, &treeiter);
	gtk_list_store_set(liststore, &treeiter, 0, NAME_QUAKE2M2, -1);
	gtk_list_store_append(liststore, &treeiter);
	gtk_list_store_set(liststore, &treeiter, 0, NAME_QUAKE2CTF, -1);

	GtkWidget *treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(liststore));
	GtkCellRenderer *cellrenderertext = gtk_cell_renderer_text_new();

	GtkTreeViewColumn *treeviewcolumn = gtk_tree_view_column_new_with_attributes("Game", cellrenderertext, "text", 0, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), treeviewcolumn);

	return treeview;
}

static void selected(GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
	GtkWidget *graphic;
	gchar *string;
	GtkTreeIter iter;
	GtkTreeModel *model = gtk_tree_view_get_model (tree_view);
	gtk_tree_model_get_iter(model, &iter, path); 
	gtk_tree_model_get (model, &iter, 0, &string, -1);

	if (strcmp(string, NAME_DOOM2) == 0) {
		sprintf(binary, BINARY_DOOM2);
		sprintf(home, HOME_DOOM2);
		sprintf(arguments, ARGS_DOOM2);
		graphic = doom2;
	} else if (strcmp(string, NAME_DOOMTNT) == 0) {
		sprintf(binary, BINARY_DOOMTNT);
		sprintf(home, HOME_DOOMTNT);
		sprintf(arguments, ARGS_DOOMTNT);
		graphic = tnt;
	} else if (strcmp(string, NAME_DOOMPLU) == 0) {
		sprintf(binary, BINARY_DOOMPLU);
		sprintf(home, HOME_DOOMPLU);
		sprintf(arguments, ARGS_DOOMPLU);
		graphic = plutonia;
	} else if (strcmp(string, NAME_DOOM3) == 0) {
		sprintf(binary, BINARY_DOOM3);
		sprintf(home, HOME_DOOM3);
		sprintf(arguments, ARGS_DOOM3);
		graphic = doom3;
	} else if (strcmp(string, NAME_DOOM3ROE) == 0) {
		sprintf(binary, BINARY_DOOM3ROE);
		sprintf(home, HOME_DOOM3ROE);
		sprintf(arguments, ARGS_DOOM3ROE);
		graphic = doom3roe;
	} else if (strcmp(string, NAME_DOOM3C) == 0) {
		sprintf(binary, BINARY_DOOM3C);
		sprintf(home, HOME_DOOM3C);
		sprintf(arguments, ARGS_DOOM3C);
		graphic = doom3classic;
	} else if (strcmp(string, NAME_QUAKE) == 0) {
		sprintf(binary, BINARY_QUAKE);
		sprintf(home, HOME_QUAKE);
		sprintf(arguments, ARGS_QUAKE);
		graphic = quake;
	} else if (strcmp(string, NAME_QUAKEM1) == 0) {
		sprintf(binary, BINARY_QUAKEM1);
		sprintf(home, HOME_QUAKEM1);
		sprintf(arguments, ARGS_QUAKEM1);
		graphic = quakearmagon;
	} else if (strcmp(string, NAME_QUAKEM2) == 0) {
		sprintf(binary, BINARY_QUAKEM2);
		sprintf(home, HOME_QUAKEM2);
		sprintf(arguments, ARGS_QUAKEM2);
		graphic = quakedoe;
	} else if (strcmp(string, NAME_QUAKEM3) == 0) {
		sprintf(binary, BINARY_QUAKEM3);
		sprintf(home, HOME_QUAKEM3);
		sprintf(arguments, ARGS_QUAKEM3);
		graphic = quakedop;
	} else if (strcmp(string, NAME_QUAKE2) == 0) {
		sprintf(binary, BINARY_QUAKE2);
		sprintf(home, HOME_QUAKE2);
		sprintf(arguments, ARGS_QUAKE2);
		graphic = quake2;
	} else if (strcmp(string, NAME_QUAKE2M1) == 0) {
		sprintf(binary, BINARY_QUAKE2M1);
		sprintf(home, HOME_QUAKE2M1);
		sprintf(arguments, ARGS_QUAKE2M1);
		graphic = quake2reckon;
	} else if (strcmp(string, NAME_QUAKE2M2) == 0) {
		sprintf(binary, BINARY_QUAKE2M2);
		sprintf(home, HOME_QUAKE2M2);
		sprintf(arguments, ARGS_QUAKE2M2);
		graphic = quake2zero;
	} else if (strcmp(string, NAME_QUAKE2CTF) == 0) {
		sprintf(binary, BINARY_QUAKE2CTF);
		sprintf(home, HOME_QUAKE2CTF);
		sprintf(arguments, ARGS_QUAKE2CTF);
		graphic = quake2ctf;
	} else {
		sprintf(binary, BINARY_DOOM);
		sprintf(home, HOME_DOOM);
		sprintf(arguments, ARGS_DOOM);
		graphic = doom;
	}

	gtk_overlay_reorder_overlay(GTK_OVERLAY (imageoverlay), graphic, -1);
 }

void	image_overlay()
{
	// Reverse order of init_tree()
	// top image goes last..
	quake2ctf = gtk_image_new_from_resource("/id/images/quake2ctf.jpg");
	quake2zero = gtk_image_new_from_resource("/id/images/quake2zero.jpg");
	quake2reckon = gtk_image_new_from_resource("/id/images/quake2reckon.jpg");
	quake2 = gtk_image_new_from_resource("/id/images/quake2.jpg");
	quakedop = gtk_image_new_from_resource("/id/images/quakedop.jpg");
	quakedoe = gtk_image_new_from_resource("/id/images/quakedoe.jpg");
	quakearmagon = gtk_image_new_from_resource("/id/images/quakearmagon.jpg");
	quake = gtk_image_new_from_resource("/id/images/quake.jpg");
	doom3classic = gtk_image_new_from_resource("/id/images/doom3classic.jpg");
	doom3roe = gtk_image_new_from_resource("/id/images/doom3roe.jpg");
	doom3 = gtk_image_new_from_resource("/id/images/doom3.jpg");
	plutonia = gtk_image_new_from_resource("/id/images/plutonia.jpg");
	tnt = gtk_image_new_from_resource("/id/images/tnt.jpg");
	doom2 = gtk_image_new_from_resource("/id/images/doom2.jpg");
	doom = gtk_image_new_from_resource("/id/images/doom.jpg");

	gtk_overlay_add_overlay (GTK_OVERLAY (imageoverlay), quake2ctf);
	gtk_overlay_add_overlay (GTK_OVERLAY (imageoverlay), quake2zero);
	gtk_overlay_add_overlay (GTK_OVERLAY (imageoverlay), quake2reckon);
	gtk_overlay_add_overlay (GTK_OVERLAY (imageoverlay), quake2);
	gtk_overlay_add_overlay (GTK_OVERLAY (imageoverlay), quakedop);
	gtk_overlay_add_overlay (GTK_OVERLAY (imageoverlay), quakedoe);
	gtk_overlay_add_overlay (GTK_OVERLAY (imageoverlay), quakearmagon);
	gtk_overlay_add_overlay (GTK_OVERLAY (imageoverlay), quake);
	gtk_overlay_add_overlay (GTK_OVERLAY (imageoverlay), doom3classic);
	gtk_overlay_add_overlay (GTK_OVERLAY (imageoverlay), doom3roe);
	gtk_overlay_add_overlay (GTK_OVERLAY (imageoverlay), doom3);
	gtk_overlay_add_overlay (GTK_OVERLAY (imageoverlay), plutonia);
	gtk_overlay_add_overlay (GTK_OVERLAY (imageoverlay), tnt);
	gtk_overlay_add_overlay (GTK_OVERLAY (imageoverlay), doom2);
	gtk_overlay_add_overlay (GTK_OVERLAY (imageoverlay), doom);
}

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	builder = gtk_builder_new_from_resource ("/id/id.glade");
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
	g_signal_connect(window, "destroy", G_CALLBACK(on_destroy), NULL);
        gtk_builder_connect_signals(builder, NULL);

	outerfixed		= GTK_WIDGET(gtk_builder_get_object(builder, "outerfixed"));
	listfixed		= GTK_WIDGET(gtk_builder_get_object(builder, "listfixed"));
	scroll			= GTK_WIDGET(gtk_builder_get_object(builder, "scroll"));
	imageoverlay		= GTK_WIDGET(gtk_builder_get_object(builder, "imageoverlay"));
	start			= GTK_WIDGET(gtk_builder_get_object(builder, "start"));
	logobutton		= GTK_WIDGET(gtk_builder_get_object(builder, "logobutton"));

	g_object_unref(builder);

	loadResCSS("id/css/main.css");
	gtk_widget_set_name(logobutton, "logo_button");

	gtk_window_set_keep_above (GTK_WINDOW(window), FALSE);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	gtk_widget_set_app_paintable(window, TRUE);
	g_signal_connect(window, "draw", G_CALLBACK(window_background), NULL);

	image_overlay();
	GtkWidget* list = init_tree();
	gtk_tree_view_set_activate_on_single_click(GTK_TREE_VIEW(list), TRUE);
	g_signal_connect(list, "row-activated", G_CALLBACK(selected), NULL);
	gtk_container_add(GTK_CONTAINER(scroll), list);
	
	gtk_widget_show_all(window);

	gtk_main();

	return EXIT_SUCCESS;
}

static gboolean window_background(GtkWidget *widget, cairo_t *cr, gpointer data)
 {
	GdkPixbuf* pixbuf;
	g_autoptr(GError) bgerror = NULL;
	pixbuf = gdk_pixbuf_new_from_resource("/id/images/background.jpg", &bgerror);

	if (bgerror != NULL)
	{
		display_error(bgerror->message);
		on_destroy();
	}

	// Colour
	// cairo_set_source_rgba(cr, 0.0, 0.0, 1.0, 0.3);
	gdk_cairo_set_source_pixbuf (cr, pixbuf, 0, 0);
	cairo_paint(cr);
	return FALSE;
 }

void	run_game(char *GAME)
{
	char cmd[65];
	char delim[] = " ";
	int count = 1;
	GPid child_pid;
	gchar * argv[32] = {};
	g_autoptr(GError) error = NULL;
	gint child_stdout, child_stderr;

	if (strlen(home) > 31 || strlen(arguments) > 127 || strlen(GAME) > 31) {
		display_error("#define values exceed max length in header file..");
		return;
	}

	sprintf(cmd, "%s/%s", home, GAME);

	argv[0] = cmd;

	char *ptr = strtok(arguments, delim);

	while(ptr != NULL)
	{
		argv[count] = ptr;
		ptr = strtok(NULL, delim);
		count++;
	}

	argv[count] = NULL;
	
	// Debug
	// int i;
	// for(i=0; i<count; i++) {
	//	printf("%s\n", argv[i]);  
	// }

	// noisy JACK server - ignore STDERR on GUI
	if (strcmp(GAME, "dhewm3") == 0 || strcmp(home, "/usr/lib/yamagi-quake2") == 0) { 
		g_spawn_async_with_pipes(home, argv, NULL, G_SPAWN_DO_NOT_REAP_CHILD, NULL,
			NULL, &child_pid, NULL, &child_stdout, NULL, &error);
	} else {
		g_spawn_async_with_pipes(home, argv, NULL, G_SPAWN_DO_NOT_REAP_CHILD, NULL,
			NULL, &child_pid, NULL, &child_stdout, &child_stderr, &error);
	}
	
	if (error != NULL)
	{
		display_error(error->message);
		return;
	}

	gpointer user_ptr = GINT_TO_POINTER(child_stderr);
	g_child_watch_add(child_pid, watch_game, user_ptr);
	return;
}
