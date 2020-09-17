#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/times.h>


int main ()
{

// EXO 1 : PARTIE 01

	// char * args[]={"ps",NULL};

	// pid_t a=fork();

	// if(a==0)
	// {

	// 	execv("/bin/ps",args);

		
	// }
	// else
	// 	wait(NULL);

// EXO 1 : PARTIE 02 

	// char * args[]={"ls","/home/user",NULL};

	// pid_t a=fork();

	// if(a==0)
	// {	

	// execv("/bin/ls",args);

	// }
	// else
	// 	wait(NULL);

// EXO 1 : PARTIE 03

	//  char * args[]={"find","/home/user","-type","f",NULL};

	//  pid_t a=fork();

	// if(a==0)
	//  {	

	//  	execv("/usr/bin/find",args);
	//  }
	// else
	//  {	wait(NULL); }

	// a=fork();

	// char * args2[]={"find","/home/user","-type","d",NULL};

	// if(a==0)
	// {	

	// 	execv("/usr/bin/find",args2);
	// }
	// else
	// 	wait(NULL);
	
// EXO 2 : PARTIE 01

	//  char * args[]={"ls","/","-R",NULL};

	//  struct tms A;
	//  pid_t pid=fork();

	//  if(pid==0)
	//  {
	//  	execv("/bin/ls",args);
	//  }
	//  else
	//  	wait(NULL);
	// times(&A);
	// printf ("le temps du fils : %ld et le temps systeme du fils %ld et la somme ; %ld \n",A.tms_cutime,A.tms_cstime,A.tms_cutime+A.tms_cstime);
	

// EXO 3  : PARTIE 01

	pid_t a;
	
		a=fork();

		if(a==0)
		{
			for (int i = 1; i <= 5; ++i)
			{
				printf("%d\n",i);
				sleep(1);
			}
		}
		else if (a) 
		{
			sleep(3);
			kill(a,SIGSTOP);
			sleep(5);
			kill(a,SIGCONT);
			wait(NULL);
		}

// EXO 4 : PARTIE 01 





return 0;



}