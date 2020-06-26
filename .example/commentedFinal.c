#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct //MAIN1
{
    int accession;
    char title[40];  // for holding the title of the book
    char author[80]; //for holdinng the name of author of book
    int countTot;    //for holding number of books of same title
    int countAva;
    time_t order;
    char catanatedSearch[127]; //to implement single search string
                               //(accession+title+author)
} book;

typedef struct //supports user
{
    char type;     //issued-i reserved-r
    int accession; //providing accession number to a book
    time_t issue;
} bkRec;

typedef struct //MAIN2 definig the user portal related details
{
    char username[10]; //username of user followed by password
    char password[10];
    bkRec record[5]; //max 5 books issued+reserved 
} user;

// typedef struct //MAIN3
// {
//     char title[40];
//     char ISBN[14];
//     int number;
// } vendor, ILDRS;

void welcome(void);        //return- 0/1 according to user/admin   //!
int mainAdminMenu(void);   //return-choice                         //!
void addRemoveBooks(void); //modify bookmaster.txt                 //!
void addRemoveUsers(void); //modify usermaster.txt                 //!
void inventory(void);      //list particular books and count       //!
void order(void);          //append to orders.txt ILDRS.txt        //? display all orders inside program
int mainUserMenu(void);    //return-choice                         //!
void search(void);         //main search and list                  //!
void bookManagement(void); //issue-return                          //!

//GLOBAL Var
time_t rawtime;
char username[10];
// book tmpBook;

int main()
{
    welcome();
}

/////////////////////////////////////////////////////////////////////////////
void welcome(void)
{
    printf("\e[1;1H\e[2J"); //for clearing the Terminal

    char todayDateStr[15]; //for storing today's date
    struct tm *timeinfo;

    time(&rawtime); //
    timeinfo = localtime(&rawtime);
    strftime(todayDateStr, strlen("DD-MMM-YYYY") + 1, "%d-%b-%Y", timeinfo);

    printf("/n/n***********************************************\n\n");

    printf("			**_Welcome to the library_**      		 \n\n");
    printf("Today is: %s\n", todayDateStr); //todays date
    printf("New arrivals of the week: \n\n");

    FILE *fp;
    fp = fopen("bookMaster.txt", "r"); //opening Bookmaster file in "read" mode
    book tmpBook;

    while (fread(&tmpBook, sizeof(tmpBook), 1, fp) == 1)
    {
        if (7 >= (difftime(rawtime, tmpBook.order) / 86400))
            printf("    %d %s\n        by %s\n\n", tmpBook.accession, tmpBook.title, tmpBook.author); //printing new arrivals from Bookmaster file
    }
    fclose(fp); //closing file Bookmaster

    int flag = 1;
    while (flag == 1)
    {
        // char username[10];
        char password[10];

        printf("\n\nEnter username: "); //asking for username, followed by password
        scanf(" %[^\n]", username);

        printf("Enter password: ");
        system("stty -echo"); //Password won't echo

        scanf(" %[^\n]", password);
        system("stty echo");

        FILE *fs;
        fs = fopen("userMaster.txt", "r"); //opening userMaster file which consist of data of a user
        user tmpUser;

        while (fread(&tmpUser, sizeof(tmpUser), 1, fp) == 1)
        {
            if (!(strcmp(tmpUser.username, username)) && !(strcmp(tmpUser.password, password))) //verifying username and password match
            {
                printf("\n			**Success**			\n");
                flag = 0;
                sleep(1);

                if (!(strcmp(username, "admin"))) //checking whether the user is admin or not
                    mainAdminMenu();
                else
                    mainUserMenu();
            }
        }
        rewind(fs); //to set the pointer to initial position
        fclose(fs);
    }
}

