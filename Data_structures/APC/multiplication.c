
#include <stdio.h>
#include <stdlib.h>
#include "apc.h"

int dl_delete_list(Dlist **head, Dlist **tail) {
    if (*head == NULL) return FAILURE;

    Dlist *temp = *head;
    while (temp) {
        Dlist *next_node = temp->next;
        free(temp);
        temp = next_node;
    }

    *head = NULL;  // Reset head pointer
    *tail = NULL;  // Reset tail pointer

    return SUCCESS;
}


int multiplication(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR) {
    if (!(*head1) || !(*head2)) {
        dl_insert_first(headR, tailR, 0);
        return SUCCESS;
    }

    Dlist *temp2 = *tail2;
    Dlist *res1_head = NULL, *res1_tail = NULL;  // Stores final result
    Dlist *res2_head = NULL, *res2_tail = NULL;  // Stores intermediate result
    Dlist *res3_head = NULL, *res3_tail = NULL;  // Temporary storage for addition
    int position = 0; // Tracks trailing zeros

    while (temp2) {  // Loop through digits of second number
        int carry = 0;
        Dlist *temp1 = *tail1; // Pointer to first number
        res2_head = NULL;
        res2_tail = NULL;

        //  Add trailing zeros for place value
        for (int i = 0; i < position; i++) {
            dl_insert_first(&res2_head, &res2_tail, 0);
        }

        //  Multiply each digit of first number with current digit of second number
        while (temp1) {
            int product = (temp1->data * temp2->data) + carry;
            carry = product / 10;
            dl_insert_first(&res2_head, &res2_tail, product % 10);
            temp1 = temp1->prev;
        }

        if (carry) {
            dl_insert_first(&res2_head, &res2_tail, carry);
        }

        if (!res1_head) {
            res1_head = res2_head;
            res1_tail = res2_tail;
        } else {
            addition(&res1_head, &res1_tail, &res2_head, &res2_tail, &res3_head, &res3_tail);
    
            // Free previous res1 list to prevent memory overflow
            dl_delete_list(&res1_head, &res1_tail);
    
            // Update res1 to new sum result
            res1_head = res3_head;
            res1_tail = res3_tail;
    
            // Ensure res3 is also properly freed if needed
            res3_head = NULL;
            res3_tail = NULL;
        }
    
        // Move to next digit of second number
        temp2 = temp2->prev;
        position++;
    }

    *headR = res1_head;
    *tailR = res1_tail;
    //this is for the removing zeros in the answer example like 123444*007874 =00ans=after removing ans
    while (*headR != NULL && (*headR)->data == 0)
    {
        dl_delete_first(headR, tailR);
    }

    // If result is empty (i.e., input numbers were equal), insert zero
    if (*headR == NULL)
    {
        dl_insert_first(headR, tailR, 0);
    }


    return SUCCESS;
}
