/************************************************************************/
/* Auteur : LECLER Quentin									    		*/
/* Groupe : 2102														*/
/* Application : toysInBox.c											*/
/* Description : toysInBox.c											*/
/* Date de dernière mise à jour : 06/05/10								*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define QUITTER			10
#define INDEX_JEUX_LEN	1800 /* 36 (sizeof(IndexJeu)) * 50 */
#define JEUX_LEN		3200 /* 64 (sizeof(Jeu)) * 50 */
#define NOM_JX_LEN		25
#define JOUEURS_LEN		7684 /* 256 (sizeof(Joueur)) * 30 */
#define NOM_JR_LEN		25
#define PRENOM_JR_LEN	25
#define ADRESSE_JR_LEN	50
#define CODEP_JR_LEN	5
#define VILLE_JR_LEN	25
#define PAYS_JR_LEN		25
#define TEL_JR_LEN		13
#define MAIL_JR_LEN		65

/************************************************************************/

struct _Date
{
	int jour;
	int mois;
	int annee;
};

typedef struct _Date Date;

struct _Heure
{
	int heure;
	int minute;
};

typedef struct _Heure Heure;

struct _Jeu
{
	int idJeu;
	char nom[NOM_JX_LEN];
	char interieur;
	char exterieur;
	int ageMin;
	int ageMax;
	int nbJoueursMin;
	int nbJoueursMax;
	Date dateAchat;
	int dureeGarantie;
};

typedef struct _Jeu Jeu;

struct _IndexJeu
{
	int idJeu;
	char nom[NOM_JX_LEN];
	int indiceJeu;
};

typedef struct _IndexJeu IndexJeu;

struct _Joueur
{
	int idJoueur;
	char nom[NOM_JR_LEN];
	char prenom[PRENOM_JR_LEN];
	Date dateNaissance;
	char adresse[ADRESSE_JR_LEN];
	char codePostal[CODEP_JR_LEN];
	char ville[VILLE_JR_LEN];
	char pays[PAYS_JR_LEN];
	char telephone[TEL_JR_LEN];
	char adMail[MAIL_JR_LEN];
	int jsvt;
};

typedef struct _Joueur Joueur;

struct _Partie
{
	int idJoueur;
	int idJeu;
	Date datePartie;
	Heure heurePartie;
	int points;
};

typedef struct _Partie Partie;

/************************************************************************/

int menu();
void action(int operation, IndexJeu *indexJeux, struct tm temps);
int determinerMaxJ(int mois, int annee);
int verifierDate(struct tm temps, Date date);
int verifierAgeDixHuit(struct tm temps, Date date);
int verifierPartie(int idJoueur, Date datePartie, Heure heurePartie);
void trierJeu(IndexJeu *indexJeux);
int lireChaine(char *pc, int max);
int lireDate(char *chDate, Date *date);
int lireHeure(char *chHeure, Heure *heure);
void lireCodePostal(char *pc, int max);
void lireIdJeu(IndexJeu *indexJeux, Jeu *jeu, IndexJeu *indexJeu);
void lireNomJeu(Jeu *jeu, IndexJeu *indexJeu);
void lireInterieurJeu(Jeu *jeu);
void lireExterieurJeu(Jeu *jeu);
void lireAgeMinJeu(Jeu *jeu);
void lireAgeMaxJeu(Jeu *jeu);
void lireNbJoueursMinJeu(Jeu *jeu);
void lireNbJoueursMaxJeu(Jeu *jeu);
void lireDateAchatJeu(Jeu *jeu, struct tm temps);
void lireDureeGarantieJeu(Jeu *jeu);
void lireJeu(IndexJeu *indexJeux, Jeu *jeu, IndexJeu *indexJeu, struct tm temps);
void lireJoueur(Joueur *joueur, struct tm temps);
char lireModification(char *modification);
void relireJeu(IndexJeu *indexJeux, Jeu *jeu, IndexJeu *indexJeu, struct tm temps);
void afficherJeu(Jeu jeu);
void afficherJoueur(Joueur joueur);
void afficherPartie(Partie partie);
int rechercherJeu(IndexJeu *indexJeux, int idJeu, int *indiceIndexJeu, int *indiceJeu);
int rechercherJoueur(int idJoueur);
int rechercherPartie(int idJeu);
int bidonnageJeu(IndexJeu *indexJeux);
int bidonnageIndexJeu(IndexJeu *indexJeux);
int sauvegardeIndexJeu(IndexJeu *indexJeux);
int bidonnageJoueur(void);
int bidonnagePartie(void);
int ajouterJeu(IndexJeu *indexJeux, struct tm temps);
int ajouterJoueur(struct tm temps);
int ajouterPartie(IndexJeu *indexJeux);
int modifierJeu(IndexJeu *indexJeux, int idJeu, struct tm temps);
int supprimerJeu(IndexJeu *indexJeux, int idJeu);
int afficherToutJeu(IndexJeu *indexJeux);
int afficherToutJoueur(void);
int afficherJoueurDixHuit(struct tm temps);
int afficherToutePartie(void);

/************************************************************************/

int main(int argc, char *argv[])
{
	int choix;
	time_t timestamp;
	struct tm *temps; /* date et heure actuelle */
	IndexJeu indexJeux[50];

	timestamp = time(NULL);
	temps = localtime(&timestamp);

	if(bidonnageJeu(&indexJeux[0]) == 1)
	{
		if(bidonnageJoueur() == 1)
		{
			if(bidonnagePartie() == 1)
			{
				do
				{
					choix = menu();
					action(choix, &indexJeux[0], *temps);
				} while(choix != QUITTER);
			}
		}

		else
		{
			perror("Joueurs.tib");
			system("pause");
		}
	}

	else
	{
		perror("Jeux.tib");
		system("pause");
	}

	sauvegardeIndexJeu(&indexJeux[0]);

	return 0;
}

/************************************************************************/
/* INPUT : void															*/
/* PROCESS : affiche le menu et demande un choix à l'utilisateur	 	*/
/* OUTPUT : choix de l'utilisateur										*/
/************************************************************************/

int menu()
{
    int choix;

    do
    {
        system("cls");
        printf("Voici les differentes possibilites d'action : \n");
		printf("\t1. Ajouter un jeu.\n");
		printf("\t2. Modifier un jeu.\n");
		printf("\t3. Supprimer un jeu.\n");
		printf("\t4. Afficher tout les jeux.\n");
		printf("\t5. Ajouter un joueur.\n");
		printf("\t6. Afficher tout les joueurs.\n");
		printf("\t7. Afficher les joueurs de plus de 18 ans.\n");
		printf("\t8. Ajouter une partie.\n");
		printf("\t9. Afficher toute les parties.\n");
        printf("\t10. Quitter.\n\n\n");
        printf("Faites votre choix : ");
        fflush(stdin);
        scanf("%d", &choix);
    } while((choix < 1) || (choix > QUITTER));

    return choix;
}

/************************************************************************/
/* INPUT : le choix de l'utilisateur (operation)						*/
/*		   le tableau index de jeu (*indexJeux)							*/
/*		   structure contenant la date et l'heure actuelle (temps)		*/
/* PROCESS : effectue une action suivant le choix de l'utilisateur		*/
/* OUTPUT : void														*/
/************************************************************************/

