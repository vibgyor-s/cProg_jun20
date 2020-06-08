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

void welcome(void);              //return- 0/1 according to user/admin  //!
int mainAdminMenu(void);        //return-choice                         //!
    void addRemoveBooks(void);  //modify bookmaster.txt                 //!
    void addRemoveUsers(void);  //modify usermaster.txt                 //!    
    void inventory (void);      //list particular books and count       //!
    void order(void);           //append to orders.txt ILDRS.txt        
int mainUserMenu(void);         //return-choice
    void search(void);          //main search and list
    void bookManagement(void);  //issue-return

//GLOBAL Var
time_t rawtime;
char username[10];
// book tmpBook;
//////////////////////////////////////////////////////////////////////////////

int main()
{
    welcome();
}



//////////////////////////////////////////////////////////////////////////////
void welcome(void)
{
    //////
    char todayDateStr[15];
    struct tm *timeinfo;
    time(&rawtime);     //!
    timeinfo = localtime(&rawtime);
    strftime(todayDateStr, strlen("DD-MMM-YYYY") + 1, "%d-%b-%Y", timeinfo);
    //////

    printf("Welcome to the library\n\n");
    printf("Today is %s\n", todayDateStr);    //todays date
    printf("New additions of the week: \n\n");
    FILE *fp;
        fp = fopen("bookMaster.txt", "r");
        book tmpBook;
        while (fread(&tmpBook, sizeof(tmpBook), 1, fp) == 1)
        {
            if(7 >= (difftime(rawtime,tmpBook.order) / 86400))
                printf("    %d %s\n        by %s\n\n",&tmpBook.accession,tmpBook.title,tmpBook.author);
        }
        fclose(fp);

    int flag=1;
    while(flag==1)
    {
        // char username[10];
        char password[10];
        printf("\n\nEnter username: ");
        scanf(" %[^\n]",username);
        printf("Enter password: ");
        system("stty -echo");       //Password won't echo
        scanf(" %[^\n]",password);
        system("stty echo");
        
        FILE *fs;
            fs = fopen("userMaster.txt", "r");
            user tmpUser;
            while (fread(&tmpUser, sizeof(tmpUser), 1, fp) == 1)
            {
                if(!(strcmp(tmpUser.username,username)) && !(strcmp(tmpUser.password,password)))
                    {
                        printf("Success");
                        flag = 0;
                        sleep(1);
                        if(!(strcmp(username,"admin")))
                            mainAdminMenu();
                        else mainUserMenu();
                    }
            }
        fclose(fs);
    }


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
            printf("Total Count:(MAX 9) ");     //?to simplify editing avaliable books
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

        while(choice==2 && flag==1)
        {
            int id;
            printf("\nEnter accession no. to delete: ");
            scanf("%d",id);

            FILE *fp; 
            book tmpBook;
            //char flag='y';
            fp=fopen("bookMaster.txt","r+");
            while ((fread(&tmpBook,sizeof(tmpBook),1,fp)) == 1)
            {
                if(tmpBook.accession == id)
                {
                    printf("\nTitle is: %s\n    by %s\nConfirm delete?y/n ",tmpBook.title,tmpBook.author);
                    scanf("%c",&flag); 

                    while(flag=='y' && tmpBook.countTot==tmpBook.countAva)  //check if no books of title are issued
                    {
                        FILE *ft;
                        ft=fopen("tempRemove.txt","w+");
                        rewind(fp);     //initialise for loop
                        
                        while (fread(&tmpBook, sizeof(tmpBook), 1, fp) == 1)
                        {
                            if (tmpBook.accession == id)
                                {
                                    fseek(ft, 0, SEEK_CUR);
                                    fwrite(&tmpBook, sizeof(tmpBook), 1, ft); //write temp file 
                                }                      
                        }

                        flag='n';
                        fclose(ft);
                    }

                }

            }
            fclose(fp);
            remove("bookMaster.txt");
            rename("tempRemove.txt", "bookMaster.txt");
            printf("Deleted.\n");
            ////
            printf("Enter 1 to continue: ");
            scanf("%d",&flag);
        }
        
        mainAdminMenu();   
    }

    void addRemoveUsers(void)
    {   
        printf("Add/Remove Users Menu: \n\nWhat would you like to do?\n");
        printf("Enter 1 to add user\nEnter 2 to remove user\nEnter 7 to go back\n");
        int choice,flag=1;
        scanf("%d",&choice);
        while(choice==1 && flag==1)   //flags to repeat
        {
            
            user tmpUser1,tmpUser2;
            printf("Enter data:\n");
            printf("Username: ");
            scanf(" %[^\n]",tmpUser1.username);
            printf("Password: ");
            scanf(" %[^\n]",tmpUser1.password);
            ////
            FILE *fp;
            fp = fopen("userMaster.txt", "r+");
            while (fread(&tmpUser2, sizeof(tmpUser2), 1, fp) == 1)
            {
                if(!(strcmp(tmpUser1.username,tmpUser2.username)))
                    {
                        printf("User exists! Assigning new password.\n");   //!FUNCTIONAliTY mess below

                        fseek(fp, ftell(fp) - sizeof(tmpUser2), 0);     //TODO: NOTE
                        fwrite(&tmpUser1, sizeof(tmpUser2), 1, fp);
                        //fclose(fp);
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
            fs = fopen("UserMaster.txt", "a+");
            fwrite(&tmpUser1, sizeof(tmpUser1), 1, fs);
            fclose(fp);
            printf("Saved successfully.\n");
            ////
            printf("Enter 1 to continue: ");    //Flag Modify
            scanf("%d",&flag);
        }

        while(choice==2 && flag==1)
        {
            char id[10];
            printf("\nEnter username to delete: ");
            scanf("%s",id);

            FILE *fp; 
            user tmpUser;
            //char flag='y';
            fp=fopen("userMaster.txt","r+");
            while ((fread(&tmpUser,sizeof(tmpUser),1,fp)) == 1)
            {
                if(!(strcmp(id,tmpUser.username)))
                {
                    printf("\nUsername is: %s\nConfirm delete?y/n ",tmpUser.username);
                    scanf("%c",&flag);      
                    if(flag=='y')
                    {
                        FILE *ft;
                        ft=fopen("tempRemove.txt","w+");
                        rewind(fp);     //initialise for loop
                        while (fread(&tmpUser, sizeof(tmpUser), 1, fp) == 1)
                        {
                            if (strcmp(tmpUser.username,id))
                                   {
                                        fseek(ft, 0, SEEK_CUR);
                                        fwrite(&tmpUser, sizeof(tmpUser), 1, ft); //write temp file 
                                   }                      
                        }
                        
                        fclose(fp);
                        fclose(ft);
                        remove("userMaster.txt");
                        rename("tempRemove.txt", "userMaster.txt");
                        printf("Deleted\n");
                    }

                }
            }

            ////
            printf("Enter 1 to continue: ");
            scanf("%d",&flag);
        }


    }

    void inventory (void)
    {
        int choice=0,flag=1;
        printf("Book inventory menu: \n\n");
        printf("Enter 1 to view by accession no.\nEnter 2 to view all books\nEnter 7 to go back\n");
        scanf("%d",&choice);

        while(choice == 1 && flag==1)   //insert flags to repeat
            {
                int input;
                printf("Enter accession no. of book: ");
                scanf("%d",&input);
                FILE *fp;
                    fp = fopen("bookMaster.txt", "r");
                    book tmpBook;
                    while (fread(&tmpBook, sizeof(tmpBook), 1, fp) == 1)
                    {
                        if (tmpBook.accession == input)
                            printf("    %d %s\n        by %s    Total Count= %d\n\n",&tmpBook.accession,tmpBook.title,tmpBook.author,tmpBook.countTot);
                    }
                fclose(fp);
                
                printf("Enter 1 to continue: ");
                scanf("%d",&flag);
            }
        while(choice == 2 && flag==1)
        {
            printf("Listing all books: \n\n");
                FILE *fp;
                    fp = fopen("bookMaster.txt", "r");
                    book tmpBook;
                    while (fread(&tmpBook, sizeof(tmpBook), 1, fp) == 1)
                    {
                        printf("    %d %s\n        by %s    Total Count= %d\n\n",&tmpBook.accession,tmpBook.title,tmpBook.author,tmpBook.countTot);
                    }
                fclose(fp);
                
        }

        
        mainAdminMenu();
    }

    void order (void)       //TODO MAKE THE TXT FILE READABLE
    {
        printf("Book order/request menu:\n\n");
        printf("Enter 1 to place vendor order\nEnter 2 to request book ILDRS\nEnter 7 to go back\n");
        int choice,flag=1;
        scanf("%d",&choice);

        while(choice == 1 && flag==1)   //flags to repeat
        {
            printf("Vendor order:\n\n");
            FILE * fp;
            fp=fopen("orders.txt","a+");
            vendor tmpOrder;
            printf("Title: ");
            scanf(" %[^\n]",tmpOrder.title);
            printf("ISBN: ");
            scanf(" %[^\n]",tmpOrder.ISBN);
            printf("Total Count: ");
            scanf("%d",&tmpOrder.number);
            fwrite(&tmpOrder, sizeof(tmpOrder), 1, fp);
            fclose(fp);
            printf("Saved successfully.\n");

            ////
            printf("Enter 1 to continue: ");
            scanf("%d",&flag);
        }

        while(choice == 2 && flag==2)
        {
            while(choice == 1 && flag==1)   //flags to repeat
        {
            printf("ILDRS order:\n\n");
            FILE * fp;
            fp=fopen("ILDRS.txt","a+");
            vendor tmpOrder;
            printf("Title: ");
            scanf(" %[^\n]",tmpOrder.title);
            printf("ISBN: ");
            scanf(" %[^\n]",tmpOrder.ISBN);
            printf("Total Count: ");
            scanf("%d",&tmpOrder.number);
            fwrite(&tmpOrder, sizeof(tmpOrder), 1, fp);
            fclose(fp);
            printf("Saved successfully.\n");

            ////
            printf("Enter 1 to continue: ");
            scanf("%d",&flag);
        }

        
        mainAdminMenu();
    }


int mainUserMenu(void)
{
    printf("Hi %s\n");
    printf("These books are due:\n");    // red colour overdue
    printf("These books are reserved:\n");  //green colour avaliable to issue

    printf("Enter 1 to search for a book:\nEnter 2 to issue/return\nEnter 7 to logout\n\n");

}

    void search(void)
    {
        printf("OneBookSearch\n\n");
        printf("Enter search query: (Esc. to go back) ");
        char search[40];
        scanf("%s",search);

        FILE *fp;
        fp = fopen("bookMaster.txt", "r");
        book tmpBook;
        
        while (fread(&tmpBook, sizeof(tmpBook), 1, fp) == 1)
        {
            char *pt;
            if((pt=strstr(tmpBook.catanatedSearch,search)) != NULL)
                printf("    %d %s\n        by %s\n\n",&tmpBook.accession,tmpBook.title,tmpBook.author);
        }
        fclose(fp);

        printf("Pl note the accession no. to manage Book.\n");

        mainUserMenu();
    }

    void bookManagement(void)
    {
        printf("Book issue/return and reserve/unreserve Menu:\n\n");
        printf("Enter 1 to issue/reserve book\nEnter 2 to return/unreserve book\nEnter 7 to go back\n")
        scanf("%d",&choice);

        // fseek(fp, ftell(fp) - sizeof(tmpUser2), 0);     //TODO: NOTE
        // fwrite(&tmpUser1, sizeof(tmpUser2), 1, fp);     //EDIT STRUCT USING THIS

    }

    //////////////
    //Supporting macros
    int latestAccessionNo(void)
    {
        FILE *fp;
        fp = fopen("bookMaster.txt", "r");
        int accession=0;
        book tmpBook;
        while (fread(&tmpBook, sizeof(tmpBook), 1, fp) == 1)
        {
            if(accession<tmpBook.accession)
                accession=tmpBook.accession;    //Assign Highest accession no.
        }
        fclose(fp);
        return(accession+1);    
    }