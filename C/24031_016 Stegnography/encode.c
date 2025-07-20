#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3; //this will print the size of the pixel excluding the header

    /*
    //to use position in header to find the .bmp size
    fseek(fptr_image,2,SEEK_SET);
    unsigned int size;
    fread(&size,4,1,fptr_image);
    printf("%u\n",width*height*3); //in image file 1 byte represent 3 bits(RGB)
    printf("%u\n",size);//this is using position in header //this will print size of the pixel with the header
    */
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    //validate if proper .bmp file and proper .txt file is passed by checking first 2 bytes (try to do after)
    //at present we just check the file extension
    if(strstr(argv[2],".bmp")!=NULL)//this means there is .bmp is found argv[2]
    {
        encInfo->src_image_fname = argv[2]; // we are acceing image_fname from structure pointer

    }
    else
    {
       
        return e_failure;
    }
    if(strstr(argv[3],".txt")!=NULL) //so this denotes we have passed right .txt. file with secret code
    {
        encInfo->secret_fname=argv[3];
    }
    else
    {
        
        return e_failure;
    }

    if(argv[4]!=NULL) //check if there is stego.bmp file
        encInfo ->stego_image_fname = argv[4];
    else
        encInfo->stego_image_fname = "stego.bmp"; //create stego.bmp file if the user didn't give with one
    
    return e_success;

}

Status check_capacity(EncodeInfo *encInfo)
{
    //size of beautiful.bmp file
    encInfo->image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image); //you can either get the size of the bmp file from the header second four bits or we multiply no of bits horizontal with vertical
    // we used the second method


    //size of secret.txt file
    encInfo->size_secret_file = get_file_size(encInfo -> fptr_secret); // this will give file size

    if(encInfo->image_capacity > (16+32+32+32+((encInfo -> size_secret_file)*8))) // refer notes for the explaination
        return e_success;
    else
        return e_failure;

   // return e_success;

}

uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char str[54];
    fseek(fptr_src_image,0,SEEK_SET);
    fread(str,54,1,fptr_src_image);
    fwrite(str,54,1,fptr_dest_image);
    return e_success;
}

Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    encode_data_to_image(magic_string, 2, encInfo);
    return e_success;
}

Status encode_data_to_image(char *data, int size, EncodeInfo *encInfo)
{
    for(int i = 0;i<size;i++)
    {
        fread(encInfo->image_data,8,1,encInfo->fptr_src_image);
        encode_byte_to_lsb(data[i],encInfo->image_data);
        fwrite(encInfo->image_data,8,1,encInfo->fptr_stego_image);
    }
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i = 0;i<8;i++)
    {
        image_buffer[i]=(image_buffer[i] & 0xFE) | ((data>>(7-i))&1); // for i = 0 => 0x01 & 0xFE -> 0x00 | 0

    }
}

Status encoode_secret_file_extn_size(int size,EncodeInfo *encInfo)
{
     encode_size_to_lsb(size,encInfo);
     return e_success;
}

Status encode_size_to_lsb(int size,EncodeInfo *encInfo)
{
    char str[32];
    fread(str,32,1, encInfo -> fptr_src_image); //read 32 bytes from beautiful.bmp

    for(int i = 0;i<32;i++)
    {
        str[i]=(str[i] & 0xFE) | ((size>>(31-i))&1); // for i = 0 => 0x01 & 0xFE -> 0x00 | 0   
    }

    fwrite(str,32,1,encInfo->fptr_stego_image);
}

Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    encode_data_to_image(file_extn,4,encInfo);
    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    encode_size_to_lsb(file_size,encInfo);
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    fseek(encInfo->fptr_secret,0,SEEK_SET);
    char str[encInfo->size_secret_file];
    fread(str,encInfo->size_secret_file,1,encInfo->fptr_secret);
    encode_data_to_image(str,encInfo->size_secret_file,encInfo);
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while(1)
    {
        fread(&ch,1,1,fptr_src);
        if(feof(fptr_src)!=0)
        {
            break;
        }
        fwrite(&ch,1,1,fptr_dest);
    }
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    //all other function call should be done here
    if(open_files(encInfo)==e_success)    //openfile takes structure address has input                                                //encInfo represents the address of the structure
    {
        printf("File has been opened successfully in encoding\n");
    }
    else
    {
        printf("File was not been able to opened in encoding\n");
        return e_failure;
    }
    if(check_capacity(encInfo)==e_success)  //check if the .bmp file is greater than the sum value of storage
    {
        printf("Check capacity function is a success\n");
    }
    else
    {
        printf("Check capacity function is a failure\n");
        return e_failure;
    }
    if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
    {
        printf("Copying BMP header is a success\n");
    }
    else{
        printf("Compying BMP file is a failure\n");
        return e_failure;
    }

    if(encode_magic_string(MAGIC_STRING, encInfo) == e_success)
    {
        printf("Encoding magic string is a success\n");
    }
    else{
        printf("Encoding magic string is a failure\n");
        return e_failure;
    }
    //Extract the extension from the file name
    strcpy(encInfo -> extn_secret_file , strchr(encInfo->secret_fname,'.'));
    if(encoode_secret_file_extn_size(strlen(encInfo->extn_secret_file),encInfo)==e_success)
    {
        printf("Encoded secret file extn size successfully\n");
    }
    else
    {
        printf("Failed to encode secret file extn size\n");
        return e_failure;
    }

    if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)
    {
        printf("Encoded secret file extenstion successfully\n");
    }
    else{
                printf("Failed to Encode secret file extenstion\n");
                return e_failure;
    }

    if(encode_secret_file_size(encInfo->size_secret_file,encInfo) == e_success)
    {
        printf("Encoded secret file size successfully\n");
    }
    else{
        printf("Failed to Encode secret file size\n");
        return e_failure;

    }

    if(encode_secret_file_data(encInfo)==e_success)
    {
        printf("Encoded secret file data\n");
    }
    else{
                printf("Unable to Encode secret file data\n");
                return e_failure;
    }

    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
    {
        printf("Copied remaining data\n");
    }
    else{
        printf("Failed to copy remaining data\n");
        return e_failure;
    }

    //closing all the files opened

    fclose(encInfo->fptr_src_image);
    fclose(encInfo->fptr_secret);
    fclose(encInfo->fptr_stego_image);
   

    return e_success;
}
