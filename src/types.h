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

// Etat de la partie
typedef enum
{
    EN_COURS,
    VICTOIRE,
    DEFAITE
} EtatPartie;

// Position du fluide : la case où il se trouve et le côté par où il va sortir
typedef struct Fluide
{
    int ligne;
    int colonne;
    Direction sortie;
} Fluide;

typedef struct Partie
{
    Grille grille;
    Contenu swap;
    int ligneEntree;
    int ligneSortie;
    int niveau;
    Fluide fluide;
    EtatPartie etat;
} Partie;

#endif