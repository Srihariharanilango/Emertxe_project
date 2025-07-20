#include"main.h"

void extract_external_commands(char **external_commands){
    int fd = open("command.txt",O_RDONLY);
    if(fd== -1){
      perror("open is failed");
      exit(0);
      }
      
    int row =0,col=0;
    char buf[40];
    char ch;
    
    while(read(fd, &ch, 1)>0 && ch!= EOF)
    {
      if(ch =='\n'){
        buf[col] = '\0';
        external_commands[row] = malloc(strlen(buf) + 1);
        strcpy(external_commands[row], buf);
        row++;
        col = 0;
      }
      else{
        buf[col] = ch;
        col++;
      }
    }
    
    external_commands[row] = NULL;
    
    
}
  

