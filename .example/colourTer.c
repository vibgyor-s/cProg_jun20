//http://web.theurbanpenguin.com/adding-color-to-your-output-from-c/

// #include<stdio.h>
// int main()
// {
// printf("\033[0;31m"); //Set the text to the color red
// printf("Hello\n"); //Display Hello in red
// printf("\033[0m"); //Resets the text to default color
// printf("Hello\n"); //Display Hello
// }

#include <stdio.h>
void red () {
  printf("\033[1;31m");
}

void yellow() {
  printf("\033[1;33m");
}

void reset () {
  printf("\033[0m");
}

int main () {
  red();
  printf("Hello ");
  yellow();
  printf("world\n");
  reset();
  return 0;
}


// CODE	COLOR
// [0;31m	Red
// [1;31m	Bold Red
// [0;32m	Green
// [1;32m	Bold Green
// [0;33m	Yellow
// [01;33m	Bold Yellow
// [0;34m	Blue
// [1;34m	Bold Blue
// [0;35m	Magenta
// [1;35m	Bold Magenta
// [0;36m	Cyan
// [1;36m	Bold Cyan
// [0m	Reset