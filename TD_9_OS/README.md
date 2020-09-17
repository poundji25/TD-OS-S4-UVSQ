L'archive td9-contents.tar contient les fichiers suivants :
- le fichier README.md, que vous êtes en train de lire ;
- le fichier reduc.c, source de l'exercice 9.2, dans laquelle vous devrez
  implémenter les fonctions et blocs d'instructions manquants ;

La compilation des fichiers se fait comme suit :
$ gcc reduc.c -o reduction -pthread

L'option -pthread permet de lier la bibliothèque pthread, et d'utiliser,
entre autres, les fonctions liées au threads POSIX.