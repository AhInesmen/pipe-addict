#include <stdio.h>
#include "types.h"
#include <stdlib.h>
#include <time.h>
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

void afficherGrille(const Partie *partie)
{
    const char lettres[] = "1234HVBD"; // premieres lettres des types de contenu des cases

    for (int i = 0; i < TAILLE; i++)
    {
        if (i == partie->ligneEntree) // caseEntree
            printf("> ");
        else
            printf("  ");

        for (int j = 0; j < TAILLE; j++)
        {
            printf("%c ", lettres[partie->grille.cases[i][j].contenu]);
        }

        if (i == partie->ligneSortie) // caseSortie
            printf("<");
        printf("\n");
    }
    printf("Case swap : %c\n", lettres[partie->swap]);
}

int main(void)
{
    srand(time(NULL));

    Partie partie;
    partie.niveau = 2;
    remplirAuHasard(&partie);
    afficherGrille(&partie);
    return 0;
}
