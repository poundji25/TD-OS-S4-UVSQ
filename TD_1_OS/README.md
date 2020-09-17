L'archive td1-contents.tar contient les fichiers suivants :
- le fichier README.md, que vous êtes en train de lire ;
- le fichier lexc.c, source de la bibliothèque du lexique, que vous allez
  devoir remplir tout au long du TD ;
- le fichier lexc.h, en-tête de la bibliothèque du lexique, contenant les
  prototypes des fonctions visibles de l'extérieur ;
- le fichier test_lexc.c, contenant la fonction principale du programme et
  les fonctions de tests unitaires et d'intégration.

La compilation du projet se fait à l'aide de la commande suivante :
$ gcc test_lexc.c lexc.c

L'option -g permet de compiler en mode debug.

Pour compiler en mode test, vous devez définir la constante LEXC_TEST,
à l'aide de l'option -D :
$ gcc -DTEST_LEXC test_lexc.c lexc.c
