/* difftime example */
// #include <stdio.h>      /* printf */
// #include <time.h>       /* time_t, struct tm, difftime, time, mktime */

// int main ()
// {
//   time_t now;
//   struct tm newyear;
//   double seconds;

//   time(&now);  /* get current time; same as: now = time(NULL)  */

//   newyear = *localtime(&now);

//   newyear.tm_hour = 0; newyear.tm_min = 0; newyear.tm_sec = 0;
//   newyear.tm_mon = 0;  newyear.tm_mday = 1;

//   seconds = difftime(now,mktime(&newyear));

//   printf ("%.f seconds since new year in the current timezone.\n", seconds);

//   return 0;
// }

// #include <stdio.h>
// #include <time.h>

// // double comparetime(time_t time1,time_t time2){
// //  return (difftime(time1,time2))/86400; //> 0.0 ? 1 : -1; 
// // } 

// int main(void){
//     time_t t1, t2;
//     struct tm *timeptr,tm1, tm2;
//     char* time1 = "Mar 21 11:51:20 AM";
//     char* time2 = "Mar 10 10:20:05 AM";

//     //(1) convert `String to tm`:  
//     if(strptime(time1, "%a %m/%d/%Y %r",&t1) == NULL)
//             printf("\nstrptime failed\n");          
//     if(strptime(time2, "%b %d %r",&tm2) == NULL)
//             printf("\nstrptime failed\n");

//     //(2)   convert `tm to time_t`:    
//     t1 = mktime(&tm1);
//     t2 = mktime(&tm2);  

//     double seconds=(difftime(t1,t2))/86400;

//     printf("\n%.f",seconds);

//     //  printf("\n t1 > t2 : %d", comparetime(t1, t2));
//     //  printf("\n t2 > t1 : %d", comparetime(t2, t1));
//     //  printf("\n");
//     //  return 1;
// }

// #include <stdio.h>
// #include <time.h>
// #include <string.h>

// char * gettime();
// char * getdate();

// int main()
// {
//     printf("The time is %s\n", gettime());
//     printf("The date is %s\n", getdate());

//     return 0;
// }


//TODO
// #include <stdio.h>
// //#include <locale.h>
// #include <time.h>

// int main(void)
// {
//     char buf[100];
//     time_t t;
//     struct tm *timeptr,result;

//     //setlocale(LC_ALL,"/QSYS.LIB/EN_US.LOCALE");
//     t = time(NULL);
//     timeptr = localtime(&t);
//     strftime(buf,sizeof(buf), "%a %m/%d/%Y %r", timeptr);

//     if(strptime(buf, "%a %m/%d/%Y %r",&result) == NULL)
//           printf("\nstrptime failed\n");
//    else
//    {
//           printf("tm_hour:  %d\n",result.tm_hour);
//           printf("tm_min:  %d\n",result.tm_min);
//           printf("tm_sec:  %d\n",result.tm_sec);
//           printf("tm_mon:  %d\n",result.tm_mon);
//           printf("tm_mday:  %d\n",result.tm_mday);
//           printf("tm_year:  %d\n",result.tm_year);
//           printf("tm_yday:  %d\n",result.tm_yday);
//           printf("tm_wday:  %d\n",result.tm_wday);
//    }

//    return 0;
// }


//TODO
// #include <stdio.h>
// #include <time.h>
 
// int main(void)
// {
//     time_t now;
//     time(&now);
 
//     struct tm beg;
//     beg = *localtime(&now);
 
//     // set beg to the beginning of the month
//     beg.tm_hour = 0;
//     beg.tm_min = 0;
//     beg.tm_sec = 0;
//     beg.tm_mday = 1;
 
//     double seconds = difftime(now, mktime(&beg))/86400;
 
//     printf("%.f day have passed since the beginning of the month.\n", seconds);
 
//     return 0;
// }






// #include <stdio.h>
// #include <time.h>

// // double comparetime(time_t time1,time_t time2){
// //  return (difftime(time1,time2))/86400; //> 0.0 ? 1 : -1; 
// // } 

// int main(void){
//     time_t t1, t2;
//     struct tm *timeptr,tm1, tm2;
//     char* time1 = "friday 5/1/2020 11:51:20 AM";
//     char* time2 = "tuesday 6/2/2020 10:20:05 AM";

//     //(1) convert `String to tm`:  
//     if(strptime(time1, "%a %m/%d/%Y %r",&tm1) == NULL)
//             printf("\nstrptime failed\n");          
//     if(strptime(time2, "%a %m/%d/%Y %r",&tm2) == NULL)
//             printf("\nstrptime failed\n");

//     //(2)   convert `tm to time_t`:    
//     t1 = mktime(&tm1);
//     t2 = mktime(&tm2);  

//     double seconds=(difftime(t2,t1))/86400;

//     printf("\n%.f\n",seconds);

//     //  printf("\n t1 > t2 : %d", comparetime(t1, t2));
//     //  printf("\n t2 > t1 : %d", comparetime(t2, t1));
//     //  printf("\n");
//     //  return 1;
// }

#include <stdio.h>
#include <time.h>
int main(void){
    time_t t1, t2;
    struct tm *timeptr,tm1, tm2;
    char* time1 = "5/23/2020 11:51:20 AM";
    //char* time2 = "tuesday 6/2/2020 10:20:05 AM";
    time(&t2);//TODO
    //(1) convert `String to tm`:  
    if(strptime(time1, "%m/%d/%Y %r",&tm1) == NULL)
            printf("\nstrptime failed\n");          
    // if(strptime(time2, "%a %m/%d/%Y %r",&tm2) == NULL)
    //         printf("\nstrptime failed\n");

    //(2)   convert `tm to time_t`:    
    t1 = mktime(&tm1);
    //t2 = mktime(&tm2);  

    double seconds=(difftime(t2,t1))/86400;

    printf("\n%.f\n",seconds);

    //  printf("\n t1 > t2 : %d", comparetime(t1, t2));
    //  printf("\n t2 > t1 : %d", comparetime(t2, t1));
    //  printf("\n");
    //  return 1;
}