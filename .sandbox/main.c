#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct //MAIN1
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

typedef struct //book record of User
{
    char type; //issued-i reserved-r
    int accession;
    time_t issue;
} bkRec;

typedef struct //MAIN2
{
    char username[10];
    char password[10];
    bkRec record[5]; //max 5 books issued+reserved 
} user;

void welcome(void);        //move on according to user/admin   
int mainAdminMenu(void);   //return-choice                         
void addRemoveBooks(void); //modify bookmaster.txt                 
void addRemoveUsers(void); //modify usermaster.txt                 
void inventory(void);      //list particular books and count       
void order(void);          //append to ordersILDRS.csv        
int mainUserMenu(void);    //return-choice                         
void search(void);         //main search and list                  
void bookManagement(void); //main issue-return                          

//GLOBAL Var
time_t rawtime;
char username[10];
//////////////////////////////////////////////////////////////////////////////

int main()
{
    welcome();
}

//////////////////////////////////////////////////////////////////////////////
void welcome(void)
{
    printf("\e[1;1H\e[2J"); //clearTerminal
    //////
    char todayDateStr[15];  //for storing today's date
    struct tm *timeinfo;
    time(&rawtime);         //store current time
    timeinfo = localtime(&rawtime);
    strftime(todayDateStr, strlen("DD-MMM-YYYY") + 1, "%d-%b-%Y", timeinfo);    //convert current time to string
    //////

    printf("			**_Welcome to the library_**      		 \n\n");    printf("Today is %s\n", todayDateStr); //todays date
    printf("New additions of the week: \n\n");

    //List new books added in last 7 days
    FILE *fp;                                   
    fp = fopen("bookMaster.txt", "r");
    book tmpBook;
    while (fread(&tmpBook, sizeof(tmpBook), 1, fp) == 1)
    {
        if (7 >= (difftime(rawtime, tmpBook.order) / 86400))
            printf("    %d %s\n        by %s\n\n", tmpBook.accession, tmpBook.title, tmpBook.author);
    }
    fclose(fp);

    int flag = 1;
    while (flag == 1)
    {
        char password[10];
        printf("\n\nEnter username: ");
        scanf(" %[^\n]", username);
        printf("Enter password: ");
        system("stty -echo"); //Password won't echo
        scanf(" %[^\n]", password);
        system("stty echo");

        //check username and password
        FILE *fs;
        fs = fopen("userMaster.txt", "r");
        user tmpUser;
        while (fread(&tmpUser, sizeof(tmpUser), 1, fp) == 1)
        {
            if (!(strcmp(tmpUser.username, username)) && !(strcmp(tmpUser.password, password)))
            {
                printf("\n			**Success**			\n");
                flag = 0;
                sleep(1);

                if (!(strcmp(username, "admin")))   //Admin and user differentiate
                    mainAdminMenu();
                else
                    mainUserMenu();
            }
        }
        rewind(fs); //not nessary
        fclose(fs);
    }
}

int mainAdminMenu(void)     //Main menu for admin uses
{
    printf("\e[1;1H\e[2J"); //clearTerminal

    printf("\n			**Hi admin**		\n\n");
    printf("Enter 1 for inventory search:\nEnter 2 to place order/ILDRS\n");
    printf("Enter 3 to add/remove Books\nEnter 4 to add/remove Users\nEnter 7 to logout\n\n");
    int choice;
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        inventory();
        break;
    case 2:
        order();
        break;
    case 3:
        addRemoveBooks();
        break;
    case 4:
        addRemoveUsers();
        break;
    default:
        welcome();
    }
}

