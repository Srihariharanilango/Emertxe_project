
#include <stdio.h>
#include<stdlib.h>
#include "apc.h"

int dl_delete_first(Dlist **head, Dlist **tail)
{
    if(*head==NULL)
    return FAILURE;
    Dlist *temp=*head;
    if((*head)->next==NULL)
    {
         free(*head);
        *head=*tail=NULL;
        return SUCCESS;
    }
    *head=temp->next;
    free(temp);
    (*head)->prev=NULL;
    return SUCCESS;
} 
void subtraction(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR)
{
    int borrow = 0, diff;
    Dlist *temp1 = *tail1;  // Traverse from least significant digit
    Dlist *temp2 = *tail2;

    while (temp1 != NULL || temp2 != NULL)
    {
        if (temp1 != NULL && temp2 != NULL)
        {
            if (temp1->data < temp2->data + borrow)  // Borrow needed
            {
                diff = (temp1->data + 10) - (temp2->data + borrow);
                borrow = 1;
            }
            else
            {
                diff = temp1->data - temp2->data - borrow;
                borrow = 0;
            }

            dl_insert_first(headR, tailR, diff);
            temp1 = temp1->prev;
            temp2 = temp2->prev;
        }
        else if (temp1 != NULL)  // Remaining digits from first number
        {
            if (temp1->data < borrow)  // Borrow propagation
            {
                diff = (temp1->data + 10) - borrow;
                borrow = 1;
            }
            else
            {
                diff = temp1->data - borrow;
                borrow = 0;
            }

            dl_insert_first(headR, tailR, diff);
            temp1 = temp1->prev;
        }
    }

    // Remove leading zeros using dl_delete_first
    while (*headR != NULL && (*headR)->data == 0)
    {
        dl_delete_first(headR, tailR);
    }

    // If result is empty (i.e., input numbers were equal), insert zero
    if (*headR == NULL)
    {
        dl_insert_first(headR, tailR, 0);
    }
}