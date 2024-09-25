#!/bin/bash

echo "Bienvenue dans la calculatrice simple !"

while true
do
    echo "Entrez le premier nombre :"
    read a
    echo "Entrez l'opérateur (+, -, *, /) :"
    read op
    echo "Entrez le deuxième nombre :"
    read b

    case $op in
        +) res=$(echo "$a + $b" | bc -l);;
        -) res=$(echo "$a - $b" | bc -l);;
        \*) res=$(echo "$a * $b" | bc -l);;
        /) res=$(echo "$a / $b" | bc -l);;
        *) echo "Opérateur non valide"; continue;;
    esac

    echo "Résultat : $res"
    
    echo "Voulez-vous faire une autre opération ? (o/n)"
    read choix
    if [[ $choix != "o" ]]; then
        break
    fi
done

echo "Merci d'avoir utilisé la calculatrice !"