void action(int operation, IndexJeu *indexJeux, struct tm temps)
{
	int idJeu;

	system("cls");

	switch(operation)
	{
		case 1:
			ajouterJeu(indexJeux, temps);
			break;
		case 2:
			do
			{
				printf("Veuillez encoder l\'identifiant du jeu a modifier : ");
				scanf("%d", &idJeu);
				if(idJeu == -1)
				{
					printf("L\'identifiant du jeu doit etre different de -1.\n");
				}
			} while(idJeu == -1); /* -1 est utilisé comme suppression logique */
			modifierJeu(indexJeux, idJeu, temps);
			break;
		case 3:
			do
			{
				printf("Veuillez encoder l\'identifiant du jeu a supprimer : ");
				scanf("%d", &idJeu);
				if(idJeu == -1)
				{
					printf("L\'identifiant du jeu doit etre different de -1.\n");
				}
			} while(idJeu == -1); /* -1 est utilisé comme suppression logique */
			supprimerJeu(indexJeux, idJeu);
			break;
		case 4:
			afficherToutJeu(indexJeux);
			system("pause");
			break;
		case 5:
			ajouterJoueur(temps);
			break;
		case 6:
			afficherToutJoueur();
			system("pause");
			break;
		case 7:
			afficherJoueurDixHuit(temps);
			system("pause");
			break;
		case 8:
			ajouterPartie(indexJeux);
			break;
		case 9:
			afficherToutePartie();
			system("pause");
			break;
	}
}

/************************************************************************/
/* INPUT : le mois (mois)												*/
/*		   l'année (annee)												*/
/* PROCESS : détermine le nombre maximum de jours						*/
/* OUTPUT : le nombre maximum de jours									*/
/************************************************************************/

int determinerMaxJ(int mois, int annee)
{
	int jour;

	switch(mois)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			jour = 31;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			jour = 30;
			break;
		case 2:
			if(((annee % 4) == 0) && ((annee % 1000) == 0 || (annee % 100) != 0))
			{
				jour = 29;
			}
			else
			{
				jour = 28;
			}
			break;
		default:
			jour = 0;
	}

	return jour;
}

/************************************************************************/
/* INPUT : structure contenant la date et l'heure actuelle (temps)		*/
/*		   structure contenant la date à vérifier (date)				*/
/* PROCESS : permet la vérification d'une date							*/
/* OUTPUT : 0/ 1/ 2														*/
/************************************************************************/

int verifierDate(struct tm temps, Date date)
{
	int retour;
	int dateDonnee;
	int dateAujourdhui;

	dateDonnee = date.jour + (date.mois * 30) + (date.annee * 365);
	dateAujourdhui = temps.tm_mday + ((temps.tm_mon + 1) * 30) + ((1900 + temps.tm_year) * 365);

	if(dateDonnee > dateAujourdhui)
	{
		retour = 2; /* date supérieure à la date du jour */
	}
	else
	{
		if(dateDonnee < dateAujourdhui)
		{
			retour = 0; /* date inférieure à la date du jour */
		}
		else
		{
			retour = 1; /* date égale à la date du jour */
		}
	}

	return retour;
}

/************************************************************************/
/* INPUT : structure contenant la date et l'heure actuelle (temps)		*/
/*		   structure contenant la date à vérifier (date)				*/
/* PROCESS : permet de vérifier si le joueur à plus de 18 ans			*/
/* OUTPUT : bool														*/
/************************************************************************/

int verifierAgeDixHuit(struct tm temps, Date date)
{
	int retour;
	int dateDonnee;
	int dateAujourdhui;

	dateDonnee = date.jour + (date.mois * 30) + (date.annee * 365);
	dateAujourdhui = temps.tm_mday + ((temps.tm_mon + 1) * 30) + ((1900 + temps.tm_year) * 365);

	if(((dateAujourdhui - dateDonnee) / 365) > 18)
	{
		retour = 1;
	}
	else
	{
		retour = 0;
	}

	return retour;
}

/************************************************************************/
/* INPUT : l'identifiant du joueur (idJoueur)							*/
/*		   la date de la partie (datePartie)							*/
/*		   l'heure de la partie (heurePartie)							*/
/* PROCESS : permet de vérifier la validité d'une partie				*/
/* OUTPUT : bool														*/
/************************************************************************/

int verifierPartie(int idJoueur, Date datePartie, Heure heurePartie)
{
	int retour;
	Partie pTemp;
	FILE *fp;

	retour = 0;
	fp = fopen("PartiesJouees.tib", "rb");

	if(fp == (FILE *) NULL)
	{
		perror("PartiesJouees.tib");
		system("pause");
	}

	else
	{
		while((retour == 0) && (feof(fp) == 0))
		{
			fread(&pTemp, sizeof(Partie), 1, fp);

			if(pTemp.idJoueur == idJoueur)
			{
				if((pTemp.datePartie.jour == datePartie.jour) && \
					(pTemp.datePartie.mois == datePartie.mois) && \
					(pTemp.datePartie.annee == datePartie.annee))
				{
					if((pTemp.heurePartie.heure == heurePartie.heure) && \
						(pTemp.heurePartie.minute == heurePartie.minute))
					{
						retour = 1;
					}
				}
			}
		}

		fclose(fp);
	}

	return retour;
}

/************************************************************************/
/* INPUT : le tableau index de jeu (*indexJeux)							*/
/* PROCESS : permet le tri du tableau index de jeu (version tri-bulle)	*/
/* OUTPUT : bool														*/
/************************************************************************/

void trierJeu(IndexJeu *indexJeux)
{
	IndexJeu tmp;
	IndexJeu *pTemp1;
	IndexJeu *pTemp2;
	int tailleIndex;
	int compteur;

	tailleIndex = 50;

	while(tailleIndex > 0)
	{
		compteur = 0;

		while(compteur < tailleIndex)
		{
			pTemp1 = (indexJeux+compteur);
			pTemp2 = (indexJeux+compteur+1);

			if(strcmp(pTemp1->nom, pTemp2->nom) > 0)
			{
				tmp = *pTemp1;
				pTemp1->idJeu = pTemp2->idJeu;
				strcpy(pTemp1->nom, pTemp2->nom);
				pTemp1->indiceJeu = pTemp2->indiceJeu;
				pTemp2->idJeu = tmp.idJeu;
				strcpy(pTemp2->nom, tmp.nom);
				pTemp2->indiceJeu = tmp.indiceJeu;
			}

			compteur = compteur + 1;
		}

		tailleIndex = tailleIndex - 1;
	}
}

/************************************************************************/
/* INPUT : l'adresse du début de la chaîne de caractères (*pc)			*/
/*		   le nombre maximum de caractères à encoder					*/
/* PROCESS : permet l'encodage d'une chaîne de caractères				*/
/* OUTPUT : bool														*/
/************************************************************************/

int lireChaine(char *pc, int max)
{
	int compteurCar;
	char car;

	compteurCar = 0;
	fflush(stdin);

	do
	{
		car = getchar();
		if(car != '\n')
		{
			*pc = car;
			pc++;
			compteurCar++;
		}
	} while((compteurCar < max-1) && (car != '\n'));

	*pc = '\0';

	return compteurCar;
}

/************************************************************************/
/* INPUT : l'adresse du début de la chaîne de caractères (*chDate)		*/
/*		   structure conentant la date à encoder (*date)				*/
/* PROCESS : permet l'encodage d'une date								*/
/* OUTPUT : bool														*/
/************************************************************************/

int lireDate(char *chDate, Date *date)
{
	int retour;

	retour = 0;

	date->jour = (*(chDate) - 48) * 10 + (*(chDate + 1) - 48);
	date->mois = (*(chDate + 3) - 48) * 10 + (*(chDate + 4) - 48);
	date->annee = (*(chDate + 6) - 48) * 1000 + (*(chDate + 7) - 48) * 100 + \
		(*(chDate + 8) - 48) * 10 + (*(chDate + 9) - 48);

	if((date->jour >= 1) && (date->jour <= 31))
	{
		if((date->mois >= 1) || (date->mois <= 12))
		{
			if(date->jour <= determinerMaxJ(date->mois, date->annee))
			{
				retour = 1;
			}
		}
	}

	return retour;
}

