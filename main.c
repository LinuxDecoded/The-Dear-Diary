#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include<sys/stat.h>
#include "diary.h"

char pmsk[5]={'a','b','c','d','\0'};

#define ESC 27
#define F1 59
#define F2 60

int argc;
char **argv;

void signup(void);
void login(void);
void readDiary(char username[15]);
void menu(int argc, char **argv);
void gotoxy(int x, int y);

struct creden {
char username[15];
char password[15];
};

FILE *fp;

char user[15];

int main() {
    int ch, value;

    SetConsoleTitle("The Dear Diary");
    HWND c_window = GetConsoleWindow();
    HMENU c_menu = GetSystemMenu(c_window, FALSE);
    EnableMenuItem(c_menu, SC_CLOSE, MF_GRAYED);

    struct stat stats;
    stat("diary", &stats);
    // Check for file existence
    if (S_ISDIR(stats.st_mode)!=TRUE)
        system("mkdir diary && attrib +h +s +r diary");

    while(1) {
        system("cls");

        //Outline Box
        gotoxy(35,5);
        printf("___________________The Dear Diary___________________\n");
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

        //Text inside Box
        gotoxy(46,7);
        printf("...........Welcome...........");
        gotoxy(36,9);
        printf("Press F1 For Login");
        gotoxy(36,11);
        printf("Press F2 For New Registration/Reset Credentials");
        gotoxy(36,13);
        printf("Press ESC For Exit");

        getch();
        ch=getch();
        switch(ch){
            case F1:
                system("cls");
                gotoxy(10,14);
                for(int i=0;i<40;i++) {
                    printf("*.");
                    Sleep(30);
                }
                login();
                break;

            case F2:
                system("cls");
                gotoxy(10,14);
                for(int i=0;i<40;i++) {
                    printf("*.");
                    Sleep(30);
                }
                signup();
                break;

            case ESC:
                exit(0);
                break;
        }
    }
    getch();
    return 0;
}

//Function declaration

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

    gotoxy(36,8);
    printf("Press F1 for New Diary Entry");
    gotoxy(36, 10);
    printf("Press F2 for Reading Old Entry");

    getch();
    ch=getch();
    switch(ch) {
        case F1:
            system("cls");
            diary(argc, argv);
            break;
        case F2:
            system("cls");
            text_open(argc, argv);
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
    fp=fopen("diary/record","rb");
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
        fp=fopen("diary/record","ab");

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
            fwrite(&s,sizeof(s),1,fp);
            gotoxy(54,17);
            printf("Data Saved....");
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
