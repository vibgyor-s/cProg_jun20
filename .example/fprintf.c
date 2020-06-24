#include <stdio.h>
#include <stdlib.h>

int main () {
   FILE * fp;

   fp = fopen ("file.txt", "a+");
   for(int i=0;i<5;i++)
    fprintf(fp, "\n%s %s %s %d\n\n", "We", "are", "fuckers of", 2020);
   
   fclose(fp);
   
   return(0);
}