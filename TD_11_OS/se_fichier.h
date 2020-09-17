#ifndef H_SE_FICHIER
#define H_SE_FICHIER

// La structure FICHIER contient les champs suivants :
// - le descripteur de fichier (entier)
// - le nom/chemin du fichier (chaîne de caractères)
// - les accès demandés à l'ouverture du fichier (entier)
typedef struct {
	int descripteur;
	char * chemin;
	int acces;
} SE_FICHIER;

// Ouverture du fichier
// \param	chemin			Nom/chemin du fichier à ouvrir
// \param	acces			Accès au fichier (cf. flags de open() )
// \return					Structure FICHIER (le descripteur est mis à -1
//							si l'ouverture a échouée)
SE_FICHIER SE_ouverture (
	const char * chemin,
	int acces);

// Fermeture du fichier
// \param	fichier			Structure FICHIER à fermer
// \return					-1 si échec, 0 sinon
int SE_fermeture (
	SE_FICHIER fichier);

// Suppresion de fichier
// \param	chemin			Nom/chemin du fichier à supprimer
// \return					-1 si échec, 0 sinon
int SE_suppression (
	const char * chemin);

// Lecture d'un caractère à partir d'un fichier
// \param	fichier			Structure FICHIER où lire
// \param	caractere		Pointeur du caractère lu
// \return					-1 si échec, 0 si fin du fichier,
//							nombres d'octets lus sinon
int SE_lectureCaractere (
	SE_FICHIER fichier,
	char * caractere);

// Ecriture d'un caractère dans un fichier
// \param	fichier			Structure FICHIER où écrire
// \param	caractere		Caractère à écrire
// \return					-1 si échec, nombre d'octets écrits sinon
int SE_ecritureCaractere (
	SE_FICHIER fichier,
	const char caractere);

// Lecture d'une chaîne de caractère dans un fichier
// \param	fichier			Structure FICHIER où lire
// \param	chaine			Chaîne lue
// \param	taille			Taille max de la chaîne lue
// \return					-1 si échec, 0 si fin du fichier,
//							nombre d'octets lus sinon
int SE_lectureChaine (
	SE_FICHIER fichier,
	char * chaine,
	int tailleMax);

// Ecriture d'une chaîne de caractère dans un fichier
// \param	fichier			Structure FICHIER où écrire
// \param	chaine			Chaîne de caractère à écrire
// \param	taille			Taille de la chaîne à écrire
// \return					-1 si échec, nombre d'octets écrits sinon
int SE_ecritureChaine (
	SE_FICHIER fichier,
	const char * chaine,
	int taille);

// Lecture d'un entier dans un fichier
// \param	fichier			Structure FICHIER où lire
// \param	entier			Entier lu
// \return					-1 si échec, 0 si fin du fichier,
//							nombre d'octets lus sinon
int SE_lectureEntier (
	SE_FICHIER fichier,
	int * entier);

// Ecriture d'un entier dans un fichier
// \param	fichier			Structure FICHIER où écrire
// \param	entier			Entier à écrire
// \return					-1 si échec, nombre d'octets écrits sinon
int SE_ecritureEntier (
	SE_FICHIER fichier,
	const int entier);

#endif
