#include<gtk/gtk.h>
#include<stdio.h>

#ifndef _MAX_FILE_SIZE
#define _MAX_FILE_SIZE 2048000  //4 MB 
#endif 

typedef struct{
    char *label;
    int sub_items;
    char sub_menu[4][10];
}MuButton;

MuButton menulist[] = {
    {"File", 2, {"Save","Exit"}},
    {"Help", 1, {"About"}}
};
const int menLimit=2;

void check_usr_folder();
void encrypt(char filepath[]);
void decrypt(gchar *filename);
void diary_interface(void);
void no_diary_msg();
void close_window();
void text_open(int argc, char **argv);
void text_save();
void button_click(GtkWidget *button, gpointer data);
void diary(int argc, char **argv);
void gotoxy(int x, int y);
void menu(int argc, char **argv);
void login();
void signup();
void start_screen();

GtkWidget *window;
GtkWidget *vbox;
GtkWidget *menu_bar;
GtkWidget *item;
GtkWidget *item_menu;
GtkWidget *subitem;
GtkWidget *scrolled_window;
GtkWidget *text_view;
GtkWidget *save_dialog;
GtkWidget *open_dialog;

void text_save(void);
void button_click(GtkWidget *, gpointer);

gchar *filename;
char file[255];
char filepath[255];
struct tm *timenow;
time_t curr_time;
char ch;

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

FILE *fp, *fp_tmp;

char user[15];
char filepath[255];
int encrypt_key;