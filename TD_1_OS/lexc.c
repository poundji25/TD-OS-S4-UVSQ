#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexc.h"

#define ACTION_SIZE 16
#define WORD_SIZE   48

/**
 * Defines a lexicon.
 */

/**
 * Initializes a lexicon data structure.
 * 
 * The lexicon must be freed using the lexc_fini() function.
 *
 * @param[in]  name             Lexicon name.
 * @param[in]  mutable          TRUE if the lexicon is mutable.
 *
 * @return     New lexicon.
 */

int is_command(char* S,int taille)
{
	
	
}
struct lexc *lexc_init(const char *name, const int mutable) // works
{
	
	LEXC* L1=malloc(sizeof(LEXC));
	
	L1->nom=name;
	L1->modif=mutable;
	L1->nbre=0;
	L1->les_mots=NULL;
	
	
    return L1;
}

/**
 * Releases a lexicon data structure.
 *
 * @param      lexc             Lexicon to release.
 */
void lexc_fini(struct lexc *lex) // works
{
	if(!lex->les_mots)
	{printf("le lexique est vide\n");}
	else
	{
		if(!lex->les_mots->suiv)
		{ 	
			free(lex->les_mots);  }
		else
		{
		MOTS pt=lex->les_mots;
		MOTS pt2=lex->les_mots->suiv;

			while(!pt)
		{
			free(pt);
			pt=pt2;
			pt2=pt2->suiv;
		}
			
			free(lex);
		}
	}
}

int lexc_add(struct lexc *lex, const char *word) 
{
	
	
	if( !lex || !lex->modif ) // cas d'erreurs : lexique NULL ou lexique non muttable ou word NULL  ou le word existe déja dans le lexique
	{printf("je suis dans add \n");
		return 1;}
	if(!word)
	{printf("je suis dans add erreur : word==NULL \n");return 1;}
	
	if(!lexc_check(lex,word))
	{printf("je suis dans add erreur : mot existe deja dans le lexique \n");return 1;}
	
		MOTS new=malloc(sizeof(struct mots));
		new->le_mot=word; 
		new->suiv=NULL;


		 if(!lex->les_mots)  // le cas d'une liste vide (lexique vide)
		{ lex->les_mots=new; return 0;	}
	
		/*if(!lex->les_mots->suiv && lex->les_mots->le_mot) //&& strcmp(word,lex->nom)!=0) // le cas où il ya un seul mot dans le lexique
	{lex->les_mots->suiv=new; return 0;}*/

	
	MOTS tmp=lex->les_mots;

	
	
	while(tmp->suiv!=NULL)
	{
	 	tmp=tmp->suiv;
	}
	
	 	tmp->suiv=new;	
		/*printf("je suis dans add voila le mot  1: %s\n",tmp->le_mot);
		printf("je suis dans add voila le mot  2: %s\n",new->le_mot);*/
	return 0;	

}

int lexc_remove(struct lexc *lex, const char *word)
{
		if(!lex || !word || !lex->modif)
	{ return 1;}
	 
	if(lexc_check(lex,word))
	{	return 1;	}
	
	if(!lex->les_mots->suiv)
	{		
		lex->les_mots=NULL;
		return 0;
	}
	
	MOTS tmp=lex->les_mots;
	MOTS tmp2=NULL;
	
	while(tmp!=NULL)
	{
			if(strcmp(word,tmp->le_mot)==0 && !tmp2)
			{
				lex->les_mots=tmp->suiv;
				free(tmp);
				return 0;
			}
				if(strcmp(word,tmp->le_mot)==0)
			{		
				tmp2->suiv=tmp->suiv;
				free(tmp);
				return 0;
			}
			
			tmp2=tmp;
			tmp=tmp->suiv;
	}
	
    return 1;
}

int lexc_check(struct lexc *lex, const char *word) //works
{
		
	if(!lex || !word )
	{ return 1;}
	
	if(!lex->les_mots)
	{	return 1;}

	MOTS tmp=lex->les_mots;
	
	while(tmp!=NULL)
	{
			
			if(strcmp(word,tmp->le_mot)==0)
			{return 0;}
			
			tmp=tmp->suiv;
	}
	
    return 1;
}

int cmd_interpret(struct lexc *lexc, const char *cmd)
{
	/*int a=1,i=0,taille=0,index_mat=0;
	
	
	char** C=malloc(sizeof(char*)*20);
	
	while(cmd[i]!="/0")
	{
		if(cmd[i]==
		
		C[index_mat]=malloc(sizeof(char)*30);
		
		while(cmd[i]!=" ")
		{
			
		}
	}
	
	
	*/
	
	
	
	
	
    return 0;
}