int mainAdminMenu(void) //this is main menu only for admin use
{
    printf("\e[1;1H\e[2J"); //clearTerminal

    printf("\n			**Hi admin**		\n\n");
    printf("Enter 1 for inventory search:\nEnter 2 to place order/ILDRS\n"); //providing options for inentory search or placing order

    printf("Enter 3 to add/remove Books\nEnter 4 to add/remove Users\nEnter 7 to logout\n\n");
    int choice;
    scanf("%d", &choice);

    switch (choice) //comparing the input by admin and performing desired task
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

void addRemoveBooks(void) //handling addition or removal of books
{
    printf("\e[1;1H\e[2J"); //clearTerminal

    printf("			**Add/Remove Books Menu**			 \n\nWhat would you like to do?\n");

    printf("Enter 1 to add book\nEnter 2 to remove book\nEnter 7 to go back\n");
    int choice, flag = 1;
    scanf("%d", &choice);

    while (choice == 1 && flag == 1) //flags to repeat
    {
        FILE *fp;
        fp = fopen("bookMaster.txt", "a+"); //opening Bookmaster file for appending and reading mode

        book tmpBook;
        printf("Enter data:\n");
        tmpBook.accession = latestAccessionNo(); //calling latestAccessionNo() method

        printf("Title: ");
        scanf(" %[^\n]", tmpBook.title);

        printf("Author: ");
        scanf(" %[^\n]", tmpBook.author);

        printf("Total Count:(MAX 9) "); //to simplify editing avaliable books
        scanf("%d", &tmpBook.countTot);

        tmpBook.countAva = tmpBook.countTot; //passing countTot to countAva of tempBook
        tmpBook.order = rawtime;

        snprintf(tmpBook.catanatedSearch, 127, "%s_%s_%d", tmpBook.title, tmpBook.author, tmpBook.accession); //redirecting output of print to catanatedSearch buffer
        fwrite(&tmpBook, sizeof(tmpBook), 1, fp);                                                             //writing the above provided data into tmpBook

        fclose(fp);

        printf("			**Saved successfully**			 \n");
        print("************************************************\n\n");

        printf("Enter 1 to continue: "); //asking user to whether he/she wishes continue with the above process
        scanf("%d", &flag);
    }

    while (choice == 2 && flag == 1)
    {
        int id;

        printf("\nEnter accession no. to delete: ");
        scanf("%d", &id);

        char confirm;

        FILE *fp;
        book tmpBook;

        //char flag='y';
        fp = fopen("bookMaster.txt", "r+");

        while ((fread(&tmpBook, sizeof(tmpBook), 1, fp)) == 1)
        {
            if (tmpBook.accession == id)
            {
                printf("\nTitle is: %s\n    by %s\nConfirm delete?y/n ", tmpBook.title, tmpBook.author);
                scanf(" %c", &confirm);

                while ((confirm == 'y') && (tmpBook.countTot == tmpBook.countAva)) //checking if the book with given title is issued or not
                {
                    FILE *ft;
                    ft = fopen("tempRemove.txt", "w+"); //opening tempRemove file in writing and reading mode

                    rewind(fp); //initialise earlier file for loop i.e. bringing back pointer to initial position

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
        remove("bookMaster.txt"); //deletes the given file so that it is no longer accessible

        rename("tempRemove.txt", "bookMaster.txt"); //renaming 'bookMaster' as new name

        printf("			**Deleted successfully**			.\n");

        printf("Enter 1 to continue: "); //asking user whether he/she wants to continue
        scanf("%d", &flag);
    }

    mainAdminMenu(); //calling mainAdminMenu to ask for another task from user
}

void addRemoveUsers(void)
{
    printf("\e[1;1H\e[2J"); //clearTerminal

    printf("			**Add/Remove Users Menu**			 \n\nWhat would you like to do?\n");

    printf("Enter 1 to add user\nEnter 2 to remove user\nEnter 7 to go back\n");

    int choice, flag = 1;
    scanf("%d", &choice);

    while (choice == 1 && flag == 1) //checking for the task user has given
    {

        user tmpUser1, tmpUser2;

        printf("			**Enter data**			\n");

        printf("Username: "); //asking for the username on which the action has to be performed, followed by password
        scanf(" %[^\n]", tmpUser1.username);

        printf("Password: ");
        scanf(" %[^\n]", tmpUser1.password);

        for (int i = 0; i < 5; i++)
        {
            tmpUser1.record[i].accession = 0; //setting  things to a value
            tmpUser1.record[i].type = 'n';
            tmpUser1.record[i].issue = rawtime;
        }
        ////
        FILE *fp;
        fp = fopen("userMaster.txt", "r+"); //opening file in reading mode

        while (fread(&tmpUser2, sizeof(tmpUser2), 1, fp) == 1)
        {
            if (!(strcmp(tmpUser1.username, tmpUser2.username)))
            {
                printf("User exists! Assigning new password.\n");

                fseek(fp, ftell(fp) - sizeof(tmpUser2), 0); //setting cursor position in the file

                fwrite(&tmpUser1, sizeof(tmpUser2), 1, fp); //writing the data from user to a specified position
            }
            // else()
            //     {
            //         fwrite(&tmpUser1, sizeof(tmpUser1), 1, fp);
            //         fclose(fp);
            //         printf("Saved successfully.\n");
            //         fclose(fp);
            //     }
        }

        fclose(fp);
        ////

        FILE *fs;
        fs = fopen("userMaster.txt", "a+"); //reopening the file in append and reading mode

        fwrite(&tmpUser1, sizeof(tmpUser1), 1, fs);

        fclose(fp);

        printf("			**Saved successfully**			\n");
        ////
        printf("Enter 1 to continue: "); //Flag Modify, asking user whether he/she wants to scontinue
        scanf("%d", &flag);
    }

    while (choice == 2 && flag == 1) //!Functionality check- user has no issued books
    {
        char id[10];

        printf("\nEnter username to delete: ");
        scanf("%s", id);

        char confirm;

        FILE *fp;
        user tmpUser;
        //char flag='y';
        fp = fopen("userMaster.txt", "r+");

        while ((fread(&tmpUser, sizeof(tmpUser), 1, fp)) == 1)
        {
            if (!(strcmp(id, tmpUser.username))) //checking by comparing username and id
            {
                printf("\nUsername is: %s\n			Confirm delete? y/n			", tmpUser.username);
                scanf(" %c", &confirm);

                if (confirm == 'y') //checking user's input, whether y-> yes or n-> no
                {
                    FILE *ft;
                    ft = fopen("tempRemove.txt", "w+");

                    rewind(fp); //initialise for loop

                    while (fread(&tmpUser, sizeof(tmpUser), 1, fp) == 1)
                    {
                        if (strcmp(tmpUser.username, id)) //checking by comparing username with id
                        {
                            fseek(ft, 0, SEEK_CUR);                   //changing the position of cursor in file
                            fwrite(&tmpUser, sizeof(tmpUser), 1, ft); //write temp file
                        }
                    }

                    fclose(fp); //closing both the file
                    fclose(ft);

                    remove("userMaster.txt");
                    rename("tempRemove.txt", "userMaster.txt"); //renaming file tempRemove as userMaster
                    printf("			**Deleted successfully**				\n");
                }
            }
        }

        printf("Enter 1 to continue: ");
        scanf("%d", &flag);
    }

    mainAdminMenu(); //calling mainAdminMenu for futhur tasks
}

void inventory(void)
{
    printf("\e[1;1H\e[2J"); //clearTerminal

    int choice = 0, flag = 1;

    printf("			**Welcome to Book inventory menu**			 \n\n");

    printf("Enter 1 to view by accession no.\nEnter 2 to view all books\nEnter 7 to go back\n"); //asking user choice
    scanf("%d", &choice);

    while (choice == 1 && flag == 1) //insert flags to repeat //if user chose 1
    {
        int input;
        printf("Enter accession no. of book: ");
        scanf("%d", &input);

        FILE *fp;
        fp = fopen("bookMaster.txt", "r"); //opening file in reading mode
        book tmpBook;

        while (fread(&tmpBook, sizeof(tmpBook), 1, fp) == 1) //running process until fill reaches to end
        {
            if (tmpBook.accession == input) //matching accession number to the input by user
                printf("    %d %s\n        by %s    Total Count= %d\n\n", tmpBook.accession, tmpBook.title, tmpBook.author, tmpBook.countTot);
        }
        fclose(fp);

        printf("Enter 1 to continue: "); //asking whether user wants to continue
        scanf("%d", &flag);
    }
    //now if user chose 2
    while (choice == 2 && flag == 1)
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

        printf("Enter 1 to list again: "); //for displaying whole data again
        scanf("%d", &flag);
    }

    mainAdminMenu();
}

void order(void) //TODO: MAKE THE TXT FILE READABLE
{
    printf("\e[1;1H\e[2J"); //clearTerminal

    printf("			**Book order/request menu**				\n\n");

    printf("Enter 1 to place vendor order\nEnter 2 to request book ILDRS\nEnter 7 to go back\n"); //asking for user's choice
    int choice, flag = 1;
    scanf("%d", &choice); //reporting user choice to 'choice' variable

    while (flag == 1) //flags to repeat
    {
        if (choice == 1) //for placing vendor order
            printf("Vendor order:\n\n");
        else
            printf("ILDRS order:\n\n");

        FILE *fp;
        fp = fopen("ordersILDRS.csv", "a+"); //opening file for appending and reading data

        char title[40], ISBN[14], to[80], type[10];
        int noBooks;

        if (choice == 1)
            strcpy(type, "Order");
        else
            strcpy(type, "ILDRS");

        printf("Title: ");
        scanf(" %[^\n]", title);

        printf("ISBN: ");
        scanf(" %[^\n]", ISBN);

        printf("Total Count: ");
        scanf("%d", &noBooks);

        printf("Request to: ");
        scanf(" %[^\n]", to);

        fprintf(fp, "\n%s,%s,%s,%d,%s", type, title, ISBN, noBooks, to); //printing mentioned walues into file pointed by file pointer

        // vendor tmpOrder;
        // printf("Title: ");
        // scanf(" %[^\n]", tmpOrder.title);
        // printf("ISBN: ");
        // scanf(" %[^\n]", tmpOrder.ISBN);
        // printf("Total Count: ");
        // scanf("%d", &tmpOrder.number);
        // fwrite(&tmpOrder, sizeof(tmpOrder), 1, fp);
        fclose(fp);
        printf("			**Saved successfully**			\n");

        ////
        printf("Enter 1 to continue, 7 to exit: "); //asking user whether he/she wanted to place order again
        scanf("%d", &flag);
    }

    // while (choice == 2 && flag == 2)

    // {
    //     printf("ILDRS order:\n\n");
    //     FILE *fp;
    //     fp = fopen("ordersILDRS.csv", "a+");
    //     // vendor tmpOrder;
    //     // printf("Title: ");
    //     // scanf(" %[^\n]", tmpOrder.title);
    //     // printf("ISBN: ");
    //     // scanf(" %[^\n]", tmpOrder.ISBN);
    //     // printf("Total Count: ");
    //     // scanf("%d", &tmpOrder.number);
    //     // fwrite(&tmpOrder, sizeof(tmpOrder), 1, fp);
    //     // fclose(fp);
    //     // printf("Saved successfully.\n");

    //     ////
    //     printf("Enter 1 to continue: ");
    //     scanf("%d", &flag);
    // }

    mainAdminMenu(); //recalling mainAdminMenu
}

int mainUserMenu(void)
{
    printf("\e[1;1H\e[2J"); //clearTerminal

    printf("		Hi %s		\n\n", username);

    user tmpUser; //defing variables of different structures
    book tmpBook;

    FILE *fp;

    char issueDate[15];
    struct tm *issueTime;

    fp = fopen("userMaster.txt", "r+"); //opening file for reading and writting

    printf("			**These books are due**			\n\n"); // red colour overdue

    while (fread(&tmpUser, sizeof(tmpUser), 1, fp))
    {
        if (!(strcmp(tmpUser.username, username)))
        {
            for (int i = 0; i < 5; i++)
            {
                if ((tmpUser.record[i].accession != 0) && tmpUser.record[i].type == 'i')
                {
                    issueTime = localtime(&tmpUser.record[i].issue);

                    strftime(issueDate, strlen("DD-MMM-YYYY") + 1, "%d-%b-%Y", issueTime); //holding date

                    if (15 <= (difftime(rawtime, tmpUser.record[i].issue) / 86400)) //for red colour
                        printf("\033[1;31m");                                       //red colour

                    printf("    Book no. %d - issued %s\n", tmpUser.record[i].accession, issueDate);
                    printf("\033[0m"); //reset colour to default
                }
            }
        }
    }
    rewind(fp); //placing pointer to starting position

    FILE *fs;
    fs = fopen("bookMaster.txt", "r+"); //opening boookMaster in reading format

    printf("			**These books are reserved**			\n\n"); //green colour avaliable to issue
    while (fread(&tmpUser, sizeof(tmpUser), 1, fp))
    {
        if (!(strcmp(tmpUser.username, username)))
        {
            for (int i = 0; i < 5; i++)
            {
                if ((tmpUser.record[i].accession != 0) && tmpUser.record[i].type == 'r')
                {
                    int countBook;

                    rewind(fs);

                    while (fread(&tmpBook, sizeof(tmpBook), 1, fs))
                    {
                        if (tmpUser.record[i].accession == tmpBook.accession)
                            countBook = tmpBook.countAva;
                    }

                    if (countBook > 0)        //checking whether the book is present or not
                        printf("\033[1;32m"); // for green colour

                    printf("    Book no. %d - reserved %s", tmpUser.record[i].accession, issueDate);

                    printf("\033[0m"); //reset colour to default
                }
            }
        }
    }

    // while (fread(&tmpUser, sizeof(tmpUser), 1, fp))
    // {
    // }
    printf("\nEnter 1 to search for a book:\nEnter 2 to issue/return\nEnter 7 to logout\n\n"); //asking for choice of user -> choice
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

void search(void) //searching book
{
    printf("\e[1;1H\e[2J"); //clearTerminal

    printf("			**OneBookSearch**			\n\n");

    printf("Enter search query: "); //asking for the string that has to be searched
    char search[40];
    scanf("%s", search);

    printf("\n");

    int flag = 1;

    FILE *fp;
    fp = fopen("bookMaster.txt", "r"); //now opening file in read mode for searching
    book tmpBook;

    while (flag == 1)
    {
        while (fread(&tmpBook, sizeof(tmpBook), 1, fp) == 1)
        {
            char *pt;

            if ((pt = strcasestr(tmpBook.catanatedSearch, search)) != NULL)                                             //comparing strings while ignoring case of letters
                printf("    \033[4m%d\033[0m %s\n        by %s\n\n", tmpBook.accession, tmpBook.title, tmpBook.author); //printing found result
        }
        fclose(fp); //closing the file

        printf("Pl note the accession no. to manage Book.\n\n");

        printf("Enter 1 to continue search: ");
        scanf("%d", &flag);
    }

    mainUserMenu();
}

void bookManagement(void)
{
    printf("\e[1;1H\e[2J"); //clearTerminal

    printf("			**Book issue/return and reserve/unreserve Menu**			\n\n");

    printf("Enter 1 to issue/reserve book\nEnter 2 to return/unreserve book\nEnter 7 to go back\n"); //asking for the choice of the user
    int choice, flag = 1;
    scanf("%d", &choice); //for holding user's choice

    while (choice == 1 && flag == 1)
    {
        printf("\n			**Issue / Reserve Menu**			\n\n");

        printf("Enter accession no.: ");
        int accNo;
        scanf("%d", &accNo);

        FILE *fp; //opening both the files for reading
        fp = fopen("bookMaster.txt", "r+");

        FILE *fs;
        fs = fopen("userMaster.txt", "r+");

        int arrayPos = 0, avaFlag = 1, bookFlag = 1;
        char choice = 'y'; //default value of choice as 'y' -> yes

        book tmpBook;
        while (fread(&tmpBook, sizeof(tmpBook), 1, fp))
        {
            if (accNo == tmpBook.accession)
            {
                if (tmpBook.countAva > 0)
                {
                    printf("Book avaliable. Issue? "); //asking to issue a book
                    scanf(" %c", &choice);
                }
                else
                {
                    printf("Book not avaliable. Reserve? "); //asking to reserve a book for future
                    scanf(" %c", &choice);

                    bookFlag = 0;
                }
            }
        }

        user tmpUser; //assigning temperory user variable to user structure
        while (fread(&tmpUser, sizeof(tmpUser), 1, fs))
        {
            if (!(strcmp(username, tmpUser.username)))
            {
                for (int i = 0; i < 5; i++)
                {
                    //printf("  %d  ",tmpUser.record[i].accession);

                    if (tmpUser.record[i].accession == 0)
                    {
                        //printf(" %d", arrayPos);
                        avaFlag = 1;
                        break;
                    }
                    arrayPos++;
                }
                //printf(" %d", arrayPos);
                //break;
            }
        }

        rewind(fp); //to place pointer to initial position for both the files
        rewind(fs);

        if (choice == 'y' && avaFlag == 1) //choice was commented
        {
            while (fread(&tmpBook, sizeof(tmpBook), 1, fp) == 1)
            {
                if (tmpBook.accession == accNo)
                {
                    if (tmpBook.countAva > 0)
                    {
                        //printf("\n%d",tmpBook.countAva);
                        tmpBook.countAva--;
                        //printf("\n%d",tmpBook.countAva);
                        fseek(fp, ftell(fp) - sizeof(tmpBook), 0); //bring pointer to desired position from 0th position
                        fwrite(&tmpBook, sizeof(tmpBook), 1, fp);  //writing content of tempBook at position equal to sizeof(tempBook)
                    }
                }
            }

            // rewind(fp);
            // while (fread(&tmpBook, sizeof(tmpBook), 1, fp) == 1)
            // {
            //     if (tmpBook.accession == accNo)
            //     {
            //         printf("\n%d\n",tmpBook.countAva);
            //     }
            // }

            while (fread(&tmpUser, sizeof(tmpUser), 1, fs) == 1)
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

            // rewind(fs);

            // while (fread(&tmpUser, sizeof(tmpUser), 1, fs) == 1)
            // {
            //     if (!(strcmp(tmpUser.username, username)))
            //     {
            //         printf(" tmp user accession %d",tmpUser.record[arrayPos].accession);
            //         printf("%c",tmpUser.record[arrayPos].type);

            //     }
            // }

            fclose(fp); //closing both the files
            fclose(fs);
        }

        ////
        printf("Enter 1 to continue: "); //asking whether user wants to continue
        scanf("%d", &flag);
    }

    // fseek(fp, ftell(fp) - sizeof(tmpUser2), 0);     //TODO: NOTE
    // fwrite(&tmpUser1, sizeof(tmpUser2), 1, fp);     //EDIT STRUCT USING THIS

    while (choice == 2 && flag == 1) //when user chose 2 as a choice -> reserve or unreserve a book
    {
        printf("Enter accession no.: "); //asking user for accession number
        int accNo;
        scanf("%d", &accNo);

        FILE *fp;
        fp = fopen("bookMaster.txt", "r+"); //openig both files in reading mode

        FILE *fs;
        fs = fopen("userMaster.txt", "r+");

        int arrayPos = 0, avaFlag = 0, bookFlag = 1;
        char choice;

        user tmpUser; //assingning tmpUser to user structre
        while (fread(&tmpUser, sizeof(tmpUser), 1, fs))
        {
            if (!(strcmp(username, tmpUser.username)))
            {
                for (int i = 0; i < 5; i++)
                {
                    if (tmpUser.record[i].accession == accNo)
                    {
                        if (tmpUser.record[i].type == 'r')
                            bookFlag = 0;

                        tmpUser.record[i].accession = 0;
                        tmpUser.record[i].type = 'n';
                        tmpUser.record[i].issue = rawtime;

                        fseek(fs, ftell(fs) - sizeof(tmpUser), 0);
                        fwrite(&tmpUser, sizeof(tmpUser), 1, fs);

                        avaFlag = 1;
                        break;
                    }
                    //arrayPos+=1;
                }
            }
        }

        book tmpBook; //assigning tmpBook to book structure

        while (fread(&tmpBook, sizeof(tmpBook), 1, fp))
        {
            if (accNo == tmpBook.accession && avaFlag == 1)
            {
                if (bookFlag)
                    tmpBook.countAva++;
                //bookFlag=0;

                fseek(fp, ftell(fp) - sizeof(tmpBook), 0); //bring pointer to desired position from 0th position
                fwrite(&tmpBook, sizeof(tmpBook), 1, fp);  //writing content of tempBook at position equal to sizeof(tempBook)
            }
        }

        if (avaFlag == 1)
            printf("			**book returned**			\n");

        fclose(fp); //closing both the files
        fclose(fs);

        printf("Enter 1 to continue: "); //asking whether user wants to continue or not
        scanf("%d", &flag);
    }

    mainUserMenu();
}
//Supporting macros
int latestAccessionNo(void)
{
    FILE *fp;
    fp = fopen("bookMaster.txt", "r"); //opening bookMaster in reading mode

    int accession = 0;
    book tmpBook;

    while (fread(&tmpBook, sizeof(tmpBook), 1, fp) == 1)
    {
        if (accession < tmpBook.accession)
            accession = tmpBook.accession; //Assign Highest accession no.
    }
    fclose(fp); //closing file
    return (accession + 1);
}