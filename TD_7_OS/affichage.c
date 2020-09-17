#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
 #include <fcntl.h>


typedef void (*sighandler_t)(int);
char base[10][50];
int co=2,fd;
char chaineLecture[50];
//static void sig_handler(int);
sighandler_t signal(int signum, sighandler_t handler);

void sig_handler(int num)
	{

	 	if(num==SIGUSR2)
	 	{
	 		printf("je suis dans le handler SIGUSR2\n\n");
	 		mkfifo("tube2.fifo",0666);
	 		fd=open("tube2.fifo",O_RDONLY);
	 		read(fd,chaineLecture,30);
	 		sprintf(base[co],"%s",chaineLecture);
	 		co++;
	 		close(fd);
	 	}
	 	else if(num==SIGUSR1)
	 	{
	 		
	 		printf("je suis dans le handler SIGUSR1\n\n");
	 		for (int i = 2; i <co ; ++i)
	 		{
	 			strcpy(base[i],"\0");
	 		}
	 		co=2;
	 	}
	}

int main()
{
	printf("voila mon PID :%d\n",getpid());
	char dateHeure [25];

	struct timeval T1;
	gettimeofday(&T1,NULL);
	struct tm *T2=localtime(&(T1.tv_sec));
	sprintf(dateHeure,"%02d/%02d/%0d---%02d:%02d:%02d",T2->tm_mday,T2->tm_mon,T2->tm_year,T2->tm_hour,T2->tm_min,T2->tm_sec);

	strncpy(base[0],"Bonjour, ceci est un affichage informatisé",(int)strlen("Bonjour, ceci est un affichage informatisé"));
	strncpy(base[1],dateHeure,strlen(dateHeure));
	

	int j=0;
	for (int i = 0; i < 50; ++i)
	{
		if(signal(SIGUSR1,sig_handler)==SIG_ERR)
		{
			perror("signal");
    		exit(EXIT_FAILURE);
		}	
			if(signal(SIGUSR2,sig_handler)==SIG_ERR)
		{
			perror("signal");
    		exit(EXIT_FAILURE);
		}
		if(j==co)
		{
			j=0;
		}
		sleep(2);
		printf("%s\n",base[j]);
		j++;
	}

	 

	return 0;
}