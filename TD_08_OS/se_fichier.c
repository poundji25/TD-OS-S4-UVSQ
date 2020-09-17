#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

#include "se_fichier.h"

SE_FICHIER SE_ouverture (const char * chemin, int acces) {
	SE_FICHIER fichier;

	// O_TMPFILE est un ajout 'récent' dans le kernel, et peut ne pas être
	// supporté sur toutes les machines. Si il n'existe pas, l'enlever de
	// la condition
	if (acces & O_CREAT)
	// if (acces & O_CREAT || acces & O_TMPFILE)
		fichier.descripteur = open (chemin, acces, 0644);
	else
		fichier.descripteur = open (chemin, acces);

	if (fichier.descripteur == -1) {
		printf ("Erreur à l'ouverture de %s : %s\n", chemin, strerror (errno) );
		return fichier;
	}

	fichier.chemin = malloc ((strlen (chemin) + 1) * sizeof (char));

	strcpy (fichier.chemin, chemin);
	fichier.acces = acces;

	return fichier;
}

int SE_fermeture (SE_FICHIER fichier) {
	if (close (fichier.descripteur) == -1) {
		printf ("Erreur à la fermeture de %s : %s\n", fichier.chemin,
			strerror (errno) );
		return -1;
	}

	free (fichier.chemin);

	return 0;
}

int SE_suppression (const char * chemin) {
	if (unlink (chemin) == -1) {
		printf ("Erreur à la suppression de %s : %s\n", chemin,
			strerror (errno) );
		return -1;
	}

	return 0;
}

int SE_lectureCaractere (SE_FICHIER fichier, char * caractere) {
	int retour;

	// O_RDONLY = 0, dans le cas où il est utilisé seul, la vérif ne peut se
	// faire que si acces = 0
	if (! (fichier.acces == O_RDONLY
		|| fichier.acces & O_RDONLY || fichier.acces & O_RDWR) ) {
		printf ("Erreur à la lecture de %s : le fichier n'a pas été ouvert "
			"avec un accès en lecture\n", fichier.chemin);
		return -1;
	}

	retour = read (fichier.descripteur, caractere, 1);

	if (retour == -1)
		printf ("Erreur à la lecture de %s : %s\n", fichier.chemin,
			strerror (errno) );

	return retour;
}

int SE_ecritureCaractere (SE_FICHIER fichier, const char caractere) {
	int retour;

	if (! (fichier.acces & O_WRONLY || fichier.acces & O_RDWR) ) {
		printf ("Erreur à l'écriture de %s : le fichier n'a pas été ouvert "
			"avec un accès en écriture\n", fichier.chemin);
		return -1;
	}

	retour = write (fichier.descripteur, &caractere, 1);

	if (retour == -1)
		printf ("Erreur à l'écriture de %s : %s\n", fichier.chemin,
			strerror (errno) );

	return retour;
}

int SE_lectureChaine (
	SE_FICHIER fichier,
	char * chaine,
	int tailleMax) {
	int i = 0;
	int reti, retour;

	if (! (fichier.acces == O_RDONLY
		|| fichier.acces & O_RDONLY || fichier.acces & O_RDWR) ) {
		printf ("Erreur à la lecture de %s : le fichier n'a pas été ouvert "
			"avec un accès en lecture\n", fichier.chemin);
		return -1;
	}

	do {
		reti = read (fichier.descripteur, chaine + i, 1); retour += reti;
		++i;
	} while (reti > 0 && i < tailleMax);

	if (reti < 0) {
		printf ("Erreur à la lecture de %s : %s\n", fichier.chemin,
				strerror (errno) );
		return reti;
	}

	chaine [i] = '\0';

	return retour;
}

int SE_ecritureChaine (
	SE_FICHIER fichier,
	const char * chaine,
	int taille) {
	int i = 0;
	int reti, retour;

	if (! (fichier.acces & O_WRONLY || fichier.acces & O_RDWR) ) {
		printf ("Erreur à l'écriture de %s : le fichier n'a pas été ouvert "
			"avec un accès en écriture\n", fichier.chemin);
		return -1;
	}

	do {
		reti = write (fichier.descripteur, chaine + i, 1); retour += reti;
		++i;
	} while (reti > 0 && i < taille);

	if (reti < 0) {
		printf ("Erreur à l'écriture de %s : %s\n", fichier.chemin,
				strerror (errno) );
		return reti;
	}

	return retour;
}

int SE_lectureEntier (
	SE_FICHIER fichier,
	int * entier) {
	char c;
	int reti, retour = 0;
	int negatif = 0;

	if (! (fichier.acces == O_RDONLY
		|| fichier.acces & O_RDONLY || fichier.acces & O_RDWR) ) {
		printf ("Erreur à la lecture de %s : le fichier n'a pas été ouvert "
			"avec un accès en lecture\n", fichier.chemin);
		return -1;
	}

	*entier = 0;

	// Boucle sur les 'white-spaces', comportement de scanf sur %d
	do {
		reti = read (fichier.descripteur, &c, 1); retour += reti;
		if (reti == -1) {
			printf ("Erreur à la lecture de %s : %s\n", fichier.chemin,
				strerror (errno) );
			return reti;
		}
	} while (isspace (c) );
	
	// Entier négatif ?
	if (c == '-') {
		negatif = 1;
		reti = read (fichier.descripteur, &c, 1); retour += reti;
		if (reti == -1) {
			printf ("Erreur à la lecture de %s : %s\n", fichier.chemin,
				strerror (errno) );
			return reti;
		}
	}

	// Valeur non valide
	if (c < '0' || c > '9')
		return -1;

	// Récupération de l'entier
	do {
		(*entier) *= 10;
		(*entier) += c - '0';
		reti = read (fichier.descripteur, &c, 1); retour += reti;
		if (reti == -1) {
			printf ("Erreur à la lecture de %s : %s\n", fichier.chemin,
				strerror (errno) );
			return reti;
		}
	} while (c >= '0' && c <= '9');

	if (negatif)
		(*entier) *= -1;
	
	return retour;
}

int SE_ecritureEntier (
	SE_FICHIER fichier,
	const int entier) {
	char * buffer;
	int tmp = entier, i = 0;
	int reti, retour = 0;

	if (! (fichier.acces & O_WRONLY || fichier.acces & O_RDWR) ) {
		printf ("Erreur à l'écriture de %s : le fichier n'a pas été ouvert "
			"avec un accès en écriture\n", fichier.chemin);
		return -1;
	}

	// 2^32 < 10^12
	buffer = malloc (12 * sizeof (char) );

	if (tmp < 0) tmp = - tmp;

	do {
		buffer [i] = '0' + tmp % 10;
		tmp /= 10;
		++i;
	} while (tmp != 0);

	if (entier < 0) {
		reti = write (fichier.descripteur, "-", 1); retour += reti;
		if (reti < 0) {
			printf ("Erreur à l'écriture de %s : %s\n", fichier.chemin,
				strerror (errno) );
			return reti;
		}
	}

	while (i > 0) {
		--i;
		reti = write (fichier.descripteur, buffer + i, 1); retour += reti;
		if (reti < 0) {
			printf ("Erreur à l'écriture de %s : %s\n", fichier.chemin,
				strerror (errno) );
			return reti;
		}
	}

	return retour;
}
