#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#define SIZE_STRING 40
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
 #include <fcntl.h>

void MsgTubesAnonymes()
{
	char chaineEcriture[SIZE_STRING];
	char chaineLecture[SIZE_STRING];
	int describteurs[2];
	sprintf(chaineEcriture,"HELLO CORONA VIRUS !!");

	if(pipe(describteurs)<0)
	{
		fprintf(stderr, "ERREUR : %s\n",strerror(errno));
	}
	
	pid_t pid_fils=fork();

	if(pid_fils!=0)
	{
			printf("ecriture dans le père \n");
		write(describteurs[1],chaineEcriture,SIZE_STRING);
		wait(NULL);
	}
	else
	{
		printf("lecture dans le fils\n");
		read(describteurs[0],chaineLecture,SIZE_STRING);
		printf("voila la chaine recuperer : %s\n",chaineLecture);
		exit(0);
	}
}

void MsgTubesNommes()
{
	char chaineLecture[SIZE_STRING];
	char chaineEcriture[SIZE_STRING];
	sprintf(chaineEcriture,"HELLO CORONA VIRUS !!");
	mkfifo("tube1.fifo",0666);
	int fd=open("tube1.fifo",O_WRONLY);
	write(fd,chaineEcriture,SIZE_STRING);
	close(fd);

	fd=open("tube1.fifo",O_RDONLY);
	int res=read(fd,chaineLecture,SIZE_STRING);
	printf("voila la chaine recuperer (taille : %d) : %s\n",res,chaineLecture);
	close(fd);
	unlink("tube1.fifo");
}

void PairTubesAnonymes()
{
	int describteurs[2];
	char chaineEcriture[10];
	char chaineLecture[3];
	sprintf(chaineEcriture,"(%d,%d)",2,3);

	pipe(describteurs);

	pid_t pid_fils=fork();

	if(pid_fils!=0)
	{
		printf("ecriture dans le père \n");
		write(describteurs[1],chaineEcriture,10);
		wait(NULL);
	}
	else
	{
		printf("lecture dans le fils \n");
		read(describteurs[0],chaineLecture,10);
		printf("voila la paire recuperer : %s\n",chaineLecture);
	}
}

void PairTubesNommes ()
{
}






int main()
{
// PARTIE 1
	//MsgTubesAnonymes();
	//MsgTubesNommes();
	//PairTubesAnonymes();
	//PairTubesNommes();

	return 0;
}