/************************************************************************/
/* INPUT : l'adresse du début de la chaîne de caractères (*chHeure)		*/
/*		   structure contenant l'heure à encoder (*heure)				*/
/* PROCESS : permet l'encodage d'une heure								*/
/* OUTPUT : bool														*/
/************************************************************************/

int lireHeure(char *chHeure, Heure *heure)
{
	int retour;

	retour = 0;

	heure->heure = (*(chHeure) - 48) * 10 + (*(chHeure + 1) - 48);
	heure->minute = (*(chHeure + 3) - 48) * 10 + (*(chHeure + 4) - 48);

	if((heure->heure >= 1) && (heure->heure <= 24))
	{
		if((heure->minute >= 0) && (heure->minute <= 60))
		{
			retour = 1;
		}
	}

	return retour;
}

/************************************************************************/
/* INPUT : l'adresse du début de la chaîne 'code postal' (*pc)			*/
/*		   le nombre maximum de caractères à encoder (max)				*/
/* PROCESS : permet l'encodage d'un code postal							*/
/* OUTPUT : void														*/
/************************************************************************/

void lireCodePostal(char *pc, int max)
{
	int compteurCar;
	char car;

	compteurCar = 0;
	fflush(stdin);

	do
	{
		car = getchar();
		if((car != '\n') && (car >= '0' && car <= '9'))
		{
			*pc = car;
			pc++;
			compteurCar++;
		}
	} while((compteurCar < max-1) || (car != '\n'));

	*pc = '\0';
}

/************************************************************************/
/* INPUT : le tableau index de jeu (*indexJeux)							*/
/*		   le jeu à encoder (*jeu)										*/
/*		   l'index du jeu à encoder (*indexJeu)							*/
/* PROCESS : permet l'encodage de l'identifiant du jeu					*/
/* OUTPUT : void														*/
/************************************************************************/

void lireIdJeu(IndexJeu *indexJeux, Jeu *jeu, IndexJeu *indexJeu)
{
	int indiceIndexJeu;
	int indiceJeu;

	do
	{
		printf("Veuillez encoder l\'identifiant du jeu : ");
		scanf("%d", &(jeu->idJeu));

		if(jeu->idJeu == -1) /* -1 est utilisé comme suppression logique */
		{
			printf("L\'identifiant du jeu doit etre different de -1.\n");
		}

		else
		{
			if(rechercherJeu(indexJeux, jeu->idJeu, &indiceIndexJeu, &indiceJeu) == 1)
			{
				printf("L\'identifiant du jeu doit etre unique.\n");
			}
		}
	} while((jeu->idJeu == -1) || \
		(rechercherJeu(indexJeux, jeu->idJeu, &indiceIndexJeu, &indiceJeu) == 1));

	indexJeu->idJeu = jeu->idJeu;
}

/************************************************************************/
/* INPUT : le jeu à encoder (*jeu)										*/
/*		   l'index sur le jeu à encoder (*indexJeu)						*/
/* PROCESS : permet l'encodage du nom du jeu							*/
/* OUTPUT : void														*/
/************************************************************************/

void lireNomJeu(Jeu *jeu, IndexJeu *indexJeu)
{
	do
	{
		printf("Veuillez encoder le nom du jeu : ");
	} while(lireChaine(jeu->nom, NOM_JX_LEN) == 0);

	strcpy(indexJeu->nom, jeu->nom);
}

/************************************************************************/
/* INPUT : le jeu à encoder (*jeu)										*/
/* PROCESS : permet l'encodage de l'intérieur du jeu					*/
/* OUTPUT : void														*/
/************************************************************************/

void lireInterieurJeu(Jeu *jeu)
{
	do
	{
		printf("Le jeu peut-il etre joue a l\'interieur (O/ N) ? ");
		fflush(stdin);
		jeu->interieur = toupper(getchar());
	} while((jeu->interieur != 'O') && (jeu->interieur != 'N'));
}

/************************************************************************/
/* INPUT : le jeu à encoder (*jeu)										*/
/* PROCESS : permet l'encodage de l'extérieur du jeu					*/
/* OUTPUT : void														*/
/************************************************************************/

void lireExterieurJeu(Jeu *jeu)
{
	do
	{
		printf("Le jeu peut-il etre joue a l\'exterieur (O/ N) ? ");
		fflush(stdin);
		jeu->exterieur = toupper(getchar());
	} while((jeu->exterieur != 'O') && (jeu->exterieur != 'N'));
}

/************************************************************************/
/* INPUT : le jeu à encoder (*jeu)										*/
/* PROCESS : permet l'encodage de l'âge minimum du jeu					*/
/* OUTPUT : void														*/
/************************************************************************/

void lireAgeMinJeu(Jeu *jeu)
{
	do
	{
		printf("Veuillez encoder l\'age minimum du jeu : ");
		scanf("%d", &(jeu->ageMin));

		if(jeu->ageMin < 0)
		{
			printf("L\'age minimum du jeu doit etre superieur ou egal a 0.\n");
		}
	} while(jeu->ageMin < 0);
}

/************************************************************************/
/* INPUT : le jeu à encoder (*jeu)										*/
/* PROCESS : permet l'encodage de l'âge maximum du jeu					*/
/* OUTPUT : void														*/
/************************************************************************/

void lireAgeMaxJeu(Jeu *jeu)
{
	do
	{
		printf("Veuillez encoder l\'age maximum du jeu : ");
		scanf("%d", &(jeu->ageMax));

		if(jeu->ageMax < 0)
		{
			printf("L\'age maximum du jeu doit etre superieur ou egal a 0.\n");
		}
	} while(jeu->ageMax < 0);
}

/************************************************************************/
/* INPUT : le jeu à encoder (*jeu)										*/
/* PROCESS : permet l'encodage du nombre minimum de joueur(s)			*/
/* OUTPUT : void														*/
/************************************************************************/

void lireNbJoueursMinJeu(Jeu *jeu)
{
	do
	{
		printf("Veuillez encoder le nombre minimum de joueurs du jeu : ");
		scanf("%d", &(jeu->nbJoueursMin));

		if(jeu->nbJoueursMin < 1)
		{
			printf("Le nombre minimum de joueurs du jeu doit etre superieur ou egal a 1.\n");
		}
	} while(jeu->nbJoueursMin < 1);
}

/************************************************************************/
/* INPUT : le jeu à encoder (*jeu)										*/
/* PROCESS : permet l'encodage du nombre maximum de joueur(s)			*/
/* OUTPUT : void														*/
/************************************************************************/

void lireNbJoueursMaxJeu(Jeu *jeu)
{
	do
	{
		printf("Veuillez encoder le nombre maximum de joueurs du jeu : ");
		scanf("%d", &(jeu->nbJoueursMax));

		if(jeu->nbJoueursMax < 1)
		{
			printf("Le nombre maximum de joueurs du jeu doit etre superieur ou egal a 1.\n");
		}
	} while(jeu->nbJoueursMax < 1);
}

/************************************************************************/
/* INPUT : le jeu à encoder (*jeu)										*/
/*  	   la structure contenant la date et l'heure actuelle (temps)	*/
/* PROCESS : permet l'encodage de la date d'achat du jeu				*/
/* OUTPUT : void														*/
/************************************************************************/

void lireDateAchatJeu(Jeu *jeu, struct tm temps)
{
	char buf[11];

	do
	{
		do
		{
			printf("Veuillez encoder la date d\'achat du jeu (DD/MM/YYYY) : ");
			lireChaine(buf, 11);

			if(lireDate(buf, &(jeu->dateAchat)) == 0)
			{
				printf("La date saisie est invalide.\n");
			}
		} while(lireDate(buf, &(jeu->dateAchat)) == 0);

		if(verifierDate(temps, jeu->dateAchat) >= 1)
		{
			printf("La date d\'achat du jeu doit etre inferieure ou egale a la date du jour.\n");
		}
	} while(verifierDate(temps, jeu->dateAchat) >= 1);
}

