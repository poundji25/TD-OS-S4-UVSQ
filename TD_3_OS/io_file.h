#ifndef H_IO_FILE
#define H_IO_FILE

// La structure IO_FILE contient les champs suivants :
// - le descripteur de fichier (entier)
// - le nom/chemin du fichier (chaîne de caractères)
// - les accès demandés à l'ouverture du fichier (entier)
typedef struct {
	int desc;
	char *path;
	int access;
} IO_FILE;

// Ouverture du fichier
// \param	path			Nom/chemin du fichier à ouvrir
// \param	access			Accès au fichier (cf. flags de open() )
// \return					Structure IO_FILE (le descripteur est mis à -1
//							si l'ouverture a échouée)
IO_FILE IO_open(
	const char *path,
	int access);

// Fermeture du fichier
// \param	file			Structure IO_FILE à fermer
// \return					-1 si échec, 0 sinon
int IO_close(
	IO_FILE file);

// Suppresion de fichier
// \param	path			Nom/chemin du fichier à supprimer
// \return					-1 si échec, 0 sinon
int IO_remove(
	const char *path);

// Lecture d'un caractère à partir d'un fichier
// \param	file			Structure IO_FILE où lire
// \param	c				Pointeur du caractère lu
// \return					-1 si échec, 0 si fin du fichier,
//							nombres d'octets lus sinon
int IO_char_read(
	IO_FILE file,
	char *c);

// Ecriture d'un caractère dans un fichier
// \param	file			Structure IO_FILE où écrire
// \param	c				Caractère à écrire
// \return					-1 si échec, nombre d'octets écrits sinon
int IO_char_write(
	IO_FILE file,
	const char c);


// Lecture d'une chaîne de caractère dans un fichier
// \param	file			Structure IO_FILE où lire
// \param	string			Chaîne lue
// \param	maxSize			Taille max de la chaîne lue
// \return					-1 si échec, 0 si fin du fichier,
//							nombre d'octets lus sinon
int IO_string_read(
	IO_FILE file,
	char *string,
	int maxSize);

// Ecriture d'une chaîne de caractère dans un fichier
// \param	file			Structure IO_FILE où écrire
// \param	string			Chaîne de caractère à écrire
// \param	size			Taille de la chaîne à écrire
// \return					-1 si échec, nombre d'octets écrits sinon
int IO_string_write(
	IO_FILE file,
	const char *string,
	int size);

// Lecture d'un entier dans un fichier
// \param	file			Structure IO_FILE où lire
// \param	n				Entier lu
// \return					-1 si échec, 0 si fin du fichier,
//							nombre d'octets lus sinon
int IO_int_read(
	IO_FILE file,
	int *n);

// Ecriture d'un entier dans un fichier
// \param	file			Structure IO_FILE où écrire
// \param	n				Entier à écrire
// \return					-1 si échec, nombre d'octets écrits sinon
int IO_int_write(
	IO_FILE file,
	const int n);


#endif
