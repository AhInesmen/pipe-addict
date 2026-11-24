#ifndef TUYAU_H
#define TUYAU_H

#include <stdbool.h>
#include "types.h"

/* Renvoie la direction opposée : HAUT <-> BAS, GAUCHE <-> DROITE. */
Direction opposee(Direction d);

/* Le fluide entre dans un contenu c par le côté entree : par où ressort-il ?
   Renvoie AUCUNE si le tuyau n'a pas d'ouverture de ce côté (cul-de-sac). */
Direction lireConnexion(Contenu c, Direction entree);

#endif