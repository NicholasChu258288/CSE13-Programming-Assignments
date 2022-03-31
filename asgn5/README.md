## Assignment 5: Public Cryptography

## Short Description
	The files keygen.c, encrypt.c, and decrypt.c are used to implemenet RSA encryption.
	keygen.c is used for generating public and private keys. encrypt.c is used for
	encrypting an input file using a public key. decrypt.c takes an encrypted input file 
	and prints using the private key, decrypts the file and writes its contents to an out
	file.

## Building
	make	makes all: eygen, encrypt, and decrypt.
	
## Running
	./keygen [b:i:n:d:s:vh] [-b n] [-i iters] [-n pbfile] [-d pvfile] [s seed] [-v]	
	./encrypt [i:o:n:vh] [-i inputfile] [-o outputfile] [-n pbfile] [-v] [-h]
	./decrypt [i:o:n:vh] [-i inputfile] [-o outputfile] [-n pbfile] [-v] [-h]
	
	Preferably run in this order if you do not have pre-existing keys or encrypted files.
	
## Errors:
	rsa.c:137:14: warning: Use of zero-allocated memory
    		block[0] = 0xFF;
	This occurs when encrypting the contents of a file, in rsa_encrypt_file().
	This is used in encrypt when saving blocks in order to prevent issues when
	modding the message that is being encrypted.
	
	Additionally there is a small error that sometimes causes either encrypt
	or decrypt sometimes skip the first byte. I have not been able to track
	down what is causing the issue and it only has been happening for specific
	cases. This issue happens only very rarely.

