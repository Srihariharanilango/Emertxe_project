#ifndef APC_H
#define APC_H


#define SUCCESS 0
#define FAILURE -1

typedef struct node
{
	struct node *prev;
	int data;
	struct node *next;
}Dlist;

/* Include the prototypes here */

/*store the operands into the list */
// void digit_to_list(Dlist **head1,Dlist **tail1,Dlist **head2,Dlist **tail2,char *argv[]);
void digit_to_list(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, char *argv[],int *sign1,int *sign2,int *check);
void dl_insert_first(Dlist **head, Dlist **tail, int data);


int dl_delete_first(Dlist **head, Dlist **tail);
void dl_insert_last(Dlist **head, Dlist **tail, int data);

int dl_delete_list(Dlist **head, Dlist **tail);
int compare_lists(Dlist *list1, Dlist *list2);

/*Addition */
int addition(Dlist **head1,Dlist **tail1,Dlist **head2,Dlist **tail2,Dlist **headR,Dlist **tailR);

/*subtraction*/
void subtraction(Dlist **head1,Dlist **tail1,Dlist **head2,Dlist **tail2,Dlist **headR,Dlist **tailR);

/*Multiplication*/
int multiplication(Dlist **head1,Dlist **tail1,Dlist **head2,Dlist **tail2,Dlist **headR,Dlist **tailR);


/*Division */
int division(Dlist **head1,Dlist **tail1,Dlist **head2,Dlist **tail2,Dlist **headR,Dlist **tailR);

/*for printing */
void print_list(Dlist *head);
#endif
