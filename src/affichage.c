#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "affichage.h"
#include "tuyau.h"

/* Dessine le tuyau d'une case révélée, dont le coin en haut à gauche est (x, y).
   Pour chaque côté où le tuyau a une ouverture, on trace un trait épais
   du centre de la case jusqu'au milieu de ce côté. */
static void dessinerTuyau(const Case *c, float x, float y)
{
    float centreX = x + TAILLE_CASE / 2;
    float centreY = y + TAILLE_CASE / 2;
    float demi = TAILLE_CASE / 2; // distance du centre jusqu'à un côté

    /* Le tuyau est bleu si le fluide y est passé, gris foncé sinon */
    ALLEGRO_COLOR couleur;
    if (c->remplie)
        couleur = al_map_rgb(50, 140, 230);
    else
        couleur = al_map_rgb(80, 80, 95);

    for (Direction d = HAUT; d <= GAUCHE; d++)
    {
        if (lireConnexion(c->contenu, d) != AUCUNE) // ouverture de ce côté ?
        {
            float bordX = centreX + decalageColonne(d) * demi;
            float bordY = centreY + decalageLigne(d) * demi;
            al_draw_line(centreX, centreY, bordX, bordY, couleur, 16);
        }
    }
    al_draw_filled_circle(centreX, centreY, 8, couleur); // arrondit le coude au centre

    /* Les malus : un rond par-dessus la croix */
    if (c->contenu == BOMBE)
        al_draw_filled_circle(centreX, centreY, 16, al_map_rgb(20, 20, 20)); // noir
    else if (c->contenu == DECHARGE)
        al_draw_filled_circle(centreX, centreY, 16, al_map_rgb(240, 200, 40)); // jaune
}

void dessinerGrille(const Partie *partie)
{
    ALLEGRO_COLOR cachee = al_map_rgb(90, 90, 110);    // gris : case pas encore révélée
    ALLEGRO_COLOR revelee = al_map_rgb(235, 225, 200); // beige : case révélée
    ALLEGRO_COLOR bord = al_map_rgb(30, 30, 40);       // trait entre les cases
    ALLEGRO_COLOR repere = al_map_rgb(230, 140, 50);   // orange : entrée et sortie

    for (int i = 0; i < TAILLE; i++)
    {
        for (int j = 0; j < TAILLE; j++)
        {
            /* Coin en haut à gauche de la case (i, j), en pixels */
            float x = GRILLE_X + j * TAILLE_CASE;
            float y = GRILLE_Y + i * TAILLE_CASE;

            const Case *c = &partie->grille.cases[i][j];
            if (c->revelee)
            {
                al_draw_filled_rectangle(x, y, x + TAILLE_CASE, y + TAILLE_CASE, revelee);
                dessinerTuyau(c, x, y);
            }
            else
                al_draw_filled_rectangle(x, y, x + TAILLE_CASE, y + TAILLE_CASE, cachee);

            al_draw_rectangle(x, y, x + TAILLE_CASE, y + TAILLE_CASE, bord, 2);
        }
    }

    /* Entrée : un petit rectangle collé à gauche de la grille, sur la ligne d'entrée */
    float yEntree = GRILLE_Y + partie->ligneEntree * TAILLE_CASE;
    al_draw_filled_rectangle(GRILLE_X - 30, yEntree + 25, GRILLE_X, yEntree + 55, repere);

    /* Sortie : la même chose, à droite de la grille, sur la ligne de sortie */
    float xDroite = GRILLE_X + TAILLE * TAILLE_CASE;
    float ySortie = GRILLE_Y + partie->ligneSortie * TAILLE_CASE;
    al_draw_filled_rectangle(xDroite, ySortie + 25, xDroite + 30, ySortie + 55, repere);
}