/************************************************************************/
/* INPUT : le jeu à encoder (*jeu)										*/
/* PROCESS : permet l'encodage de la durée de garantie du jeu			*/
/* OUTPUT : void														*/
/************************************************************************/

void lireDureeGarantieJeu(Jeu *jeu)
{
	do
	{
		printf("Veuillez encoder la duree de la garantie du jeu : ");
		scanf("%d", &(jeu->dureeGarantie));

		if(jeu->dureeGarantie < 0)
		{
			printf("La duree de la garantie doit etre superieure ou egale a 0.\n");
		}
	} while(jeu->dureeGarantie < 0);
}

/************************************************************************/
/* INPUT : le tableau index de jeu (*indexJeux)							*/
/*		   le jeu à encoder (*jeu)										*/
/*		   l'index sur le jeu à encoder (*indexJeu)						*/
/*		   structure contenant l'heure et la date actuelle (temps)		*/
/* PROCESS : permet l'encodage d'un jeu									*/
/* OUTPUT : void														*/
/************************************************************************/

void lireJeu(IndexJeu *indexJeux, Jeu *jeu, IndexJeu *indexJeu, struct tm temps)
{
	/* identifiant jeu */

	lireIdJeu(indexJeux, jeu, indexJeu);

	/* nom jeu */

	lireNomJeu(jeu, indexJeu);

	/* intérieur jeu */

	lireInterieurJeu(jeu);

	/* extérieur jeu */

	lireExterieurJeu(jeu);

	do
	{
		/* âge minimum jeu */

		lireAgeMinJeu(jeu);

		/* âge maximum jeu */

		lireAgeMaxJeu(jeu);

		if(jeu->ageMin >= jeu->ageMax)
		{
			printf("L\'age minimum doit etre inferieur a l\'age maximum du jeu.\n");
		}
	} while(jeu->ageMin >= jeu->ageMax);

	do
	{
		/* nombre de joueurs minimum jeu */

		lireNbJoueursMinJeu(jeu);

		/* nombre de joueurs maximum jeu */

		lireNbJoueursMaxJeu(jeu);

		if(jeu->nbJoueursMin > jeu->nbJoueursMax)
		{
			printf("Le nombre de joueurs minimum doit etre inferieur ");
			printf("ou egal au nombre de joueurs maximum du jeu.\n");
		}
	} while(jeu->nbJoueursMin > jeu->nbJoueursMax);

	/* date d'achat jeu (inférieure ou égal à la date du jour) */

	lireDateAchatJeu(jeu, temps);

	/* durée garantie jeu */

	lireDureeGarantieJeu(jeu);
}

/************************************************************************/
/* INPUT : le joueur à encoder (*joueur)								*/
/*		   structure contenant l'heure et la date actuelle (temps)		*/
/* PROCESS : permet l'encodage d'un joueur								*/
/* OUTPUT : void														*/
/************************************************************************/

void lireJoueur(Joueur *joueur, struct tm temps)
{
	int retourMail;
	char buf[11];

	/* identifiant joueur */

	do
	{
		printf("Veuillez encoder l\'identifiant du joueur : ");
		scanf("%d", &(joueur->idJoueur));

		if(rechercherJoueur(joueur->idJoueur) == 1)
		{
			printf("L\'identifiant du joueur doit etre unique.\n");
		}
	} while(rechercherJoueur(joueur->idJoueur) == 1);

	/* nom joueur */

	do
	{
		printf("Veuillez encoder le nom du joueur : ");
	} while(lireChaine(joueur->nom, NOM_JR_LEN) == 0);

	/* prénom joueur */

	do
	{
		printf("Veuillez encoder le prenom du joueur : ");
	} while(lireChaine(joueur->prenom, PRENOM_JR_LEN) == 0);

	/* date de naissance joueur (inférieure à la date du jour) */

	do
	{
		do
		{
			printf("Veuillez encoder la date d\'anniversaire du joueur (DD/MM/YYYY) : ");
			lireChaine(buf, 11);
			if(lireDate(buf, &(joueur->dateNaissance)) == 0)
			{
				printf("La date saisie est invalide.\n");
			}
		} while(lireDate(buf, &(joueur->dateNaissance)) == 0);

		if(verifierDate(temps, joueur->dateNaissance) != 0)
		{
			printf("La date d\'anniversaire doit etre inferieure a la date du jour.\n");
		}
	} while(verifierDate(temps, joueur->dateNaissance) != 0);

	/* adresse joueur */

	printf("Veuillez encoder l\'adresse du joueur : ");
	lireChaine(joueur->adresse, ADRESSE_JR_LEN);

	/* code postal joueur */

	printf("Veuillez encoder le code postal du joueur : ");
	lireCodePostal(joueur->codePostal, CODEP_JR_LEN);

	/* ville joueur */

	printf("Veuillez encoder la ville du joueur	: ");
	lireChaine(joueur->ville, VILLE_JR_LEN);

	/* pays joueur */

	printf("Veuillez encoder le pays du joueur : ");
	lireChaine(joueur->pays, PAYS_JR_LEN);

	/* téléphone joueur */

	printf("Veuillez encoder le numero de telephone du joueur : ");
	lireChaine(joueur->telephone, TEL_JR_LEN);

	/* adresse mail joueur */

	do
	{
		printf("Veuillez encoder l\'adresse mail du joueur : ");
		retourMail = lireChaine(joueur->adMail, MAIL_JR_LEN);

		if((retourMail != 0) && ((strchr(joueur->adMail, '@') == NULL) || \
			(strchr(joueur->adMail, '.') == NULL)))
		{
			printf("Veuillez encoder une adresse mail valide.\n");
		}
	} while((retourMail != 0) && ((strchr(joueur->adMail, '@') == NULL) || \
		(strchr(joueur->adMail, '.') == NULL)));

	/* joueur suivant */

	joueur->jsvt = -1;
}

/************************************************************************/
/* INPUT : la modification (*modification)								*/
/* PROCESS : permet de demander l'autorisation d'une modification		*/
/* OUTPUT : bool														*/
/************************************************************************/

char lireModification(char *modification)
{
	char retour;

	do
	{
		printf("%s", modification);
		fflush(stdin);
		retour = toupper(getchar());
	} while((retour != 'O') && (retour != 'N'));

	return retour;
}

/************************************************************************/
/* INPUT : le tableau index de jeu (*indexJeux)							*/
/*		   le jeu à réencoder (*jeu)									*/
/*		   l'index sur le jeu à encoder (*indexJeu)						*/
/*		   structure contenant l'heure et la date actuelle (temps)		*/
/* PROCESS : permet le réencodage d'un jeu								*/
/* OUTPUT : void														*/
/************************************************************************/