void addRemoveBooks(void)   //handling addition or removal of books
{
    printf("\e[1;1H\e[2J"); //clearTerminal

    printf("			**Add/Remove Books Menu**			 \n\nWhat would you like to do?\n");
    printf("Enter 1 to add book\nEnter 2 to remove book\nEnter 7 to go back\n\n");
    int choice, flag = 1;
    scanf("%d", &choice);

    while (choice == 1 && flag == 1) //ADD BOOK
    {
        FILE *fp;
        fp = fopen("bookMaster.txt", "a+");
        ////
        book tmpBook;
        printf("Enter data:\n");
        tmpBook.accession = latestAccessionNo();    //Assign latest accessionNo.
        printf("Title: ");
        scanf(" %[^\n]", tmpBook.title);
        printf("Author: ");
        scanf(" %[^\n]", tmpBook.author);
        printf("Total Count: "); 
        scanf("%d", &tmpBook.countTot);
        tmpBook.countAva = tmpBook.countTot;
        tmpBook.order = rawtime;
        //Create special catanated string for search purpose
        snprintf(tmpBook.catanatedSearch, 127, "%s_%s_%d", tmpBook.title, tmpBook.author, tmpBook.accession);
        
        fwrite(&tmpBook, sizeof(tmpBook), 1, fp);
        fclose(fp);
        printf("			**Saved successfully**			 \n");
        ////
        printf("Enter 1 to add more,7 for back: ");
        scanf("%d", &flag);
    }

    while (choice == 2 && flag == 1)    //REMOVE BOOK   
    {
        int id;
        printf("\nEnter accession no. to delete: ");
        scanf("%d", &id);
        char confirm;

        FILE *fp;
        book tmpBook;
        fp = fopen("bookMaster.txt", "r+");
        
        while ((fread(&tmpBook, sizeof(tmpBook), 1, fp)) == 1)
        {
            if (tmpBook.accession == id)
            {
                printf("\nTitle is: %s\n    by %s\nConfirm delete?y/n ", tmpBook.title, tmpBook.author);
                scanf(" %c", &confirm);

                while ( (confirm == 'y') && (tmpBook.countTot == tmpBook.countAva) ) //check if no books of title are issued
                {
                    FILE *ft;
                    ft = fopen("tempRemove.txt", "w+");
                    rewind(fp); //initialise earlier file for loop

                    while (fread(&tmpBook, sizeof(tmpBook), 1, fp) == 1)
                    {
                        if (tmpBook.accession != id)
                        {
                            fseek(ft, 0, SEEK_CUR);
                            fwrite(&tmpBook, sizeof(tmpBook), 1, ft); //write temp file
                        }
                    }

                    confirm = 'n';
                    fclose(ft);
                }
            }
        }
        fclose(fp);
        //Using 2 file Updation
        remove("bookMaster.txt");
        rename("tempRemove.txt", "bookMaster.txt");
        printf("Deleted.\n");
        ////
        printf("Enter 1 to remove more,7 for back: ");
        scanf("%d", &flag);
    }

    mainAdminMenu();
}

void addRemoveUsers(void)   //handling addition or removal of users
{
    printf("\e[1;1H\e[2J"); //clearTerminal

    printf("			**Add/Remove Users Menu**			 \n\nWhat would you like to do?\n");
    printf("Enter 1 to add user\nEnter 2 to remove user\nEnter 7 to go back\n\n");
    int choice, flag = 1;
    scanf("%d", &choice);

    while (choice == 1 && flag == 1) //flags to repeat
    {

        user tmpUser1, tmpUser2;    //1 for input 2 for
        printf("Enter data:\n");
        printf("Username: ");
        scanf(" %[^\n]", tmpUser1.username);
        printf("Password: ");
        scanf(" %[^\n]", tmpUser1.password);

        for (int i = 0; i < 5; i++)     //Initialise user records
        {
            tmpUser1.record[i].accession = 0;
            tmpUser1.record[i].type = 'n';
            tmpUser1.record[i].issue = rawtime;
        }
        //IF USERNAME ALREADY EXISTS
        FILE *fp;
        fp = fopen("userMaster.txt", "r+");
        while (fread(&tmpUser2, sizeof(tmpUser2), 1, fp) == 1)
        {
            if (!(strcmp(tmpUser1.username, tmpUser2.username)))
            {
                printf("User exists! Assigning new password.\n"); //Functionality to change password

                fseek(fp, ftell(fp) - sizeof(tmpUser2), 0); 
                fwrite(&tmpUser1, sizeof(tmpUser2), 1, fp);
                
            }
            
        }

        fclose(fp);
        //IF USERNAME IS NEW

        FILE *fs;
        fs = fopen("userMaster.txt", "a+");
        fwrite(&tmpUser1, sizeof(tmpUser1), 1, fs);
        fclose(fp);

        printf("			**Saved successfully**			\n");
        ////
        printf("Enter 1 add more,7 for back: "); //Flag Modify
        scanf("%d", &flag);
    }

    while (choice == 2 && flag == 1)    //!DOESNT check if user has issued books
    {
        char id[10];
        printf("\nEnter username to delete: ");
        scanf("%s", id);
        
        char confirm;
        FILE *fp;
        user tmpUser;
        
        fp = fopen("userMaster.txt", "r+");
        while ((fread(&tmpUser, sizeof(tmpUser), 1, fp)) == 1)
        {
            if (!(strcmp(id, tmpUser.username)))
            {
                printf("\n      Username is: %s\nConfirm delete?y/n ", tmpUser.username);
                scanf(" %c", &confirm);
                if (confirm == 'y')
                {
                    FILE *ft;
                    ft = fopen("tempRemove.txt", "w+");     //create temp file 
                    rewind(fp); //initialise for loop
                    while (fread(&tmpUser, sizeof(tmpUser), 1, fp) == 1)
                    {
                        if (strcmp(tmpUser.username, id))
                        {
                            fseek(ft, 0, SEEK_CUR);
                            fwrite(&tmpUser, sizeof(tmpUser), 1, ft); //write temp file
                        }
                    }

                    fclose(fp);
                    fclose(ft);
                    remove("userMaster.txt");
                    rename("tempRemove.txt", "userMaster.txt");
                    printf("			**Deleted successfully**				\n");
                }
            }
        }

        ////
        printf("Enter 1 to remove more,7for back: ");
        scanf("%d", &flag);
    }
    
    mainAdminMenu();
}

