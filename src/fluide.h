#ifndef FLUIDE_H
#define FLUIDE_H
#include "tuyau.h"

/* Place le fluide sur une case imaginaire à gauche de l'entrée,
prêt à entrer dans la grille. */
void initialiserFluide(Partie *partie);

/*Fait avancer le fluide dans les cases*/
void passerCaseSuivante(Partie *partie);
#endif
