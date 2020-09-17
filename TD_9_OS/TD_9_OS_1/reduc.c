#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Enumération des différents opérateurs possibles
typedef enum {
	OCD_SOMME,
	OCD_MOYENNE,
	OCD_MAX,
	OCD_MIN,
	OCD_TOTAL
} opcode_t;

// Structure contenant les arguments du programme
typedef struct {
	int nbThreads;			//< Nombre de threads à utiliser
	int tailleTableau;		//< Taille du tableau utilisé pour la réduction
	opcode_t code;			//< Opération à réaliser
} arg_t;

// Structure contenant les informations nécessaires au thread
typedef struct {
	int * tableau;			//< Tableau d'entiers à traiter
	int debut;				//< Indice de début de traitement
	int fin;				//< Indice de fin de traitement (non compris)
	int resultat;			//< Résultat local
} message_t;

// Alias de pointeurs de fonction
typedef void * (* ptrFonction) (void *);
typedef int (* ptrReduc) (message_t *, int);
typedef int (* ptrVerif) (int *, int, int);

// Fonction thread -- calcule la somme locale d'un tableau
// \param	arg 			Message transmis par le thread père
// \return					NULL
void * sommeTableau (void * arg) {
	message_t * msg = (message_t *) arg;
	int i;

	msg->resultat = 0;

	for (i = msg->debut; i < msg->fin; ++i)
		msg->resultat += msg->tableau [i];

	return NULL;
}

// Fin de la réduction -- calcule la somme globale
// \param	msg				Messages issus de l'exécution des threads,
//							contiennent les résultats locaux
// \param	nbThreads		Nombre de threads, et donc de messages
// \return					Résultat global
int reducSomme (message_t * msg, int nbThreads) {
	int i, res = 0;

	for (i = 0; i < nbThreads; ++i)
		res += msg [i] .resultat;

	return res;
}

// Fonction thread -- calcule la moyenne locale d'un tableau
// \param	arg 			Message transmis par le thread père
// \return					NULL
void * moyenneTableau (void * arg) {
	message_t * msg = (message_t *) arg;
	int i;

	msg->resultat = 0;

	for (i = msg->debut; i < msg->fin; ++i)
		msg->resultat += msg->tableau [i];

	return NULL;
}

// Fin de la réduction -- calcule la moyenne globale
// \param	msg				Messages issus de l'exécution des threads,
//							contiennent les résultats locaux
// \param	nbThreads		Nombre de threads, et donc de messages
// \return					Résultat global
int reducMoyenne (message_t * msg, int nbThreads) {
	int i, tailleTableau = 0, res = 0;

	for (i = 0; i < nbThreads; ++i) {
		res += msg [i] .resultat;
		tailleTableau += msg [i] .fin - msg [i] .debut;
	}

	if (! tailleTableau) return res;
	return res / tailleTableau;
}

// Fonction thread -- calcule le maximum local d'un tableau
// \param	arg 			Message transmis par le thread père
// \return					NULL
void * maxTableau (void * arg) {
	message_t * msg = (message_t *) arg;
	int i;

	msg->resultat = 0;

	for (i = msg->debut; i < msg->fin; ++i)
		msg->resultat = (msg->resultat < msg->tableau [i]
			? msg->tableau [i]
			: msg->resultat);

	return NULL;
}

// Fin de la réduction -- calcule le maximum global
// \param	msg				Messages issus de l'exécution des threads,
//							contiennent les résultats locaux
// \param	nbThreads		Nombre de threads, et donc de messages
// \return					Résultat global
int reducMax (message_t * msg, int nbThreads) {
	int i, res = 0;

	for (i = 0; i < nbThreads; ++i)
		res = (res < msg [i] .resultat ? msg [i] .resultat : res);

	return res;
}

// Fonction thread -- calcule le minimum local d'un tableau
// \param	arg 			Message transmis par le thread père
// \return					NULL
void * minTableau (void * arg) {
	message_t * msg = (message_t *) arg;
	int i;

	msg->resultat = 101;

	for (i = msg->debut; i < msg->fin; ++i)
		msg->resultat = (msg->resultat > msg->tableau [i]
			? msg->tableau [i]
			: msg->resultat);

	return NULL;
}