void inventory(void)
{
    printf("\e[1;1H\e[2J"); //clearTerminal
    
    int choice = 0, flag = 1;
    printf("			**Welcome to Book inventory menu**			 \n\n");
    printf("Enter 1 to view by accession no.\nEnter 2 to view all books\nEnter 7 to go back\n\n");
    scanf("%d", &choice);

    while (choice == 1 && flag == 1) //View by accession
    {
        int input;
        printf("Enter accession no. of book: ");
        scanf("%d", &input);
        FILE *fp;
        fp = fopen("bookMaster.txt", "r");
        book tmpBook;
        while (fread(&tmpBook, sizeof(tmpBook), 1, fp) == 1)    //running loop until EOF
        {
            if (tmpBook.accession == input) //if accession matches
                printf("    %d %s\n        by %s    Total Count= %d\n\n", tmpBook.accession, tmpBook.title, tmpBook.author, tmpBook.countTot);
        }
        fclose(fp);

        printf("Enter 1 to search more,7 for back: ");
        scanf("%d", &flag);
    }

    while (choice == 2 && flag == 1)    //View all books
    {
        printf("Listing all books: \n\n");
        FILE *fp;
        fp = fopen("bookMaster.txt", "r");
        book tmpBook;
        while (fread(&tmpBook, sizeof(tmpBook), 1, fp) == 1)
        {
            printf("    %d %s\n        by %s\n        Total Count= %d\n\n", tmpBook.accession, tmpBook.title, tmpBook.author, tmpBook.countTot);
        }
        fclose(fp);
        
        printf("Enter 1 to list again, 7 for back: ");
        scanf("%d", &flag);
    }

    
    mainAdminMenu();
}

void order(void) 
{
    printf("\e[1;1H\e[2J"); //clearTerminal
    
    printf("			**Book order/request menu**				\n\n");
    printf("Enter 1 to place vendor order\nEnter 2 to request book ILDRS\nEnter 7 to go back\n\n");
    int choice, flag = 1;
    scanf("%d", &choice);

    while (flag == 1) //flags to repeat
    {
        if(choice==1)
            printf("Vendor order:\n\n");
        else printf("ILDRS order:\n\n");

        FILE *fp;
        fp = fopen("ordersILDRS.csv", "a+");    //opening csv file

        char title[40],ISBN[14],to[80],type[10];
        int noBooks;

        if(choice==1)
            strcpy(type, "Order");
        else strcpy(type, "ILDRS");

        printf("Title: ");
        scanf(" %[^\n]", title);
        printf("ISBN: ");
        scanf(" %[^\n]",ISBN);
        printf("Total Count: ");
        scanf("%d", &noBooks);
        printf("Request to: ");
        scanf(" %[^\n]",to);

        fprintf(fp,"\n%s,%s,%s,%d,%s",type,title,ISBN,noBooks,to);  //print in csv format
        
        fclose(fp);
        printf("Saved successfully.\n");

        ////
        printf("Enter 1 to add more, 7 to exit: ");
        scanf("%d", &flag);
    }

    mainAdminMenu();
}

