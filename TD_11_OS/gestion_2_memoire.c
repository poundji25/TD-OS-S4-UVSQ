#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "se_fichier.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>



// int nbre_demandes_totale=0,fils=-1;
// int *demandes,adr_physique=-1,checked=0,adr_logique=-1;
// pthread_cond_t* les_cond;
// pthread_cond_t* les_cond_pere;

// structure contenant les tubes 
typedef struct 
{
	const char *t1;
	char *t2;
}tube;
// structure contenant le infos de configuration
typedef struct 
{
	int nbre_frames;
	int nbre_pages;
	int taille_page;
	int nbre_threads_fils;
	int nbre_demandes;
	tube les_tubes;
}config;


// Structure contenant les informations nécessaires au thread (les arguments)
typedef struct {						
	int nbre_demandes;
	int indice_fils;
	config parametre;
	pthread_mutex_t *mutex;
	tube les_tubes;
}Arg;

int lecture_tube (const char* tube)
{
	int res,k;
	SE_FICHIER fd = SE_ouverture(tube,O_RDONLY);

	if(fd.descripteur==-1)
	{
		fprintf(stderr,"ERREUR : ouverture tube\n");
		k=SE_suppression(tube);
		if(k==-1)
		{
			fprintf(stderr,"ERREUR : suppression tube\n");
			exit(-1);
		}
		k=SE_suppression(tube);
		if(k==-1)
		{
			fprintf(stderr,"ERREUR : suppression tube\n");
			exit(-1);
		}
	}
	if (SE_lectureEntier(fd,&res)==-1)
	{
		k=SE_suppression(tube);
		if(k==-1)
		{
			fprintf(stderr,"ERREUR : suppression tube\n");
			exit(-1);
		}
		k=SE_suppression(tube);
		if(k==-1)
		{
			fprintf(stderr,"ERREUR : suppression tube\n");
			exit(-1);
		}
	}
	SE_fermeture(fd);
	return res;
}

void ecriture_tube (char* tube,int adr)
{
	int k;
	SE_FICHIER fd = SE_ouverture(tube,O_WRONLY);
    if (fd.descripteur==-1) 
    {
    	fprintf(stderr,"ERREUR : ouverture tube\n");
		k=SE_suppression(tube);
		if(k==-1)
		{
			fprintf(stderr,"ERREUR : suppression tube\n");
			exit(-1);
		}
		k=SE_suppression(tube);
		if(k==-1)
		{
			fprintf(stderr,"ERREUR : suppression tube\n");
			exit(-1);
		}
    }
    if (SE_ecritureEntier(fd, adr) == -1)
   	{
    	fprintf(stderr,"ERREUR : ecriture dans le tube\n");
    	k=SE_suppression(tube);
		if(k==-1)
		{
			fprintf(stderr,"ERREUR : suppression tube\n");
			exit(-1);
		}
		k=SE_suppression(tube);
		if(k==-1)
		{
			fprintf(stderr,"ERREUR : suppression tube\n");
			exit(-1);
		}
   		exit(-1);
   	}
   	SE_fermeture(fd);
}
void* fonc_demande_adresse (void* arguments)
{
	srand(time(NULL));
	Arg* A = (Arg*)arguments;
	int res,adr_p;
	
	for (int i = 0; i < A->nbre_demandes; ++i)
	{
		pthread_mutex_lock(A->mutex);
		adr_p=rand()%A->parametre.nbre_pages;
		// ecriture de l'adresse
		ecriture_tube(A->les_tubes.t2,adr_p);
		// lectures de la réponse du père
		res=lecture_tube(A->les_tubes.t1);
		if(res==-2)
		{
			printf("NOT a hit\n");
		}
		else
		{
			printf("IT S a hit\n");
		}
		pthread_mutex_unlock (A->mutex);
	}

	/*while(1)//A->nbre_demandes)
	{
		fprintf(stderr,"<FILS %d>j'attends mon tour\n",A->indice_fils);
		//pthread_mutex_lock(&mutex);
		while(fils!=A->indice_fils)
		{
			sleep(2);
		}
		pthread_mutex_lock(&mutex); 
		pthread_cond_signal(&les_cond_pere[A->indice_fils]);
		//pthread_mutex_unlock (&mutex);
		pthread_cond_wait (&les_cond[A->indice_fils], &mutex);
		
		//pthread_mutex_lock (&mutex);
		//if(adr_physique==-1)
			//{
			//checked++;
		

		adr_physique=rand()%(A->nbre_pages);
		fprintf(stderr,"HERE : %d \n",adr_physique);
		fprintf(stderr,"<FILS %d> je commence, voici l'adresse physique créer %d\n",A->indice_fils,adr_physique);
		demandes[A->indice_fils]++;
		pthread_mutex_unlock (&mutex);
		sleep(1);
		
	}*/
	return NULL;
}

