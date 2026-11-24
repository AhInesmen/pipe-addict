#ifndef TYPES_H
#define TYPES_H
#include <stdbool.h>
#define TAILLE 6 // grille de 6x6 cases

// Enumeration de chaque direction possible
typedef enum
{
    HAUT,
    DROITE,
    BAS,
    GAUCHE,
    AUCUNE
} Direction;

// Enumeration de chaque type de contenu possible
typedef enum
{
    COUDE_HAUT_DROITE,
    COUDE_DROITE_BAS,
    COUDE_GAUCHE_BAS,
    COUDE_HAUT_GAUCHE,
    HORIZONTAL,
    VERTICAL,
    BOMBE,
    DECHARGE
} Contenu;

typedef struct Case
{
    Contenu contenu;
    bool revelee; // case deja vue?
    bool remplie; // le fluide est passé dans la case
} Case;

typedef struct Grille
{
    Case cases[TAILLE][TAILLE];
} Grille;

typedef struct Partie
{
    Grille grille;
    Contenu swap;
    int ligneEntree;
    int ligneSortie;
    int niveau;
} Partie;

#endif