int mainUserMenu(void)
{
    printf("\e[1;1H\e[2J"); //clearTerminal
    
    printf("        Hi %s\n\n", username);
    user tmpUser;   //initialise variables
    book tmpBook;
    FILE *fp;

    char issueDate[15]; //date related
    struct tm *issueTime;

    fp = fopen("userMaster.txt", "r+");

    printf("These books are due:\n\n"); // red colour overdue
    while (fread(&tmpUser, sizeof(tmpUser), 1, fp))
    {
        if (!(strcmp(tmpUser.username, username)))
        {
            for (int i = 0; i < 5; i++)
            {
                if ((tmpUser.record[i].accession != 0) && tmpUser.record[i].type =='i')
                {
                    issueTime = localtime(&tmpUser.record[i].issue);
                    strftime(issueDate, strlen("DD-MMM-YYYY") + 1, "%d-%b-%Y", issueTime);
                    ////////
                    
                    if (15 <= (difftime(rawtime, tmpUser.record[i].issue) / 86400))   //If time difference is more than 15 days
                        printf("\033[1;31m");//red colour
                    
                    printf("    Book no. %d - issued %s\n", tmpUser.record[i].accession, issueDate);
                    printf("\033[0m");  //reset colour
                }
            }
        }
    }
    rewind(fp); //reset User pointer

    FILE *fs;
    fs = fopen("bookMaster.txt", "r+");

    printf("\nThese books are reserved:\n\n"); //green colour avaliable to issue
    while (fread(&tmpUser, sizeof(tmpUser), 1, fp))
    {
        if (!(strcmp(tmpUser.username, username)))
        {
            for (int i = 0; i < 5; i++)
            {
                if ((tmpUser.record[i].accession != 0) && tmpUser.record[i].type == 'r')    //if book is reserved
                {
                    int countBook;
                    rewind(fs);
                    while (fread(&tmpBook, sizeof(tmpBook), 1, fs))
                    {
                        if (tmpUser.record[i].accession == tmpBook.accession)   //note avaliable count of book
                            countBook = tmpBook.countAva;
                    }

                    if (countBook > 0)      //if book is avaliable
                        printf("\033[1;32m");    //green colour
                    printf("    Book no. %d - reserved %s\n", tmpUser.record[i].accession, issueDate);
                    printf("\033[0m");  //reset colour
                }
            }
        }
    }

    
    printf("\nEnter 1 to search for a book:\nEnter 2 to issue/return\nEnter 7 to logout\n\n");
    int choice;
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            search();
            break;
        case 2:
            bookManagement();
            break;
        default:
            welcome();
    }
}

void search(void)
{
    printf("\e[1;1H\e[2J"); //clearTerminal
    
    printf("			**OneBookSearch**			\n\n");

    printf("Enter search query: ");
    char search[40];
    scanf("%s", search);
    printf("\n");
    int flag=1;
    
    FILE *fp;
    fp = fopen("bookMaster.txt", "r");
    book tmpBook;

    while (flag == 1)
    {
        while (fread(&tmpBook, sizeof(tmpBook), 1, fp) == 1)
        {
            char *pt;
            if ((pt = strcasestr(tmpBook.catanatedSearch, search)) != NULL)     //Do a case insensitive search
                printf("    \033[4m%d\033[0m %s\n        by %s\n\n", tmpBook.accession, tmpBook.title, tmpBook.author); //Underline the accession number
        }
        fclose(fp);

        printf("Pl note the accession no. to manage Book.\n\n");

        printf("Enter 1 to continue search,7 to go back: ");
        scanf("%d", &flag);
    }
    
    mainUserMenu();
}

