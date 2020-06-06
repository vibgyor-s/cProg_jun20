#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct  //MAIN1
{
    int accession; //?or char
    char title[40];
    char author[80];
//    char ISBN[14]; //Not needed
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
int mainAdminMenu(void);        //return-choice
    void addRemoveBooks(void);  //modify bookmaster.txt 
    void addRemoveUsers(void);  //modify usermaster.txt
    void inventory (void);      //list particular books and count
    void order(void);           //append to orders.txt ILDRS.txt 
int mainUserMenu(void);         //return-choice
    void search(void);          //main search and list
    void bookManagement(void);  //issue-return

//GLOBAL Var
time_t rawtime;
// book tmpBook;
//////////////////////////////////////////////////////////////////////////////

int main()
{

}



//////////////////////////////////////////////////////////////////////////////
int welcome(void)
{
    //////
    char todayDateStr[15];
    struct tm *timeinfo;
    time(&rawtime);     //!
    timeinfo = localtime(&rawtime);
    strftime(todayDateStr, strlen("DD-MMM-YYYY") + 1, "%d-%b-%Y", timeinfo);
    //////

    printf("Welcome to the library\n\n");
    printf("Today is %s", todayDateStr);    //todays date
    printf("New additions of the week: \n");

    char username[10];
    char password[10];
    printf("\nEnter username: ");
    scanf(" %[^\n]",username);
    printf("Enter password: ");
    scanf(" %[^\n]",password);



}

int mainAdminMenu(void)
{
    printf("Hi admin\n");
    printf("Enter 1 for inventory search:\nEnter 2 to place order/ILDRS\n");
    printf("Enter 3 to add/remove Books\nEnter 4 to add/remove Users\nEnter 7 to logout\n\n");
}
    void addRemoveBooks(void)
    {
        printf("Add/Remove Books Menu: \n\nWhat would you like to do?\n");
        printf("Enter 1 to add book\nEnter 2 to remove book\nEnter 7 to go back\n");
        int choice,flag=1;
        scanf("%d",&choice);
        while(choice==1 && flag==1)   //flags to repeat
        {
            FILE *fp;
            fp = fopen("bookMaster.txt", "a+");
            ////
            book tmpBook;
            printf("Enter data:\n");
            tmpBook.accession=latestAccessionNo();
            printf("Title: ");
            scanf(" %[^\n]",tmpBook.title);
            printf("Author: ");
            scanf(" %[^\n]",tmpBook.author);
            printf("Total Count:(MAX 9) ");     //to simplify editing avaliable books
            scanf(" %d",tmpBook.countTot);
            tmpBook.countAva= tmpBook.countTot;
            tmpBook.order=rawtime;
            snprintf(tmpBook.catanatedSearch, 127, "%s_%s_%d",tmpBook.title, tmpBook.author,tmpBook.accession);

            printf("Enter 1 to continue: ");
            scanf("%d",&flag);
        }
        while(choice==2 && flag==1)
        {

        }
        if(choice==7)   
    }

    void addRemoveUsers(void)
    {   
        printf("Add/Remove Users Menu: \n\nWhat would you like to do?\n");
        printf("Enter 1 to add user\nEnter 2 to remove user\nEnter 7 to go back\n");
        int choice;
        scanf("%d",&choice);
        if(1)   //flags to repeat
        if(2)
        if(7)   
    }

    void inventory (void)
    {
        printf("Book inventory menu: \n\n");
        printf("Enter 1 to view by accession no.\nEnter 2 to view all books\nEnter 7 to go back\n");
        scanf("%d",&choice);

        if(1)   //insert flags to repeat
            printf("Enter accession no. of book: (0 to display all books) ");
            scanf("%d",&input);
        if(2)
        if(7)
    }

    void order(void)
    {
        printf("Book order/request menu:\n\n");
        printf("Enter 1 to place vendor order\nEnter 2 to request book ILDRS\nEnter 7 to go back\n");
        scanf("%d",&choice);

        if(1)   //flags to repeat
        if(2)
        if(7)
    }


int mainUserMenu(void)
{
    printf("Hi %s\n");
    printf("These books are due:\n");    // red colour overdue
    printf("These books are reserved:\n");  //green colour avaliable to issue

    printf("Enter 1 to search for a book:\nEnter 2 to issue/return\nEsc. to logout\n\n");

}

    void search(void)
    {
        printf("OneBookSearch\n\n");
        printf("Enter search query: (Esc. to go back) ");
        scanf("%s");

    }

    void bookManagement(void)
    {
        printf("Book issue/return and reserve/unreserve Menu:\n\n");
        printf("Enter 1 to issue/reserve book\nEnter 2 to return/unreserve book\nEnter 7 to go back\n")
        scanf("%d",&choice);

    }

    //////////////
    //Supporting macros
    int latestAccessionNo(void)
    {
        FILE *fp;
        fp = fopen("bookMaster.txt", "r");
        int accession=0;
        while (fread(&tmpBook, sizeof(tmpBook), 1, fp) == 1)
        {
            if(accession<tmpBook.accession)
                accession=tmpBook.accession;    //Assign Highest accession no.
        }
        fclose(fp);
        return(accession+1);    
    }