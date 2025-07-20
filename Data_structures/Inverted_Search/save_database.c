#include "inverted_Search.h"

int save_database( Wlist *head[])
{
    char f_name[FNAME_SIZE];                                
    printf("Enter the file name that you want to save : ");  //statement displayed followed by string user input
    scanf("%s", f_name);
    FILE *fptr = fopen(f_name, "w");                        //Opens file name in write mode
    if(fptr == NULL)                                        //checks whether file is not present
    {
	    printf("Error : File is not present\n");             //if true then returns with statement
	    return FAILURE;
    } 
    Wlist *temp = NULL;                                   //declaring local reference
    for(int index=0;index<27;index++)                               
    {
		temp = head[index];
    	while(temp)                                         //traversing through the list in the table   
       	{
    	    fprintf(fptr, "#:%d\n%s:%d:",index,temp->word,temp->file_count);
    	    Ltable *new = temp -> Tlink;
    	    while(new)										
    	    {
    		    fprintf(fptr, "%s:%d:#", new -> file_name, new -> word_count);
    		    new = new -> table_link;
    	    }
    	    fprintf(fptr,"\n");
    	    temp=temp->link; 
    	}
    }
    printf("Database saved successfully in %s\n",f_name);                       //displays statement and closes the file
    fclose(fptr);
    return SUCCESS;
}
