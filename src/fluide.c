/*
 * Pipe-Addict : le bon tuyau - Projet C ING3, ECE Paris 2026-2027, groupe DU01
 * Fichier fluide.c : écoulement du fluide case par case, victoire, cul-de-sac, effets des malus
 *
 * Utilisation de l'IA (Claude, Anthropic) dans ce fichier : environ 30 %
 * initialiserFluide et passerCaseSuivante écrites par l'équipe étape par étape, avec les
 *  explications et corrections de Claude (copie au lieu d'un pointeur, « ! » manquant) ;
 *  appliquerMalus et avancerFluide fournies par Claude.
 * Exemples de prompts :
 *  - « je sais pas comment écrire la suite de la fonction passerCaseSuivante, dans le contexte de victoire »
 * Vérification : code relu, compilé et testé par l'équipe (tests en console et parties jouées).
 */

#include "fluide.h"
#include "tuyau.h"
#include "grille.h"

void initialiserFluide(Partie *partie)
{

    Fluide *fluide = &partie->fluide; // pointeur : on modifie le vrai fluide de la partie

    fluide->ligne = partie->ligneEntree;
    fluide->colonne = -1;           // case imaginaire juste à gauche de la colonne 0
    fluide->sortie = DROITE;        // il va entrer dans la grille par la gauche
    partie->etat = EN_COURS;        // l'état est dans Partie, pas dans Fluide
    fluide->attente = DELAI_DEPART; // 5 s avant le départ
    fluide->delaiCase = DELAI_CASE; // puis DELAI_CASE secondes par case
    fluide->dureeAttente = DELAI_DEPART;
}

void passerCaseSuivante(Partie *partie)
{
    Fluide *fluide = &partie->fluide;

    int ligne = fluide->ligne + decalageLigne(fluide->sortie);
    int colonne = fluide->colonne + decalageColonne(fluide->sortie);

    if (ligne == partie->ligneSortie && colonne == TAILLE)
    {
        partie->etat = VICTOIRE;
        return;
    }

    if (!estDansGrille(ligne, colonne))
    {
        partie->etat = DEFAITE;
        return;
    }

    // par quel coté le fluide rentre et par lequel il ressort
    Case *c = &partie->grille.cases[ligne][colonne];
    Direction entree = opposee(partie->fluide.sortie);
    Direction nouvelleSortie = lireConnexion(c->contenu, entree);

    if (c->remplie || nouvelleSortie == AUCUNE)
    {
        c->revelee = true; // on revele la case pour savoir ce qui a bloqué le fluide
        partie->etat = DEFAITE;
        return;
    }

    /*Le fluide entre. La case devient révélée et remplie,
    et le fluide prend la nouvelle ligne, la nouvelle colonne et
    la nouvelle sortie.*/
    c->remplie = true;
    c->revelee = true;
    c->coteEntree = entree; // retenu pour l'affichage
    fluide->ligne = ligne;
    fluide->colonne = colonne;
    fluide->sortie = nouvelleSortie;
}

/* Effet du malus de la case où le fluide vient d'entrer.
   Dans les deux cas, le fluide traverse la case tout droit (c'est la table des connexions). */
static void appliquerMalus(Partie *partie)
{
    Fluide *fluide = &partie->fluide;
    Case *c = &partie->grille.cases[fluide->ligne][fluide->colonne];

    if (c->contenu == DECHARGE) // le fluide accélère : il traverse les cases 2 fois plus vite
    {
        fluide->delaiCase = fluide->delaiCase / 2;
        if (fluide->delaiCase < DELAI_MINIMUM)
            fluide->delaiCase = DELAI_MINIMUM;
        fluide->attente = fluide->delaiCase;
    }
    else if (c->contenu == BOMBE) // explosion : tout se recache, le fluide s'arrête 5 s
    {
        masquerCasesNonRemplies(partie);
        fluide->attente = PAUSE_BOMBE;
    }
}

void avancerFluide(Partie *partie, float dt)
{
    if (partie->etat != EN_COURS) // partie finie : le fluide ne bouge plus
        return;

    Fluide *fluide = &partie->fluide;
    fluide->attente = fluide->attente - dt; // le temps passe

    if (fluide->attente <= 0) // l'attente est finie : une case de plus
    {
        passerCaseSuivante(partie);
        fluide->attente = fluide->delaiCase; // et on recommence à attendre

        if (partie->etat == EN_COURS) // le fluide est bien entré dans une nouvelle case
            appliquerMalus(partie);

        fluide->dureeAttente = fluide->attente; // on retient la durée totale, pour l'animation
    }
}