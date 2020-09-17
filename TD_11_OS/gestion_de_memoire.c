#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

 //int etat_thread_courant = -1;
 						//			 0 : thread en lecture
 						//			 1 : thread en ecriture
 //int thread_courant=-1; 
 						//			-1 : aucun thread fait de demandes
 						// 			sinon indice du thread_courant
int nbre_demandes_totale=0;
int *demandes,adr_physique=-1,checked=0,adr_logique=-1;
// pthread_cond_t cond PTHREAD_COND_INITIALIZER;
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

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
	int adr_physique;			
	int nbre_demandes;
	int indice_fils;
}Arg;


void* fonc_demande_adresse (void* arguments)
{
	Arg* A = (Arg*)arguments;
	if(checked==1)
	{
		printf("<FILS>		voici l'adresse logique que mon père m'a envoyer : %d\n",adr_logique);
	}
	else
	{
		printf("<FILS>		HEY je suis dans fonc_demande_adresse et je commence la boucle\net voici l'adresse physique créer %d\n",A->adr_physique);
		nbre_demandes_totale++;
		//pthread_mutex_lock (&mutex);
		adr_physique=A->adr_physique;
	}
	//pthread_mutex_unlock (&mutex);

	//for(int i=0;i<50;i++)//thread_courant!= A->indice_fils)
	//{
		//pthread_mutex_lock (&mutex);
		//printf("voila les variables du fils %d (adr_physique :%d):\n-thread_courant : %d\n-etat_thread_courant : %d\n-nbre_demandes_totales : %d\n\n",A->indice_fils,adr_physique,thread_courant,etat_thread_courant,nbre_demandes_totale);
		
		// if(demandes[A->indice_fils]>=2)
		// {
		// 	printf("\n<FILS>       le fils numéro %d vas s'arreter \n",A->indice_fils);
		// 	thread_courant=-1;
		// 	break;
		// }
		// if(thread_courant==A->indice_fils)
		// {
		// 	if(etat_thread_courant==0)
		// 	{
		// 		pthread_mutex_lock (&mutex);
		// 		etat_thread_courant=-1;
		// 		thread_courant=-1;
		// 		adr_physique=-1;
		// 		pthread_mutex_unlock (&mutex);
		// 	}
		// }
		// if(thread_courant==-1)
		// {
		// 	if(etat_thread_courant==-1)
		// 	{		
		// 		pthread_mutex_lock (&mutex);
		// 		thread_courant=A->indice_fils; 
		// 		adr_physique = A->adr_physique;
		// 		demandes[A->indice_fils]++;
		// 		nbre_demandes_totale++;
		// 		etat_thread_courant=1;
		// 		pthread_mutex_unlock (&mutex);
		// 	}	
		// 	else
		// 	{
		// 			printf("<FILS>		ERREUR : l'etat du thread_courant est incorrecte \n");
		// 	}
		// }	
		
		//pthread_mutex_unlock (&mutex);
	//}
	
	printf("<FILS>		HEY je suis dans fonc_demande_adresse et j'ai terminé la boucle\n");
	return NULL;
}
/*void LRU ()
{

}*/
/*void* fonc_pere (void* args)
{
	
	return NULL;

}*/
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

	printf("voila : %d %d %d %d %d\n",res.nbre_frames,res.taille_page,res.nbre_pages,res.nbre_threads_fils,res.nbre_demandes);

	fclose(f);
	return res;
}

