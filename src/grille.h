#ifndef GRILLE_H
#define GRILLE_H

#include <stdbool.h>
#include "types.h"

/* Génère une nouvelle grille : lignes d'entrée et de sortie, malus selon
   le niveau, tuyaux (avec au moins un chemin possible), case swap.
   Toutes les cases sont cachées, puis la grille initiale est sauvegardée. */
// void genererGrille(Partie *partie, int niveau);

/* Copie la grille et la case swap actuelles dans grilleInitiale et swapInitial. */
// void sauvegarderInitiale(Partie *partie);

/* Bouton Refaire : remet la grille et la case swap initiales, toutes les
   cases cachées, mêmes entrée et sortie. */
// void refaire(Partie *partie);

/* Bouton Nouveau : génère une grille entièrement nouvelle (nouvelles entrée
   et sortie), au niveau de la partie en cours. */
// void nouveau(Partie *partie);

/* 1er clic : révèle la case (ligne, colonne). */
// void revelerCase(Partie *partie, int ligne, int colonne);

/* 2e clic : échange le tuyau de la case avec celui de la case swap.
   Renvoie false si l'échange est impossible : case cachée, remplie ou malus. */
// bool swapperCase(Partie *partie, int ligne, int colonne);

/* Explosion d'une bombe : recache toutes les cases que le fluide n'a pas
   remplies, sans changer leur contenu. */
// void masquerCasesNonRemplies(Partie *partie);

/* Renvoie true si (ligne, colonne) se trouve dans la grille. */
// bool estDansGrille(int ligne, int colonne);

#endif