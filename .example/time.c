#include<stdio.h>
#include<time.h>
#include<string.h>

int main()
{
    char todayDateStr[15];
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(todayDateStr, strlen("DD-MMM-YYYY") + 1, "%d-%b-%Y", timeinfo);
    printf("todayDateStr = %s ... \n", todayDateStr);
}