void relireJeu(IndexJeu *indexJeux, Jeu *jeu, IndexJeu *indexJeu, struct tm temps)
{
	char modification[100];

	/* identifiant jeu */

	strcpy(modification, "Voulez-vous modifier l\'identifiant du jeu (O/ N) ? ");
	if(lireModification(modification) == 'O')
	{
		lireIdJeu(indexJeux, jeu, indexJeu);
	}

	/* nom jeu */

	strcpy(modification, "Voulez-vous modifier le nom du jeu (O/ N) ? ");
	if(lireModification(modification) == 'O')
	{
		lireNomJeu(jeu, indexJeu);
	}

	/* intérieur jeu */

	strcpy(modification, "Voulez-vous modifier si le jeu peut-etre joue a l\'interieur (O/ N) ? ");
	if(lireModification(modification) == 'O')
	{
		lireInterieurJeu(jeu);
	}

	/* extérieur jeu */

	strcpy(modification, "Voulez-vous modifier si le jeu peut-etre joue a l\'exterieur (O/ N) ? ");
	if(lireModification(modification) == 'O')
	{
		lireExterieurJeu(jeu);
	}

	do
	{
		/* âge minimum jeu */

		strcpy(modification, "Voulez-vous modifier l\'age minimum du jeu (O/ N) ? ");
		if(lireModification(modification) == 'O')
		{
			lireAgeMinJeu(jeu);
		}

		/* âge maximum jeu */

		strcpy(modification, "Voulez-vous modifier l\'age maximum du jeu (O/ N) ? ");
		if(lireModification(modification) == 'O')
		{
			lireAgeMaxJeu(jeu);
		}

		if(jeu->ageMin >= jeu->ageMax)
		{
			printf("L\'age minimum doit etre inferieur a l\'age maximum du jeu.\n");
		}
	} while(jeu->ageMin >= jeu->ageMax);

	do
	{
		/* nombre de joueurs minimum jeu */

		strcpy(modification, "Voulez-vous modifier le nombre minimum de joueurs (O/ N) ? ");
		if(lireModification(modification) == 'O')
		{
			lireNbJoueursMinJeu(jeu);
		}

		/* nombre de joueurs maximum jeu */

		strcpy(modification, "Voulez-vous modifier le nombre maximum de joueurs (O/ N) ? ");
		if(lireModification(modification) == 'O')
		{
			lireNbJoueursMaxJeu(jeu);
		}

		if(jeu->nbJoueursMin > jeu->nbJoueursMax)
		{
			printf("Le nombre de joueurs minimum doit etre inferieur ");
			printf("ou egal au nombre de joueurs maximum du jeu.\n");
		}
	} while(jeu->nbJoueursMin > jeu->nbJoueursMax);

	/* date d'achat jeu (inférieure ou égal à la date du jour) */

	strcpy(modification, "Voulez-vous modifier la date d\'achat du jeu (O/ N) ? ");
	if(lireModification(modification) == 'O')
	{
		lireDateAchatJeu(jeu, temps);
	}

	/* durée garantie jeu */

	strcpy(modification, "Voulez-vous modifier la duree de la garantie du jeu (O/ N) ? ");
	if(lireModification(modification) == 'O')
	{
		lireDureeGarantieJeu(jeu);
	}
}

/************************************************************************/
/* INPUT : le jeu à afficher (jeu)										*/
/* PROCESS : permet l'affichage d'un jeu								*/
/* OUTPUT : void														*/
/************************************************************************/

void afficherJeu(Jeu jeu)
{
	printf("\nIdentifiant : %d\n", jeu.idJeu);
	printf("Nom : %s\n", jeu.nom);
	printf("Interieur : %c\n", jeu.interieur);
	printf("Exterieur : %c\n", jeu.exterieur);
	printf("Age minimum : %d\n", jeu.ageMin);
	printf("Age maximum : %d\n", jeu.ageMax);
	printf("Nombre de joueurs minimum : %d\n", jeu.nbJoueursMin);
	printf("Nombre de joueurs maximum : %d\n", jeu.nbJoueursMax);
	printf("Date d\'achat : %d/%d/%d\n", jeu.dateAchat.jour, jeu.dateAchat.mois, \
		jeu.dateAchat.annee);
	printf("Duree de la garantie : %d\n", jeu.dureeGarantie);
}

/************************************************************************/
/* INPUT : le joueur à afficher (joueur)								*/
/* PROCESS : permet l'affichage d'un joueur								*/
/* OUTPUT : void														*/
/************************************************************************/

void afficherJoueur(Joueur joueur)
{
	printf("\nIdentifiant : %d\n", joueur.idJoueur);
	printf("Nom : %s\n", joueur.nom);
	printf("Prenom : %s\n", joueur.prenom);
	printf("Date de naissance : %d/%d/%d\n", joueur.dateNaissance.jour, \
		joueur.dateNaissance.mois, joueur.dateNaissance.annee);
	printf("Adresse : %s\n", joueur.adresse);
	printf("Code postal : %s\n", joueur.codePostal);
	printf("Ville : %s\n", joueur.ville);
	printf("Pays : %s\n", joueur.pays);
	printf("Telephone : %s\n", joueur.telephone);
	printf("Adresse mail : %s\n", joueur.adMail);
}

/************************************************************************/
/* INPUT : la partie à afficher (partie)								*/
/* PROCESS : permet l'affichage d'une partie							*/
/* OUTPUT : void														*/
/************************************************************************/

void afficherPartie(Partie partie)
{
	printf("\nIdentifiant du jeu : %d\n", partie.idJeu);
	printf("Identifiant du joueur : %d\n", partie.idJoueur);
	printf("Date de la partie : %d/%d/%d\n", partie.datePartie.jour, \
		partie.datePartie.mois, partie.datePartie.annee);
	printf("Heure de la partie : %d:%d\n", partie.heurePartie.heure,\
		partie.heurePartie.minute);
	printf("Point(s) : %d\n", partie.points);
}

/************************************************************************/
/* INPUT : l'identifiant du jeu à rechercher (idJeu)					*/
/*		   le tableau index de jeu (*indexJeux)							*/
/*		   l'indice dans le file index (*indiceIndexJeu)				*/
/*		   l'indice dans le file jeux (*indiceJeu)						*/
/* PROCESS : permet la recherche d'un jeu suivant son identifiant		*/
/* OUTPUT : bool														*/
/************************************************************************/

int rechercherJeu(IndexJeu *indexJeux, int idJeu, int *indiceIndexJeu, int *indiceJeu)
{
	int retour;
	int compteur;

	for(compteur = 0, retour = 0; (compteur < 50 && (retour == 0)); compteur++, indexJeux++)
	{
		if(indexJeux->idJeu == idJeu)
		{
			*indiceIndexJeu = compteur;
			*indiceJeu = indexJeux->indiceJeu;
			retour = 1;
		}
	}

	return retour;
}

/************************************************************************/
/* INPUT : l'identifiant du joueur (idJoueur)							*/
/* PROCESS : permet la recherche d'un joueur suivant son identifiant	*/
/* OUTPUT : bool														*/
/************************************************************************/

int rechercherJoueur(int idJoueur)
{
	int retour;
	int pto;
	Joueur pTemp;
	FILE *fp;

	fp = fopen("Joueurs.tib", "rb");

	if(fp == (FILE *) NULL)
	{
		perror("Joueurs.tib");
		system("pause");
		retour = 0;
	}

	else
	{
		fread(&pto, sizeof(int), 1, fp);

		if(pto == -1) /* file vide */
		{
			retour = 0;
		}

		else /* file non vide */
		{
			retour = 0;
			fseek(fp, pto, SEEK_SET);

			do
			{
				fread(&pTemp, sizeof(Joueur), 1, fp);

				if(pTemp.idJoueur == idJoueur)
				{
					retour = 1;
				}

				fseek(fp, pTemp.jsvt, SEEK_SET);
			} while((retour == 0) && (pTemp.jsvt != -1) && (feof(fp) == 0));

			if((retour == 0) && (pTemp.jsvt == -1) && (feof(fp) == 0))
			{
				if(pTemp.idJoueur == idJoueur)
				{
					retour = 1;
				}
			}
		}

		fclose(fp);
	}

	return retour;
}

/************************************************************************/
/* INPUT : l'identifiant du jeu à rechercher (idJeu)					*/
/* PROCESS : permet la recherche d'un jeu participant à une partie		*/
/* OUTPUT : bool														*/
/************************************************************************/

int rechercherPartie(int idJeu)
{
	int retour;
	Partie pTemp;
	FILE *fp;

	fp = fopen("PartiesJouees.tib", "rb");

	if(fp == (FILE *) NULL)
	{
		perror("PartiesJouees.tib");
		system("pause");
		retour = 0;
	}

	else
	{
		retour = 0;

		while((retour == 0) && (feof(fp) == 0))
		{
			fread(&pTemp, sizeof(Partie), 1, fp);

			if(pTemp.idJeu == idJeu)
			{
				retour = 1;
			}
		}

		fclose(fp);
	}

	return retour;
}

