 <<	TD_8_OS  >>


# Description

L'archive XXXXXX.tar contient les fichiers suivants :
- le fichier README.md, que vous êtes en train de lire
- le fichier million.c qui vous permettra d'exécuter le programme
- le fichier lottery.cfg fichier de configuration exemple

# Commandes

- pour compiler million.c : 
	$ gcc million.c -o million -g -Wall
- pour lancer le serveur :
	$ ./million server lottery.cfg
- pour envoyer le ticket d'un client (X = numéro): 
	$ ./million client X X X X 

# Explications de l'exécution de million (cas où il n y a aucune erreur)

- processus serveur : 
	lecture des numéros gagnants stockés dans le fichier passé en argument
	le processus crée un tube nommé "data.fifo" en mode lecture et reste dans un état prêt pour recevoir les tickets des clients (en attente d'un autre processus)
	lecture des numéros de ticket d'un client dans le tube "data.fifo" aprés avoir était rempli par l'autre processus (le client)
	vérification de correspondance des numéros lus dans "data.fifo" avec les numéros gagnants 
	le processus crée un tube nommé "earnings.fifo" en mode ecriture et reste dans un état prêt pour envoyer les gains du client à l'autre processus 

- processus client :
	le processus ouvre le tube "data.fifo" en mode ecriture et stocke dans le tube les numéros du client passés en argument
	le processus ouvre le tube "earnings.fifo" en mode lecture et récupere les gains du client via le tube 
	affichage de gains et le nombre de correspondance des numéros du client avec les numéros gagnants 
