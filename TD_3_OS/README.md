L'archive td3-contents.tar contient les fichiers suivants :
- le fichier README.md, que vous êtes en train de lire ;
- le fichier io_file.h, en-tête de la bibliothèque que vous allez implémenter ;
- le fichier main.c, source principale de l'exercice 3.2, dans laquelle vous
  devrez implémenter les scénarii demandés utilisant la bibliothèque ;
- le fichier main_opt.c, source principale de l'exercice 3.3, contenant des
  scénarii de test pour les fonctions avancées de la bibliothèque que vous
  devrez implémenter.

La compilation des fichiers est décrite dans la feuille de TD.

Compilation de la bibliothèque :
$ gcc -c -fPIC io_file.c
$ ar rcs libiof.a io_file.o

Compilation d'un fichier utilisant la bibliothèque :
$ gcc test.c -L. -liof