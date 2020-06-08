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

time_t rawtime;
// book tmpBook;
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
int latestAccessionNo(void)
    {
        FILE *fp;
        fp = fopen("bookMaster.txt", "r");
        book tmpBook;
        int accession=0;
        while (fread(&tmpBook, sizeof(tmpBook), 1, fp) == 1)
        {
            if(accession<tmpBook.accession)
                accession=tmpBook.accession;    //Assign Highest accession no.
        }
        fclose(fp);
        return(accession+1);    
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
            scanf("%d",&tmpBook.countTot);
            tmpBook.countAva= tmpBook.countTot;
            tmpBook.order=rawtime;
            snprintf(tmpBook.catanatedSearch, 127, "%s_%s_%d",tmpBook.title, tmpBook.author,tmpBook.accession);
            fwrite(&tmpBook, sizeof(tmpBook), 1, fp);
            fclose(fp);
            printf("Saved successfully.\n");
            ////
            printf("Enter 1 to continue: ");
            scanf("%d",&flag);
        }
    }

int main()
{
    welcome();
    addRemoveBooks();
    book tmpBook;
    FILE *fp;
    fp = fopen("bookMaster.txt", "r+");
    struct tm *timeinfo;
    
    while (fread(&tmpBook, sizeof(tmpBook), 1, fp) == 1)
        {
            printf("%d %s %s\n%d %d\n%s\n",tmpBook.accession,tmpBook.title,tmpBook.author,tmpBook.countTot,tmpBook.countAva,tmpBook.catanatedSearch);
            timeinfo = localtime(&tmpBook.order);
            char todayDateStr[15];
            strftime(todayDateStr, strlen("DD-MMM-YYYY") + 1, "%d-%b-%Y", timeinfo);
            printf("orderdate = %s\n", todayDateStr);
        }
    fclose(fp);
    
    
}

