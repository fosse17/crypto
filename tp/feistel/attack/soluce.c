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

/*
// Inversion de la bijection
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


*/
