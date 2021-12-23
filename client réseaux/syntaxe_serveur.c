
/* 
 Ce fichier regroupe les constantes qui identifiront les differents codes
 * d'erreur qui seront detectes par le serveur et les fonctions qui verifieront
 * ces syntaxes
*/

/* Code pour la syntaxe de la chaine de caractere recue par le serveur */

#include "syntaxe_serveur.h"

ErreurSyntaxe init_erreur_syntaxe (ErreurSyntaxe erreur){
	Erreur erreur_carte ;
	Erreur erreur_compte ;
	Erreur erreur_terminal ;
	Erreur erreur_montant ;
	Erreur erreur_autre ;
	
	erreur_carte.message_erreur = "Le mot cle carte n est pas present dans la chaine\0" ;
	erreur_carte.code_erreur = NUM_CARTE_NO_EXISTS ;
	
	erreur_compte.message_erreur = "Le mot cle compte n est pas present dans la chaine\0" ; 
	erreur_compte.code_erreur = NUM_COMPTE_NO_EXISTS ;
							 
	erreur_terminal.message_erreur = "Le mot cle id n est pas present dans la chaine\0" ;
	erreur_terminal.code_erreur = ID_NO_EXISTS ;
	
	erreur_montant.message_erreur = "Le mot cle montant n est pas present dans la chaine\0" ;
	erreur_montant.code_erreur = MONTANT_INCORRECT ;
	
	erreur_autre.message_erreur = "':' ou ';' absent\0" ;
	erreur_autre.code_erreur = SYNTAX_ERROR ;
	
	erreur.mot_cle_carte = erreur_carte ; 
	erreur.mot_cle_compte = erreur_compte ;
	erreur.mot_cle_terminal = erreur_terminal ; 
	erreur.mot_cle_montant = erreur_montant ;
	erreur.autre_syntaxe = erreur_autre ;
	
	return erreur ;
}

int syntaxe_valide (char *chaine , int *type_erreur){
	int i = 0 ;
	int compt_virgule = 0 ;
	int compt_point = 0 ;
	
	while (i < strlen (chaine) /*&& chaine [i] != " "*/){ 
		if (chaine [i] == ';'){
			compt_virgule++ ;
		}
		if (chaine [i] == ':'){
			compt_point++ ;
		}
		i++ ;
	}
	
	/* On detecte une erreur si on a croise un espace ou il y'a moins de 4 ";"
	 * ou s'il y'a moins de 4 ":"
	*/
	if (i < strlen (chaine) || compt_virgule < 3 || compt_point < 3){
		*type_erreur = SYNTAX_ERROR ;
		return SYNTAX_ERROR ; // return directement error
	}
	
	/* On detecte une erreur si l'un des mots cles (carte , compte , id , montant)
	 * ne sont presents dans la chaine de caracteres
	*/
	int erreur_carte = mot_cle_carte (chaine) ;
	int erreur_montant = mot_cle_montant (chaine) ;
	int erreur_compte = mot_cle_compte (chaine) ;
	
	if (erreur_carte  == NUM_CARTE_NO_EXISTS){
		*type_erreur = NUM_CARTE_NO_EXISTS ;
		return  SYNTAX_ERROR ;
	}
	if (erreur_montant  == MONTANT_INCORRECT){
		*type_erreur = MONTANT_INCORRECT ;
		return SYNTAX_ERROR ;
	}
	
	if (erreur_compte  == NUM_COMPTE_NO_EXISTS){
		*type_erreur = NUM_COMPTE_NO_EXISTS ;
		return SYNTAX_ERROR ;
	}
	/*if (erreur_espace == ESPACE){
		*type_erreur = ESPACE ;
		error = SYNTAX_ERROR ;
	}*/
	
	return 0 ;
}

int mot_cle_carte (char *chaine){
	int error = 0 ;
	char *sous_chaine = strstr (chaine , "carte") ;
	if (sous_chaine == NULL){
		error = NUM_CARTE_NO_EXISTS ;
	}
	return error ;
}

int mot_cle_montant (char *chaine){
	int error = 0 ;
	char *sous_chaine = strstr (chaine , "montant") ;
	if (sous_chaine == NULL){
		error = MONTANT_INCORRECT ;
	}
	return error ;
}

int mot_cle_terminal (char *chaine){
	int error = 0 ;
	char *sous_chaine = strstr (chaine , "id") ;
	if (sous_chaine == NULL){
		error = ID_NO_EXISTS ;
	}
	return error ;
}

