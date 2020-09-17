#include <stdio.h>
#include "atlacan.h"

void find(char * way)
{
	atl_cd(way);

	char **places;
	int  number,found,i=0;
	struct atl_stat A;
	atl_ls(&places,&number);

	while(i<number)
	{
		atl_stat(places[i],&A);
		
		if(atl_is_file(A.mode) && A.gid!=GID_PERTONTAN)
			{
				atl_cat(places[i],&found); 
			}
		if(atl_is_dir(A.mode)  && (A.uid!=UID_CLAYTON) && atl_is_ormode(A.mode) && atl_is_owmode(A.mode) && atl_is_oxmode(A.mode))
			{	
				find(places[i]);
			}

		i++;
	}
	
	atl_free_ls(places,number);

	atl_cd(".."); 
}



int main()
{
	atl_init();

	char **places;
	int  number,i,clue;
	struct atl_stat A;

	atl_ls(&places,&number);

	i=0;
	while(i<number)
	{
		atl_stat(places[i],&A);
		
		if(atl_is_file(A.mode))
			{

				atl_cat(places[i],&clue); 
				  
			}
		if(atl_is_dir(A.mode))
			{
			
				find(places[i]);
			}
		i++;
	}

	atl_free_ls(places,number);
	atl_fini();

return 0;
}