#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>


void printTypeF(char * path)
{

	struct stat S;

	int tmp=stat(path,&S);

	if(tmp==-1)
	{
		fprintf(stderr,"ERREUR dans stat %s\n",strerror(errno));
	}
	if(S_ISREG(S.st_mode))
		printf("c'est un fichier \n");
	
	if(S_ISDIR(S.st_mode))
		printf("c'est un repertoire\n");
	
	if(S_ISFIFO(S.st_mode))
		printf("c'est un FIFO \n");
	
	if(S_ISSOCK(S.st_mode))
		printf("c'est un socket \n");
	
	if(S_ISLNK(S.st_mode))
		printf("c'est un lien symbolique \n");

	if(S_ISCHR(S.st_mode) || S_ISBLK(S.st_mode))
		printf("c'est un peripherique \n");

}

void printperm(char* path)
{
	struct stat S;

	int tmp=stat(path,&S);

	if(tmp==-1)
	{
		fprintf(stderr,"ERREUR dans stat %s\n",strerror(errno));
	}

	if(S.st_mode & S_IRUSR)
		printf("le propriétaire a le droit de lecture\n");

	if(S.st_mode & S_IWUSR)
		printf("le propriétaire a le droit d'ecriture\n");

	if(S.st_mode & S_IXUSR)
		printf("le propriétaire a le droit d'exécution\n");

	if(S.st_mode &  S_IRGRP)
		printf("le groupe a le droit de lecture\n");

	if(S.st_mode &   S_IWGRP)
		printf("le groupe a le droit d'ecriture\n");

	if(S.st_mode &  S_IXGRP)
		printf("le groupe a le droit d'exécution\n");

	if(S.st_mode & S_IROTH)
		printf("les autres ont le droit de lecture\n");

	if(S.st_mode & S_IWOTH)
		printf("les autres ont le droit d'ecriture\n");

	if(S.st_mode &  S_IXOTH)
		printf("les autres ont le droit d'exécution\n");


}

void printOwnerID(char* path)
{

	struct stat S;

	int tmp=stat(path,&S);

	if(tmp==-1)
	{
		fprintf(stderr,"ERREUR dans stat %s\n",strerror(errno));
	}

	printf("voici l'ID du user %d et voici l'ID du groupe %d\n",S.st_uid,S.st_gid);
}

void printSize(char * path)
{
	struct stat S;

	int tmp=stat(path,&S);

	if(tmp==-1)
	{
		fprintf(stderr,"ERREUR dans stat %s\n",strerror(errno));
	}

	printf("la taille du fichier : %ld\n",S.st_size);
}

void printDir(char * path)
{
	DIR * D=opendir(path);

	struct dirent *A;

	while()

}

int main ()
{
char path[]="test1";

printTypeF(path);
printperm(path);
printOwnerID(path);
printSize(path);

return 0;
}