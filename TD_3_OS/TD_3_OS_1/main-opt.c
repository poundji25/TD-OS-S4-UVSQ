#include <stdio.h>
#include <fcntl.h>
#include "se_fichier.h"

void testChaine () {
	SE_FICHIER fic;
	char buffer [32];

	printf ("String test:\nbase: Hello world!\nfile: ");

	fic = SE_ouverture ("test.txt", O_CREAT | O_WRONLY);

	SE_ecritureChaine (fic, "Hello world!\n", 13);

	SE_fermeture (fic);

	fic = SE_ouverture ("test.txt", O_RDONLY);

	SE_lectureChaine (fic, buffer, 31);

	SE_fermeture (fic);

	SE_suppression ("test.txt");

	printf ("%s", buffer);
}

void testEntier () {
	SE_FICHIER fic;
	int i;

	printf ("Integer test:\nbase: %d %d %d %d\nfile: ", 42, -17, 0, 42359428);

	fic = SE_ouverture ("test.txt", O_CREAT | O_WRONLY);

	SE_ecritureEntier (fic, 42);
	SE_ecritureCaractere (fic, ' ');
	SE_ecritureEntier (fic, -17);
	SE_ecritureCaractere (fic, ' ');
	SE_ecritureEntier (fic, 0);
	SE_ecritureCaractere (fic, ' ');
	SE_ecritureEntier (fic, 42359428);
	SE_ecritureCaractere (fic, ' ');

	SE_fermeture (fic);

	fic = SE_ouverture ("test.txt", O_RDONLY);

	SE_lectureEntier (fic, &i);
	printf ("%d ", i);
	SE_lectureEntier (fic, &i);
	printf ("%d ", i);
	SE_lectureEntier (fic, &i);
	printf ("%d ", i);
	SE_lectureEntier (fic, &i);
	printf ("%d\n", i);

	SE_fermeture (fic);

	SE_suppression ("test.txt");
}

int main (int argc, char ** argv) {
	testChaine (); printf ("\n");
	testEntier ();

	return 0;
}