// Fin de la réduction -- calcule le minimum global
// \param	msg				Messages issus de l'exécution des threads,
//							contiennent les résultats locaux
// \param	nbThreads		Nombre de threads, et donc de messages
// \return					Résultat global
int reducMin (message_t * msg, int nbThreads) {
	int i, res = 101;

	for (i = 0; i < nbThreads; ++i)
		res = (res > msg [i] .resultat ? msg [i] .resultat : res);

	return res;	
}

// Fonction de vérification -- somme des éléments du tableau
// \param	tableau			Tableau d'entiers
// \param	tailleTableau	Taille du tableau d'entiers
// \param	resultat		Résultat obtenu par la réduction
// \return					VRAI si le résultat est juste, FAUX sinon
int verifSomme (int * tableau, int tailleTableau, int resultat) {
	int i, tmp = 0;

	for (i = 0; i < tailleTableau; ++i)
		tmp += tableau [i];

	return resultat == tmp;
}

// Fonction de vérification -- moyenne des éléments du tableau
// \param	tableau			Tableau d'entiers
// \param	tailleTableau	Taille du tableau d'entiers
// \param	resultat		Résultat obtenu par la réduction
// \return					VRAI si le résultat est juste, FAUX sinon
int verifMoyenne (int * tableau, int tailleTableau, int resultat) {
	int i, tmp = 0;

	if (tailleTableau == 0)
		return resultat == 0;

	for (i = 0; i < tailleTableau; ++i)
		tmp += tableau [i];

	return resultat == (tmp / tailleTableau);
}

// Fonction de vérification -- maximum du tableau
// \param	tableau			Tableau d'entiers
// \param	tailleTableau	Taille du tableau d'entiers
// \param	resultat		Résultat obtenu par la réduction
// \return					VRAI si le résultat est juste, FAUX sinon
int verifMax (int * tableau, int tailleTableau, int resultat) {
	int i, tmp = 0;

	for (i = 0; i < tailleTableau; ++i)
		tmp = (tmp < tableau [i] ? tableau [i] : tmp);

	return resultat == tmp;
}

// Fonction de vérification -- minimum du tableau
// \param	tableau			Tableau d'entiers
// \param	tailleTableau	Taille du tableau d'entiers
// \param	resultat		Résultat obtenu par la réduction
// \return					VRAI si le résultat est juste, FAUX sinon
int verifMin (int * tableau, int tailleTableau, int resultat) {
	int i, tmp = 101;

	for (i = 0; i < tailleTableau; ++i)
		tmp = (tmp > tableau [i] ? tableau [i] : tmp);

	return resultat == tmp;
}

// Analyse des arguments du programme. Quitte si le nombre d'arguments est
// différent de 4. Remplit la structure de données des arguments.
// \param	argc			Nombre d'arguments
// \param	argv			Arguments
// \return					Structure de données des arguments
arg_t analyseArguments (int argc, char ** argv) {
	arg_t a;

	if (argc != 4) {
		printf ("Usage : %s m n [+,/,M,m]\n", argv [0]);
		exit (-1);
	}

	a.nbThreads = atoi (argv [1]);
	a.tailleTableau = atoi (argv [2]);

	switch (argv [3][0]) {
		case '+': a.code = OCD_SOMME; break;
		case '/': a.code = OCD_MOYENNE; break;
		case 'M': a.code = OCD_MAX; break;
		case 'm': a.code = OCD_MIN; break;
		default: a.code = OCD_TOTAL;
	}

	return a;
}

// Récupération de la fonction de thread à partir de l'opcode
// \param	opcode 			Code de l'opération
// \return					Pointeur de la fonction de vérification
ptrFonction decodeOpcode (const opcode_t o) {
	switch (o) {
		case OCD_SOMME:		return sommeTableau;
		case OCD_MOYENNE:	return moyenneTableau;
		case OCD_MAX:		return maxTableau;
		case OCD_MIN:		return minTableau;
		default:
			printf ("L'opération n'a pas été reconnue\n");
			exit (-1);
	}
}

