// #include<stdio.h>
// #include<string.h>

// int main()
// {
//     char strg1[50], strg2[50]="^[\0";
 
//     printf("Enter first string: ");
//     gets(strg1);
//     printf("\n%s\n",strg1);
 
//     // printf("Enter second string: ");
//     // gets(strg2);
 
//     if(strcmp(strg1, strg2)==0)
//     {
//         printf("\nYou entered the same string two times");
//     }
 
//     else
//     {
//         printf("\nEntered strings are not same!");
//     }
 
//     // signal to operating system program ran fine
//     return 0;
// }


#include <stdio.h>
#include <stdlib.h>

#define esc 27

int main()
{
    char ch;
    do
    {
        ch = getchar();
        switch(ch)
        {
            case esc:
                // your logic goes here
                printf("esc detected\n");
                break;
        }
    }
    while(1);
}

#include <conio.h>
#include <stdio.h>
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

int c = 0;
_getch();
switch ((c = _getch())) {
case KEY_UP:
    printf("Up\n");
    break;
case KEY_DOWN:
    printf("Down\n");
    break;
case KEY_LEFT:
    printf("Left\n");
    break;
case KEY_RIGHT:
    printf("Right\n");
    break;
default:
    printf("Null\n");
    break;