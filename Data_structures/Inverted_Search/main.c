#include "inverted_Search.h"

int main(int argc, char *argv[])
{
    system("clear");

    Wlist *head[27] = {NULL};
    //validation

    if(argc <= 1)
    {
        printf("Enter the valid no. arguments\n");
        printf("./Slist.exe file1.txt file2.txt ....\n");
        return 0;
    }

    // createtion of file linked list 
    // declare file head pointer 

    Flist *f_head = NULL;

    //validate files 
    file_validation_n_file_list(&f_head , argv);

    if(f_head == NULL)
    {
        printf("No files added to file linked list\n");
        printf("Hence the process got terminated \n");
        return 1;
    }

    system("clear");
    printf("********************************************************************************\n");
    printf("********************************************************************************\n");

    printf("Welcome to Inverted Search Engine\n");
    printf("This program is used to create a database of words and their locations in files.\n");
    printf("You can search for a word and find its location in the files.\n");
    printf("You can also update the database by adding or removing files.\n");
    printf("You can save the database to a file.\n");
    printf("You can exit the program at any time.\n");
    printf("This program is created by Srihariharan Ilango\n");
    printf("********************************************************************************\n");
    printf("********************************************************************************\n");
    int continue1 = 1;
    while(continue1){

printf("\n\nSelect your choice among following options:\n");
printf("1. Create DATABASE\n");
printf("2. Display Database\n");
printf("3. Update DATABASE\n");
printf("4. Search\n");
printf("5. Save Database\n");
printf("6. Exit\n");
int choice;
printf("Enter your choice: ");
scanf("%d" , &choice);
switch(choice)
{

    case 1 :
            create_database(f_head , head);
            printf("\nThe data base has been created successfully\n");
            printf("The database is created with the following files\n\n");
            Flist *temp = f_head;
            while(temp)
            {
                printf("%s\n" , temp->file_name);
                temp = temp->link;
            }
            
            break;
    case 2 :
            display_database(head);
            break;
    case 3 : 
            update_database(head,&f_head);
            break;
    case 4 : 
    {
            char word[WORD_SIZE];
            printf("Enter the word\n");
            scanf("%s" , word);
            int index = tolower(word[0]) % 97;

            search(head[index] , word);
            break;
    }
    case 5 :{ 
            save_database(head);
            break;
    }
    case 6 :
    {
        printf("\n\n********************************************************************************\n");
        printf("********************************************************************************\n");
            
            printf("Exiting the program\n");

                printf("Thank you for using the Inverted Search\n");
                printf("\n\n********************************************************************************\n");
                printf("********************************************************************************\n");
            return 0;
    }
    default:
    {
            printf("Invalid choice\n");
            break;
    }
    
}
end:
printf("Do you want to continue? (y/n) \n");
char ch;
scanf(" %c" , &ch);
if(ch == 'y' || ch == 'Y')
{
    continue1 = 1;       
}
else if(ch == 'n' || ch == 'N')
{
    continue1 = 0;
}       
else
{
        printf("Invalid choice\n");
        printf("Please enter y or n\n");
        goto end;
}
        
}
printf("Exiting the program\n");

}


