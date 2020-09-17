#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "se_fichier.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int client(int argc,char **argv)
{

		int i,gains=-1,fd=open("tube.fifo",O_WRONLY);
		if(errno && fd==-1)
		{
				printf(" \n\nthere is no onegoing lottery please wait for the next lottery number %d \n\n",(int)getpid());
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
		int fd2=open("gains.fifo",O_RDONLY);
		if(errno && fd2==-1)
		{
				perror("THE CLIENT DIDN T RECEIVE THE MONEY ");
				close(fd2);
				close(fd);
				system("rm *.fifo");
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
				printf("You have won  %d $ !!! // ",gains);
				read(fd2,&gains,sizeof(int));
				printf("matches %d/%d \n\n",gains,(argc-2));
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

	SE_FICHIER F1=SE_ouverture(argv[2],O_RDONLY);
	int n,i=0,k;
	SE_lectureEntier(F1,&n);
	int T[(n*3)+1];
		while(i<n*3)
		{ 
		 	SE_lectureEntier(F1,&k);
		 	T[i]=k;
		 	i++;
		}
			SE_fermeture(F1);
			printf("                     NEW LOTTERY\n\n\nHERE'S THE LUCKY NUMBERS :               \n");
			for (int j = 0; j < n; ++j)
			{
				printf(" %d ",T[j]);
			}
			printf("\n\n");
			mkfifo("tube.fifo",0644); 
			mkfifo("gains.fifo",0644); 
			int fd2=1,fd=1,B=1;

		while(B)
		{
			fd2=open("tube.fifo",O_RDONLY);
			if(errno && fd2==-1)
			{
				perror("THE SERVER DIDN T RECEIVE THE NUMBERS ");
			}
			else
			{
				i=0;
				int j=1;
				int T2[n+1],nombre=0;
					
				read(fd2,&k,sizeof(int));
				printf("client %d numbers : ",k);
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
				printf(" // number of matches : %d/%d  ",nombre,n);
			
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

				fd=open("gains.fifo",O_WRONLY);
				if(errno && fd!=-1)
				{
					perror("THE SERVER CAN T SEND THE MONEY EVEN TOUGH THE CLIENT SEND THE NUMBERS ");
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
		unlink("gains.fifo");
		unlink("tube.fifo");
}
			

int main(int argc, char **argv)
{

	if(argc<3)
	{ 
		perror("ERREUR : nombre insuffisant d'arguments\n");
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
			perror("ERREUR : arguments non valide\n");
			return -1;
		}

	}
	return 0;
}