void bookManagement(void)
{
    printf("\e[1;1H\e[2J"); //clearTerminal
    
    printf("			**Book issue/return and reserve/unreserve Menu**			\n\n");
    printf("Enter 1 to issue/reserve book\nEnter 2 to return/unreserve book\nEnter 7 to go back\n\n");
    int choice, flag = 1;
    scanf("%d", &choice);

    while (choice == 1 && flag == 1)    //Issue reserve
    {
        printf("\n  Issue / Reserve Menu:\n\n");
        printf("Enter accession no.: ");
        int accNo;
        scanf("%d", &accNo);

        FILE *fp;
        fp = fopen("bookMaster.txt", "r+");

        FILE *fs;
        fs = fopen("userMaster.txt", "r+");

        int arrayPos = 0, avaFlag = 1, bookFlag = 1;
        char choice='y';

        book tmpBook;       //CHECK BOOK AVALIABLITY
        while (fread(&tmpBook, sizeof(tmpBook), 1, fp)) 
        {
            if (accNo == tmpBook.accession)
            {
                if (tmpBook.countAva > 0)
                {
                    printf("Book avaliable. Issue? ");
                    scanf(" %c",&choice);
                }
                else
                {
                    printf("Book not avaliable. Reserve? ");
                    scanf(" %c", &choice);

                    bookFlag = 0;
                }
            }
        }

        user tmpUser;       //CHECK IF USER HAS FREE SPACE return empty array space
        while (fread(&tmpUser, sizeof(tmpUser), 1, fs))
        {
            if (!(strcmp(username, tmpUser.username)))
            {
                for (int i = 0; i < 5; i++)
                {
                                      
                    if (tmpUser.record[i].accession == 0)
                    {
                        
                        avaFlag = 1;
                        break;
                    }
                    arrayPos++;
                }
                
            }
        }
        
        ///////////
        rewind(fp); //reset file filepointers
        rewind(fs);

        if (choice == 'y' && avaFlag == 1)      
        {
            while (fread(&tmpBook, sizeof(tmpBook), 1, fp) == 1)    //Decrease Avaliable Book count
            {
                if (tmpBook.accession == accNo)
                {
                    if (tmpBook.countAva > 0)
                    {
                        
                        tmpBook.countAva--;
                        
                        fseek(fp, ftell(fp) - sizeof(tmpBook), 0);  //Update By overwriting previous structure
                        fwrite(&tmpBook, sizeof(tmpBook), 1, fp);
                    }
                }
            }

            while (fread(&tmpUser, sizeof(tmpUser), 1, fs) == 1)    //Record book in User record
            {
                if (!(strcmp(tmpUser.username, username)))
                {
                    tmpUser.record[arrayPos].accession = accNo;
                    tmpUser.record[arrayPos].issue = rawtime;
                    if (bookFlag)
                        tmpUser.record[arrayPos].type = 'i';
                    else
                        tmpUser.record[arrayPos].type = 'r';

                    fseek(fs, ftell(fs) - sizeof(tmpUser), 0);
                    fwrite(&tmpUser, sizeof(tmpUser), 1, fs);
                }
            }

            fclose(fp);
            fclose(fs);
        }

        ////
        printf("Enter 1 to issue more,7 to go back: ");
        scanf("%d", &flag);
    }
    
    while (choice == 2 && flag == 1)    //Return unreserve
    {
        printf("Enter accession no.: ");
        int accNo;
        scanf("%d", &accNo);

        FILE *fp;
        fp = fopen("bookMaster.txt", "r+");

        FILE *fs;
        fs = fopen("userMaster.txt", "r+");

        int arrayPos = 0, avaFlag = 0, bookFlag = 1;
        char choice;

        user tmpUser;   //reset book record and overwrite structure
        while (fread(&tmpUser, sizeof(tmpUser), 1, fs))
        {
            if (!(strcmp(username, tmpUser.username)))
            {
                for (int i = 0; i < 5; i++)
                {
                    if (tmpUser.record[i].accession == accNo)
                    {
                        if(tmpUser.record[i].type == 'r')   //note if book was reserved
                            bookFlag=0;
                        
                        tmpUser.record[i].accession = 0;
                        tmpUser.record[i].type = 'n';
                        tmpUser.record[i].issue = rawtime;

                        fseek(fs, ftell(fs) - sizeof(tmpUser), 0);
                        fwrite(&tmpUser, sizeof(tmpUser), 1, fs);

                        avaFlag = 1;    //note if book was returned
                        break;
                    }
                    
                }
            }
        }

        //Increment avaliable book count if issued book was returned
        book tmpBook;
        while (fread(&tmpBook, sizeof(tmpBook), 1, fp)) 
        {
            if (accNo == tmpBook.accession && avaFlag == 1)
            {
                if (bookFlag)
                    tmpBook.countAva++;
                
                fseek(fp, ftell(fp) - sizeof(tmpBook), 0);
                fwrite(&tmpBook, sizeof(tmpBook), 1, fp);
            }
        }

        if(avaFlag == 1)
            printf("			**Book returned**			\n");

        fclose(fp);
        fclose(fs);
        ////
        printf("Enter 1 to return more,7 for back: ");
        scanf("%d", &flag);
    }

    mainUserMenu();
}
//////////////
//Supporting macros
int latestAccessionNo(void)
{
    FILE *fp;
    fp = fopen("bookMaster.txt", "r");
    int accession = 0;
    book tmpBook;
    while (fread(&tmpBook, sizeof(tmpBook), 1, fp) == 1)
    {
        if (accession < tmpBook.accession)
            accession = tmpBook.accession; //Assign Highest accession no.
    }
    fclose(fp);
    return (accession + 1);
}