int mot_cle_compte (char *chaine){
	int error = 0 ;
	char *sous_chaine = strstr (chaine , "compte") ;
	if (sous_chaine == NULL){
		error = NUM_COMPTE_NO_EXISTS ;
	}
	return error ;
}

Erreur choix_erreur (int type_erreur , ErreurSyntaxe erreur){
	Erreur retourner ;
	switch (type_erreur){
		case NUM_CARTE_NO_EXISTS :
			retourner = erreur.mot_cle_carte ;
			break ;
		case MONTANT_INCORRECT : 
			retourner = erreur.mot_cle_montant ;
			break ;
		case ID_NO_EXISTS : 
			retourner = erreur.mot_cle_terminal ;
			break ;
		case NUM_COMPTE_NO_EXISTS :
			retourner = erreur.mot_cle_compte ;
			break ;
		/*case ESPACE :
			retourner = erreur.espace ;
			break ;*/
		case SYNTAX_ERROR :
			retourner = erreur.autre_syntaxe ;
			break ;
		default :
			;
	}
	return retourner ;
}

char *afficher_erreur (Erreur erreur){
	return erreur.message_erreur ;
}	

int syntaxe_id_terminal (char *chaine , int *type_erreur){
	bool virgule = false ;
	bool point = false ;
	
	for (int i = 0 ; i < strlen (chaine) ; i++){
		if (chaine [i] == ';'){
			virgule = true ;
		}
		if (chaine [i] == ':'){
			point = true ;
		}
	}
	if (virgule == false || point == false){
		*type_erreur = SYNTAX_ERROR ;
		return SYNTAX_ERROR ;
	}
	if (mot_cle_terminal (chaine) == ID_NO_EXISTS){
		*type_erreur = ID_NO_EXISTS ;
		return SYNTAX_ERROR ;
	}
	
	return 0 ;
}



bool point_virgule (char c){
	return c == ';' ;
}

bool deux_points (char c){
	return c == ':' ;
}

bool point (char c){
	return c == '.' ;
}

bool syntaxe_globale_correcte (char *chaine){
	int i = 0 ;
	int posfound = 0 ;
	bool syntax_correct = false ;
	bool syntax_correct_m = false ;
	bool syntax_correct_compte = false ;
	int size = strlen (chaine) ;
	posfound = recup_pos_chaine (chaine , "carte") ;
	if (posfound != 0){
		if (posfound + 1 < size && chaine [posfound + 1] == ':'){
			int j = posfound + 2 ;
			int compt = 0 ;
			while (j < size && est_chiffre (chaine [j]) && compt < 16){
				compt++ ;
				j++ ;
			}
			if (chaine [j] == ';' && compt == 16){
				syntax_correct = true ;
			}
		}
	}
	posfound = recup_pos_chaine (chaine , "compte") ;
	if (posfound != 0){
		if (posfound + 1 < size && chaine [posfound + 1] == ':'){
			int j = posfound + 2 ;
			int compt = 0 ;
			while (j < size && est_chiffre (chaine [j]) && compt < 8){
				compt++ ;
				j++ ;
			}
			if (chaine [j] == ';' && compt == 8){
				syntax_correct_compte = true ;
			}
		}
	}
		
	posfound = recup_pos_chaine (chaine , "montant") ;
	if (posfound != 0){
		if (posfound + 1 < size && chaine [posfound + 1] == ':'){
			int j = posfound + 2 ;
			int compt = 0 ;
			while (j < size && (est_chiffre (chaine [j]) || point (chaine [j])) && compt < 7){
				compt++ ;
				j++ ;
			}
			if (chaine [j] == ';' && compt <= 7){
				syntax_correct_m = true ;
			}
		}
	}
	
	return  syntax_correct && syntax_correct_m && syntax_correct_compte ;
}

bool syntaxe_id_correct (char *chaine){
	bool syntax_correct = false ;
	int posfound = recup_pos_chaine (chaine , "id") ;
	int size = strlen (chaine) ;
	if (posfound != 0){
		if (posfound + 1 < size && chaine [posfound + 1] == ':'){
			int j = posfound + 2 ;
			int compt = 0 ;
			while (j < size && est_chiffre (chaine [j]) && compt < 8){
				compt++ ;
				j++ ;
			}
			if (chaine [j] == ';' && compt == 8){
				syntax_correct = true ;
			}
		}
	}
	return syntax_correct ;
}