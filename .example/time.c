#include<stdio.h>
#include<time.h>
#include<string.h>

int main()
{
    char todayDateStr[100];
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(todayDateStr, strlen("DD-MMM-YYYY HH:MM") + 1, "%d-%b-%Y %H:%M", timeinfo);
    printf("todayDateStr = %s ... \n", todayDateStr);
}