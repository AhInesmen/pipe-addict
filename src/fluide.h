#ifndef FLUIDE_H
#define FLUIDE_H
#include "tuyau.h"

#define DELAI_DEPART 5.0f  // le fluide part 5 s après le début de la partie
#define DELAI_CASE 10.0f   // puis il met 10 s pour traverser chaque case
#define PAUSE_BOMBE 5.0f   // après une bombe, le fluide repart 5 s plus tard
#define DELAI_MINIMUM 0.5f // une décharge ne peut pas accélérer le fluide au-delà

/* Place le fluide sur une case imaginaire à gauche de l'entrée,
prêt à entrer dans la grille. */
void initialiserFluide(Partie *partie);

/*Fait avancer le fluide dans les cases*/
void passerCaseSuivante(Partie *partie);

/* À appeler à chaque tic du chronomètre : dt est le temps écoulé depuis
   le tic précédent, en secondes. Fait avancer le fluide quand son attente est finie. */
void avancerFluide(Partie *partie, float dt);
#endif
