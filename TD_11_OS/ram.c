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


int nbre_demandes_totale=0,fils=-1;
int *demandes,adr_physique=-1,adr_logique=-1;
pthread_cond_t* les_cond;
pthread_cond_t* les_cond_pere;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// structure contenant le infos de configuration
typedef struct 
{
	int nbre_frames;
	int nbre_pages;
	int taille_page;
	int nbre_threads_fils;
	int nbre_demandes;
}config;


// Structure contenant les informations nécessaires au thread (les arguments)
typedef struct {						
	int nbre_demandes;
	int indice_fils;
 	config parametre;
}Arg;

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

void* fonc_demande_adresse (void* arguments) 				// thread fils
{
	srand(time(NULL));
	Arg* A = (Arg*)arguments;
	while(1)
	{
															//printf("<FILS %d>j'attends mon tour\n",A->indice_fils);
		pthread_cond_wait (&les_cond[A->indice_fils],&mutex);
		adr_physique=rand()%(A->parametre.nbre_pages);
															//printf("<FILS %d> je commence, voici l'adresse physique créer %d\n",A->indice_fils,adr_physique);
		demandes[A->indice_fils]++;
		pthread_cond_signal(&les_cond_pere[A->indice_fils]);
		
	}
	return NULL;
}

void* fonc_pere (void* args) 								// thread père 
{
	config * parametre = (config *)args;
	int k;
	int adr_p;
	int frames [parametre->nbre_frames];
	int hits [parametre->nbre_threads_fils];

															// intialiser le tableau des frames et des hits  
	for (int j = 0; j <parametre->nbre_frames; ++j)
	{
		frames[j]=0;
	}
	
	for (int j = 0; j <parametre->nbre_threads_fils ; ++j) // intialiser le tableau de hits
	{
		hits[j]=0;
}
	while(1)
	{	
		pthread_mutex_lock(&mutex);
		k=0;
		do 										// boucle qui permet de trouver un fils qui n'a pas encore fait tous ses demandes
		{
		fils=rand()%parametre->nbre_threads_fils;
												//printf("demandes[%d] : %d \n\n",fils,demandes[fils]);
		k++;
		}
		while(demandes[fils]>=parametre->nbre_demandes && k<10);
												//printf("<PERE>voici le fils choisi %d \n",fils);

		if(demandes[fils] >= parametre->nbre_demandes)
		{
			printf("\n\nEND\n\n");
			exit(EXIT_SUCCESS);
		}
												//printf("<PERE>  j'ai choisi le FILS %d\n\n",fils);
		pthread_cond_signal(&les_cond[fils]);

		pthread_cond_wait (&les_cond_pere[fils],&mutex);

		if(adr_physique!=-1 && adr_physique>=0 && adr_physique<parametre->nbre_pages)
		{
												// partie LRU 

			adr_logique=adr_physique+100;
												//printf("<PERE> l'adresse physique %d ---> l'adresse logique: %d  \n",adr_physique,adr_logique);
			hits[fils]++;
		}
		else
		{
			fprintf(stderr,"<PERE> ERREUR : le fils n'a rien envoyer \n");
		}
		nbre_demandes_totale++;
		adr_physique=-1;
		if(nbre_demandes_totale>=parametre->nbre_threads_fils*parametre->nbre_demandes)
		{
			for (int i = 0; i < parametre->nbre_threads_fils ; ++i)
			{
				printf("le pourcentage de hits du fils %d : %d \n",i,(hits[i]*100)/parametre->nbre_demandes);
			}
			printf("\n\nEND\n\n");
			exit(EXIT_SUCCESS);	
		}
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

int main(int argc, char const *argv[])
{	
	config parametre=lecture_fichier(argv[1]);
	pthread_t t0,t1[parametre.nbre_threads_fils];
	Arg arguments[parametre.nbre_threads_fils];

	les_cond=calloc(sizeof(pthread_cond_t),parametre.nbre_threads_fils);
	les_cond_pere=calloc(sizeof(pthread_cond_t),parametre.nbre_threads_fils);

	for (int i = 0;i<parametre.nbre_threads_fils; ++i)
	{
		pthread_cond_init(&les_cond[i],NULL);
		pthread_cond_init(&les_cond_pere[i],NULL);
	}

	for (int i = 0;i<parametre.nbre_threads_fils; ++i)
	{	
		arguments[i].indice_fils=i;
		arguments[i].nbre_demandes=parametre.nbre_demandes;	
		arguments[i].parametre=parametre;
	}

	demandes=calloc(sizeof(int),parametre.nbre_threads_fils);
	
	pthread_create(&t0,NULL,fonc_pere,&parametre);

	for (int j = 0;j<parametre.nbre_threads_fils; ++j) // boucle de lancement des threads 
	{
		pthread_create(&t1[j],NULL,fonc_demande_adresse,&arguments[j]);
	}

	for (int i = 0; i < parametre.nbre_threads_fils; ++i)
	{
		pthread_join(t1[i],NULL);
	}
	pthread_join(t0,NULL);

	return 0;
}