#include<gtk/gtk.h>
#include<stdio.h>
#include<stdlib.h>
#include "declarations.h"

void close_window() {
    gtk_main_quit();
    g_print("Exiting...\n");
    Sleep(200);
    exit(0);
}

void diary_interface() {
    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    vbox=gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_title(GTK_WINDOW(window), "The Dear Diary");
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 500);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(close_window),NULL);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    menu_bar=gtk_menu_bar_new();
    int i;
    for(i=0;i<menLimit;i++) {
        item=gtk_menu_item_new_with_label(menulist[i].label);
        item_menu=gtk_menu_new();
        gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar),item);
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(item),item_menu);
        int j;
        int limit=menulist[i].sub_items;
        for(j=0;j<limit;j++) {
            subitem=gtk_menu_item_new_with_label(menulist[i].sub_menu[j]);
            g_signal_connect(GTK_WIDGET(subitem), "activate", G_CALLBACK(button_click),menulist[i].sub_menu[j]);
            gtk_menu_shell_append(GTK_MENU_SHELL(item_menu), subitem);
        }
    }
    gtk_container_add(GTK_CONTAINER(vbox),menu_bar);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

    GtkTextBuffer *buffer;
    text_view = gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);
    buffer=gtk_text_buffer_new(NULL);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(text_view), buffer);
}

void text_open(int argc, char **argv) {
    gtk_init(&argc, &argv);

    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    vbox=gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_title(GTK_WINDOW(window), "The Dear Diary");
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 500);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(close_window),NULL);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);
    text_view = gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);

    gsize nBytesInBuff;
    gchar *contents;
    GtkTextBuffer *buffer;
    GError *error = NULL;
    GtkFileFilter *filter;

    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    open_dialog=gtk_file_chooser_dialog_new("Choose Your Entry...",GTK_WINDOW(window),action,"Cancel",GTK_RESPONSE_CANCEL,"Open",GTK_RESPONSE_ACCEPT,NULL);

    gint res = gtk_dialog_run(GTK_DIALOG(open_dialog));
    if(res == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser=GTK_FILE_CHOOSER(open_dialog);
        filename=gtk_file_chooser_get_filename(chooser);
        gtk_widget_destroy(open_dialog);
    }

    if(filename==NULL) {
        GtkWidget *msg;
        msg=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"Failed to get file!!");
        gtk_dialog_run(GTK_DIALOG(msg));
        gtk_widget_destroy(msg);
        exit(0);
    }

    buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    if(! g_file_get_contents(filename, &contents, &nBytesInBuff, &error)) {
        printf(error->message);
        g_clear_error(&error);
        g_free(filename);
        exit(1);
    }

    gtk_text_buffer_set_text(buffer, contents, -1);
    g_free(filename);
    g_free(contents);
    gtk_widget_show_all(window);

    gtk_main();
}

void text_save() {
    GtkTextBuffer *buffer;
    GtkTextIter start, end;
    gchar *text;
    FILE *fp;

    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    text=gtk_text_buffer_get_text(buffer, &start, &end, TRUE);
    
    window=gtk_window_new(GTK_WINDOW_POPUP);
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
    save_dialog=gtk_file_chooser_dialog_new("Enter Filename...",GTK_WINDOW(window),action,"Cancel",GTK_RESPONSE_CANCEL,"Choose",GTK_RESPONSE_ACCEPT,NULL);
    gtk_widget_show_all(save_dialog);

    gint res = gtk_dialog_run(GTK_DIALOG (save_dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(save_dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        fp=fopen(filename, "wb");
        if(!fp) {
            return;
        }
        fwrite(text, strlen(text), 1, fp);
        fclose(fp);
        gtk_widget_destroy(save_dialog);
    }
    else {
        gtk_widget_destroy(save_dialog);
    }
}

void button_click(GtkWidget *button, gpointer data) {
    char *btn=(char*)data;
    if(strcmp(btn, "Save")==0) {
        text_save();
        g_print("Diary Entry Saved!!!");
        Sleep(200);
        exit(0);
    }
    if(strcmp(btn, "Exit")==0) {
        g_print("Exitting..\n");
        Sleep(200);
        exit(0);
    }
    if(strcmp(btn, "About")==0) {
        GtkWidget *msg;
        msg=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"\tThe Dear Diary\t\nSimple Diary Entry Program\n\tPowered by C");
        gtk_dialog_run(GTK_DIALOG(msg));
        gtk_widget_destroy(msg);
    }
}

void diary(int argc, char **argv) {
    gtk_init(&argc, &argv);
    diary_interface();
    gtk_widget_show_all(window);
    gtk_main();
}