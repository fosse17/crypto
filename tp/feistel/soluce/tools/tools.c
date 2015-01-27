/*****************************************************************************/
/*****************************************************************************/
#include <stdio.h>
#include "tools.h"

byte Messages[NB][8];
byte Chiffres[NB][8];

/*****************************************************************************/
/* Bijections                                                                */
/*****************************************************************************/

/*
 * Calcul de la bijection inverse
 * - src : la bijection à inverser.
 * - dst : la bijection résultante.
 */
void inverse_mix64(byte src[64], byte dst[64]) {
  int i,j;
  for (i=0; i<64; i++) {
    j=0;
    while(src[j] != i) j++;
    dst[i] = j;
  }
}

/*
 * Applique une bijection sur un tableau de 64 bits
 * - src : 64 bits codés sur un tableau de 8 octets.
 * - dst : 64 bits codés sur un tableau de 8 octets.
 */
void mix64(byte bijection[64], byte *src, byte *dst) {
  int i;
  for (i=0; i<64; i++)
    set_nth_bit(dst,get_nth_bit(src,bijection[i]),i);
}

/*
 * Affiche une bijection
 */
void display_mix64(byte bijection[64]) {
	int i;
	for (i=0; i<64; i++) {
		printf("%2d ", bijection[i]);
		if (i%8==7) printf("\n");
	}
}

/*
 * Lecture d'une bijection à partir d'un fichier
 */
void mix64_of_file(const char * filename, byte bijection[64]) {
	int i;
	FILE *f;
	
	if ((f=fopen(filename, "r"))==NULL) {
		printf("Unable to load bijection file : %s\n", filename);
		exit(0);
	}
	
	for (i=0; i<64; i++)
		fscanf(f, "%d", &bijection[i]);
}

/*****************************************************************************/

void read_message_list() {
  int i,k;
  short l;
  char str[1024];
  
  for (k=0; k<NB; k++) {

	#if OS==CYGWIN
		fread(str,1,37,stdin);
	#elif OS==LINUX
		fread(str,1,38,stdin);
	#endif
	
    for(i=0; i<36; i++)
      if (str[i]==' ')
				str[i]='0';

    str[36]=0;  
    str[16]=0;

    char2byte_array(str, Messages[k], &l);  
    char2byte_array(str+20, Chiffres[k], &l);  
  }
}

void byte_array_xor (byte * src1, byte * src2, byte * dest, int len) {
  int i;
  for (i=0; i<len; i++)
    dest[i] = src1[i]^src2[i];
}

int get_nth_bit(byte * arr, int n) {
  return (arr[n/8] & (1 << (7-n%8))) >> (7-n%8);
}

void set_nth_bit(byte * arr, int val, int n) {
  if (val==1) {
    arr[n/8] = arr[n/8] | (1 << (7-n%8)); 
  } else
    arr[n/8] = arr[n/8] & ~(1 << (7-n%8)); 
}


void byte_array_copy (byte * src, byte * dst, int len) {
  int i;
  for (i=0; i<len; i++)
    dst[i] = src[i];
}

void sprint_byte_array (byte * arr, byte * dest, int len) {
  int i;
  for(i=0; i<len; i++)
    sprintf(dest+2*i,"%02X", arr[i]);
  dest[2*len]=0;
}

int val_hexa(char c) {
  if ((48 <= c) && (c <= 57))
    return c-48;
  if ((65 <= c) && (c <= 70))
    return (c-65) + 10;
  if ((97 <= c) && (c <= 102))
    return (c-97) + 10;
  printf("erreur de conversion hexa\n");
  exit(0);
}

int char2ascii_array_internal(char * str, byte * dest) {
  int i=0, k=0;

  while (str[i] != 0) {
    if ((str[i]==' ') || (str[i]=='\t') || (str[i]=='\n')) {
      i++;
      continue;
    }
    dest[k] = val_hexa(str[i])*16 + val_hexa(str[i+1]);
    i += 2;
    k++;
  }
  return k;
}

void char2byte_array(char * str, byte * dest, short * len) {
  *len = char2ascii_array_internal(str, dest);
}

void sprint_bits(char str[65], byte arr[8]) {
  int i;
  for (i=0; i<64; i++)
    sprintf(&str[i], "%d", get_nth_bit(arr, i));
}


/*****************************************************************************/
/* Generateur de tableau aleatoire                                           */
/*****************************************************************************/
#include "des_gnupg.h"

long int c=100;

byte seed[8] = { 29, 97, 7, 11, 13, 17, 19, 23 };
byte currentRandom[8];
byte BufRand[8];
des_ctx context;

void freshmsg(byte msg[8]) {
  int i;
  des_setkey(context, seed);

  des_ecb_encrypt(context, currentRandom, currentRandom);
  byte_array_copy(currentRandom, msg,8);
}

/*****************************************************************************/
/*****************************************************************************/