void* fonc_pere (void* args)
{
	config * parametre = (config *)args;
	int k;
	int adr_p;

	for (int i = 0; i < parametre->nbre_threads_fils * parametre-> nbre_demandes; ++i)
	{

		// lecture de l'adresse
		adr_p=lecture_tube(parametre->les_tubes.t1);
		// LRU
		// ecriture d'une réponse
		ecriture_tube(parametre->les_tubes.t2,adr_p+100);

	}

/*int pages [parametre->nbre_pages];
int hits [parametre->nbre_threads_fils];

// intialiser le tableau de pages 
	for (int j = 0; j <parametre->nbre_pages ; ++j)
	{
		pages[j]=0;
	}
	
	for (int j = 0; j <parametre->nbre_threads_fils ; ++j) // intialiser le tableau de hits
	{
		hits[j]=0;
}*/
	/*while(1)//(parametre->nbre_threads_fils*parametre->nbre_demandes))
	{	
		k=0;
		do
		{
		fils=rand()%parametre->nbre_threads_fils;
		printf("demandes[%d] : %d \n\n",fils,demandes[fils]);
		k++;
		}
		while(demandes[fils]>=parametre->nbre_demandes && k<10);
		if(demandes[fils] >= parametre->nbre_demandes)
		{
			printf("\n\nEND\n\n");
			exit(EXIT_SUCCESS);
		}
		printf("<PERE>  j'ai choisi le FILS %d\n\n",fils);
		//pthread_mutex_lock(&mutex);
		pthread_cond_signal(&les_cond[fils]);
		//pthread_mutex_unlock (&mutex);
		// else
		// {
		// 	printf("<PERE> HEY je suis occupé avec le FILS %d\n",fils);
		// }
			//printf("<PERE> j'attends une réponse \n");
			//sleep(1);
		pthread_mutex_lock(&mutex); 
		pthread_cond_wait (&les_cond_pere[fils],&mutex);
		printf("HEY I RECIEVED AN ANSWER \n");
		sleep(1);

		//printf("<PERE> j'attend mon tour \n");
		//pthread_mutex_lock(&mutex); 
		// pthread_cond_wait (&les_cond_pere[fils], &mutex);

		if(adr_physique!=-1)
		{
			// if(adr_physique>=0 && adr_physique< parametre->nbre_pages)
			// {
				adr_logique=adr_physique+100;
				printf("<PERE> l'adresse physique %d ---> l'adrsse logique: %d  \n",adr_physique,adr_logique);
			//}
		}
		else
		{
			printf("LE FILS N'A R1 FAIT\n");
		}
		nbre_demandes_totale++;
		adr_physique=-1;
		if(nbre_demandes_totale>=parametre->nbre_threads_fils*parametre->nbre_demandes)
		{
			printf("\n\nEND\n\n");
			exit(EXIT_SUCCESS);
			//break;	
		}
		pthread_mutex_unlock (&mutex);
	}*/
	return NULL;
}

config lecture_fichier(const char* nom_fichier)
{
	config res;

	FILE* f=fopen(nom_fichier,"r");
	if(f==NULL)
	{
		fprintf(stderr, "<PERE>		ERREUR : %s\n",strerror(errno));
		exit(1);
	}

	fscanf(f,"%d %d %d %d %d",&res.nbre_frames,&res.taille_page,&res.nbre_pages,&res.nbre_threads_fils,&res.nbre_demandes);

	printf("nbre_frames %d  taille_page %d nbre_pages %d nbre_threads_fils %d nbre_demandes %d\n",res.nbre_frames,res.taille_page,res.nbre_pages,res.nbre_threads_fils,res.nbre_demandes);

	fclose(f);
	return res;
}

int main(int argc, char const *argv[])
{	
	//printf("voila le fichier : %s\n",argv[1]);
	config parametre=lecture_fichier(argv[1]);

	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_t t0,t1[parametre.nbre_threads_fils];
	Arg arguments[parametre.nbre_threads_fils];


	// les_cond=malloc(sizeof(pthread_cond_t)*parametre.nbre_threads_fils);
	// les_cond_pere=malloc(sizeof(pthread_cond_t)*parametre.nbre_threads_fils);

	// for (int i = 0;i<parametre.nbre_threads_fils; ++i)
	// {
	// 	pthread_cond_init(&les_cond[i],NULL);
	// 	pthread_cond_init(&les_cond_pere[i],NULL);
	// }
	
	// création des deux tubes de communication 
	tube T;
	T.t1="/tmp/t1";
	T.t2="/tmp/t2";
	int j=mkfifo(T.t1,0666);
	if(j==-1)
	{
		fprintf(stderr,"ERREUR : création de tube \n");
		exit(-1);
	}
	j=mkfifo(T.t2,0666);
	if(j==-1)
	{
		fprintf(stderr,"ERREUR : création de tube \n");
		exit(-1);
	}

	parametre.les_tubes=T;
	for (int i = 0;i<parametre.nbre_threads_fils; ++i)
	{	
		arguments[i].indice_fils=i;
		arguments[i].nbre_demandes=parametre.nbre_demandes;	
		arguments[i].mutex=&mutex;
		arguments[i].les_tubes=T;
	}

	//demandes=calloc(sizeof(int),parametre.nbre_threads_fils);
	
	pthread_create(&t0,NULL,fonc_pere,&parametre);

	for (int j = 0;j<parametre.nbre_threads_fils; ++j) // boucle de lancement des threads 
	{
		printf("				passage numero %d\n",j+1);	
		pthread_create(&t1[j],NULL,fonc_demande_adresse,&arguments[j]);
	}

	for (int i = 0; i < parametre.nbre_threads_fils; ++i)
	{
		pthread_join(t1[i],NULL);
	}
	pthread_join(t0,NULL);

	return 0;
}