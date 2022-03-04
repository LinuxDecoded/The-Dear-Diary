#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>
#include<sys/stat.h>
#include<time.h>
#include<gtk/gtk.h>
#include "declarations.h"

char pmsk[5]={'a','b','c','d','\0'};

#define ESC 27
#define F1 59
#define F2 60

int argc;
char **argv;

struct creden {
char username[15];
char password[15];
int key;
};

FILE *fp;

char filepath[255] ,user[15];

//GUI part starts here
void close_window() {
    gtk_main_quit();
    menu(argc, argv);
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
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(open_dialog), "diary");

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
    
    curr_time = time(NULL);
    timenow = localtime(&curr_time);
    strftime(file, sizeof(file), "Diary_%d_%b_%Y-%H_%M_%S", timenow);
    strcpy(filepath,getenv("USERPROFILE"));
    strcat(filepath, "\\Diary\\");
    strcat(filepath, user);
    struct stat stats;

    stat(filepath, &stats);
    // Check for user folder existence
    if (S_ISDIR(stats.st_mode)!=TRUE) {
        char command[255];
        strcpy(command, "mkdir ");
        strcat(command, filepath);
        system(command);
    }

    strcat(filepath, "\\");
    strcat(filepath, file);
    fp=fopen(filepath, "wb");
    if(!fp) {
        return;
    }
    fwrite(text, strlen(text), 1, fp);
    GtkWidget *msg;
    msg=gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"File Saved !!");
    gtk_dialog_run(GTK_DIALOG(msg));
    gtk_widget_destroy(msg);
    fclose(fp);
}

void button_click(GtkWidget *button, gpointer data) {
    char *btn=(char*)data;
    if(strcmp(btn, "Save")==0) {
        text_save();
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

//Console part starts here
void gotoxy(int x, int y) {
    COORD co = {0,0};
    co.X=x;
    co.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),co);
}

void menu(int argc, char **argv) {
    char ch;
    
    system("cls");
    gotoxy(56,3);
    printf("Hello, %s", user);
    gotoxy(35,5);
    printf("_________________________Menu_______________________\n");
    gotoxy(34,6);
    printf("|                                                    |");
    gotoxy(34,7);
    printf("|                                                    |");
    gotoxy(34,8);
    printf("|                                                    |");
    gotoxy(34,9);
    printf("|                                                    |");
    gotoxy(34,10);
    printf("|                                                    |");
    gotoxy(34,11);
    printf("|                                                    |");
    gotoxy(34,12);
    printf("|                                                    |");
    gotoxy(35,12);
    printf("______________________********______________________\n");

    gotoxy(36,7);
    printf("Press F1 for New Diary Entry");
    gotoxy(36, 9);
    printf("Press F2 for Reading Old Entry");
    gotoxy(36, 11);
    printf("Press ESC to exit");

    getch();
    ch=getch();
    switch(ch) {
        case F1:
            system("cls");
            printf("Loading Diary Window, be patient ...");
            diary(argc, argv);
            break;
        case F2:
            system("cls");
            text_open(argc, argv);
            break;
        case ESC:
            exit(0);
            break;
    }
}

void login() {

    int i=0;
    // char user[15], pass[15], c;
    struct creden user_input;
    char c;

    struct creden l;

    //Outline Box
    system("cls");
    gotoxy(35,5);
    printf("___________________    Login     ___________________\n");
    gotoxy(34,6);
    printf("|                                                    |");
    gotoxy(34,7);
    printf("|                                                    |");
    gotoxy(34,8);
    printf("|                                                    |");
    gotoxy(34,9);
    printf("|                                                    |");
    gotoxy(34,10);
    printf("|                                                    |");
    gotoxy(34,11);
    printf("|                                                    |");
    gotoxy(34,12);
    printf("|                                                    |");
    gotoxy(34,13);
    printf("|                                                    |");
    gotoxy(34,14);
    printf("|                                                    |");
    gotoxy(34,15);
    printf("|                                                    |");
    gotoxy(35,15);
    printf("______________________********______________________\n");

    gotoxy(36,8);
    fp=fopen(filepath,"rb");
    printf("Enter UserName  :   ");
    scanf("%s",user_input.username);
    gotoxy(36,10);
    printf("Enter Password  :   ");

    while((c=getch())!=13) {
        user_input.password[i]=c;
        i++;
        printf("*");
    }

    user_input.password[i]='\0';

    if(fp==NULL) {
        gotoxy(35,17);
        printf("No user data exists!!   Signup First!!");
        Sleep(30);
    }
    else {
        while(fread(&l,sizeof(l),1,fp)) {
            for(int i=0; (i<15 && l.username[i] != '\0'); i++) {
                l.username[i] -= 3;
            }
            for(int i=0; (i<15 && l.password[i] != '\0'); i++) {
                l.password[i] -= 3;
            }
            if(strcmp(user_input.username,l.username)==0 && strcmp(user_input.password, l.password)==0) {
                for(int i=0; i<15; i++) {
                    user[i] = l.username[i];
                }
                menu(argc, argv);
            }
            else {
                gotoxy(35,17);
                printf("Invalid Username or Password!!");
                Sleep(30);
            }
        }
    }

    fclose(fp);
    getch();
}

 void signup(){

    struct creden s;
    char msk[5],c;
    int i;

    system("cls");
    gotoxy(35,5);
    printf("___________________    Enter Master Key    ___________________\n");
    gotoxy(35,7);
    printf("--->");
    for(i=0;i<4;i++) {
        c=getch();
        msk[i]=c;
    }
    msk[i]='\0';

    if(strcmp(msk, pmsk)==0) {

        system("cls");
        fp=fopen(filepath,"ab");

        gotoxy(35,5);
        printf("___________________    Signup    ___________________\n");
        gotoxy(34,6);
        printf("|                                                    |");
        gotoxy(34,7);
        printf("|                                                    |");
        gotoxy(34,8);
        printf("|                                                    |");
        gotoxy(34,9);
        printf("|                                                    |");
        gotoxy(34,10);
        printf("|                                                    |");
        gotoxy(34,11);
        printf("|                                                    |");
        gotoxy(34,12);
        printf("|                                                    |");
        gotoxy(34,13);
        printf("|                                                    |");
        gotoxy(34,14);
        printf("|                                                    |");
        gotoxy(34,15);
        printf("|                                                    |");
        gotoxy(35,15);
        printf("______________________********______________________\n");

        gotoxy(36,8);
        printf("Set UserName    :   ");
        scanf("%s", s.username);
        for(int i=0; (i<15 && s.username[i] != '\0'); i++) {
            s.username[i] += 3;
        }
        gotoxy(36,10);
        printf("Set Password    :   ");
        scanf("%s", s.password);
        for(int i=0; (i<15 && s.password[i] != '\0'); i++) {
            s.password[i] += 3;
        }
        gotoxy(36,12);
        printf("Press Enter to continue.........");
        if(getch()==13) {
            srand(time(0));
            s.key = rand();
            s.key *= 2;
            fwrite(&s,sizeof(s),1,fp);
            gotoxy(54,17);
            if(fp==NULL) {
                printf("Error!! Could not save credentials!!");
            }
            else {
                printf("Data Saved....");
            }
        }
        else
            return;

        fclose(fp);
        getch();
    }
    else {
        printf("Wrong Master Key Entered!!!");
        Sleep(300);
        return;
    }
 }