// Récupération de la fonction de réduction à partir de l'opcode
// \param	opcode 			Code de l'opération
// \return					Pointeur de la fonction de vérification
ptrReduc decodeOpcodeReduc (const opcode_t o) {
	switch (o) {
		case OCD_SOMME:		return reducSomme;
		case OCD_MOYENNE:	return reducMoyenne;
		case OCD_MAX:		return reducMax;
		case OCD_MIN:		return reducMin;
		default:
			printf ("L'opération n'a pas été reconnue\n");
			exit (-1);
	}
}

// Récupération de la fonction de vérification à partir de l'opcode
// \param	opcode 			Code de l'opération
// \return					Pointeur de la fonction de vérification
ptrVerif decodeOpcodeVerif (const opcode_t o) {
	switch (o) {
		case OCD_SOMME:		return verifSomme;
		case OCD_MOYENNE:	return verifMoyenne;
		case OCD_MAX:		return verifMax;
		case OCD_MIN:		return verifMin;
		default:
			printf ("L'opération n'a pas été reconnue\n");
			exit (-1);
	}
}

// Génération du tableau avec des entiers compris entre 1 et 100.
// \param	tailleTableau	Taille du tableau d'entiers
// \return					Tableau d'entiers
int * genereTableau (int tailleTableau) {
	int i, * tab = malloc (tailleTableau * sizeof (int) );

	for (i = 0; i < tailleTableau; ++i)
		tab [i] = 1 + (rand () % 100);

	return tab;
}

// Fonction chargée de la réduction multi-threadé, elle va initialiser les
// différentes variables utilisées par les threads (tableau d'entier, messages,
// etc.) puis créer les threads. Une fois l'exécution des threads terminée et
// la réduction opérée, la vérification du résultat est faite.
// \param	arg 			Arguments du programme décodés
void programmePrincipal (const arg_t arg) {
	// Déclaration des variables
	pthread_t * tid;
	message_t * msg;
	ptrFonction foncThread = decodeOpcode (arg.code);
	ptrReduc foncReduc = decodeOpcodeReduc (arg.code);
	int i, * tab, res;

	// Allocation de la mémoire
	tid = malloc (arg.nbThreads * sizeof (pthread_t) );
	msg = malloc (arg.nbThreads * sizeof (message_t) );

	// Initialisation des variables et création des threads
	tab = genereTableau (arg.tailleTableau);
	for (i = 0; i < arg.nbThreads; ++i) {
		msg [i] .tableau = tab;
		msg [i] .debut = i * arg.tailleTableau / arg.nbThreads;
		msg [i] .fin = (i + 1) * arg.tailleTableau / arg.nbThreads;
		pthread_create (tid + i, NULL, foncThread, msg + i);
	}

	// Jointure
	for (i = 0; i < arg.nbThreads; ++i) {
		pthread_join (tid [i], NULL);
	}

	// Réduction et affichage du résultat
	res = (* foncReduc) (msg, arg.nbThreads);
	printf ("Le resultat est : %d\n", res);

	// NE PAS TOUCHER
	if ( (* (decodeOpcodeVerif (arg.code) ) ) (tab, arg.tailleTableau, res) )
		printf ("Le resultat est juste.\n");
	else printf ("Le resultat est faux.\n");
	// FIN

	// Libération de la mémoire
	free (tid); free (msg); free (tab);
}

// Fonction main (NE PAS TOUCHER) -- analyse les arguments donnés au
// programme, puis exécute le programme principal, chargé de la réduction
// multi-threadée.
// \param	argc			Nombre d'arguments
// \param	argv			Arguments
// \return					Code de retour du programme (0 en cas de réussite)
int main (int argc, char ** argv) {
	arg_t a;

	srand (time (NULL) );

	a = analyseArguments (argc, argv);
	programmePrincipal (a);
	
	return 0;
}
