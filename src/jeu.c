/*
 * Pipe-Addict : le bon tuyau - Projet C ING3, ECE Paris 2026-2027, groupe DU01
 * Fichier jeu.c : nouvelle partie, Refaire, Terminer, temps limite, compteur de victoires
 *
 * Utilisation de l'IA (Claude, Anthropic) dans ce fichier : environ 70 %
 * Fonctions fournies par Claude à partir de la liste de fonctionnalités de l'équipe.
 * Exemples de prompts :
 *  - « Afficher le temps d'une partie, limiter le temps d'une partie »
 *  - « commençons étape par étape en suivant l'ordre que tu proposes » (boutons Nouveau, Refaire, Terminer)
 * Vérification : code relu, compilé et testé par l'équipe (tests en console et parties jouées).
 */

#include "jeu.h"
#include "grille.h"
#include "fluide.h"

void nouvellePartie(Partie *partie, int niveau)
{
    partie->niveau = niveau; // à mettre avant remplirAuHasard, qui s'en sert pour les malus
    remplirAuHasard(partie);
    partie->grilleDepart = partie->grille; // une structure se copie en entier avec =
    partie->swapDepart = partie->swap;
    initialiserFluide(partie);
    partie->tempsRestant = TEMPS_LIMITE;
}

void refairePartie(Partie *partie)
{
    partie->grille = partie->grilleDepart; // on remet la grille du début, toute cachée
    partie->swap = partie->swapDepart;
    initialiserFluide(partie);
    partie->tempsRestant = TEMPS_LIMITE;
}

void terminerPartie(Partie *partie)
{
    /* Le fluide avance case par case jusqu'à la victoire ou la défaite.
       La limite de 100 pas est une sécurité : le fluide ne peut pas passer plus de 36 cases. */
    int pas = 0;
    while (partie->etat == EN_COURS && pas < 100)
    {
        passerCaseSuivante(partie);
        pas++;
    }
    if (partie->etat == VICTOIRE)
        partie->victoires++;
}

void mettreAJour(Partie *partie, float dt)
{
    if (partie->etat != EN_COURS) // partie finie : plus rien ne bouge
        return;

    partie->tempsRestant = partie->tempsRestant - dt;
    if (partie->tempsRestant <= 0) // temps écoulé : perdu
    {
        partie->tempsRestant = 0;
        partie->etat = DEFAITE;
        return;
    }

    avancerFluide(partie, dt);
    if (partie->etat == VICTOIRE) // la partie vient d'être gagnée
        partie->victoires++;
}