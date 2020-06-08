#include<stdio.h>
#include<stdlib.h>
int main()
{
char s[100];
system("stty -echo");
scanf("%s",s); /* Won't echo to screen while being typed */
system("stty echo");
printf("You have entered:");
printf("%s\n", s);
return 0;
}