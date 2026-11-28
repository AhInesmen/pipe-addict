#ifndef TYPES_H
#define TYPES_H
#include <stdbool.h>
#define TAILLE 6   // case de taille 6x6
#define NOM_MAX 12 // nombre maximum de lettres du nom du joueur

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
    bool remplie;
    Direction coteEntree; // côté par lequel le fluide est entré (seulement si remplie)
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
    float attente;      // secondes avant que le fluide passe à la case suivante
    float delaiCase;    // secondes pour traverser une case
    float dureeAttente; // durée totale de l'attente en cours (pour l'animation du remplissage)
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
    float tempsRestant;       // secondes avant la fin du temps limite
    Grille grilleDepart;      // copie de la grille du début, pour le bouton Refaire
    Contenu swapDepart;       // et de la case swap du début
    int numeroPartie;         // compteur de parties jouées
    int victoires;            // compteur de parties gagnées
    char joueur[NOM_MAX + 1]; // nom du joueur (+ 1 pour le '\0' de fin de chaîne)
} Partie;

#endif