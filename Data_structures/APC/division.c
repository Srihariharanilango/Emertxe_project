
#include "apc.h"
#include <stdio.h>
#include <stdlib.h>


int division(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR) {
    if (*head2 == NULL || (*head2)->data == 0) { // 
        printf("Error: Division by zero is not allowed.\n");
        return FAILURE;
    }

    Dlist *remainder = NULL, *remTail = NULL;
    Dlist *dividend = *head1;
    int quotientDigit;

    while (dividend) {
        dl_insert_last(&remainder, &remTail, dividend->data); 

        
        while (remainder && remainder->data == 0 && remainder->next)
            dl_delete_first(&remainder, &remTail);

        quotientDigit = 0;

        
        while (compare_lists(remainder, *head2) >= 0) {
            Dlist *tempRem = NULL, *tempRemTail = NULL;
            subtraction(&remainder, &remTail, head2, tail2, &tempRem, &tempRemTail);
            remainder = tempRem;
            remTail = tempRemTail;
            quotientDigit++;
        }

        
        dl_insert_last(headR, tailR, quotientDigit);

        
        dividend = dividend->next;
    }

    
    if (*headR == NULL) {
        dl_insert_last(headR, tailR, 0);
    }
    while (*headR != NULL && (*headR)->data == 0)
    {
        dl_delete_first(headR, tailR);
    }

    
    if (*headR == NULL)
    {
        dl_insert_first(headR, tailR, 0);
    }

    return SUCCESS;
}


   


int compare_lists(Dlist *list1, Dlist *list2) {
    
   
    int len1 = 0, len2 = 0;
    Dlist *temp1 = list1, *temp2 = list2;
    while (temp1) { len1++; temp1 = temp1->next; }
    while (temp2) { len2++; temp2 = temp2->next; }

   
    if (len1 > len2) return 1;
    if (len1 < len2) return -1;

    while (list1 && list2) {
        if (list1->data > list2->data) return 1;
        if (list1->data < list2->data) return -1;
        list1 = list1->next;
        list2 = list2->next;
    }

    return 0; 
}
