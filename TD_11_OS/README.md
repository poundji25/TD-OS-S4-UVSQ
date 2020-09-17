#Commandes 
	pour compiler :
	"gcc ram.c -o ram -pthread -Wall"
	pour executer :
	"./ram config.cfg" 
#Remarque
	-une adresse logique = une adresse physique + 100
	-la partie LRU n'a pas était faite 
	-le seul critére pour avoir un hit c'est que l'adresse physique soit correcte ce qui justifie les pourcentages