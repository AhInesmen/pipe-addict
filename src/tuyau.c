#include "tuyau.h"

/* CONNEXIONS[contenu][côté d'entrée] = côté de sortie.
   Colonnes : entrée par HAUT, DROITE, BAS, GAUCHE (ordre de Direction).
   Lignes : dans l'ordre de Contenu (types.h). */
static const Direction CONNEXIONS[8][4] = {
    /* COUDE_HAUT_DROITE */ {DROITE, HAUT, AUCUNE, AUCUNE},
    /* COUDE_DROITE_BAS  */ {AUCUNE, BAS, DROITE, AUCUNE},
    /* COUDE_GAUCHE_BAS  */ {AUCUNE, AUCUNE, GAUCHE, BAS},
    /* COUDE_HAUT_GAUCHE */ {GAUCHE, AUCUNE, AUCUNE, HAUT},
    /* HORIZONTAL        */ {AUCUNE, GAUCHE, AUCUNE, DROITE},
    /* VERTICAL          */ {BAS, AUCUNE, HAUT, AUCUNE},
    /* BOMBE             */ {BAS, GAUCHE, HAUT, DROITE}, // Le fluide repart de son coté opposé lors d'une bombe
    /* DECHARGE          */ {BAS, GAUCHE, HAUT, DROITE}, // le fluide continue son chemin en face de son point d'entrée
};

Direction opposee(Direction d)
{
    if (d == AUCUNE)
        return AUCUNE;
    return (d + 2) % 4;
}

Direction lireConnexion(Contenu c, Direction entree)
{
    if (entree == AUCUNE)
        return AUCUNE;
    return CONNEXIONS[c][entree];
}