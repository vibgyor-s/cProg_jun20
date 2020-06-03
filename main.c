#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct  //MAIN1
{
    char accession[7]; //or INT?
    char title[40];
    char author[80];
    char ISBN[14]; //Not needed
    int countTot;
    int countAva;
    time_t order;
    char catanatedSearch[127]; //to implement single search string
                               //(accession+title+author)
} book;

typedef struct  //supports user 
{
    char type;  //issued-i reserved-r
    char accession[7];
    time_t issue;
}bkRec;

typedef struct  //MAIN2
{
    char username[10];
    char password[10];
    bkRec record[5];    //max 5 books issued+reserved PROBLEMS?
}user;

typedef struct  //MAIN3
{
    char title[40];
    char ISBN[14];
    int number;
}vendor,ILDRS;

int welcome(void);              //return- 0/1 according to user/admin
int mainUserMenu(void);         //return-choice
    void search(void);          //main search and list
    void bookManagement(void);  //issue-return
int mainAdminMenu(void);        //return-choice
    void addRemove(void);       //modify bookmaster.txt usermaster.txt
    void inventory (void);      //list particular books and count
    void order(void);           //append to orders.txt ILDRS.txt 

int main()
{

}