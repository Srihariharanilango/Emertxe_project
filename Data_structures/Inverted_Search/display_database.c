#include "inverted_Search.h"


void display_database(Wlist *head[]) {
    printf("---------------------------------------------------------------\n");
    printf("| Index |      Word       | File Count |   File Name & Count  |\n");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < 27; i++) {
        if (head[i] != NULL) {
            print_word_count(head[i]);
        }
    }
}

int print_word_count(Wlist *head) {
    while (head != NULL) {
        printf("| %-5d | %-15s | %-10d |", tolower(head->word[0]) % 97, head->word, head->file_count);
        Ltable *Thead = head->Tlink;

        while (Thead) {
            printf(" %-10s (%d) |", Thead->file_name, Thead->word_count);
            Thead = Thead->table_link;
        }
        printf("\n");
        head = head->link;
    }
    return 0;
}