/************************************************************************/
/* INPUT : le tableau index de jeu (*indexJeux)							*/
/* PROCESS : permet de bidonner le fichier de jeu						*/
/* OUTPUT : bool														*/
/************************************************************************/

int bidonnageJeu(IndexJeu *indexJeux)
{
	int retour;
	int compteur;
	Jeu pTemp;
	FILE *fp;

	fp = fopen("Jeux.tib", "rb");

	if(fp == (FILE *) NULL)
	{
		fp = fopen("Jeux.tib", "wb");

		if(fp == (FILE *) NULL)
		{
			perror("Jeux.tib");
			system("pause");
			retour = 0;
		}

		else
		{
			for(compteur = 0; compteur < JEUX_LEN; compteur+=sizeof(Jeu))
			{
				pTemp.idJeu = -1; /* suppression logique */
				strcpy(pTemp.nom, "");
				fwrite(&pTemp, sizeof(Jeu), 1, fp);
			}

			fclose(fp);
			bidonnageIndexJeu(indexJeux); /* bidonnage du file index */
			retour = 1;
		}
	}

	else
	{
		fclose(fp);
		bidonnageIndexJeu(indexJeux); /* bidonnage du file index */
		retour = 1;
	}

	return retour;
}

/************************************************************************/
/* INPUT : le tableau index de jeu (*indexJeux)							*/
/* PROCESS : permet de bidonner le fichier index de jeu					*/
/* OUTPUT : bool														*/
/************************************************************************/

int bidonnageIndexJeu(IndexJeu *indexJeux)
{
	int retour;
	int compteur;
	IndexJeu *indexJeu;
	Jeu jeu;
	FILE *fp;

	fp = fopen("IndexJeux.tib", "rb");

	if(fp == (FILE *) NULL)
	{
		fp = fopen("Jeux.tib", "rb");

		if(fp == (FILE *) NULL)
		{
			perror("Jeux.tib");
			system("pause");
			retour = 0;
		}

		else
		{
			indexJeu = indexJeux;

			while(feof(fp) == 0)
			{
				fread(&jeu, sizeof(Jeu), 1, fp);

				if(feof(fp) == 0)
				{
					indexJeu->idJeu = jeu.idJeu;
					strcpy(indexJeu->nom, jeu.nom);
					indexJeu->indiceJeu = ftell(fp) - sizeof(Jeu);
					indexJeu++;
				}
			}

			fclose(fp);
			trierJeu(indexJeux); /* tri de l'index */
			retour = 1;
		}
	}

	else
	{
		for(compteur = 0; compteur < 50; compteur++, indexJeux++)
		{
			fread(indexJeux, sizeof(IndexJeu), 1, fp);
		}

		fclose(fp);
		retour = 1;
	}

	return retour;
}

/************************************************************************/
/* INPUT : le tableau index de jeu (*indexJeux)							*/
/* PROCESS : permet de sauvegarder le fichier index						*/
/* OUTPUT : bool														*/
/************************************************************************/

int sauvegardeIndexJeu(IndexJeu *indexJeux)
{
	int retour;
	int compteur;
	IndexJeu pTemp;
	FILE *fp;

	fp = fopen("IndexJeux.tib", "wb");

	if(fp == (FILE *) NULL)
	{
		retour = 0;
	}

	else
	{
		for(compteur = 0; compteur < 50; compteur++, indexJeux++)
		{
			pTemp.idJeu = indexJeux->idJeu;
			strcpy(pTemp.nom, indexJeux->nom);
			pTemp.indiceJeu = indexJeux->indiceJeu;
			fwrite(&pTemp, sizeof(IndexJeu), 1, fp);
		}

		fclose(fp);
		retour = 1;
	}

	return retour;
}

/************************************************************************/
/* INPUT : void															*/
/* PROCESS : permet de bidonner le fichier de joueurs					*/
/* OUTPUT : bool														*/
/************************************************************************/

int bidonnageJoueur(void)
{
	int retour;
	int pto;
	FILE *fp;

	fp = fopen("Joueurs.tib", "rb");

	if(fp == (FILE *) NULL)
	{
		fp = fopen("Joueurs.tib", "wb");

		if(fp == (FILE *) NULL)
		{
			perror("Joueurs.tib");
			system("pause");
			retour = 0;
		}

		else
		{
			pto = -1; /* file vide */
			fwrite(&pto, sizeof(int), 1, fp);
			fclose(fp);
			retour = 1;
		}
	}

	else
	{
		fclose(fp);
		retour = 1;
	}

	return retour;
}

/************************************************************************/
/* INPUT : void															*/
/* PROCESS : permet de bidonner le fichier de parties jouées			*/
/* OUTPUT : bool														*/
/************************************************************************/

int bidonnagePartie(void)
{
	int retour;
	FILE *fp;

	fp = fopen("PartiesJouees.tib", "rb");

	if(fp == (FILE *) NULL)
	{
		fp = fopen("PartiesJouees.tib", "wb");

		if(fp == (FILE *) NULL)
		{
			perror("PartiesJouees.tib");
			system("pause");
			retour = 0;
		}

		else
		{
			fclose(fp);
			retour = 1;
		}
	}

	else
	{
		fclose(fp);
		retour = 1;
	}

	return retour;
}

/************************************************************************/
/* INPUT : le tableau index de jeu (*indexJeux)							*/
/*		   la structure contenant la date et l'heure actuelle (temps)	*/
/* PROCESS : permet l'ajout d'un jeu									*/
/* OUTPUT : bool														*/
/************************************************************************/

int ajouterJeu(IndexJeu *indexJeux, struct tm temps)
{
	int retour;
	int indiceIndexJeu;
	int indiceJeu;
	IndexJeu *indexJeu;
	Jeu jeu;
	FILE *fp;

	/* recherche place disponible */
	if(rechercherJeu(indexJeux, -1, &indiceIndexJeu, &indiceJeu) == 0)
	{
		retour = 0;
	}

	else
	{
		fp = fopen("Jeux.tib", "rb+");

		if(fp == (FILE *) NULL)
		{
			perror("Jeux.tib");
			system("pause");
			retour = 0;
		}

		else
		{
			indexJeu = indexJeux + indiceIndexJeu;
			lireJeu(indexJeux, &jeu, indexJeu, temps); /* on encode les infos */
			indexJeu->indiceJeu = indiceJeu;
			fseek(fp, indiceJeu, SEEK_SET);
			fwrite(&jeu, sizeof(Jeu), 1, fp);
			fclose(fp);
			trierJeu(indexJeux); /* tri de l'index */
			retour = 1;
		}
	}

	return retour;
}

/************************************************************************/
/* INPUT : structure contenant l'heure et la date actuelle (temps)		*/
/* PROCESS : permet l'ajout d'une joueur								*/
/* OUTPUT : bool														*/
/************************************************************************/

