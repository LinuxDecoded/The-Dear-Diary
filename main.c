#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include "interface.h"

int main() {
    int ch, value;

    SetConsoleTitle("The Dear Diary");
    HWND c_window = GetConsoleWindow();
    HMENU c_menu = GetSystemMenu(c_window, FALSE);
    EnableMenuItem(c_menu, SC_CLOSE, MF_GRAYED);

    struct stat stats;
    stat("%USERPROFILE%\\Diary", &stats);
    // Check for file existence
    if (S_ISDIR(stats.st_mode)!=TRUE) {
        system("mkdir %USERPROFILE%\\Diary");
    }
    strcat(strcpy(filepath, getenv("USERPROFILE")), "\\Diary\\diary.dat");

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