int main(int argc, char const *argv[])
{	
	printf("voila le fichier : %s\n",argv[1]);
	config parametre=lecture_fichier(argv[1]);

	srand(time(NULL));

	pthread_t t0,t1[2];//parametre.nbre_threads_fils];
	Arg arguments[2];//parametre.nbre_threads_fils];

	
	for (int i = 0; i < 2;i++)//parametre.nbre_threads_fils; ++i)
	{
		arguments[i].nbre_demandes=parametre.nbre_demandes;	
		arguments[i].adr_physique=-1;
	}
	demandes=calloc(sizeof(int),2);//parametre.nbre_threads_fils);

	//pthread_cond_init(pthread_cond_t *cond,const pthread_condattr_t *cond_attr);

	/*for (int j = 0; j < 2;j++)//parametre.nbre_threads_fils; ++j)  // boucle de lancement des threads 
	{
		printf("				passage numero %d\n",j+1);	
		
		arguments[j].adr_physique=rand()%parametre.nbre_pages;
		arguments[j].indice_fils=j;
		printf("voila l'adr_physique : %d\net l'indice_fils : %d\n",arguments[j].adr_physique,j);
		pthread_create(&t1[j],NULL,&fonc_demande_adresse,&arguments[j]); 
	}*/
	//config* parametre=(config*)args;
	int pages [parametre.nbre_pages];
	int hits [parametre.nbre_threads_fils];

	// intialiser le tableau de pages 
	for (int j = 0; j <parametre.nbre_pages ; ++j)
	{
		pages[j]=0;
	}
	
	for (int j = 0; j <parametre.nbre_threads_fils ; ++j) // intialiser le tableau de hits
	{
			hits[j]=0;
	} 

	int i=0,j=0;
	while(nbre_demandes_totale<4)//parametre.nbre_demandes*parametre.nbre_threads_fils)
	{
		j=0;
		i=rand()%2;
		printf("i= %d \n\n",i);
		if(demandes[i]<2)
		{
			if(arguments[i].adr_physique==-1)
			{
				arguments[i].indice_fils=i;
				arguments[i].adr_physique=(rand()%parametre.nbre_pages)+100; //pages[rand()%parametre.nbre_pages];
			}
			pthread_create(&t1[i],NULL,&fonc_demande_adresse,&arguments[i]);
			pthread_join (t1[i],NULL);
			printf("adr_physique= %d  arguments.indice_fils= %d \n",adr_physique,arguments[i].indice_fils);
			demandes[i]++;

			
			if(adr_physique>=0 && adr_physique<parametre.nbre_pages+100)
			{
					checked=1;
					adr_logique=adr_physique-100;
					printf("<PERE>		l'adresse physique reçu %d est valide est voici l'adresse logique qui lui correspond %d \n",adr_physique,adr_logique);
				
					pthread_create(&t1[i],NULL,&fonc_demande_adresse,&arguments[i]);
					pthread_join (t1[i],NULL);
			}
				adr_physique=-1;
				checked=0;
				adr_logique=-1;
		}
	//for (int i = 0; i < 50; ++i)//parametre.nbre_demandes*parametre.nbre_threads_fils)
	//{
		// LE MAIN ATTEND AVEC JOIN = IL NE VA PAS REPONDRE AU FILS
		// SOLUTION FAIRE UNE BOUCLE WHILE AVEC COMME CONDITION (NBRE_DE_DEMANDES>NBRE_DE_DEMANDES_TOTALE)
		// ET A L'INTERIEUR ON CREE UN FILS ET PUIS 


		//pthread_mutex_lock (&mutex);
		//  <<<<<<<<<<<<<<<<
			// if(thread_courant!=-1)
			// {
			// 	//read(describteurs[0],&tmp,sizeof(int));
			// 	if(etat_thread_courant==1)
			// 	{
			// 		printf("\n<PERE>		recu du fils numero %d : %d",thread_courant,adr_physique);
			// 		etat_thread_courant=0;
			// 	}
			// 	else
			// 	{
			// 		printf("<PERE>		ERREUR : l'etat du thread_courant est incorrecte \n");
			// 	}

																//partie LRU mais on j v juste afficher
			

																//puis on répond 
																// && condition disponibilite de la page)
			
			//}
			// >>>>>>>>>>>>>>>>
			//pthread_mutex_unlock (&mutex);
	//}

		//pthread_create(&t0,NULL,&fonc_pere,&parametre);
	
		//for (int i = 0; i < 2;i++)
		//{//parametre.nbre_pages; ++i) {

		//}
		//pthread_join(t0,NULL);
		
		//i++;
	}
	printf("\n\n<PERE>		FINISHED\n");

	//lire le fichier de config (nbre de frames,nbre de pages,taille de page,nbre de demandes ...)

	//créer et intialiser l'ensemble des structures qui represente la mémoire

		//les structures : matrice de taille (nbre de pages * taille page) = RAM
		// 				   tableau de taille ? = les parties réservé aux processus de la RAM qui commence de @physique1 à @physique2 ( @physique2 est indicative c.a.d kader plus ou moins)

	//générer les adresses physiques

	return 0;
}