int ajouterJoueur(struct tm temps)
{
	int retour;
	int pto;
	int ptl;
	int compteur;
	Joueur pNew;
	Joueur pTemp;
	Joueur pPrec;
	Joueur pPrecPrec;
	FILE *fp;

	fp = fopen("Joueurs.tib", "rb+");

	if(fp == (FILE *) NULL)
	{
		perror("Joueurs.tib");
		system("pause");
		retour = 0;
	}

	else
	{
		fseek(fp, 0, SEEK_SET);
		fread(&pto, sizeof(int), 1, fp);
		fseek(fp, 0, SEEK_END);
		ptl = ftell(fp);
		lireJoueur(&pNew, temps); /* on encode les infos */

		if(pto == -1) /* file vide */
		{
			pto = ptl;
			fseek(fp, 0, SEEK_SET);
			fwrite(&pto, sizeof(int), 1, fp);
			fseek(fp, ptl, SEEK_SET);
			fwrite(&pNew, sizeof(Joueur), 1, fp);
		}

		else /* file non vide */
		{
			compteur = 0;
			fseek(fp, pto, SEEK_SET);
			fread(&pTemp, sizeof(Joueur), 1, fp);
			pPrec = pTemp;

			while((pTemp.jsvt != -1) && (strcmp(pNew.nom, pTemp.nom) >= 0) && (feof(fp) == 0))
			{
				pPrecPrec = pPrec;
				pPrec = pTemp;
				fseek(fp, pTemp.jsvt, SEEK_SET);
				fread(&pTemp, sizeof(Joueur), 1, fp);
				compteur++;
			}

			if(compteur == 0) /* record au début du file */
			{
				/* new record avant old */
				if(strcmp(pNew.nom, pTemp.nom) < 0 || (strcmp(pNew.nom, pTemp.nom) == 0 && \
					(strcmp(pNew.prenom, pTemp.prenom) < 0)))
				{
					pNew.jsvt = pto;
					fseek(fp, ptl, SEEK_SET);
					fwrite(&pNew, sizeof(Joueur), 1, fp);
					pto = ptl;
					fseek(fp, 0, SEEK_SET);
					fwrite(&pto, sizeof(int), 1, fp);
				}

				else /* new record après old */
				{
					pTemp.jsvt = ptl;
					fseek(fp, pto, SEEK_SET);
					fwrite(&pTemp, sizeof(Joueur), 1, fp);
					fseek(fp, ptl, SEEK_SET);
					fwrite(&pNew, sizeof(Joueur), 1, fp);
				}
			}

			else /* record au milieu ou à la fin du file */
			{
				if(pTemp.jsvt == -1) /* record à la fin du file */
				{
					/* new record avant old */
					if(strcmp(pNew.nom, pTemp.nom) < 0 || (strcmp(pNew.nom, pTemp.nom) == 0 && \
						strcmp(pNew.prenom, pTemp.prenom) < 0))
					{
						pNew.jsvt = pPrec.jsvt;
						fseek(fp, ptl, SEEK_SET);
						pPrec.jsvt = ptl;
						fwrite(&pNew, sizeof(Joueur), 1, fp);

						if(compteur == 1)
						{
							fseek(fp, pto, SEEK_SET);
							fwrite(&pPrec, sizeof(Joueur), 1, fp);
						}

						else
						{
							fseek(fp, pPrecPrec.jsvt, SEEK_SET);
							fwrite(&pPrec, sizeof(Joueur), 1, fp);
						}
					}

					else /* new record après old */
					{
						pTemp.jsvt = ptl;
						fseek(fp, pPrec.jsvt, SEEK_SET);
						fwrite(&pTemp, sizeof(Joueur), 1, fp);
						fseek(fp, ptl, SEEK_SET);
						fwrite(&pNew, sizeof(Joueur), 1, fp);
					}
				}

				else /* record au mileu du file */
				{
					pNew.jsvt = pPrec.jsvt;
					fseek(fp, ptl, SEEK_SET);
					fwrite(&pNew, sizeof(Joueur), 1, fp);
					pPrec.jsvt = ptl;

					if(compteur == 1)
					{
						fseek(fp, pto, SEEK_SET);
						fwrite(&pPrec, sizeof(Joueur), 1, fp);
					}

					else
					{
						fseek(fp, pPrecPrec.jsvt, SEEK_SET);
						fwrite(&pPrec, sizeof(Joueur), 1, fp);
					}
				}
			}
		}

		fclose(fp);
		retour = 1;
	}

	return retour;
}

/************************************************************************/
/* INPUT : le tableau index de jeu (*indexJeux)							*/
/* PROCESS : permet l'ajout de joueurs ayant participés à une partie	*/
/* OUTPUT : bool														*/
/************************************************************************/

int ajouterPartie(IndexJeu *indexJeux)
{
	int retour;
	int indiceIndexJeu;
	int indiceJeu;
	int compteurPartie;
	int idJoueur;
	char continuer;
	char bufDate[11];
	char bufHeure[6];
	Partie partie;
	Jeu jeu;
	FILE *fpp;
	FILE *fpj;

	fpj = fopen("Jeux.tib", "rb");

	if(fpj == (FILE *) NULL)
	{
		perror("Jeux.tib");
		system("pause");
		retour = 0;
	}

	else
	{
		continuer = 'O';
		compteurPartie = 0;

		/* identifiant jeu */

		do
		{
			printf("Veuillez encoder l\'identifiant du jeu : ");
			scanf("%d", &(partie.idJeu));

			if(partie.idJeu == -1)
			{
				printf("L\'identifiant du jeu doit etre different de -1.\n");
			}

			else
			{
				if(rechercherJeu(indexJeux, partie.idJeu, &indiceIndexJeu, &indiceJeu) == 0)
				{
					printf("L\'identifiant ne correspond a aucun jeu.\n");
				}
			}
		} while((partie.idJeu == -1) || (rechercherJeu(indexJeux, partie.idJeu, &indiceIndexJeu, &indiceJeu) == 0));

		fseek(fpj, indiceJeu, SEEK_SET);
		fread(&jeu, sizeof(Jeu), 1, fpj);
		fclose(fpj);

		/* date partie */

		do
		{
			printf("Veuillez encoder la date de la partie (DD/MM/YYYY) : ");
			lireChaine(bufDate, 11);

			if(lireDate(bufDate, &(partie.datePartie)) == 0)
			{
				printf("La date saisie est invalide.\n");
			}
		} while(lireDate(bufDate, &(partie.datePartie)) == 0);

		/* heure partie */

		do
		{
			printf("Veuillez encoder l\'heure de la partie (HH:MM) : ");
			lireChaine(bufHeure, 6);
			if(lireHeure(bufHeure, &(partie.heurePartie)) == 0)
			{
				printf("L'heure saisie est invalide.\n");
			}
		} while(lireHeure(bufHeure, &(partie.heurePartie)) == 0);

		do
		{
			/* identifiant joueur*/

			do
			{
				printf("Veuillez encoder l\'identifiant du joueur : ");
				scanf("%d", &idJoueur);

				if(rechercherJoueur(idJoueur) == 0)
				{
					printf("L\'identifiant ne correspond a aucun joueur.\n");
				}

				else
				{
					if(verifierPartie(idJoueur, partie.datePartie, partie.heurePartie) == 1)
					{
						printf("Le joueur est occupe dans une autre partie a cette date.\n");
					}
				}
			} while((rechercherJoueur(idJoueur) == 0) || \
				(verifierPartie(idJoueur, partie.datePartie, partie.heurePartie) == 1));

			partie.idJoueur = idJoueur;

			/* points partie */

			do
			{
				printf("Veuillez encoder le nombre de point(s) : ");
				scanf("%d", &(partie.points));
			} while(partie.points < 0);

			fpp = fopen("PartiesJouees.tib", "ab");
			fwrite(&partie, sizeof(Partie), 1, fpp);
			fclose(fpp);
			compteurPartie++;

			do
			{
				printf("Voulez-vous continuer l\'encodage de joueurs (O/ N) ? ");
				fflush(stdin);
				continuer = toupper(getchar());

				if(continuer == 'N')
				{
					if(compteurPartie < jeu.nbJoueursMin)
					{
						printf("Le jeu doit comporter au moins %d joueur(s).\n", \
							jeu.nbJoueursMin);
					}
				}

				else
				{
					if(compteurPartie == jeu.nbJoueursMax)
					{
						continuer = 'N';
					}
				}
			} while((continuer != 'O') && (continuer != 'N'));
		} while((compteurPartie < jeu.nbJoueursMin) || \
			(compteurPartie > jeu.nbJoueursMax) || (continuer == 'O'));

		retour = 1;
	}

	return retour;
}

