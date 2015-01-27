/***************************************************************************/
/* Securite : Travaux Pratique 1                                           */
/* Olivier Ly - Renaud Tabary                                              */
/* Date: 2008-01                                                           */
/***************************************************************************/
#ifndef TOOLS_H
#define TOOLS_H

//#define OS CYGWIN
#define OS LINUX

#include <stdlib.h>

typedef unsigned int u32;
typedef unsigned char byte;

/* le nombre de message */
#define NB 1000

byte Messages[NB][8];
byte Chiffres[NB][8];

/***************************************************************************/
/* Bijections                                                              */
/***************************************************************************/

/*
 * Calcul de la bijection inverse
 * - src : la bijection à inverser.
 * - dst : la bijection résultante.
 */
void inverse_mix64(byte src[64], byte dst[64]);

/*
 * Applique une bijection sur un tableau de 64 bits
 * - src : 64 bits codés sur un tableau de 8 octets.
 * - dst : 64 bits codés sur un tableau de 8 octets.
 */
void mix64(byte bijection[64], byte *src, byte *dst);

/*
 * Affiche une bijection
 */
void display_mix64(byte bijection[64]);

/*
 * Lecture d'une bijection à partir d'un fichier
 */
void mix64_of_file(const char * filename, byte bijection[64]);

/***************************************************************************/
/* Fichier de clairs/chiffrés                                              */
/***************************************************************************/

/*
 *  load the file of plains/ciphers messages from stdin
 */
void read_message_list();

/***************************************************************************/
/* Manipulations bit à bit                                                 */
/***************************************************************************/

/*
 * compute a bit-to-bit xor of two byte arrays
 */
void byte_array_xor (byte * src1, byte * src2, byte * dest, int len);

/*
 * return the value of the n-th bit of arr
 */
int get_nth_bit(byte * arr, int n);

/*
 * Set the n-th bit of arr to val
 */
void set_nth_bit(byte * arr, int val, int n);

/*
 * Print the bits of arr into str
 */
void sprint_bits(char str[65], byte arr[8]);

/***************************************************************************/
/* Manipulations octets par octets                                         */
/***************************************************************************/

/*
 * copy byte arrays
 */
void byte_array_copy (byte * src, byte * dst, int len);

/*
 * byte array pretty print
 */
void sprint_byte_array (byte * arr, byte * dest, int len);

/* 
 * conversion of a string in hexa into a byte array
 * puts the len of the resulting byte array into len
 * str is of form "01 23 EE ..."
 */
void char2byte_array(char * str, byte * dest, short * len);


/*****************************************************************************/
/* Generateur de tableau aleatoire                                           */
/*****************************************************************************/

/*
 * produit un tableau aleatoire de 64 bits et le place dans msg
 * (par itération du DES)
 */
void freshmsg(byte msg[8]);

#endif
/*****************************************************************************/
/*****************************************************************************/
