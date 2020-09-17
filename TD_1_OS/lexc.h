#ifndef __H_LEXC__
#define __H_LEXC__

/**
 * Defines a lexicon.
 */


struct mots{
	
	const char* le_mot;
	struct mots * suiv;
	};
typedef struct mots *MOTS;

struct lexc
{
	const char * nom;
	MOTS les_mots;
	int nbre;
	int modif;
};
typedef struct lexc LEXC;
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
struct lexc *lexc_init(const char *name, const int);

/**
 * Releases a lexicon data structure.
 *
 * @param      lexc             Lexicon to release.
 */
void lexc_fini(struct lexc *lexc);

int lexc_add(struct lexc *lexc, const char *word);
int lexc_remove(struct lexc *lexc, const char *word);
int lexc_check(struct lexc *lexc, const char *word);

int cmd_interpret(struct lexc *lexc, const char *cmd);

#endif
