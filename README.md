# PIPE-ADDICT-C

Projet collaboratif en langage C développé dans un cadre pédagogique (ECE Paris).
Cette application implémente un jeu sur une interface graphique, organisée de façon modulaire :

- ‘main.c‘ : interface utilisateur (menu et affichage)
- ‘pipes.c‘ / ‘pipes.h‘ : fonctions de jeu
- ‘tests.c‘ : tests unitaires vérifiant automatiquement le bon fonctionnement du jeu

## Fonctionnalités

- A COMPLETER

## Structure du projet

calculatrice-c/
|-- main.c
|-- pipes.c
|-- pipes.h
|-- tests.c
|-- .gitignore
|-- README.md

## Compilation et exécution

### 1. Compiler la calculatrice

gcc main.c pipes.c -o pipe-addict
./pipe-addict

### 2. Compiler et exécuter les tests

Les tests automatisés vérifient la validité de chaque fonction du jeu via ‘assert()‘.
gcc tests.c pipe.c -o tests
./tests
Si tous les tests réussissent, le programme affichera :
Tous les tests ont réussi.
En cas d’erreur, l’exécution s’arrêtera immédiatement, indiquant quel test a échoué.

## A propos de ‘tests.c‘

Le fichier ‘tests.c‘ contient un ensemble de tests unitaires. Ils permettent de vérifier automatiquement que
→ les fonctions :

- A COMPLETER
  retournent bien les résultats attendus.
  Ces tests garantissent la **fiabilité** et la **stabilité** du code tout au long du développement
  → collaboratif.
  > Recommandation : exécuter les tests après toute modification du code.

## Équipe

- Ines : Interface utilisateur (‘main.c‘), gestion du dépôt GitHub
- Yasmine : Développement des fonctions de calcul (‘calculs.c‘, ‘calculs.h‘)
- Andy : Tests unitaires (‘tests.c‘), intégration et relecture
- Romain : Tests unitaires (‘tests.c‘), intégration et relecture

## Objectifs pédagogiques

- Utiliser Git et GitHub dans un vrai workflow collaboratif
- Organiser un projet C modulaire
- Mettre en place des tests unitaires
- Travailler avec branches, pull requests et validation de code

## Dépendances

- Un compilateur C (GCC ou Clang)
