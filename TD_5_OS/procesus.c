#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

void creat_arbo_procesus(int compteur,const int max,int nbre_de_fils) //A VERFIER
{
	if(compteur==max)
	{
		return;
	}
	else
	{	
		int status;
		pid_t pid_fils;
		for (int i = 0; i < nbre_de_fils; ++i)
		{
			pid_fils=fork();

			if(!pid_fils)
			{
				printf("le PID du procesus %d : %d\n",compteur,getpid());
				compteur=compteur+nbre_de_fils;
				creat_arbo_procesus(compteur,max,nbre_de_fils);
				exit(0);
			}
			//printf("\n");
			wait(&status);
		}
	}

}

int main()
{

/*///// PARTIE 5.1




	pid_t pid_fils=fork();


	if(pid_fils)
	//printf(" Mon PID est : %d et celui de mon fils est : %d\n",(int)getpid(),pid_fils);
	{
		srand(getpid());
		int N=rand();
		printf("le nombre %d \n",N);
	}
	else
	printf(" Mon PID est : %d et celui de mon père est : %d\n",(int)getpid(),(int)getppid());
*/



/*//// PARTIE 5.2
	pid_t pid;
	int status;
	srand(getpid());

	for (int i = 0; i < 10; ++i)
	{	
		pid=fork();
		//pid_fils=;
		if(!pid)
		{
			sleep(rand()%4);
			printf("le pid du fils num %d : %d\n",i+1,(int)getpid());
			exit(0);
		}
		printf("le pid de mon fils %d \n",(int)wait(&status));
	}
*/

 
///// PARTIE 5.3
	
	/*int n,m,status,status2;
	pid_t pid_fils,petit_fils;

	printf("veuillez entrez les deux entires m et n \n");
	scanf("%d %d",&m,&n);*/
	printf("le PID du père %d \n\n",(int)getpid());

/*  le père va créer m fois n procesus fils
	for(int i=0;i<m;i++)  
	{

		for(int j=0;j<n;j++)
		{
			pid_fils=fork();
			
			if(!pid_fils)
				{
					printf("	le PID : %d du fils num : %d %d\n",(int)getpid()0,i+1,j+1);
					exit(0);
				}
			wait(&status);
		}

		printf("\n");
	} 
*/
	
/* le procesus père va créer m procesus fils, puis chaque procesus fils va créer n procesus petit fils
	for (int i = 0; i < m; ++i)
	{
		

		pid_fils=fork();


		if(!pid_fils)
		{
			printf("	le PID du fils %d num %d\n\n",(int)getpid(),i+1);
			for (int j = 0; j < n; ++j)
			{
				petit_fils=fork();

				if(!petit_fils)
				{
					printf("		le PID %d du petit_fils num %d %d\n",(int)getpid(),i+1,j+1);
					exit(0);
				}
				
				wait(&status2);
				printf("\n");
			}
			exit(0);
		}
		wait(&status);
		printf("\n\n");
	}
*/

	creat_arbo_procesus(0,9,3);



	return 0;
}