/************************************************************************/
/* INPUT : le tableau index de jeu (*indexJeux)							*/
/*		   l'identifiant du jeu à modifier (idJeu)						*/
/*		   structure contenant l'heure et la date actuelle (temps)		*/
/* PROCESS : permet la modification d'un jeu							*/
/* OUTPUT : bool														*/
/************************************************************************/

int modifierJeu(IndexJeu *indexJeux, int idJeu, struct tm temps)
{
	int retour;
	int indiceIndexJeu;
	int indiceJeu;
	IndexJeu *indexJeu;
	Jeu jeu;
	FILE *fp;

	/* le jeu n'a pas été trouvé */
	if(rechercherJeu(indexJeux, idJeu, &indiceIndexJeu, &indiceJeu) == 0)
	{
		retour = 0;
	}

	else
	{
		fp = fopen("Jeux.tib", "rb+");

		if(fp == (FILE *) NULL)
		{
			perror("Jeux.tib");
			system("pause");
			retour = 0;
		}

		else
		{
			/* lecture des anciennes données */
			indexJeu = indexJeux + indiceIndexJeu;
			fseek(fp, indiceJeu, SEEK_SET);
			fread(&jeu, sizeof(Jeu), 1, fp);
			/* encodage des nouvelles données */
			printf("Veuillez encoder les nouvelles informations : \n");
			relireJeu(indexJeux, &jeu, indexJeu, temps); /* on encode les infos */
			indexJeu->indiceJeu = indiceJeu;
			fseek(fp, indiceJeu, SEEK_SET);
			fwrite(&jeu, sizeof(Jeu), 1, fp);
			fclose(fp);
			trierJeu(indexJeux); /* tri de l'index */
			retour = 1;
		}
	}

	return retour;
}

/************************************************************************/
/* INPUT : le tableau index de jeu (*indexJeux)							*/
/*		   l'identifiant du jeu à supprimer (idJeu)						*/
/* PROCESS : permet la suppression d'un jeu								*/
/* OUTPUT : bool														*/
/************************************************************************/

int supprimerJeu(IndexJeu *indexJeux, int idJeu)
{
	int retour;
	int indiceIndexJeu;
	int indiceJeu;
	Jeu jeu;
	FILE *fp;

	/* le jeu n'a pas été trouvé */
	if(rechercherJeu(indexJeux, idJeu, &indiceIndexJeu, &indiceJeu) == 0)
	{
		retour = 0;
	}

	else
	{
		fp = fopen("Jeux.tib", "rb+");

		if(fp == (FILE *) NULL)
		{
			perror("Jeux.tib");
			system("pause");
			retour = 0;
		}

		else
		{
			/* si le jeu ne participe à aucune partie (intégrité référentielle) */
			if(rechercherPartie(idJeu) == 0)
			{
				(indexJeux+indiceIndexJeu)->idJeu = -1; /* suppression logique */
				jeu.idJeu = -1; /* suppression logique */
				fseek(fp, indiceJeu, SEEK_SET);
				fwrite(&jeu, sizeof(Jeu), 1, fp);
				retour = 1;
			}

			else
			{
				printf("Impossible de supprimer un jeu qui participe a une partie.\n");
				system("pause");
				retour = 0;
			}

			fclose(fp);
		}
	}

	return retour;
}

/************************************************************************/
/* INPUT : le tableau index de jeu (*indexJeux)							*/
/* PROCESS : permet d'afficher tout les jeux							*/
/* OUTPUT : bool														*/
/************************************************************************/

int afficherToutJeu(IndexJeu *indexJeux)
{
	int retour;
	int compteur;
	Jeu jeu;
	FILE *fp;

	fp = fopen("Jeux.tib", "rb");

	if(fp == (FILE *) NULL)
	{
		perror("Jeux.tib");
		system("pause");
		retour = 0;
	}

	else
	{
		for(compteur = 0; compteur < 50; compteur++, indexJeux++)
		{
			if(indexJeux->idJeu != -1) /* suppression logique */
			{
				fseek(fp, indexJeux->indiceJeu, SEEK_SET);
				fread(&jeu, sizeof(Jeu), 1, fp);
				afficherJeu(jeu);
			}
		}

		fclose(fp);
		retour = 1;
	}

	return retour;
}

/************************************************************************/
/* INPUT : void															*/
/* PROCESS : permet d'afficher tout les joueurs							*/
/* OUTPUT : bool														*/
/************************************************************************/

int afficherToutJoueur(void)
{
	int retour;
	int pto;
	Joueur pTemp;
	FILE *fp;

	fp = fopen("Joueurs.tib", "rb");

	if(fp == (FILE *) NULL)
	{
		perror("Joueurs.tib");
		system("pause");
		retour = 0;
	}

	else
	{
		fread(&pto, sizeof(int), 1, fp);

		if(pto == -1) /* file vide */
		{
			retour = 0;
		}

		else /* file non vide */
		{
			fseek(fp, pto, SEEK_SET);
			fread(&pTemp, sizeof(Joueur), 1, fp);

			while((pTemp.jsvt != -1) && (feof(fp) == 0))
			{
				afficherJoueur(pTemp);
				fseek(fp, pTemp.jsvt, SEEK_SET);
				fread(&pTemp, sizeof(Joueur), 1, fp);
			}

			if((pTemp.jsvt == -1) && (feof(fp) == 0))
			{
				afficherJoueur(pTemp);
			}

			fclose(fp);
			retour = 1;
		}
	}

	return retour;
}

/************************************************************************/
/* INPUT : structure contenant l'heure et la date actuelle (temps)		*/
/* PROCESS : permet d'afficher les joueurs ayant plus de 18 ans			*/
/* OUTPUT : bool														*/
/************************************************************************/

int afficherJoueurDixHuit(struct tm temps)
{
	int retour;
	int pto;
	Joueur pTemp;
	FILE *fp;

	fp = fopen("Joueurs.tib", "rb");

	if(fp == (FILE *) NULL)
	{
		perror("Joueurs.tib");
		system("pause");
		retour = 0;
	}

	else
	{
		fread(&pto, sizeof(int), 1, fp);

		if(pto == -1) /* file vide */
		{
			retour = 0;
		}

		else /* file non vide */
		{
			fseek(fp, pto, SEEK_SET);
			fread(&pTemp, sizeof(Joueur), 1, fp);

			while((pTemp.jsvt != -1) && (feof(fp) == 0))
			{
				if(verifierAgeDixHuit(temps, pTemp.dateNaissance) == 1)
				{
					afficherJoueur(pTemp);
				}

				fseek(fp, pTemp.jsvt, SEEK_SET);
				fread(&pTemp, sizeof(Joueur), 1, fp);
			}

			if((pTemp.jsvt == -1) && (feof(fp) == 0))
			{
				if(verifierAgeDixHuit(temps, pTemp.dateNaissance) == 1)
				{
					afficherJoueur(pTemp);
				}
			}

			fclose(fp);
			retour = 1;
		}
	}

	return retour;
}

/************************************************************************/
/* INPUT : void															*/
/* PROCESS : permet l'affichage des parties jouées						*/
/* OUTPUT : bool														*/
/************************************************************************/

int afficherToutePartie(void)
{
	int retour;
	Partie pTemp;
	FILE *fp;

	fp = fopen("PartiesJouees.tib", "rb");

	if(fp == (FILE *) NULL)
	{
		perror("PartiesJouees.tib");
		retour = 0;
	}

	else
	{
		while(feof(fp) == 0)
		{
			fread(&pTemp, sizeof(Partie), 1, fp);

			if(feof(fp) == 0)
			{
				afficherPartie(pTemp);
			}
		}

		fclose(fp);
		retour = 1;
	}

	return retour;
}