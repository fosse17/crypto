/***************************************************************************/
/* Securite : Travaux Pratiques, chiffrement symmétrique                    */
/* Olivier Ly - Renaud Tabary                                              */
/* Date: 2008-01                                                           */
/***************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "tools.h"

/*****************************************************************************/
/* Variables 8 octets <-> 64 bits                                            */
/*****************************************************************************/

byte K[8];
byte M[8];
byte C[8];

/*****************************************************************************/
/* Bijections                                                                */
/*****************************************************************************/

/*
 * Bijection de [0,63] -> [0,63]
 * - ip[k] est l'image de k.
 * (à charger depuis un fichier)
 */
byte ip[64];

/*
 * Bijection inverse (à précalculer)
 */
byte ip_inv[64];

/*****************************************************************************/
/* Algorithme de Chiffrement et de Dechiffrement                             */
/*****************************************************************************/

void encrypt(byte M[8], byte K[8], byte C[8]) {
  byte BUFFER[8];
  byte_array_xor(M,K,BUFFER,8);  // xor avec la clé.
  mix64(ip, BUFFER, C);          // mélange des bits.
}

void decrypt(byte C[8], byte K[8], byte M[8]) {
  byte BUFFER[8];
  mix64(ip_inv, C, BUFFER);      // mélange inverse.
  byte_array_xor(BUFFER,K,M,8);  // xor avec la clé.
}

/*****************************************************************************/
/* Prodution d'un lot de clairs / chiffrés                                   */
/*****************************************************************************/

void new_plain_cipher(int n) {
  int i;
  for (i=0; i<n; i++) {
    freshmsg(M);
    byte_array_copy(M, Messages[i], 8);

    encrypt(M, K, C);
    byte_array_copy(C, Chiffres[i], 8);
  } 
}

/*****************************************************************************/
int main(int argc, char * argv[]) {
/*****************************************************************************/
	
   if(argc<3) 
     {
	printf("Usage:\n");
	printf("%s --encrypt <bloc de 64 bits en hexa> <cle de 64 bits en hexa> : encrypte un bloc\n",argv[0]);
	printf("%s --decrypt <bloc de 64 bits en hexa> <cle de 64 bits en hexa> : decrypte un bloc\n",argv[0]);
	printf("%s --print-bits <bloc de 64 bits en hexa> : affiche un bloc hexa en binaire\n",argv[0]);
	return -1;
     }
   
   int i;
	short l;
	char str[1024];		
	
	/* Preliminaires */
	mix64_of_file("../bijection.txt", ip);
	inverse_mix64(ip, ip_inv);

	/* 
	 * Encryption d'un bloc  
	 * arg1 : la cle
	 * arg2 : le message clair
	 *
	 * Exemple:
	 * > sym-cipher --encrypt "0123456789ABCDEF" "0123456789ABCDEF"
	 */
  if (strcmp(argv[1],"--encrypt")==0) {
    char2byte_array(argv[2], K, &l);
    char2byte_array(argv[3], M, &l);
		encrypt(M, K, C);
		sprint_byte_array(C, str, 8); printf("%s\n",str); 
		exit(0);
	}

	/* 
	 * Decryption d'un bloc
	 * arg1 : la cle
	 * arg2 : le message chiffré
	 * Exemple:
	 * > sym-cipher --decrypt "0123456789ABCDEF" "0123456789ABCDEF"
	 */
  if (strcmp(argv[1],"--decrypt")==0) {
    char2byte_array(argv[2], K, &l);
    char2byte_array(argv[3], C, &l);
		decrypt(C, K, M);
		sprint_byte_array(M, str, 8); printf("%s\n",str); 
		exit(0);
	}

  /* 
	 * Production d'un fichier de clair-chiffres 
	 */
  if (strcmp(argv[1],"--new-plain-cipher")==0) {
    int i;		

		/* Load the bijection */
		mix64_of_file("../bijection-secrete.txt", ip);
		inverse_mix64(ip, ip_inv);
		
		/* Define the key */
		for (i=0; i<8; i++) K[i] = argv[2][i];
		
		/* Produce the plain-cipher pairs */
   	new_plain_cipher(NB);
    
		/* Display the pairs */
		for (i=0; i<NB; i++) {
			sprint_byte_array(Messages[i], str, 8); printf("%s -> ",str); 
			sprint_byte_array(Chiffres[i], str, 8); printf("%s\n",str);    
	  }
		exit(0);
	}

	/********************************************/
	/* Exemple d'utilisation de la bibliothèque */
	/********************************************/
	
	/* 
	 * Affichage des bits d'un message hexa 
	 * Exemple:
	 * > sym-cipher --print-bits "0123456789ABCDEF"
   */
  if (strcmp(argv[1],"--print-bits")==0) {
    char2byte_array(argv[2], M, &l);
		sprint_bits(str, M); printf("%s\n",str);    
    exit(0);
  }

  /* 
	 * Lecture d'un fichier de clair-chiffres:
	 * Affiche le contenu d'un fichier de clair/chiffrés
	 */
  if (strcmp(argv[1],"--read-plain-cipher")==0) {
		read_message_list();
		for (i=0; i<NB; i++) {
			sprint_byte_array(Messages[i], str, 8); printf("%s -> ",str); 
			sprint_byte_array(Chiffres[i], str, 8); printf("%s",str);    
		}
    exit(0);
  } 
}
