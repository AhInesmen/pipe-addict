/*
 * Pipe-Addict : le bon tuyau - Projet C ING3, ECE Paris 2026-2027, groupe DU01
 * Fichier tuyau.c : table des connexions des tuyaux, directions opposées, décalages
 *
 * Utilisation de l'IA (Claude, Anthropic) dans ce fichier : environ 40 %
 * Table CONNEXIONS remplie par l'équipe après explication ; decalageLigne et decalageColonne
 *  fournies par Claude ; corrections et commentaires proposés par Claude.
 * Exemples de prompts :
 *  - « on complete un tableau de directions? d'ou viennent les dimensions du tableau? »
 * Vérification : code relu, compilé et testé par l'équipe (tests en console et parties jouées).
 */

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

int decalageLigne(Direction d)
{
    if (d == HAUT)
        return -1;
    if (d == BAS)
        return 1;
    return 0;
}

int decalageColonne(Direction d)
{
    if (d == GAUCHE)
        return -1;
    if (d == DROITE)
        return 1;
    return 0;
}