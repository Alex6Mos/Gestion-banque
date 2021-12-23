#ifndef __SYNTAXE__SERVEUR__H__
#define __SYNTAXE__SERVEUR__H__


#define SYNTAX_ERROR 200 // pour toute erreur de syntaxe

#define NUM_CARTE_NO_EXISTS 500 // si num_carte n'existe pas dans la BD
#define MONTANT_INCORRECT 600 // si le montant est incorrect
#define ESPACE 100
#define ID_NO_EXISTS 300 // si id_terminal n'existe pas dans la BD
#define NUM_COMPTE_NO_EXISTS 400 // si un num de carte

typedef struct erreur{
	char *message_erreur ;
	int code_erreur ;
} Erreur ;

typedef struct syntaxe{
	Erreur mot_cle_carte ;
	Erreur mot_cle_compte  ;
	Erreur mot_cle_terminal ;
	Erreur mot_cle_montant  ;
	Erreur autre_syntaxe ;
} ErreurSyntaxe ;

ErreurSyntaxe init_erreur_syntaxe (ErreurSyntaxe erreur) ;
int syntaxe_valide (char *chaine , int *type_erreur) ;
int mot_cle_carte (char *chaine) ;
int mot_cle_montant (char *chaine) ;
int mot_cle_terminal (char *chaine) ;
int mot_cle_compte (char *chaine) ;
Erreur choix_erreur (int type_erreur , ErreurSyntaxe erreur) ;
char *afficher_erreur (Erreur erreur) ;
int zero_espace (char *chaine) ;
bool est_chiffre (char c) ;
bool point_virgule (char c) ;
bool deux_points (char c) ;
bool syntaxe_globale_correcte (char *chaine) ;
int syntaxe_id_terminal (char *chaine , int *type_erreur) ;
bool syntaxe_id_correct (char *chaine) ;

#endif