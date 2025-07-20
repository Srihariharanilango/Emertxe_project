#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>
#include "contact.h"
#include "file.h"
#include "populate.h"
#include<ctype.h>

int is_invalid_name(char *str_name)
{
    int len = strlen(str_name);
    int valid = 0;
    for(int i =0;i<len;i++)
    {
        if((str_name[i]>='a' && str_name[i]<='z') || (str_name[i]>='A' && str_name[i]<='Z') || str_name[i]==' ')
        {
            valid = 1;
        }
        else{
            return 0;
        }
        
    }
    return valid;
}

int is_invalid_email(char *email)
{
    int atpos = -1, dotpos = -1, len = strlen(email);

    
    if (len < 3) {
        return 0;
    }

    for (int i = 0; i < len; i++) {
        char c = email[i];

        if (c == '@') {
            // Ensure only one '@' is present and it's not the first or last character
            if (atpos != -1 || i == 0 || i == len - 1) {
                return 0;
            }
            atpos = i;
        } else if (c == '.') {
            // Record the last dot position and ensure no consecutive dots
            if (i > 0 && email[i - 1] == '.') {
                return 0;
            }
            dotpos = i;
        } else if (!isalnum(c) && c != '-' && c != '_') {
            // Invalid character check
            return 0;
        }
    }

    // Ensure '@' exists and is followed by a valid domain
    if (atpos == -1 || dotpos == -1 || dotpos < atpos + 2 || dotpos == len - 1) {
        return 0;
    }

    return 1;
}

void listContacts(AddressBook *addressBook) 

{

        printf("\n********************************************************************************");
        printf("\n********************************************************************************");
        printf("\n                       Contact List                                             ");
        printf("\n********************************************************************************");
        printf("\n********************************************************************************\n");
    __fpurge(stdin);
    printf("Name\tPhone number\tEmail id\t\n");
    // Sort contacts based on the chosen criteria
    for(int  i =0;i<addressBook->contactCount;i++)
    {
        printf("%s\t%s\t%s\t\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }

    
}

void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    populateAddressBook(addressBook);
    
    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);
}

/*void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}*/


void createContact(AddressBook *addressBook)
{

    printf("\n********************************************************************************");
        printf("\n********************************************************************************");
        printf("\n                      Creating the contact                                       ");
        printf("\n********************************************************************************");
        printf("\n********************************************************************************");
	/* Define the logic to create a Contacts */

    char str_name[50];
    
    int c=0;
    while(c==0)
    {
        
    name:
    __fpurge(stdin);
    printf("\nEnter the name of the new Contact: ");
    scanf("%49[^\n]",str_name);

     c = is_invalid_name(str_name);

    if(c==0)
    {
        printf("\nEnter a valid name");
        goto name;

    }

    else
    {
        strcpy(addressBook->contacts[addressBook->contactCount].name, str_name);
        
        number:
            __fpurge(stdin);
            printf("\nEnter the new phone number: ");
            scanf("%19[^\n]",addressBook->contacts[addressBook->contactCount].phone);
            int n = strlen(addressBook->contacts[addressBook->contactCount].phone);
        if(n!=10)
        {
            printf("\nEnter a valid number");
            goto number;
            
        }
        else
        {
        for(int i =0;i<addressBook->contactCount;i++)
        {
            int strc = 0;
            strc = strcmp(addressBook->contacts[i].phone,addressBook->contacts[addressBook->contactCount].phone);
            if(strc == 0)
            {
                printf("The number is already present, re-enter");
                goto number;
            }
        }
        }
        }

        email:
        __fpurge(stdin);
        printf("\nEnter the new mail id: ");
        scanf("%19[^\n]",addressBook->contacts[addressBook->contactCount].email);
        c = is_invalid_email(addressBook->contacts[addressBook->contactCount].email);

        if(c==0)
        {
        printf("\nEnter a valid email id");
        goto email;

        }
        else
        {
        for(int i =0;i<addressBook->contactCount;i++)
        {
            int strc = 0;
            strc = strcmp(addressBook->contacts[i].email,addressBook->contacts[addressBook->contactCount].email);
            if(strc == 0)
            {
                printf("The email id is already present, re-enter");
                goto email;
            }
        }
        }
        
    }

    addressBook->contactCount++;



}
    


