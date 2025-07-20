#include "inverted_Search.h"

extern char *fname;
int insert_at_last(Wlist **head, data_t *data)
{
    //1. create a node
    Wlist *new = malloc(sizeof(Wlist));
    if(new == NULL)
        return FAILURE;
    new->file_count = 1;
    strcpy(new->word , data);
    new->Tlink = NULL;
    new->link = NULL;

    //call fun() to create ltable node
    update_link_table(&new);

    //2 .list is empty or not 
    if(*head == NULL)
    {
        *head = new;
        return SUCCESS;
    }

    //non empty
    Wlist *temp = *head;

    while(temp->link)
    {
        temp = temp->link;
    }
    temp->link = new;
    return SUCCESS;
}

int update_link_table(Wlist *head[])
{
    //create a node
    Ltable *new = malloc(sizeof(Ltable));
    if(new == NULL)
        return FAILURE;
    new->word_count = 1;
    strcpy(new->file_name,fname);
    new->table_link = NULL;

    (*head)->Tlink = new;
    return SUCCESS;
}