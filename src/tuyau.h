#ifndef TUYAU_H
#define TUYAU_H

#include <stdbool.h>
#include "types.h"

/* Renvoie la direction opposée : HAUT <-> BAS, GAUCHE <-> DROITE. */
Direction opposee(Direction d);

/* Le fluide entre dans un contenu c par le côté entree : par où ressort-il ?
   Renvoie AUCUNE si le tuyau n'a pas d'ouverture de ce côté (cul-de-sac). */
Direction lireConnexion(Contenu c, Direction entree);

/* Renvoie -1, 0 ou +1 : de combien de lignes on bouge pour aller dans
   la direction d (HAUT : -1, BAS : +1, GAUCHE et DROITE : 0). */
int decalageLigne(Direction d);

/* Renvoie -1, 0 ou +1 : de combien de colonnes on bouge pour aller dans
   la direction d (GAUCHE : -1, DROITE : +1, HAUT et BAS : 0). */
int decalageColonne(Direction d);

#endif