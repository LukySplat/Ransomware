#include "ransomlib.h"
#include <dirent.h> // for socket
#include <sys/socket.h>
#include <unistd.h> 
#include <arpa/inet.h>
#include <openssl/rand.h>

// int generate_key(unsigned char *key, int sizeKey, unsigned char *iv, int sizeIv,char *pKey, char *pIv);
// int generate_key()
int generate_key(unsigned char *key, int sizeKey, unsigned char *iv, int sizeIv)
{
	// Generate cryptographically strong pseudo-random bytes for key and IV 
	if (!RAND_bytes(key, sizeKey) || !RAND_bytes(iv, sizeIv)) 
	{
		// OpenSSL reports a failure, act accordingly 
		handleErrors();
	}

	// for(int cpt=0; cpt<sizeKey; cpt++)
	// {
			// printf("%d", key[cpt]);
	// }
	printf("%d",key);
	
	// for(int cpt=0; cpt<sizeIv; cpt++)
	// {
			// printf("%ld", iv[cpt]);
	// }
	OPENSSL_cleanse(key,sizeof(key));	
	OPENSSL_cleanse(iv,sizeof(iv));
}

int main ()
{
	// Key to use for encrpytion and decryption 
	unsigned char key[AES_256_KEY_SIZE+1];
	// Initialization Vector 
	unsigned char iv[AES_BLOCK_SIZE+1];

	int sizeKey = AES_256_KEY_SIZE; 
	int sizeIv = AES_BLOCK_SIZE;
	generate_key(key, sizeKey, iv, sizeIv);
}