int searchContact(AddressBook *addressBook) 
{

    printf("\n********************************************************************************");
        printf("\n********************************************************************************");
        printf("\n                      Searching a contact                                       ");
        printf("\n********************************************************************************");
        printf("\n********************************************************************************\n\n");
    /* Define the logic for search */
    __fpurge(stdin);
    char sch;
    int ret;
    char str[50];
    int flag = 0;
    char rep;
    conform1:
    printf("Enter an input:\n1.Name\n2.Phone no\n3.Email\n");
    scanf("%c",&sch);

    switch(sch)
    {
        case '1':
        {
            printf("Enter the name to search: ");
            scanf(" %49[^\n]",str);
            for (int i = 0; str[i]; i++) {
                str[i] = tolower((unsigned char)str[i]);
            }
            for(int i =0;i<addressBook->contactCount;i++)
            {
                /*if(strcmp(addressBook->contacts[i].name,str)==0)
                {
                    ret = i;
                    printf("Name: %s\nPhone no: %s\nEmail: %s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
                    
                }*/
                repeat:
                char str2[50];
                strncpy(str2, addressBook->contacts[i].name, sizeof(str2) - 1);
                str2[sizeof(str2) - 1] = '\0'; 
                for(int j = 0; str2[j]; j++) 
                {
                str2[j] = tolower((unsigned char)str2[j]);
                }

                if (strstr(str2, str)!=NULL) 
                {
                    printf("\nContact %d\n",i+1);
                    printf("Name: %s\nPhones: %s\nEmail: %s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
                    
                    
                    __fpurge(stdin);
                    printf("\nIs this your contact you are searching for (Y/N) If N the search will be continued: ");
                    scanf("%c",&rep);
                    if(rep =='y' || rep=='Y')
                    {
                        flag = 1;
                        ret =i;
                        break;                       
                    }
                    else{
                        
                         i++;
                        goto repeat;
                    }
                    
                    
                }
                flag =0;

            }
            if(flag ==0)
            {
                printf("\nContact not found");
                ret=-1;
                
            }
            break;
        }

        case '2':
        {
            printf("Enter the number to search: ");
            scanf(" %49[^\n]",str);
            for(int i =0;i<addressBook->contactCount;i++)
            {
                if(strcmp(addressBook->contacts[i].phone,str)==0)
                {
                    ret = i;
                    printf("Name: %s\nPhone no: %s\nEmail: %s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
                    flag = 1;
                    break;
                }
            }
            if(flag ==0)
            {
                printf("\nContact not found");
                ret = -1;
                
            }
            break;
        }


        case '3':
        {
            printf("Enter the Email address to search: ");
            scanf(" %49[^\n]",str);
            for(int i =0;i<addressBook->contactCount;i++)
            {
                if(strcmp(addressBook->contacts[i].email,str)==0)
                {
                    ret = i;
                    printf("Name: %s\nPhone no: %s\nEmail: %s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
                    flag = 1;
                    break;
                }
            }
            if(flag ==0)
            {
                printf("\nContact not found");
                ret = -1;
                
            }
            break;
        }
        default:
        {
            printf("\n You have chose the wrong option Enter the correct option!!!!!\n");
            goto conform1;
            break;
        }

    }
    return ret;
}



void editContact(AddressBook *addressBook)
{
	/* Define the logic for Editcontact */
    printf("\n********************************************************************************");
        printf("\n********************************************************************************");
        printf("\n                      Editing a contact                                       ");
        printf("\n********************************************************************************");
        printf("\n********************************************************************************\n\n");
        printf("Please enter details of the contact you need to edit\n\n");
    choice2:
        __fpurge(stdin);
     int sch;
     int j =0;
    char str[50];
    int flag = 0;
    conform:
        int i = searchContact(addressBook);

     __fpurge(stdin);

     char cho;
     if(i == -1)
     {
        goto End;
     }
     else
     {
    printf("\nDo you want to edit this Contact(Y/N): ");
    scanf("%c",&cho);
    char choice1;
    char edit[100];

    if(cho =='y' || cho == 'Y')
    {
        __fpurge(stdin);
        Edit:
        printf("\nWhat do you want to edit\n1.Name\n2.Email\n3.Phone number\nEnter your choice: ");
        scanf("%ch",&choice1);

        switch(choice1)
        {
            case '1': 
                {
                    chab1:
                    __fpurge(stdin);
                    printf("\nEnter the editing name: ");
                    scanf("%99[^\n]",edit);
                    int chab = is_invalid_name(edit);
                    if(chab == 0)
                    {
                        printf("\nPlease enter a valid name");
                        goto chab1;
                    }
                    else
                    {
                    strcpy(addressBook->contacts[i].name,edit);
                    }
                    break;
                }
            case '2':
                 {
                    chab2:
                    __fpurge(stdin);
                    printf("\nEnter the editing mail id: ");
                    scanf("%99[^\n]",edit);
                    int chab = is_invalid_email(edit);
                    if(chab == 0)
                    {
                        printf("\nPlease enter a valid email id");
                        goto chab2;
                    }
                    else{
                    strcpy(addressBook->contacts[i].email,edit);
                    }
                    break;
                 }

            case '3':
               {
                    chab3:
                    __fpurge(stdin);
                    printf("\nEnter the editing Phone number: ");
                    scanf("%99[^\n]",edit);
                    if(strlen(edit)!=10)
                    {
                        printf("\nEnter a valid phone number");
                        goto chab3;
                    }
                    else{
                    strcpy(addressBook->contacts[i].phone,edit);
                    }
                    break;
                 }

            default:
            {
                printf("\n Enter the correct option");
                goto Edit;
                break;
            }

        }


    }
    }
    
    __fpurge(stdin);
    char choice2;
    End:
    printf("\nDo you want to continue(Y/N): ");
    scanf("%c",&choice2);

    if(choice2=='y' || choice2=='Y')
    {
        goto choice2;
    }
    else{
        printf("\n****************The editing option is closed and saved******************\n");
    }
    
}

void deleteContact(AddressBook *addressBook)
{
	/* Define the logic for deletecontact */
    printf("\n********************************************************************************");
        printf("\n********************************************************************************");
        printf("\n                      Deleting a contact                                       ");
        printf("\n********************************************************************************");
        printf("\n********************************************************************************");
    Delete1:
    printf("\n\nEnter the details of contact you need delete\n\n");
    int j = searchContact(addressBook);
    if(j==-1)
    {
        goto End1;
    }

     __fpurge(stdin);

    char cho;
    printf("\nDo you want to delete the Contact(Y/N): ");
    scanf("%c",&cho);

    if(cho =='y' || cho == 'Y')
        {
    for(;j<addressBook->contactCount;j++)
    {
        strcpy(addressBook->contacts[j].name,addressBook->contacts[j+1].name);
        strcpy(addressBook->contacts[j].phone,addressBook->contacts[j+1].phone);
        strcpy(addressBook->contacts[j].email,addressBook->contacts[j+1].email);

   }
    addressBook->contactCount--;
    addressBook->contacts[addressBook->contactCount].name[0] = '\0';
        addressBook->contacts[addressBook->contactCount].phone[0] = '\0';
        addressBook->contacts[addressBook->contactCount].email[0] = '\0';
    printf("\nContact has been deleted");
    }
    else
    {
        printf("\nThe contact has not been deleted\n\n");

    }  

    End1:
        __fpurge(stdin);
        char choice3;
        printf("\nDo you want to continue the deleting option(Y/N)");
        scanf("%c",&choice3);
        if(choice3=='y' || choice3 == 'Y')
        {
            goto Delete1;
        } 
        else{
        printf("\n****************The deleting option has been closed******************\n");
    }
   
}
