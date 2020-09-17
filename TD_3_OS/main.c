#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "io_file.h"
#include <string.h>
#include <errno.h>

#define SIZE_MAX_BUFFER 70
// Affichage du contenu d'un fichier
// \param	path			Nom/chemin du fichier à afficher
// \return					-1 si échec à l'ouverture, 0 sinon
int print(const char *path) 
{  	

	char buff[SIZE_MAX_BUFFER+1];

	IO_FILE F=IO_open(path,O_RDONLY);

	//printf("voila le champ access de F (les flags) : %d\n",F.access);

	char *a=calloc(sizeof(char),1);
	int tmp;
	int i=0;

	while((tmp=IO_char_read(F,a)))
	{

		if(tmp==-1)
		{	fprintf(stderr,"ERREUR de lecture a partir du fichier dans print() %s\n",strerror(errno)); return -1;}
		
		buff[i]=*a;
		i++;
	}
		buff[i]='\0';
	

	// l'ecriture de fichier 
	i=0;
	IO_FILE F2;
	F2.access=O_WRONLY;
	F2.desc=STDOUT_FILENO;

	while((tmp=IO_char_write(F2,buff[i])) && i<strlen(buff))
	{
		if(tmp==-1)
		{	fprintf(stderr,"ERREUR d'ecriture dans le fichier %s\n",strerror(errno)); return -1;}

		i++;
	}

		IO_close(F);

	

	return 0;
}
// Copie de fichier
// \param	path1			Nom/chemin du fichier source
// \param	path2			Nom/chemin du fichier destination
// \return					-1 si échec à l'ouverture, 0 sinon
int copy(const char *path1, const char *path2) 
{ 

	char buff[SIZE_MAX_BUFFER+1];

	IO_FILE F=IO_open(path1,O_RDONLY);

	char *a=calloc(sizeof(char),1);
	int tmp;
	int i=0;

	while((tmp=IO_char_read(F,a)))
	{

		if(tmp==-1)
		{	fprintf(stderr,"ERREUR de lecture a partir du fichier dans copy() %s\n",strerror(errno)); return -1;}

		
		buff[i]=*a;
		i++;
	}
		buff[i]='\0';



		//la copie du contenue du premier fichier dans le deuxième 

			IO_FILE F2=IO_open(path2,O_WRONLY | O_CREAT);

			i=0;

			while((tmp=IO_char_write(F2,buff[i])) && i<strlen(buff))
	{
		if(tmp==-1)
		{	fprintf(stderr,"ERREUR d'ecriture dans le fichier %s\n",strerror(errno)); return -1;}

		i++;
	}

		IO_close(F);IO_close(F2);

	
	return 0;
}             
// Déplacement de fichier
// \param	path1			Nom/chemin du fichier source
// \param	path2			Nom/chemin du fichier destination
// \return					-1 si échec à l'ouverture, 0 sinon
int move(const char *path1, const char *path2) 
{
	char buff[SIZE_MAX_BUFFER+1];

	IO_FILE F=IO_open(path1,O_RDONLY);

	char *a=calloc(sizeof(char),1);
	int tmp;
	int i=0;

	while((tmp=IO_char_read(F,a)))
	{

		if(tmp==-1)
		{	fprintf(stderr,"ERREUR de lecture a partir du fichier dans move() %s\n",strerror(errno)); return -1;}

		buff[i]=*a;
		i++;
	}
		buff[i]='\0';


		// couper le fichier 1 dans path2

				IO_FILE F2=IO_open(path2,O_WRONLY | O_CREAT);

			i=0;

			while((tmp=IO_char_write(F2,buff[i])) && i<strlen(buff))
	{
		if(tmp==-1)
		{	fprintf(stderr,"ERREUR d'ecriture dans le fichier %s\n",strerror(errno)); return -1;}

		i++;
	}

		IO_close(F2);
		IO_remove(F.path);
 return 0;
}              
// Comparaison entre deux fichiers
// \param	path1			Nom/chemin du premier fichier
// \param	path2			Nom/chemin du second fichier
// \return					-1 si échec à l'ouverture,
//							1 si fichiers identiques, 0 sinon
int are_the_same(const char *path1, const char *path2) 
{  
	char buff[SIZE_MAX_BUFFER+1];
	char buff2[SIZE_MAX_BUFFER+1];

	IO_FILE F=IO_open(path1,O_RDONLY);

	char *a=calloc(sizeof(char),1);
	int tmp;
	int i=0;

	while((tmp=IO_char_read(F,a))) // LECTURE DU FICHIER 1
	{

		if(tmp==-1)
		{	fprintf(stderr,"ERREUR de lecture a partir du fichier 1 %s\n",strerror(errno)); return -1;}

		
		buff[i]=*a;
		i++;
	}
		buff[i]='\0';

	IO_FILE F2=IO_open(path2,O_RDONLY);
	i=0;

	while((tmp=IO_char_read(F2,a)))  // LECTURE DU FICHIER 2
	{

		if(tmp==-1)
		{	fprintf(stderr,"ERREUR de lecture a partir du fichier 2 %s\n",strerror(errno)); return -1;}

		
		buff2[i]=*a;
		i++;
	}

	buff2[i]='\0';

	if((int)strlen(buff)!=(int)strlen(buff2))
		return 0;
	else
	{

		i=0;

		while(i<strlen(buff) && buff[i]==buff2[i])
		{
			i++;
		}

		if(i<strlen(buff))
			return 0;
		else
			return 1;
	}
	IO_close(F);IO_close(F2);
	free(a);
	return 0;
}        

