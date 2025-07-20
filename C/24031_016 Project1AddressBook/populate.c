#include "contact.h"
// Dummy contact data
static Contact dummyContacts[] = {      // In this we are storing dummy contact list as static to use in this file alone 
    {"John Doe", "1234567890", "john@example.com"},        //- in Contact structure with dummyContacts[] as the  the array of structure    
    {"Alice Smith", "0987654321", "alice@example.com"},
    {"Bob Johnson", "1112223333", "bob@company.com"},
    {"Carol White", "4445556666", "carol@company.com"},
    {"David Brown", "7778889999", "david@example.com"},
    {"Eve Davis", "6665554444", "eve@example.com"},
    {"Frank Miller", "3334445555", "frank@example.com"},
    {"Grace Wilson", "2223334444", "grace@example.com"},
    {"Hannah Clark", "5556667777", "hannah@example.com"},
    {"Ian Lewis", "8889990000", "ian@example.com"}
};

void populateAddressBook(AddressBook* addressBook)
{

    //copying the element in the dummy contact to address book struct
    int numDummyContacts = sizeof(dummyContacts) / sizeof(dummyContacts[0]);

    //used to store the dummy contact and also to increase the contactCount 
    for (int i = 0; i < numDummyContacts && addressBook->contactCount < MAX_CONTACTS; ++i) {
        addressBook->contacts[addressBook->contactCount++] = dummyContacts[i];
    }
}