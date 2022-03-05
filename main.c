#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include "interface.h"

int main() {

    SetConsoleTitle("The Dear Diary");
    HWND c_window = GetConsoleWindow();
    HMENU c_menu = GetSystemMenu(c_window, FALSE);
    EnableMenuItem(c_menu, SC_CLOSE, MF_GRAYED);

    start_screen();
    
    return 0;
}