// int generate_key(unsigned char *key, int sizeKey, unsigned char *iv, int sizeIv,char *pKey, char *pIv);
int generate_key()
{
    // Key to use for encrpytion and decryption 
    unsigned char key[AES_256_KEY_SIZE];
    // Initialization Vector 
    unsigned char iv[AES_BLOCK_SIZE];
	
	int sizeKey = AES_256_KEY_SIZE; 
	int sizeIv = AES_BLOCK_SIZE;

	// Generate cryptographically strong pseudo-random bytes for key and IV 
    if (!RAND_bytes(key, sizeof(key)) || !RAND_bytes(iv, sizeof(iv))) 
    {
        // OpenSSL reports a failure, act accordingly 
        fprintf(stderr, "ERROR: RAND_bytes error: %s\n", strerror(errno));
        return errno;
    }

	for(int cpt=0; cpt<sizeKey; cpt++)
	{
			printf("%c", key[cpt]);
	}
	for(int cpt=0; cpt<sizeIv; cpt++)
	{
			printf("%c", iv[cpt]);
	}
	OPENSSL_cleanse(key,sizeof(key));	
	OPENSSL_cleanse(iv,sizeof(iv));
}


int main ()
{
	generate_key();
}
