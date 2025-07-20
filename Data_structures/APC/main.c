
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "apc.h"

int list_num(Dlist *head){
	int sum = 0;
    while (head) {
        sum = sum + head->data;
        if (head->next != NULL) {
            sum = sum * 10;
        }
        head = head->next;  // Move to the next node
    }
    return sum;
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("The number of arguments is less than or greater than 3. Please provide 3 arguments.\n");
        return FAILURE;
    }

    /* Declare pointers */
    Dlist *head1 = NULL, *tail1 = NULL;
    Dlist *head2 = NULL, *tail2 = NULL;
    Dlist *headR = NULL, *tailR = NULL;
    
    int sign1, sign2, result_sign;
    
    int check;
    digit_to_list(&head1,&tail1,&head2,&tail2,argv,&sign1,&sign2,&check);
	if(check == -1)
	{
		printf("\nThe given argument contain alphabet so invalid\n");
		return FAILURE;
	}
	int num1 = list_num(head1);
	int num2 = list_num(head2);

    char operator = argv[2][0];

    switch (operator)
    {
        case '+':
            printf("\nOperand 1: %s\nOperator: Addition\nOperand 2: %s\n", argv[1], argv[3]);
            if(sign1 == 1 && sign2==1)
            {
                addition(&head1,&tail1,&head2,&tail2,&headR,&tailR);
                
            }
            else if(sign1 == -1 && sign2==1)
            {
                //since one sign is negative subtraction should be done
                if(num1>num2){

                
                subtraction(&head1,&tail1,&head2,&tail2,&headR,&tailR);
                }
                else
                {
                    subtraction(&head2,&tail2,&head1,&tail1,&headR,&tailR);
                }
                result_sign = (num1>num2)?-1:1;

            }

            else if(sign1 == 1 && sign2== -1)
            {
                //since one sign is negative subtraction should be done
                if(num1>num2){

                
                    subtraction(&head1,&tail1,&head2,&tail2,&headR,&tailR);
                    }
                    else
                    {
                        subtraction(&head2,&tail2,&head1,&tail1,&headR,&tailR);
                    }
                    result_sign = (num1>num2)?-1:1;
    
                result_sign = (num2>num1)?-1:1;

            }
            else{
                result_sign = -1;
                addition(&head1,&tail1,&head2,&tail2,&headR,&tailR);

            }
            break;

        case '-':
            printf("\nOperand 1: %s\nOperator: Subtraction\nOperand 2: %s\n", argv[1], argv[3]);
            if(sign1 == 1 && sign2==1)
					{
						if(num1>num2){

                
                            subtraction(&head1,&tail1,&head2,&tail2,&headR,&tailR);
                            }
                            else
                            {
                                subtraction(&head2,&tail2,&head1,&tail1,&headR,&tailR);
                            }
						result_sign = (num2>num1)?-1:1;
						
					}
					else if(sign1 == -1 && sign2 == 1){
						addition(&head1,&tail1,&head2,&tail2,&headR,&tailR);
						result_sign = -1;
					}
                    else if(sign1 == -1 && sign2 == -1){
						addition(&head1,&tail1,&head2,&tail2,&headR,&tailR);
						result_sign = -1;
					}
            break;

        case 'x':
            printf("\nOperand 1: %s\nOperator: Multiplication\nOperand 2: %s\n", argv[1], argv[3]);
            result_sign = sign1 * sign2;
            multiplication(&head1, &tail1, &head2, &tail2, &headR, &tailR);
            break;
        
        case '/':
            printf("\nOperand 1: %s\nOperator: Division\nOperand 2: %s\n", argv[1], argv[3]);
            if (head2 == NULL || (head2->data == 0))
            {
                printf("Error: Division by zero is not allowed.\n");
                return FAILURE;
            }
            result_sign = sign1 * sign2;
            division(&head1, &tail1, &head2, &tail2, &headR, &tailR);
            break;
        
        default:
            printf("Invalid Input\n");
            return FAILURE;
    }
    if(result_sign == -1 && headR->data != 0)
    {
        printf("Result: -");
    }
    else
    {
        printf("Result: ");
    }
    print_list(headR);
    dl_delete_list(&head1, &tail1);
    dl_delete_list(&head2, &tail2);
    dl_delete_list(&headR, &tailR);
    return SUCCESS;
}