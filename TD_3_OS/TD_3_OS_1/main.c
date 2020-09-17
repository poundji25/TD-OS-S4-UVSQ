#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "se_fichier.h"

int affichage (const char * chemin) {
	SE_FICHIER fic;
	char c;

	fic = SE_ouverture (chemin, O_RDONLY);

	if (fic.descripteur == -1)
		return -1;

	while (SE_lectureCaractere (fic, &c) > 0)
		printf ("%c", c);

	SE_fermeture (fic);

	return 0;
}

int copie (const char * chemin1, const char * chemin2) {
	SE_FICHIER fic1, fic2;
	char c;

	fic1 = SE_ouverture (chemin1, O_RDONLY);
	fic2 = SE_ouverture (chemin2, O_CREAT | O_WRONLY);

	if (fic1.descripteur == -1 || fic2.descripteur == -1)
		return -1;

	while (SE_lectureCaractere (fic1, &c) > 0)
		SE_ecritureCaractere (fic2, c);

	SE_fermeture (fic1);
	SE_fermeture (fic2);

	return 0;
}

int deplacement (const char * chemin1, const char * chemin2) {
	SE_FICHIER fic1, fic2;
	char c;

	fic1 = SE_ouverture (chemin1, O_RDONLY);
	fic2 = SE_ouverture (chemin2, O_CREAT | O_WRONLY);

	if (fic1.descripteur == -1 || fic2.descripteur == -1)
		return -1;

	while (SE_lectureCaractere (fic1, &c) > 0)
		SE_ecritureCaractere (fic2, c);

	SE_fermeture (fic1);
	SE_fermeture (fic2);

	SE_suppression (chemin1);

	return 0;
}

int sontIdentiques (const char * chemin1, const char * chemin2) {
	SE_FICHIER fic1, fic2;
	char c1, c2;
	int ret1, ret2;

	fic1 = SE_ouverture (chemin1, O_RDONLY);
	fic2 = SE_ouverture (chemin2, O_RDONLY);

	if (fic1.descripteur == -1 || fic2.descripteur == -1)
		return -1;

	do {
		ret1 = SE_lectureCaractere (fic1, &c1);
		ret2 = SE_lectureCaractere (fic2, &c2);
	}
	while (ret1 > 0 && ret2 > 0 && c1 == c2);

	SE_fermeture (fic1);
	SE_fermeture (fic2);

	// Fin de fichier pour les deux donc OK
	if (ret1 == 0 && ret2 == 0)
		return 1;
	return 0;
}

void verifAffichage () {
	int nstdout = creat ("test2.txt", 0644);
	int tmp, tmp2;

	system ("echo 'This file\nis\n\nGREAT!\n' >test1.txt");

	tmp = dup (fileno (stdout) );

	// Vider (entre autre) le buffer de stdout
	fflush (NULL);
	dup2 (nstdout, fileno (stdout) );

	affichage ("test1.txt");

	// Vider (entre autre) le buffer de stdout
	fflush (NULL);
	dup2 (tmp, fileno (stdout) );

	close (nstdout); close (tmp);

	if (! system ("diff test1.txt test2.txt >diff.log") )
		printf ("'affichage()' test has succeeded.\n");
	else
		printf ("'affichage()' test has failed.\n");

	system ("rm -f diff.log test1.txt test2.txt");
}

void verifCopie () {
	system ("echo 'This file\nis\n\nGREAT!\n' >test1.txt");

	copie ("test1.txt", "test2.txt");

	if (! system ("diff test1.txt test2.txt >diff.log") )
		printf ("'copie()' test has succeeded.\n");
	else
		printf ("'copie()' test has failed.\n");

	system ("rm -f diff.log test1.txt test2.txt");
}

void verifDeplacement () {
	system ("echo 'This file\nis\n\nGREAT!\n' >test1.txt");
	system ("cat test1.txt >test2.txt");

	deplacement ("test2.txt", "test3.txt");

	if (! system ("test -e test2.txt") )
		printf ("'deplacement() test has failed.\n");
	else if (! system ("diff test1.txt test3.txt >diff.log") )
		printf ("'deplacement()' test has succeeded.\n");
	else
		printf ("'deplacement()' test has failed.\n");

	system ("rm -f diff.log test1.txt test3.txt");
}

void verifSontIdentiques () {
	system ("echo 'This file\nis\n\nGREAT!\n' >test1.txt");
	system ("echo 'This file\nis\n\nGREAT!\n' >test2.txt");
	system ("echo 'This file\nis\n\nGREAT!\n\n' >test3.txt");
	system ("echo 'This fil\nis\n\nGREAT!\n' >test4.txt");
	system ("echo 'This file\nis\nGREAT!\n' >test5.txt");

	if (! sontIdentiques ("test1.txt", "test2.txt") )
		printf ("'sontIdentiques()' test#1 has failed.\n");
	else if (sontIdentiques ("test1.txt", "test3.txt") )
		printf ("'sontIdentiques()' test#2 has failed.\n");
	else if (sontIdentiques ("test2.txt", "test4.txt") )
		printf ("'sontIdentiques()' test#3 has failed.\n");
	else if (sontIdentiques ("test3.txt", "test5.txt") )
		printf ("'sontIdentiques()' test#4 has failed.\n");
	else
		printf ("'sontIdentiques()' test has succeeded.\n");

	system ("rm -f test1.txt test2.txt test3.txt test4.txt test5.txt");
}

int main (int argc, char ** argv) {
	verifAffichage ();
	verifCopie ();
	verifDeplacement ();
	verifSontIdentiques ();

	return 0;
}
