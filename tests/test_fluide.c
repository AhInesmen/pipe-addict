/*
 * Pipe-Addict : le bon tuyau - Projet C ING3, ECE Paris 2026-2027, groupe DU01
 * Fichier test_fluide.c : tests de l'écoulement du fluide sur des grilles écrites à la main
 *
 * Utilisation de l'IA (Claude, Anthropic) dans ce fichier : environ 100 %
 * Tests écrits par Claude, lancés et vérifiés par l'équipe.
 * Exemples de prompts :
 *  - « guide moi pour l'étape 6 » (tests proposés pour vérifier passerCaseSuivante)
 * Vérification : code relu, compilé et testé par l'équipe (tests en console et parties jouées).
 */

/*
 * Tests du fluide, en console, sans main.c ni Allegro.
 * On écrit une grille à la main, on fait couler le fluide et on vérifie le résultat.
 *
 * Compiler et lancer (depuis le dossier pipe-addict) :
 *   gcc -Wall -Wextra -Isrc tests/test_fluide.c src/fluide.c src/grille.c src/tuyau.c -o test_fluide && ./test_fluide
 */
#include <stdio.h>
#include "types.h"
#include "fluide.h"

static int echecs = 0;

/* Affiche OK ou ERREUR devant le message, et compte les erreurs. */
static void verifier(int condition, const char *message, int cases)
{
    printf("%s  %s (%d cases traversées)\n", condition ? "OK    " : "ERREUR", message, cases);
    if (!condition)
        echecs++;
}

/* Remplit toute la grille avec le même tuyau, cases cachées et vides. */
static void remplirAvec(Partie *partie, Contenu contenu)
{
    for (int i = 0; i < TAILLE; i++)
        for (int j = 0; j < TAILLE; j++)
        {
            partie->grille.cases[i][j].contenu = contenu;
            partie->grille.cases[i][j].revelee = false;
            partie->grille.cases[i][j].remplie = false;
        }
}

/* Fait couler le fluide jusqu'à la fin et renvoie le nombre de cases traversées.
   S'arrête de force après 100 tours, au cas où le fluide ne finirait jamais. */
static int faireCouler(Partie *partie)
{
    int cases = 0;
    initialiserFluide(partie);
    while (partie->etat == EN_COURS && cases < 100)
    {
        passerCaseSuivante(partie);
        if (partie->etat == EN_COURS)
            cases++;
    }
    return cases;
}

int main(void)
{
    Partie p;
    int n;

    /* Test 1 : une ligne droite de tuyaux horizontaux, entrée et sortie sur la même ligne */
    remplirAvec(&p, HORIZONTAL);
    p.ligneEntree = 2;
    p.ligneSortie = 2;
    n = faireCouler(&p);
    verifier(p.etat == VICTOIRE && n == 6, "ligne droite : victoire après 6 cases", n);

    /* Test 2 : même chose, mais la sortie est sur une autre ligne */
    remplirAvec(&p, HORIZONTAL);
    p.ligneEntree = 2;
    p.ligneSortie = 4;
    n = faireCouler(&p);
    verifier(p.etat == DEFAITE && n == 6, "sortie sur une autre ligne : défaite au bord", n);

    /* Test 3 : un tuyau vertical sur le chemin, colonne 4 : cul-de-sac */
    remplirAvec(&p, HORIZONTAL);
    p.ligneEntree = 2;
    p.ligneSortie = 2;
    p.grille.cases[2][3].contenu = VERTICAL;
    n = faireCouler(&p);
    verifier(p.etat == DEFAITE && n == 3, "tuyau vertical : défaite après 3 cases", n);
    verifier(p.grille.cases[2][3].revelee && !p.grille.cases[2][3].remplie,
             "la case qui bloque est révélée mais pas remplie", n);

    /* Test 4 : un coude qui fait descendre, puis un autre qui fait repartir à droite */
    remplirAvec(&p, HORIZONTAL);
    p.ligneEntree = 0;
    p.ligneSortie = 1;
    p.grille.cases[0][2].contenu = COUDE_GAUCHE_BAS;  // entre par la gauche, sort en bas
    p.grille.cases[1][2].contenu = COUDE_HAUT_DROITE; // entre par le haut, sort à droite
    n = faireCouler(&p);
    verifier(p.etat == VICTOIRE && n == 7, "deux coudes : le fluide change de ligne et gagne", n);

    /* Test 5 : une bombe sur le chemin : le fluide passe tout droit */
    remplirAvec(&p, HORIZONTAL);
    p.ligneEntree = 3;
    p.ligneSortie = 3;
    p.grille.cases[3][1].contenu = BOMBE;
    n = faireCouler(&p);
    verifier(p.etat == VICTOIRE && n == 6, "bombe sur le chemin : le fluide la traverse", n);

    /* Test 6 : le fluide fait une boucle et revient dans la bombe, déjà remplie */
    remplirAvec(&p, HORIZONTAL);
    p.ligneEntree = 2;
    p.ligneSortie = 2;
    p.grille.cases[2][1].contenu = BOMBE;             // traversée de gauche à droite
    p.grille.cases[2][2].contenu = COUDE_GAUCHE_BAS;  // descend
    p.grille.cases[3][2].contenu = COUDE_HAUT_GAUCHE; // part à gauche
    p.grille.cases[3][1].contenu = COUDE_HAUT_DROITE; // remonte vers la bombe
    n = faireCouler(&p);
    verifier(p.etat == DEFAITE && n == 5, "boucle : retour dans une case déjà remplie, défaite", n);

    printf("\n%s\n", echecs == 0 ? "Tous les tests sont réussis." : "Des tests ont échoué.");
    return echecs == 0 ? 0 : 1;
}