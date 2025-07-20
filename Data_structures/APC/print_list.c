#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "apc.h"
void print_list(Dlist *head)
{
    while (head)
    {
        printf("%d", head->data);
        head = head->next;
    }
    printf("\n");
}