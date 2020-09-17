#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>



int main (int argc,char **argv)
{
	printf("voila les arguments : %d %s %s \n",atoi(argv[1]),argv[2],argv[3]);
	if(strcmp(argv[2],"reset")==0)
	{
		kill(atoi(argv[1]),SIGUSR1);
	}
	else 
	{
		if(strcmp(argv[2],"add")==0)
		{
		kill(atoi(argv[1]),SIGUSR2);

		mkfifo("tube2.fifo",0666);
	 	int fd=open("tube2.fifo",O_WRONLY);
	 	write(fd,argv[3],30);
	 	close(fd);

		}
		else
		{
			kill(atoi(argv[1]),SIGINT);
		}
	}
	return 0;
}