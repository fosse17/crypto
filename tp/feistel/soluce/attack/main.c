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
byte BUFFER[8];

/*
 * Bijection de [0,63] -> [0,63]
 * - ip[k] est l'image de k.
 */
byte ip[64];
byte ip_inv[64];

/*****************************************************************************/
/* Système de sélection de message                                           */
/*****************************************************************************/
byte selected[NB];
int selected_nb;

/* Remise à zero de la sélection */
void reset_selection() { 
	int m;
	for (m=0; m<NB; m++)
		selected[m] = 0;
	selected_nb = 0;
}

/* sélectionne un message */
void select_msg(int m) { 
	if (selected[m]==0) selected_nb++;
	selected[m] = 1; 
}

/* désélectionne un message */
void unselect_msg(int m) { 
	if (selected[m]==1) selected_nb--;
	selected[m] = 0;
}

/* est-ce que le message m est sélectionné ? */
byte is_selected(int m) {	
	return selected[m]; 
}

/*****************************************************************************/
int main(int argc, char * argv[]) {
/*****************************************************************************/
	int i,m;
	char str[1024];
	
	// Préliminaires
	for (i=0; i<1024; i++) str[i] = 0;
	
	// Lecture des messages depuis l'entrée standard.
	printf("\nLecture des messages:\n");
	read_message_list();
	for (m=0; m<5; m++) {
		sprint_byte_array(Messages[m], str, 8); printf("%s -> ",str); 
		sprint_byte_array(Chiffres[m], str, 8); printf("%s\n",str);    
  }
	printf ("...\nok.\n\n");

	// **********************
	// Calcul de la bijection
	// **********************
	printf("Décodage de la bijection:\n");
	
	/**************************************************************************/
	/* A VOUS DE JOUER !!!                                                    */
	/**************************************************************************/
	// Le coeur de l'attaque se trouve ici. Il s'agit de retrouver la bijection
	// inconnue. Pour ce faire, vous implémenterez l'algorithme suivant:
	
	/* Recherche de la bijection secrète:
	Pour toute colonne b \in [0,63] faire
	|	1. Selectionner les messages dont le bit à la position b vaut 1.
	| 2. Rechercher dans les chiffrés correspondants une colonne constante.
	| 3. La position de la colonne constante est l'image de b par la bijection.	
	La bijection résultante sera placée dans le tableau ip */
	/**************************************************************************/

 {
 	int somme[63];
	int b,b1;
	
	// Pour toute colonne b \in [0,63] :
	for (b=0; b<64; b++) {
			
			// 1. Selectionner les messages dont le bit à la position b vaut 1.
			reset_selection();
			for (m=0; m<NB; m++)
				if (get_nth_bit(Messages[m], b))
					select_msg(m);
			
			// 2. Rechercher dans les chiffrés correspondants une colonne constante.
			// Pour cela on fait la somme de toutes les colonnes, puis on chercher
			// une colonne de somme nulle ou totale.
			for (b1=0; b1<64; b1++)
				somme[b1] = 0;
				
			for (m=0; m<NB; m++)
				if (is_selected(m))
					for (b1=0; b1<64; b1++)
						somme[b1] += get_nth_bit(Chiffres[m], b1);
			
			// 3. La position de la colonne constante est l'image de b par la bijection.
			for (b1=0; b1<64; b1++)
				if (somme[b1]==0 || somme[b1]==selected_nb) {
					ip[b1] = b;
					break;
				}

	}
}

   
   printf("Bijection secrete:\n");
	display_mix64(ip);
	printf("\n");
	
	/**************************************************************************/
	/* A VOUS DE JOUER !!!                                                    */
	/**************************************************************************/
	// Maintenant que vous avez la bijection secrète, le plus dur est fait
    // A votre avis, comment récupérer la clé ?
    //
    //Indice: aidez-vous des fonctions inverse_mix64, mix64 et byte_array_xor
    /**************************************************************************/
    
    printf("Clé secrete:\n");
	inverse_mix64(ip, ip_inv);
	printf("Bijection inverse:\n");
	display_mix64(ip_inv);
	printf("\n");
	
	// On calcule maintenant la clé à partir du premier couple de clair chiffré.
    mix64(ip_inv, Chiffres[0], BUFFER);
    byte_array_xor(BUFFER, Messages[0],K,8);

	// Affiche la clé
	printf("GOTCHA! Voici la clé:\n");
	sprint_byte_array (K, str, 8); printf("\t%s (hexadecimal)\n",str);
	byte_array_copy (K, str, 8); str[8]=0;
	printf("\t%s (ascii)\n",str);

    
}









