#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void * question1 (void * arg) {
	printf ("Hello World!\n");

	return NULL;
}

void * question2 (void * arg) {
	printf ("Nombre généré : %d\n", * (int *) arg);

	return NULL;
}

void * question3 (void * arg) {
	int * a = malloc (sizeof (int));

	*a = 1 + rand () % 100;

	printf ("Nombre généré : %d\n", *a);

	return a;
}

void * question4 (void * arg) {
	int * tab = (int *) arg;
	int moy = 0, i;

	for (i = 0; i < 5; ++i) moy += tab [i];
	moy /= 5;

	printf ("Moyenne : %d\n", moy);

	return NULL;
}

typedef struct {int * t; int s;} a_t;

void * question5 (void * arg) {
	a_t * tab = (a_t *) arg;
	int moy = 0, i;

	for (i = 0; i < tab->s; ++i) moy += tab->t [i];
	moy /= tab->s;

	printf ("Moyenne : %d\n", moy);

	return NULL;
}

int main (int argc, char ** argv) {
	srand (time (NULL));
	pthread_t tid;
	int a, i, * r;
	a_t t;

	printf ("Q1 :\n");
	pthread_create (&tid, NULL, question1, NULL);
	pthread_join (tid, NULL);

	printf ("Q2 :\n");
	a = 1 + rand () % 100;
	printf ("Pere : %d\n", a);
	pthread_create (&tid, NULL, question2, &a);
	pthread_join (tid, NULL);

	printf ("Q3 :\n");
	pthread_create (&tid, NULL, question3, NULL);
	pthread_join (tid, (void **) &r);
	printf ("Pere : %d\n", *r);
	free (r);

	printf ("Q4 :\n");
	t.t = malloc (5 * sizeof (int));
	t.s = 5;
	printf ("Pere : tab ->");
	for (i = 0; i < 5; ++i) {
		t.t [i] = 1 + rand () % 100;
		printf (" %d", t.t [i]);
	} printf ("\n");
	pthread_create (&tid, NULL, question4, t.t);
	pthread_join (tid, NULL);

	printf ("Q5 :\n");
	pthread_create (&tid, NULL, question5, &t);
	pthread_join (tid, NULL);

	free (t.t);

	return 0;	
}
