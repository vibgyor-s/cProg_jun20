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

typedef struct //supports user
{
    char type; //issued-i reserved-r
    int accession;
    time_t issue;
} bkRec;

typedef struct //MAIN2
{
    char username[10];
    char password[10];
    bkRec record[5]; //max 5 books issued+reserved PROBLEMS?
} user;

typedef struct //MAIN3
{
    char title[40];
    char ISBN[14];
    int number;
} vendor, ILDRS;

void welcome(void);        //return- 0/1 according to user/admin  //!
int mainAdminMenu(void);   //return-choice                         //!
void addRemoveBooks(void); //modify bookmaster.txt                 //!
void addRemoveUsers(void); //modify usermaster.txt                 //!
void inventory(void);      //list particular books and count       //!
void order(void);          //append to orders.txt ILDRS.txt
int mainUserMenu(void);    //return-choice
void search(void);         //main search and list
void bookManagement(void); //issue-return

//GLOBAL Var
time_t rawtime;
char username[10];
// book tmpBook;

int main()
{
    printf("Add/Remove Users Menu: \n\nWhat would you like to do?\n");
    printf("Enter 1 to add user\nEnter 2 to remove user\nEnter 7 to go back\n");
    int choice, flag = 1;
    scanf("%d", &choice);
    while (choice == 1 && flag == 1) //flags to repeat
    {

        user tmpUser1, tmpUser2;
        printf("Enter data:\n");
        printf("Username: ");
        scanf(" %[^\n]", tmpUser1.username);
        printf("Password: ");
        scanf(" %[^\n]", tmpUser1.password);

        for (int i = 0; i < 4; i++)
        {
            tmpUser1.record[i].accession = 0;
            tmpUser1.record[i].type = 'n';
            tmpUser1.record[i].issue = rawtime;
        }
        ////
        FILE *fp;
        fp = fopen("userMaster.txt", "r+");
        while (fread(&tmpUser2, sizeof(tmpUser2), 1, fp) == 1)
        {
            if (!(strcmp(tmpUser1.username, tmpUser2.username)))
            {
                printf("User exists! Assigning new password.\n"); //!FUNCTIONAliTY mess below

                fseek(fp, ftell(fp) - sizeof(tmpUser2), 0); //TODO: NOTE
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
        fs = fopen("userMaster.txt", "a+");
        fwrite(&tmpUser1, sizeof(tmpUser1), 1, fs);
        fclose(fp);
        printf("Saved successfully.\n");
        ////
        printf("Enter 1 to continue: "); //Flag Modify
        scanf("%d", &flag);
    }

    while (choice == 2 && flag == 1)
    {
        char id[10];
        printf("\nEnter username to delete: ");
        scanf("%s", id);

        FILE *fp;
        user tmpUser;
        //char flag='y';
        fp = fopen("userMaster.txt", "r+");
        while ((fread(&tmpUser, sizeof(tmpUser), 1, fp)) == 1)
        {
            if (!(strcmp(id, tmpUser.username)))
            {
                printf("\nUsername is: %s\nConfirm delete?y/n ", tmpUser.username);
                scanf("%c", &flag);
                if (flag == 'y')
                {
                    FILE *ft;
                    ft = fopen("tempRemove.txt", "w+");
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
                    printf("Deleted\n");
                }
            }
        }

        ////
        printf("Enter 1 to continue: ");
        scanf("%d", &flag);
    }
}
