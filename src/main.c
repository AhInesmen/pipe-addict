#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "types.h"
#include "grille.h"
#include "fluide.h"

void afficherGrille(const Partie *partie)
{
    const char lettres[] = "1234HVBD"; // premieres lettres des types de contenu des cases
    printf("     1 2 3 4 5 6\n");      // numéros de colonne
    for (int i = 0; i < TAILLE; i++)
    {
        printf("%d ", i + 1);         // numéro de ligne
        if (i == partie->ligneEntree) // caseEntree
            printf("E> ");
        else
            printf("   ");

        for (int j = 0; j < TAILLE; j++)
        {
            if (partie->grille.cases[i][j].revelee == false)
            {
                printf("? ");
            }
            else
            {
                printf("%c ", lettres[partie->grille.cases[i][j].contenu]);
            }
        }

        if (i == partie->ligneSortie) // caseSortie
            printf(">S");
        printf("\n");
    }
    printf("Case swap : %c\n", lettres[partie->swap]);
}

/* Attend que le joueur appuie sur Entrée : lit les caractères un par un
   jusqu'au retour à la ligne (ou la fin du fichier). */
void attendreEntree(void)
{
    int caractere;
    while ((caractere = getchar()) != '\n' && caractere != EOF)
        ;
}

int main(void)
{
    srand(time(NULL));

    Partie partie;
    partie.niveau = 2;
    remplirAuHasard(&partie);

    /* Phase 1 : le joueur révèle et échange des cases */
    int ligne, colonne;
    while (true)
    {
        afficherGrille(&partie);
        printf("Ligne et colonne (1 a 6), ou 0 0 pour lancer le fluide : ");
        if (scanf("%d %d", &ligne, &colonne) != 2 || ligne == 0)
            break;

        ligne--; // le joueur tape de 1 à 6, le tableau va de 0 à 5
        colonne--;

        if (!estDansGrille(ligne, colonne))
        {
            printf("Cette case n'est pas dans la grille.\n");
            continue;
        }

        if (partie.grille.cases[ligne][colonne].revelee == false)
            revelerCase(&partie, ligne, colonne);
        else if (!swapperCase(&partie, ligne, colonne))
            printf("Echange impossible : c'est un malus.\n");
    }
    /* Phase 2 : le fluide coule, une case à chaque appui sur Entrée */
    initialiserFluide(&partie);
    attendreEntree(); // vide le reste de la ligne tapée au scanf (la touche Entrée)
    printf("Le fluide part ! Appuie sur Entree pour le faire avancer.\n");
    while (partie.etat == EN_COURS)
    {
        attendreEntree();
        passerCaseSuivante(&partie);
        afficherGrille(&partie);
    }

    if (partie.etat == VICTOIRE)
        printf("Victoire ! Le fluide a atteint la sortie.\n");
    else
        printf("Defaite : le fluide est bloque.\n");
    return 0;
}