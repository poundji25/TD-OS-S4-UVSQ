  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include<stdlib.h>
  #include "io_file.h"
  #include <string.h>
  #include <unistd.h>
  #include <errno.h>
  #include <stdio.h>

IO_FILE IO_open(const char *path, int access)
{
	IO_FILE F1;

	if (access & O_CREAT)
		{	F1.desc=creat(path,0644);}
	else
	{	
			F1.desc=open(path,access);

	}
	

	F1.access=access;
	F1.path=malloc(sizeof(char)*((int)strlen(path)+1));
	strcpy(F1.path,path);
	
	return F1;
}

int IO_close(IO_FILE file)
{
	
	int a= close(file.desc);	

	if(a)
	{
		fprintf(stderr," ERREUR de fermeture %s\n : ",strerror(errno));
		free(file.path);
		return -1;
	}
	free(file.path);
	return 0;
}


int IO_remove(const char *path)
{
	return remove(path);
}

int IO_char_read(IO_FILE file,char *c)
{
	if(file.access & O_WRONLY)
	{
		fprintf(stderr,"ERREUR le fichier ne peux pas etre lu %s\n",strerror(errno));
		return -1;
	}

	return read(file.desc,c,sizeof(char));

}

int IO_char_write(IO_FILE file,const char c)
{
	if(file.access & O_RDONLY)
	{
		fprintf(stderr,"ERREUR on ne peux pas ecrire dans le fichier %s\n ",strerror(errno));
		return -1;
	}

	char tmp[2];
	tmp[0]=c;
	tmp[1]='\0';

	return (int)write(file.desc,tmp,strlen(tmp));
}

int IO_string_read(IO_FILE file,char *string,int maxSize)
{
	return read(file.desc,string,maxSize);
}


int IO_string_write(IO_FILE file,const char *string,int size)
{
	if(file.access & O_RDONLY)
	{
		fprintf(stderr,"ERREUR on ne peux pas ecrire dans le fichier %s\n ",strerror(errno));
		return -1;
	}

	return (int)write(file.desc,string,size);
}

int IO_int_read(IO_FILE file,int *n)
{
/*if(file.access & O_WRONLY)
	{
		fprintf(stderr,"ERREUR le fichier ne peux pas etre lu %s\n",strerror(errno));
		return -1;
	}
	// convertir l'entier en char*
	char tmp[10],c[2];
	int i;
	
	i=IO_char_read(file,c);
	
	if(i==-1)
	{
		fprintf(stderr,"ERREUR de lecture %s\n",strerror(errno));
		return -1;
	}
	if(i==0)
	{
		return 0;
	}

	if(c<'A'||c>'z')
	{

	}

	*n=0;
	int j=0;
	while(j<strlen(tmp))
	{
		*n=((int)tmp[j])+*n;
		j++;
	}
*/
	if(file.desc<0)
	{
		return -1;
	}

	return read(f.desc,n,sizeof(int));
}

int IO_int_write(IO_FILE file,const int n)
{
	/*
	if(file.access & O_RDONLY)
	{
		fprintf(stderr,"ERREUR on ne peux pas ecrire dans le fichier %s\n ",strerror(errno));
		return -1;
	}

	char tmp[10];
	int i=0,a=n;

	while(a!=0 && i<strlen(tmp))
	{
		tmp[i]=(char)a%10;
		a=a/10;
	}
	tmp[i]='\0';
   */
	if(file.desc < 0)
	{
		return -1;
	}

	return write(file.desc,ns,sizeof(int));

}