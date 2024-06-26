/*
 * Copyright (c) 2024 SakuraOneLove Moscow
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *		list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this
 *		list of conditions and the following disclaimer in the documentation and/or other
 *		materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors may
 *		be used to endorse or promote products derived from this software without specific
 *		prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "ezcrypt.h"

/**
 * Create a 256 bit key and IV using the supplied key_data. salt can be added for taste.
 * Fills in the encryption and decryption ctx objects and returns 0 on success
 **/

int aes_init(unsigned char *key_data, int key_data_len, unsigned char *salt,
								EVP_CIPHER_CTX *e_ctx, EVP_CIPHER_CTX *d_ctx)
{
  int i, nrounds = 5;
  unsigned char key[32], iv[32];
  
  /*
   * Gen key & IV for AES 256 CBC mode. A SHA1 digest is used to hash the supplied key material.
   * nrounds is the number of times the we hash the material. More rounds are more secure but
   * slower.
   */
  i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), salt, key_data, key_data_len, nrounds, key, iv);
  if (i != 32) {
    printf("Key size is %d bits - should be 256 bits\n", i);
    return -1;
  }

  EVP_CIPHER_CTX_init(e_ctx);
  EVP_EncryptInit_ex(e_ctx, EVP_aes_256_cbc(), NULL, key, iv);
  EVP_CIPHER_CTX_init(d_ctx);
  EVP_DecryptInit_ex(d_ctx, EVP_aes_256_cbc(), NULL, key, iv);

  return 0;
}

/*
 * Encrypt *len bytes of data
 * All data going in & out is considered binary (unsigned char[])
 */
unsigned char *aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int *len)
{
  /* max ciphertext len for a n bytes of plaintext is n + AES_BLOCK_SIZE -1 bytes */
  int c_len = *len + AES_BLOCK_SIZE, f_len = 0;
  unsigned char *ciphertext = malloc(c_len);

  /* allows reusing of 'e' for multiple encryption cycles */
  EVP_EncryptInit_ex(e, NULL, NULL, NULL, NULL);

  /* update ciphertext, c_len is filled with the length of ciphertext generated,
    *len is the size of plaintext in bytes */
  EVP_EncryptUpdate(e, ciphertext, &c_len, plaintext, *len);

  /* update ciphertext with the final remaining bytes */
  EVP_EncryptFinal_ex(e, ciphertext+c_len, &f_len);

  *len = c_len + f_len;
  return ciphertext;
}

/*
 * Decrypt *len bytes of ciphertext
 */
unsigned char *aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext, int *len)
{
  /* plaintext will always be equal to or lesser than length of ciphertext*/
  int p_len = *len, f_len = 0;
	unsigned char *plaintext = malloc(p_len);
  
  EVP_DecryptInit_ex(e, NULL, NULL, NULL, NULL);
  EVP_DecryptUpdate(e, plaintext, &p_len, ciphertext, *len);
  EVP_DecryptFinal_ex(e, plaintext+p_len, &f_len);

  *len = p_len + f_len;
  return plaintext;
}

/*
 * Error codes:
 * 1 - can't execute EVP_MD_CTX_NEW
 * 2 - can't execute EVP_DigestInit_ex
 * 3 - can't execute EVP_DigestUpdate
 * 4 - can't execute EVP_DigestFinal_ex
 * Good code:
 * 0 - OK
 */
int sha256_digest(const unsigned char *msg, size_t msg_len,	char *digest_msg)
{
	int rc = 0;
	EVP_MD_CTX *mdctx;
	const EVP_MD *md = EVP_sha256();
	unsigned char* temp_buffer = (unsigned char*)malloc(sizeof(char) * 65);

	if ((mdctx = EVP_MD_CTX_new()) == NULL) {
		puts("Can't execute EVP_MD_CTX_NEW");
		rc = 1;
		goto err_label;
	}
	if(1 != EVP_DigestInit_ex(mdctx, md, NULL)) {
		puts("Can't execute EVP_DigestInit_ex");
		rc = 2;
		goto err_free_mem_label;
	}
	if(1 != EVP_DigestUpdate(mdctx, msg, msg_len)) {
		puts("Can't execute EVP_DigestUpdate");
		rc = 3;
		goto err_free_mem_label;
	}
	if(1 != EVP_DigestFinal_ex(mdctx, temp_buffer, 0)) {
		puts("Can't execute EVP_DigestFinal_ex");
		rc = 4;
	}
	/* Copy casted value to char* destination */
	strcpy(digest_msg, (const char*)temp_buffer);	
err_free_mem_label:
	EVP_MD_CTX_free(mdctx);
err_label:
	free(temp_buffer);
	return rc;
}
/*int main(int argc, char **argv)*/
/*{*/
	/* "opaque" encryption, decryption ctx structures that libcrypto uses to record
		 status of enc/dec operations */
	/*EVP_CIPHER_CTX* en = EVP_CIPHER_CTX_new();*/
	/*EVP_CIPHER_CTX* de = EVP_CIPHER_CTX_new();*/

	/* 8 bytes to salt the key_data during key generation. This is an example of
		 compiled in salt. We just read the bit pattern created by these two 4 byte 
		 integers on the stack as 64 bits of contigous salt material - 
		 ofcourse this only works if sizeof(int) >= 4 */
	/*unsigned int salt[] = {12345, 54321};*/
	/*unsigned char *key_data;*/
	/*int key_data_len, i;*/
	/*char *input[] = {"a", "abcd", "this is a test", "this is a bigger test", */
									 /*"\nWho are you ?\nI am the 'Doctor'.\n'Doctor' who ?\nPrecisely!",*/
									 /*NULL};*/

	/* the key_data is read from the argument list */
	/*key_data = (unsigned char *)argv[1];*/
	/*key_data_len = strlen(argv[1]);*/
	
	/* gen key and iv. init the cipher ctx object */
	/*if (aes_init(key_data, key_data_len, (unsigned char *)&salt, en, de)) {*/
		/*printf("Couldn't initialize AES cipher\n");*/
		/*return -1;*/
	/*}*/

	/* encrypt and decrypt each input string and compare with the original */
	/*for (i = 0; input[i]; i++) {*/
		/*char *plaintext;*/
		/*unsigned char *ciphertext;*/
		/*int olen, len;*/
		
		/* The enc/dec functions deal with binary data and not C strings. strlen() will 
			 return length of the string without counting the '\0' string marker. We always
			 pass in the marker byte to the encrypt/decrypt functions so that after decryption 
			 we end up with a legal C string */
		/*olen = len = strlen(input[i])+1;*/
		
		/*ciphertext = aes_encrypt(en, (unsigned char *)input[i], &len);*/
		/*plaintext = (char *)aes_decrypt(de, ciphertext, &len);*/

		/*if (strncmp(plaintext, input[i], olen)) */
			/*printf("FAIL: enc/dec failed for \"%s\"\n", input[i]);*/
		/*else */
			/*printf("OK: enc/dec ok for \"%s\"\n", plaintext);*/
			/*printf("Encrypt text: \"%s\"\n", ciphertext);*/
		
		/*free(ciphertext);*/
		/*free(plaintext);*/
	/*}*/

	/*EVP_CIPHER_CTX_free(en);*/
	/*EVP_CIPHER_CTX_free(de);*/

	/*return 0;*/
/*}*/
