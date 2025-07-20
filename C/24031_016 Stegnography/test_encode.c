#include <stdio.h>
#include<string.h>
#include "encode.h"
#include"decode.h"
#include "types.h"

int main(int argc, char **argv)
{

    //check if  if atleast 3 CLA are passed

    if(argc<3)
    {
        printf("You should provide with atleast 3 Command line arguments\n");
        return 1;
    }

    else
    {
    //check if the user has selected encodign or decoding

    if(check_operation_type(argv) ==e_encode)
    {
        printf("Selected encoding \n");
        EncodeInfo encoding_structure;
        if(read_and_validate_encode_args(argv,&encoding_structure) == e_success)
        {
            printf("Read and validate argument function is a success\n");
            printf("<--------------Started to encode-------------->\n");
            if(do_encoding(&encoding_structure)== e_success)
            {
                printf("Successfully completed the encoding\n");
            }
            else
            {
                printf("Failed to perform encoding\n");
                return 3;
            }
        }
        else
        {
            printf("Read and validate function is a failure\n");
            return 2;
        }
    }
    else  if(check_operation_type(argv) ==e_decode)
    {
        printf("Selected decoding \n");
        DecodeInfo decoding_structure; 
        if(read_and_validate_decode_args(argv,&decoding_structure)==e_success)
        {
            printf("Read and validate argument function is a success\n");
            printf("<--------------Started to decode-------------->\n");
            if(do_decoding(&decoding_structure)== e_success)
            {
                printf("Successfully completed the decoding\n");
            }
            else
            {
                printf("Failed to perform decoding\n");
                return 3;
            }
        }
        
        
        
    }
    else{
        printf("Invalid option \nFor encoding: ./a.out -e beautiful.bmp secret.text [stego.bmp]\nFor decoding: ./a.out -d stego.bmp decode.txt\n");
    }  

    }


    return 0;
}

OperationType check_operation_type(char *argv[])
{

    if(strcmp(argv[1],"-e")==0)  //since in the second location the type will be there
        return e_encode;
    else if(strcmp(argv[1],"-d")==0)
        return e_decode;
    else   
        return e_unsupported;

}