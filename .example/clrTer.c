// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>
// int main()
// {
//     int i = 0;
//     double time_taken;
//     clock_t t1, t2;

//     // // a loop for showing geeks for geeks
//     // // repeating by clearing console using
//     // // system("clear")

//     // // Noting start time
//     // t1 = clock();
//     // for (i; i < 100; i++)
//     // {
//     //     system("clear");
//     //     printf("geeks for geeks %d\n", i);
//     // }

//     // // Calculating total time taken by
//     // // system("clear")
//     // t1 = clock() - t1;

//     // Noting start time of regex
//     t2 = clock();
//     for (i; i < 100; i++)
//     {
//         printf("\e[1;1H\e[2J");
//         printf("geeks for geeks %d\n", i);
//     }

//     // calculating total time taken by regex
//     t2 = clock() - t2;

//     // printing taken by both
//     printf("Time taken by system\(\"clear\") %f\n",
//            ((double)t1) / CLOCKS_PER_SEC);
//     printf("Time taken regex %f",
//            ((double)t2) / CLOCKS_PER_SEC);

//     return 0;
// }

#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("\e[1;1H\e[2J");
    printf("3\n");
    sleep(1);

    printf("\e[1;1H\e[2J");
    printf("2\n");
    sleep(1);

    printf("\e[1;1H\e[2J");
    printf("1\n");
    sleep(1);

    printf("\e[1;1H\e[2J");
    printf("0\n");

    return 0;
}