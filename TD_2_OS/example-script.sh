#!/bin/sh

# Affectation :
# - membre gauche (qui reçoit la valeur), sans '$'
# - membre droit (valeur utilisée), avec '$'
# - attention, pas d'espace dans l'utilisation d'opérateurs binaires (=,+,-,etc.)

# Définition d'une fonction 'add', qui prend un argument '$1'
add() {
    sum=0
    # Pour i allant de 1 à $1
    for i in `seq 1 $1`
    do
	# Concaténation de la chaîne 'sum' avec '+' et '$i'
        sum=$sum+$i
    done
    echo $sum
}

check=1
# Si 'check' est égal à 1
if [ $check -eq 1 ]
then
    echo "Ok!"
# Sinon si 'check' est égal à 0
elif [ $check -eq 0 ]
then
    echo "Never"
# Sinon
else
    echo "Never again"
fi

values="mickey donald dingo pluto"
# Pour chaque sous-chaîne de 'values'
for i in $values
do
    echo $i
done

add 10
