#ifndef JEU_H
#define JEU_H

#include "fluide.h" // pour DELAI_DEPART et DELAI_CASE
#include "types.h"

/* Durée maximale d'une partie : l'attente du départ, plus le temps de traverser 30 cases.
   Elle suit toujours la vitesse du fluide (avec 10 s par case : 5 + 30 x 10 = 305 s). */
#define TEMPS_LIMITE (DELAI_DEPART + 30 * DELAI_CASE)

/* Bouton Nouveau : grille au hasard, fluide prêt à partir dans 5 s, chrono à TEMPS_LIMITE.
   Garde une copie de la grille de départ pour le bouton Refaire. */
void nouvellePartie(Partie *partie, int niveau);

/* Bouton Refaire : même grille, mêmes entrée et sortie, tout est recaché. */
void refairePartie(Partie *partie);

/* Bouton Terminer : le fluide va instantanément au bout de son chemin. */
void terminerPartie(Partie *partie);

/* À appeler à chaque tic du chronomètre (dt = temps écoulé, en secondes) :
   le temps limite diminue et le fluide avance. À 0 seconde, la partie est perdue. */
void mettreAJour(Partie *partie, float dt);

#endif