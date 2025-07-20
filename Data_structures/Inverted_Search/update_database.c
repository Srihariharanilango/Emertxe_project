#include "inverted_Search.h"

int isfileexist(char *filename)
{
    struct stat file_stat;

    // Check if the file exists
    if (stat(filename, &file_stat) != 0)
    {
        return FILE_NOTAVAILABLE; // File does not exist
    }

    // Check if the file is empty
    if (file_stat.st_size == 0)
    {
        return FILE_EMPTY; // File is empty
    }

    return SUCCESS; // File exists and is not empty
}

int update_database(Wlist *head[], Flist **f_head)
{ 
    int status, result;  
    char file[FNAME_SIZE];

    printf("Enter the filename that you want to add to the database: ");
    scanf("%s", file);

    // Validate file existence
    status = isfileexist(file);
    if (status == FILE_NOTAVAILABLE)
    {
        printf("The file %s is not present.\nHence, we are not adding this file into the database.\n", file);
        return FAILURE;
    }
    else if (status == FILE_EMPTY)
    {
        printf("The file %s is empty.\nHence, we are not adding %s into the database.\n", file, file);
        return FAILURE;
    }

    // Check if file is already in the database
    result = to_create_list_of_files(f_head, file);
    if (result == SUCCESS)
    {
        printf("Successfully updated the file into the database.\n");
    }
    else if (result == REPEATATION)
    {
        printf("File is repeated.\nSo we cannot update the file into the database.\n");
        return FAILURE;
    }
    else
    {
        printf("Failed to update the file into the database.\n");
        return FAILURE;
    }

    // Update database with the new file
    Flist *temp = *f_head;
    while (temp)
    {
        if (strcmp(temp->file_name, file) == 0)
        {
            create_database(temp, head);
            printf("Database updated successfully.\n");
            return SUCCESS;
        }
        temp = temp->link;
    }

    return FAILURE;
}