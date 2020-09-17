#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>



void *fonction (void* arg)
{
	
	int r=*(int*)arg;
	//printf("voila r : %d\n",r);

	if(r==1)
	{
		printf("HELLO WORLD 1 !!!");
	}

	if(r==2)
	{
		printf("HELLO WORLD 2 !!!");
	}

	pthread_exit(NULL);
	return NULL;
}



int main()
{
	pthread_t thread1,thread2;
	// srand(getpid());
	int a=4,b=6;
	// printf("voila a %d b %d\n",a,b);

	pthread_create(&thread1,NULL,fonction,&a);
	pthread_join(thread1, NULL);

	pthread_create(&thread2,NULL,fonction,&b);
	pthread_join(thread1, NULL);

	return 0;
}