//https://stackoverflow.com/a/5172154/13535698
#include <stdio.h>

int main()
{
   int i;
   char tst[]={"vibgyor singhvi"};
   char buf[25];

   for (i = 0; i < 100; i++) {
      snprintf(buf, 25, "%s_%d_suff",tst, i); // puts string into buffer
      printf("%s\n", buf); // outputs so you can see it
   }
}