/*
 * Pipe-Addict : le bon tuyau - Projet C ING3, ECE Paris 2026-2027, groupe DU01
 * Fichier grille.c : remplissage au hasard, révéler, échanger avec la case swap, recacher
 *
 * Utilisation de l'IA (Claude, Anthropic) dans ce fichier : environ 60 %
 * remplirAuHasard fournie par Claude puis expliquée ; révéler et échanger écrites avec ses
 *  corrections ; masquerCasesNonRemplies (bombe, mode mémoire) fournie par Claude.
 * Exemples de prompts :
 *  - « quand on révèle une autre case sur le damier, si elle n'est pas dans le chemin du fluide, on la recache »
 * Vérification : code relu, compilé et testé par l'équipe (tests en console et parties jouées).
 */

#include <stdlib.h>
#include "grille.h"

/* Remplit la grille au hasard : tuyaux, entrée, sortie, case swap,
   puis les malus selon le niveau. Toutes les cases sont cachées. */
void remplirAuHasard(Partie *partie)
{
    /* 1. Un tuyau au hasard dans chaque case (les 6 tuyaux sont les valeurs 0 à 5) */
    for (int i = 0; i < TAILLE; i++)
    {
        for (int j = 0; j < TAILLE; j++)
        {
            partie->grille.cases[i][j].contenu = rand() % 6;
            partie->grille.cases[i][j].revelee = false;
            partie->grille.cases[i][j].remplie = false;
            partie->grille.cases[i][j].coteEntree = AUCUNE;
        }
    }

    /* On place l'entrée, la sortie et la case swap aléatoirement*/
    partie->ligneEntree = rand() % TAILLE;
    partie->ligneSortie = rand() % TAILLE;
    partie->swap = rand() % 6;

    /* On place les malus aléatoirement */
    int nbMalus = 2 * partie->niveau;
    int places = 0;
    while (places < nbMalus) // tant que tous les malus ne sont pas placés
    {
        int i = rand() % TAILLE; // choix des coordonées aléatoire
        int j = rand() % TAILLE;
        Case *c = &partie->grille.cases[i][j];

        bool dejaMalus = (c->contenu == BOMBE || c->contenu == DECHARGE);
        bool caseEntree = (i == partie->ligneEntree && j == 0);
        bool caseSortie = (i == partie->ligneSortie && j == TAILLE - 1);

        if (!dejaMalus && !caseEntree && !caseSortie)
        {
            if (places % 2 == 0)
                c->contenu = DECHARGE; // décharge est de type pair
            else
                c->contenu = BOMBE;
            places++;
        }
    }
}
/* Renvoie true si (ligne, colonne) est dans la grille. */
bool estDansGrille(int ligne, int colonne)
{
    return ligne >= 0 && ligne < TAILLE && colonne >= 0 && colonne < TAILLE;
}

/* 1er clic : la case devient visible. */
void revelerCase(Partie *partie, int ligne, int colonne)
{
    partie->grille.cases[ligne][colonne].revelee = true;
}

/* 2e clic : échange le tuyau de la case avec la case swap.
   Renvoie false si c'est impossible (case cachée, remplie, ou malus). */
bool swapperCase(Partie *partie, int ligne, int colonne)
{
    Case *c = &partie->grille.cases[ligne][colonne];

    if (!c->revelee || c->remplie)
        return false;
    if (c->contenu == BOMBE || c->contenu == DECHARGE)
        return false;

    Contenu temporaire = c->contenu; // on met le tuyau de la case de côté
    c->contenu = partie->swap;       // la case reçoit le tuyau du swap
    partie->swap = temporaire;       // le swap reçoit l'ancien tuyau de la case
    return true;
}

/* Recache toutes les cases que le fluide n'a pas remplies.
   Les cases restent à leur place : on ne change que « revelee ». */
void masquerCasesNonRemplies(Partie *partie)
{
    for (int i = 0; i < TAILLE; i++)
    {
        for (int j = 0; j < TAILLE; j++)
        {
            if (!partie->grille.cases[i][j].remplie)
                partie->grille.cases[i][j].revelee = false;
        }
    }
}