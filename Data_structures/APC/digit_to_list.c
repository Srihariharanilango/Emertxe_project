#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "apc.h"

// Function to insert at the end of the doubly linked list
void dl_insert_last(Dlist **head, Dlist **tail, int data)
{
    Dlist *new_node = (Dlist *)malloc(sizeof(Dlist));
    if (!new_node)
    {
        printf("Memory allocation failed!\n");
        return;
    }
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = *tail;
    
    if (*tail)
        (*tail)->next = new_node;
    else
        *head = new_node;
    
    *tail = new_node;
}


void digit_to_list(Dlist **head1,Dlist **tail1,Dlist **head2,Dlist **tail2,char *argv[],int *sign1,int *sign2,int *check)
{
    
    *check = 1;
    // Check for invalid characters
    // Check for alphabets (a-z or A-Z) in the first number (argv[1])
    int i = (argv[1][0] == '-') ? 1 : 0;  // Skip '-' for negative numbers
    for (; argv[1][i] != '\0'; i++) {
        if ((argv[1][i] >= 'a' && argv[1][i] <= 'z') || (argv[1][i] >= 'A' && argv[1][i] <= 'Z')) {
            *check = -1;  // Invalid character found
            return;
        }
    }

    // Check for alphabets (a-z or A-Z) in the second number (argv[2])
    i = (argv[3][0] == '-') ? 1 : 0;
    for (; argv[3][i] != '\0'; i++) {
        if ((argv[3][i] >= 'a' && argv[3][i] <= 'z') || (argv[3][i] >= 'A' && argv[3][i] <= 'Z')) {
            *check = -1;
            return;
        }
    }


    // Extract and store sign
    *sign1 = (argv[1][0] == '-') ? -1 : 1;
    *sign2 = (argv[3][0] == '-') ? -1 : 1;
    
    // Skip the sign if negative
     i = (argv[1][0] == '-') ? 1 : 0;
    for (; argv[1][i] != '\0'; i++)
    {
        dl_insert_last(head1, tail1, argv[1][i] - '0');
    }
    
    i = (argv[3][0] == '-') ? 1 : 0;
    for (; argv[3][i] != '\0'; i++)
    {
        dl_insert_last(head2, tail2, argv[3][i] - '0');
    }
}
    


void dl_insert_first(Dlist **head, Dlist **tail, int data)
{
    Dlist *new_node = (Dlist *)malloc(sizeof(Dlist));
    if (!new_node)
    {
        printf("Memory allocation failed!\n");
        return;
    }
    new_node->data = data;
    new_node->prev = NULL;
    new_node->next = *head;
    
    if (*head)
        (*head)->prev = new_node;
    else
        *tail = new_node;
    
    *head = new_node;
}
