#include<stdio.h>
#include<conio.h>

int main() {
    char pass[15];
    printf("Enter something: ");
    scanf(" %s", &pass);
    for(int i=0; (i<15 && pass[i] != '\0'); i++) {
        pass[i] += 3;
    }
    printf("Encrypted password: %s\n", pass);
    for(int i=0; (i<15 && pass[i] != '\0'); i++) {
        pass[i] -= 3;
    }
    printf("Decrypted password: %s", pass);
    getch();
    return 0;
}