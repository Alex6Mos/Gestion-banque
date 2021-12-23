#include "serveur.c"
#include "syntaxe_serveur.c"
/* Serveur TCP 
 *
 * Ce code implémente un seveur réseau dans le cadre de nore projet BD/Réseaux
 *
 * ETAME CEDRIC <etamecedric7@gmail.com>
 */
 
 int main (int argc , char **argv){
	/* Variables pour notre connexion serveur_réseau-serveur_client */
	int serveur_ecoute , serveur_dial , client_len ;
	int option = 1 ;
	struct sockaddr_in serv_addr , cli_addr ;
	char buf [TAILLE_BUFFER] ;
	int so_reuseaddr = 1 ;

	/* Variables qui serviront a recuperer les informations necessaires a la transaction */
	double montant ;
	char *num_carte = NULL ;
	char *id_terminal = NULL ;
	char *num_compte = NULL ;
	char *societe = NULL ;
	char *date ;
	/*Variables necessaires pour la gestion des erreurs*/
	int syntaxe ;
	int type_erreur ;
	int code_retour = 0 ;

	/* Variables pour notre connexion serveur_reseau-serveur_BD */
	PGconn *connect ;
	PGresult *result = NULL ;
	
	#ifdef WIN32
	int code_resultat;
	WSADATA wsaData;
	code_resultat = WSAStartup(2, &wsaData);
	if (code_resultat != 0) {
		printf("Erreur lors de l'initialisation de Winsocks (code d'erreur %d).\n", code_resultat);
		exit(-1);
	}
#endif

	/* on désigne l'adresse+port qu'on va ouvrir */
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl (INADDR_ANY) ; /* on attend sur toutes nos adresses */
	serv_addr.sin_port = htons (PORT_DEFAUT) ; /* on attend sur le port 5000 */
	memset (&serv_addr.sin_zero, 0, sizeof (serv_addr.sin_zero)) ;

	/* on crée la socket d'ecoute et on l'associe au couple (adresse,port) defini ci-dessus */
	serveur_ecoute = socket (PF_INET , SOCK_STREAM , 0);
	if(setsockopt (serveur_ecoute , SOL_SOCKET, SO_REUSEADDR, (const char*)&so_reuseaddr , sizeof (so_reuseaddr)) < 0){
        perror("setsockopt(SO_REUSEADDR) failed") ;
	}

#ifdef SO_REUSEPORT
    if (setsockopt (serveur_ecoute , SOL_SOCKET, SO_REUSEPORT, (const char*)&so_reuseaddr, sizeof (so_reuseaddr)) < 0){
        perror("setsockopt(SO_REUSEPORT) failed") ;
	}
#endif

	bind (serveur_ecoute, (struct sockaddr*)&serv_addr, sizeof (serv_addr)) ;
	listen (serveur_ecoute, 5) ; /* on definit la socket serveur_ecoute, comme etant une socket d'ecoute*/
	/* On ajoute serveuyr_ecoute au maitre configure*/
	/*On garde une trace sur le plus grand descripteur configure*/
	
	client_len = sizeof (cli_addr) ;
	
	/* On se met en ecoute sur la socket. C'est une fonction blocante qui ne
	   se debloque que lorsqu'un client vient se connecter sur cette socket
	   d'ecoute. La valeur de retour est alors le descripteur de la socket
	   de connexion permettant  de dialoguer avec CE client. */
	serveur_dial = accept (serveur_ecoute , (struct sockaddr*) &cli_addr , (socklen_t*) &client_len);
	generer_log_v2("CLIENT_CONNECTED\n\0");

	connect = se_connecter_bd (connect) ;
	generer_log_v2 ("CONNECTED_TO_DATA_BASE\n\0") ;

	while (strncmp (buf , "quit" , 4)){
		memset (buf , '\0' , TAILLE_BUFFER) ;

		int min = get_min_actuelle () ;
		int next_min = min ;
		int code_rcv = -1 ;
		/*On boucle sur la fonction recv ici non bloquante pendant une minute a l'attente d'un message du client*/
		while (min == next_min && code_rcv == -1){
			code_rcv = recv (serveur_dial, buf , TAILLE_BUFFER , MSG_DONTWAIT) ;
			next_min = get_min_actuelle () ;
		}
		/*On a rien reçu et le timer s'est ecoule on estime que il y'a eu deconnexion du client*/
		if (code_rcv == -1){
			generer_log_v2 ("CONNECTION_LOST\n\0") ;
			strcpy (buf, "quit");
		}
		/* Si le client a clique sur le boutton effectuer une transaction (le serveur lui reçoit 'et') */
		if (strncmp (buf , "et" , 2) == 0){
			
			write (serveur_dial , "OK" , strlen ("OK")) ;
			printf ("OK\n") ;
			generer_log_v2("TRANSACTION_STARTED\n\0") ;
			memset (buf , '\0' , TAILLE_BUFFER) ; 

			min = get_min_actuelle () ;
			next_min = min ;
			code_rcv = -1 ;
			while (min == next_min && code_rcv == -1){
				code_rcv = recv (serveur_dial, buf, TAILLE_BUFFER, MSG_DONTWAIT) ;
				next_min = get_min_actuelle () ;
				//printf ("next_min = %d et min = %d\n", next_min , min) ;
			}

			if (code_rcv == -1){
				printf ("CLIENT DECONNECTE_2\n") ;
				generer_log_v2 ("CONNECTION_LOST\n\0") ;
				strcpy(buf , "quit") ;
				break ;
			}

			/* On recupere l'id_terminal et on verifie s'il est bien present dans la BD*/
			if (syntaxe_id_correct (buf)){
				int syntaxe_id = syntaxe_id_terminal (buf, &type_erreur) ;
				if (syntaxe_id == 0){
					/*On estime que l'id_terminal est correct alors on procede a l'identification du client*/
					generer_log_v2 ("SYNTAX_ID_CORRECT\n\0") ;
					recup_id_terminal (buf , &id_terminal) ;

					/*On verifie si l'id reçu existe bien dans la base de donnees*/
					if (id_exists (id_terminal , "Terminal" , "id_terminal" , connect , &code_retour)){
						generer_log_v2 ("ID_TERMINAL_EXISTS\n") ;
						recup_societe_terminal (id_terminal, connect, &code_retour , &societe) ; // On recupere la societe associe a l'id_terminal afin de l'identifier dans les logs

						if (code_retour != SOCIETE_INTROUVABLE){
							concatener_chaine(societe, " : CONNECTED\n\0");
							generer_log_v2 (societe) ;
							write (serveur_dial , id_terminal , strlen (id_terminal)) ;
						}
						memset (buf, '\0', TAILLE_BUFFER) ;
						min = get_min_actuelle () ;
						next_min = min ;
						code_rcv = -1;
						while (min == next_min && code_rcv == -1){
							code_rcv = recv(serveur_dial, buf, TAILLE_BUFFER, MSG_DONTWAIT);
							next_min = get_min_actuelle() ;
						}

						if (code_rcv == -1){
							printf("CLIENT DECONNECTE_3\n");
							generer_log_v2 ("CONNECTION_LOST\n\0") ;
							strcpy (buf, "quit");
							break ;
						}

						syntaxe = syntaxe_valide (buf, &type_erreur);
						/*On verifie la syntaxe globale de la chaine de caractere reçu selon des criteres determines*/
						if (syntaxe_globale_correcte (buf)){
							/*On estime que la chaine est correcte pour la manipulation*/
							if (syntaxe == 0){
								/* On recupere les parametres necessaires pour effectuer les requetes pour la transaction */
								generer_log_v2("GLOBAL_SYNTAX_CORRECT\n\0");
								/*On recupere chacune des informations necessaires a la transaction dans la chaine reçue*/
								montant = recup_montant (buf) ;
								recup_num_carte (buf , &num_carte) ;
								recup_num_compte (buf , &num_compte) ;
								write (serveur_dial , "OK", strlen ("OK")) ;

								if (montant != 0){
								/* On verifie si le num_carte , id_terminal ou encore num_compte existent bien dans la BD */
									if (id_exists (num_carte , "Carte" , "num_carte", connect , &code_retour)){
										generer_log_v2("NUM_CARD_OK\n\0");
										/*Verfication du num_compte*/
										if (id_exists (num_compte , "Compte" , "num_compte" , connect , &code_retour)){
											generer_log_v2 ("NUM_ACCOUNT_OK\n\0") ;
											/*Si et seulement si toutes les informations reçues sont valides alors on effectue la transaction
											 *Le retour de la fonction "effectuer_paiement" nous indiquera si touts s'est bien passe ou non
											*/
											result = effectuer_paiement (connect , montant , num_carte , id_terminal , num_compte , &code_retour) ;
											
											/*Selon la valeur du code_retour alors on detectera les erreurs survenues pour le rapport dans les logs*/	
											if (code_retour == ECHEC_REQUETE){
												fprintf (stderr , "Requete echouee : %s", PQerrorMessage (connect)) ;
												write (serveur_dial , "TRANSACTION FAILED\n" , strlen ("TRANSACTION FAILED\n"));
												generer_log(ECHEC_REQUETE);
											}
											if (code_retour == MONTANT_INSUFFISANT){
												write (serveur_dial , "TRANSACTION_FAILED\n" , strlen ("TRANSACTION FAILED\n")) ;
												generer_log (MONTANT_INSUFFISANT) ;
											}
											if (code_retour == 0) {
												write (serveur_dial , "PAYMENT_ACCEPTED\n" , strlen ("PAYMENT_ACCEPTED\n")) ;
												generer_log_v2 ("PAYMENT_ACCEPTED\n\0");
											}
											if (result != NULL){
												PQclear(result);
											}
					/*Au cas où l'une des conditions executees plus haut n'est pas verifiees alors le serveur renvoie "TRANSACTION FAILED"
					 *Tandis que dans les logs il sera un petit peu plus precis quant à la cause de l'echec de la transaction
					 */
										}
										else{
											write (serveur_dial , "TRANSACTION FAILED\n" , strlen ("TRANSACTION FAILED\n")) ;
											generer_log (NUM_COMPTE_NO_EXISTS) ;
										}
									}
									else{ 
										write (serveur_dial, "TRANSACTION FAILED\n", strlen("TRANSACTION FAILED\n"));
										generer_log (NUM_CARTE_NO_EXISTS);
									}
								}
								else{
									generer_log (MONTANT_INCORRECT) ;
									write (serveur_dial , "TRANSACTION FAILED\n" , strlen ("TRANSACTION FAILED\n")) ;
								}	
							}
						}
						else{
							printf ("\nSYNTAX_ERROR_4\n") ;
							write(serveur_dial , "TRANSACTION FAILED\n" , strlen("TRANSACTION FAILED\n"));
							generer_log (SYNTAX_ERROR) ;
						}	
					}
					else{
						write(serveur_dial , "TRANSACTION FAILED\n" , strlen ("TRANSACTION FAILED\n"));
						printf("\nError : id_terminal inexistant dans la Base de Donnees\n");
						generer_log (ID_NO_EXISTS) ;
					}
				}
			}
			else{
				write(serveur_dial , "TRANSACTION FAILED\n" , strlen ("TRANSACTION FAILED\n"));
				generer_log (SYNTAX_ERROR) ;
			}
		}
		
		/*On n'oublie pas de nettoyer les memoires occupees pour chaque allocation memoire (si elles ont ete occupees)*/
		if (societe != NULL && num_carte != NULL && num_compte != NULL && id_terminal != NULL){
			free (societe) ;
			free (num_carte) ;
			free (id_terminal) ;
			free (num_compte) ;
		}
	}
	
	close (serveur_dial) ;
	close (serveur_ecoute) ;
	generer_log_v2 ("DIAL_CLOSED\n") ;
#ifdef WIN32
	WSACleanup() ;
#endif
	PQfinish (connect) ;
	return 0 ;
}
		
		