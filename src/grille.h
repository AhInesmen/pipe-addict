#ifndef GRILLE_H
#define GRILLE_H

#include <stdbool.h>
#include <stdlib.h>
#include "types.h"

/* Remplit la grille au hasard : tuyaux, entrée, sortie, case swap,
   puis les malus selon le niveau */
void remplirAuHasard(Partie *partie);

/* Renvoie true si (ligne, colonne) est dans la grille. */
bool estDansGrille(int ligne, int colonne);

/* 1er clic : la case devient visible. */
void revelerCase(Partie *partie, int ligne, int colonne);

/* 2e clic : échange le tuyau de la case avec la case swap.
   Renvoie false si c'est impossible (case cachée, remplie, ou malus). */
bool swapperCase(Partie *partie, int ligne, int colonne);

/* Recache toutes les cases où le fluide n'est pas passé. */
void masquerCasesNonRemplies(Partie *partie);
#endif