#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int lectureEntier (int fd,char* chemin,int * entier) 
{
	char c;
	int reti, retour = 0;
	*entier = 0;

	do {
		reti = read (fd, &c, 1); retour += reti;
		if (reti == -1) {
			printf ("Erreur à la lecture de %s : %s\n",chemin,
				strerror (errno) );
			return reti;
		}
	} while (isspace (c) );
	
		if (c < '0' || c > '9')
		return -1;

	do {
		(*entier) *= 10;
		(*entier) += c - '0';
		reti = read (fd, &c, 1); retour += reti;
		if (reti == -1) {
			printf ("Erreur à la lecture de %s : %s\n",chemin,
				strerror (errno) );
			return reti;
		}
	} while (c >= '0' && c <= '9');

	
	return retour;
}

int client(int argc,char **argv)
{

		int i,gains=-1,fd=open("data.fifo",O_WRONLY);
		if(errno && fd==-1)
		{
				printf(" \n\n<CLIENT>there is no onegoing lottery please wait for the next lottery number %d \n\n",(int)getpid());
				close(fd);
				return 2;
		}
		else
		{
			int k=1;
			i=(int)getpid();
			write(fd,&i,sizeof(int));
			i=2;
			while(i<argc)
			{	
				k=atoi(argv[i]);
				write(fd,&k,sizeof(int));
				i++;
			}
			close(fd);
		
		}
		int fd2=open("earnings.fifo",O_RDONLY);
		if(errno && fd2==-1)
		{
				perror("<CLIENT>THE CLIENT DIDN T RECEIVE THE MONEY ");
				close(fd2);
				close(fd);
				unlink("earnings.fifo");
				return -1;
		}
		read(fd2,&gains,sizeof(int));
		if(gains==0)
		{
				i=2;
			while(i<argc)
			{	
				printf(" %d ",atoi(argv[i]));
				i++;
			}
				printf(" // you have won  %d $\n\n",gains);
				gains=0;
		}
		else
		{
				printf("<CLIENT>You have won  %d $ !!! // ",gains);
				read(fd2,&gains,sizeof(int));
				printf("matching numbers %d/%d \n\n",gains,(argc-2));
				if(gains==argc-2)
				gains=1;
				else
				gains=0;

		}

		close(fd2);
		close(fd);
		return gains;
}


void server(char **argv)
{

	int fd=open(argv[2],O_RDONLY);
	int n,i=0,k,B;
	k=lectureEntier(fd,argv[2],&n);
		if(k<0)
		{
			perror("<SERVER> READING ERROR ");
			exit(EXIT_FAILURE);
		}
	int T[(n*3)+1];
	printf("<SERVER>\n\n            NEW LOTTERY, HERE'S THE LUCKY NUMBERS :            \n\n<SERVER>");
		while(i<n*3)
		{	 
	 		B=lectureEntier(fd,argv[2],&k);
	 		if(B<0)
			{
				printf("<SERVER> READING ERROR IN THE LOTTERY CONFIGURATION \n\n");
				exit(EXIT_FAILURE);
			}
	 		if(i<n)
	 		printf(" %d ",k);
	 	
	 	T[i]=k;
	 	i++;
	}
	printf("\n\n");
	close(fd);

	mkfifo("data.fifo",0644); 
	mkfifo("earnings.fifo",0644); 
	int fd2=1;
	B=1;

	while(B)
	{
			fd2=open("data.fifo",O_RDONLY);
			if(errno && fd2==-1)
			{
				perror("<SERVER> THE SERVER DIDN T RECEIVE THE NUMBERS ");
			}
			else
			{
				i=0;
				int j=1;
				int T2[n+1],nombre=0;
					
				read(fd2,&k,sizeof(int));
				printf("<SERVER> client %d numbers : ",k);
				while(j<n+1)
				{
					read(fd2,&k,sizeof(int));
					T2[j]=k;
					printf(" %d ",k);
					if(T2[j]==T[i])
					 	{
					 		nombre++;
					 	}
					 i++; j++;
				}
				printf(" // matching numbers : %d/%d  ",nombre,n);
			
				int gains=0;
				i=n;
				while(i<(n*3)-1)
				{
						if(T[i]==nombre)
						{
							gains=T[i+1];
						}
						i=i+2;
				}
				printf("// earnings : %d $\n\n",gains);

				fd=open("earnings.fifo",O_WRONLY);
				if(errno && fd!=-1)
				{
					perror("<SERVER>THE SERVER CAN T SEND THE MONEY EVEN TOUGH THE CLIENT SEND THE NUMBERS ");
				}
				else
				{
					write(fd,&gains,sizeof(int));
					if(gains==T[n+1])
					{
						 B=0;
					}
					gains=nombre;
					write(fd,&gains,sizeof(int));
				}
				close(fd);

					
			}
			close(fd2);
	}
	unlink("earnings.fifo");
	unlink("data.fifo");
}
			

int main(int argc, char **argv)
{

	if(argc<3)
	{ 
		printf("ERREUR : nombre insuffisant d'arguments\n\n");
		return -1;
	}

	if(strcmp(argv[1],"client")==0)
	{
		int res=client(argc,argv);
		return res;
	}
	
	else
	{
		if(strcmp(argv[1],"server")==0)
		{
			server(argv);
		}
		else
		{
			printf("ERREUR : arguments non valide\n\n");
			return -1;
		}

	}
	return 0;
}