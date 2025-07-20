#ifndef CONTACT_H
#define CONTACT_H

#define MAX_CONTACTS 100

typedef struct {
    char name[50];
    char phone[20];
    char email[50];
} Contact;  //using typedef the name is aliased into Contact for this struct


//so since we need to create upto max 100 contact using same struct we are using array stuct
typedef struct {
    Contact contacts[100];   //struct array has been initialized so 100 contact can be created  
                             //This will create like Student s1[100];
    int contactCount;   // used to store how many number contact has been saved
} AddressBook;

void createContact(AddressBook *addressBook);  // *addressBook is a structure pointer as AddressBook is structure datetype
int searchContact(AddressBook *addressBook);  // So in this we are passing the address of structure as we are collecting structure pointer
void editContact(AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);
void listContacts(AddressBook *addressBook);
void initialize(AddressBook *addressBook);
//void saveContactsToFile(AddressBook *AddressBook);

/*
    Structure variable can accessed by contact[0].name
    Structure pointer by
    addressbook ->contact count (or) (*addressbook).contactcount
    
    */

#endif
