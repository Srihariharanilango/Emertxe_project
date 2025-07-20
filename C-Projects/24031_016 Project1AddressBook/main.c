#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdio_ext.h>
#include<unistd.h>

#include "contact.h"  // user defined header file to create variable for the program
#include"file.h"

int main() {
    int choice;
    AddressBook addressBook;   //Datatype structure addressBook is a structure variable
    initialize(&addressBook); // Initialize the address book
    loadContactsFromFile(&addressBook);

    do {

        printf("\n******************************************************************************");
        printf("\n******************************************************************************");
        printf("\n                  Project 1: Address Book                                     ");
        printf("\n******************************************************************************");
        printf("\n******************************************************************************");
        
        __fpurge(stdin);
        printf("\nAddress Book Menu:\n");
        printf("1. Create contact\n");
        printf("2. Search contact\n");
        printf("3. Edit contact\n");
        printf("4. Delete contact\n");
        printf("5. List all contacts\n");
    	printf("6. Save contacts\n");		
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        //to select the required action to perform
        switch (choice) {
            case 1:
                createContact(&addressBook);
                break;
            case 2:
                searchContact(&addressBook);
                break;
            case 3:
                editContact(&addressBook);
                break;
            case 4:
                deleteContact(&addressBook);
                break;
            case 5:          
                listContacts(&addressBook);
                break;
            case 6:
                printf("\n\n\n*******************************************************************\n");
                printf("*******************************************************************\n");
                printf("Saving to the file and exiting");
                saveContactsToFile(&addressBook);
                //sleep(2);
                break;
            case 7:
                exit(0);   // used to exit from the loop and stop the execution this is a function from stdlib
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (1);
    
       return 0;
}