void check_print() {

	int nstdout = creat("test2.txt", 0644);
	int tmp,tmp2;

	system("echo 'This file\nis\n\nGREAT!\n' >test1.txt");

	tmp = dup(fileno(stdout));

	// Vider (entre autre) le buffer de stdout
	fflush(NULL);

	dup2(nstdout, fileno(stdout));

	print("test1.txt");

	// Vider (entre autre) le buffer de stdout
	fflush(NULL);

	dup2(tmp, fileno(stdout));

	close(nstdout); close(tmp);

	if (!system("diff test1.txt test2.txt >diff.log"))
		printf("'print()' test has succeeded.\n");
	else
		printf("'print()' test has failed.\n");

	system("rm -f diff.log test1.txt test2.txt");
}

void check_copy() {
	system("echo 'This file\nis\n\nGREAT!\n' >test1.txt");

	copy("test1.txt", "test2.txt");

	if (!system("diff test1.txt test2.txt >diff.log"))
		printf("'copy()' test has succeeded.\n");
	else
		printf("'copy()' test has failed.\n");

	system("rm -f diff.log test1.txt test2.txt");
}

void check_move() {
	system("echo 'This file\nis\n\nGREAT!\n' >test1.txt");
	system("cat test1.txt >test2.txt");

	move("test2.txt", "test3.txt");

	if (!system("test -e test2.txt") )
		printf("'move() test has failed.\n");
	else if (!system("diff test1.txt test3.txt >diff.log"))
		printf("'move()' test has succeeded.\n");
	else
		printf("'move()' test has failed.\n");

	system("rm -f diff.log test1.txt test3.txt");
}

void check_are_the_same () {
	system("echo 'This file\nis\n\nGREAT!\n' >test1.txt");
	system("echo 'This file\nis\n\nGREAT!\n' >test2.txt");
	system("echo 'This file\nis\n\nGREAT!\n\n' >test3.txt");
	system("echo 'This fil\nis\n\nGREAT!\n' >test4.txt");
	system("echo 'This file\nis\nGREAT!\n' >test5.txt");

	if (!are_the_same("test1.txt", "test2.txt"))
		printf("'are_the_same()' test#1 has failed.\n");
	else if (are_the_same("test1.txt", "test3.txt"))
		printf("'are_the_same()' test#2 has failed.\n");
	else if (are_the_same("test2.txt", "test4.txt"))
		printf("'are_the_same()' test#3 has failed.\n");
	else if (are_the_same("test3.txt", "test5.txt"))
		printf("'are_the_same()' test#4 has failed.\n");
	else
		printf("'are_the_same()' test has succeeded.\n");

	system("rm -f test1.txt test2.txt test3.txt test4.txt test5.txt");
}

int main(int argc, char **argv) {
	check_print();
	check_copy();
	check_move();
	check_are_the_same();

	return 0;
}
