#include "fluide.h"
#include "tuyau.h"
#include "grille.h"

void initialiserFluide(Partie *partie)
{

    Fluide *fluide = &partie->fluide; // pointeur : on modifie le vrai fluide de la partie

    fluide->ligne = partie->ligneEntree;
    fluide->colonne = -1;    // case imaginaire juste à gauche de la colonne 0
    fluide->sortie = DROITE; // il va entrer dans la grille par la gauche
    partie->etat = EN_COURS; // l'état est dans Partie, pas dans Fluide
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
    fluide->ligne = ligne;
    fluide->colonne = colonne;
    fluide->sortie = nouvelleSortie;
}