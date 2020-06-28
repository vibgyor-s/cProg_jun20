#############################################################
ReadMe for assignment submitted by 2019B5PS0326P
Practical section number: P10
Asignment question attempted: 2
#############################################################
Team:
1. 2019B5PS0326P   VIBGYOR SINGHVI
2. 2017A7PS0182P   CHETAN  SHARMA
3. 2019B2TS1251P   ISHAN   TONK   
#############################################################
Description of how to run the code and observe the output: 

Program structure-
                                Welcome Page
                                    /\
                        Admin Menu /  \ User Menu (Alerts+notifications)
                -Add/remove users       -Search Books
                -Add/remove books       -Manage Books
                -Inventory Man.
                -Order/ILDRS

1. The exe is called 2019B5PS0326P_P10.exe. It is a menu-driven application. 
2. The input files are -
    bookMaster.txt --Used to store book structures
    userMaster.txt --Used to store user records
    ordersILDRS.csv --Used to keep record of requests in Excel-compatible csv format

    The book and user records are stored in their respective structure format in their files, and whole structure is 
    called while processing of data. 
    Movement to subsequent structures is automatically done by the file pointer.

3. When you run the exe, it will prompt you to enter your username and password. Password is not echoed (using sys-call)
    -Todays date is shown.
    -Latest book additions of last 7 days will be shown. 
    
    This and other time related functionality use 
    standard c library time.h.

4. Further, there would be two different menus depending on type of User- Admin and general user

5. ADMIN MENU
    -option to add/remove books (implemented using 2 file modification)
        -highest avaliable accession number is automatically alloted
        -Inputs of details are taken
        -time of record generation is automatically stored alongside for new books functionality
        -a catanated string containing book title, author, accession no. 
         is automatically generated for easy search.
    -option to add/remove user
       !-username and password strings have max. length of 10 ie max 9 characters stored
        -Only password is changed if the username exists (ADDITIONAL fn)
    -list all/some books (inventory) 
        -individual books by accession no.
    -input orders/requests which will be saved in csv file

6. USER MENU (A user can have max 5 books issued+reserved at a time, by design)
    -displays issued books in form of their accession numbers,
        -due books, issued for more than 15 days are shown in RED colour
    -displays reserved books
        -now avaliable reserved books are shown in GREEN colour
    -One search
        -takes any search input like accession no., author name, or title
    -BOOK MANAGEMENT (based on accession no. of book)
        -modifications are done by overwriting the updated structure on older structure

        -Book issue
            book if avaliable is issued, record is stored with user and book avaliablity count is deducted by 1
            book if not avaliable is reserved,record is only stored with user
        -Book return
            issued book is returned,record stored with user is erased and book avaliablity count is incremented by 1
            reserved book is unreserved, and record stored with user is erased

7. Program depends on stdio.h, stdlib.h, string.h and time.h std. libraries.
   Program is compiled with 
                    gcc version 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)
   Compilation warnings
                    main.c:100:17: warning: implicit declaration of function ‘sleep’; did you mean ‘strsep’? [-Wimplicit-function-declaration]
                    main.c:158:29: warning: implicit declaration of function ‘latestAccessionNo’ [-Wimplicit-function-declaration]
                    main.c:533:23: warning: implicit declaration of function ‘strcasestr’; did you mean ‘strcasecmp’? [-Wimplicit-function-declaration]
                    main.c:533:21: warning: assignment makes pointer from integer without a cast [-Wint-conversion]
   Book and User structures are defined in line 6 to 31.

8. blah blah blah
############################################################
Known limitations of the code :

Every requested feature is implemented, however some notes-

1. (admin)User to be deleted is not checked to have issued books
2. (user)to issue reserved book, unreserve it and then issue it. 
3. (user)User have multiple copies of same book issued and reserved. (this is by design!)  
#############################################################
Contributions of the team members:
Combined Brainstorming and live code editing was done.
Mem. had multiple commits to to the final code. 
#############################################################