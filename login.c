#include<gtk/gtk.h>
#include<string.h>
#include "interface.h"

GtkWidget *window;
GtkWidget *box;
GtkWidget *title;
GtkWidget *uLabel;
GtkWidget *uEntry;
GtkWidget *pLabel;
GtkWidget *pEntry;
GtkWidget *login;
const gchar *username;
const gchar *password;

void load_css(void) {
	GtkCssProvider *provider;
	GdkDisplay *display;

	const gchar *css_file = "style.css";
	GFile *css_fp = g_file_new_for_path(css_file);

	provider = gtk_css_provider_new();
	display = gdk_display_get_default();

	gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	gtk_css_provider_load_from_file(provider, css_fp);

	g_object_unref(provider);
}

void loginFunc(GtkWidget *widget, gpointer data) {
	username = gtk_editable_get_text(GTK_EDITABLE(uEntry));
	password = gtk_editable_get_text(GTK_EDITABLE(pEntry));
	// g_print("Your entered username is: %s\n",username);
	// g_print("Your entered password is: %s",password);
	if(strcmp(username, "Ajay Sharma")==0){
		if(strcmp(password, "ajaysharma08")==0){
			
		}
	}
}

void activate(GtkApplication *app, gpointer user_data) {

	load_css();
	
	window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Login App");
	gtk_window_set_default_size(GTK_WINDOW(window), 600, 600);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

	box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
	gtk_widget_set_halign (box, GTK_ALIGN_CENTER);
  	// gtk_widget_set_valign (box, GTK_ALIGN_CENTER);
	
	gtk_window_set_child(GTK_WINDOW(window), box);
	
	title = gtk_label_new("The Dear Diary");
	gtk_widget_set_name(title, "title");

	uLabel = gtk_label_new("Username: ");
	uEntry = gtk_entry_new();
	gtk_entry_set_placeholder_text(GTK_ENTRY(uEntry), "USERNAME");
	
	pLabel = gtk_label_new("Password: ");
	pEntry = gtk_password_entry_new(); 
	gtk_password_entry_set_show_peek_icon (GTK_PASSWORD_ENTRY(pEntry), TRUE);
	g_object_set (pEntry, "placeholder-text", "PASSWORD", "activates-default", TRUE, NULL);

	login = gtk_button_new_with_label("Login");
	g_signal_connect(login, "clicked", G_CALLBACK(loginFunc), NULL);
	gtk_widget_set_name(login, "submit");

	gtk_box_append(GTK_BOX(box), title);
	gtk_box_append(GTK_BOX(box), uLabel);
	gtk_box_append(GTK_BOX(box), uEntry);
	gtk_box_append(GTK_BOX(box), pLabel);
	gtk_box_append(GTK_BOX(box), pEntry);
	gtk_box_append(GTK_BOX(box), login);

	gtk_widget_show(window);
}

int main(int argc, char **argv) {
	GtkApplication *app;
	int status;
	app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	return status;
}
