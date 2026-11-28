#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "types.h"

#define FENETRE_LARGEUR 800 // taille de la fenêtre, en pixels
#define FENETRE_HAUTEUR 600
#define TAILLE_CASE 80 // côté d'une case, en pixels
#define GRILLE_X 160   // coin en haut à gauche de la grille : (800 - 6 x 80) / 2 = 160
#define GRILLE_Y 60
#define SWAP_X 680 // la case swap, à droite de la grille
#define SWAP_Y 60

#define BOUTON_X 10 // les boutons, à gauche de la grille
#define BOUTON_LARGEUR 110
#define BOUTON_HAUTEUR 40

/* Les boutons de l'écran. AUCUN_BOUTON : le clic n'est sur aucun bouton. */
typedef enum
{
   AUCUN_BOUTON,
   BOUTON_NOUVEAU,
   BOUTON_REFAIRE,
   BOUTON_TERMINER,
   BOUTON_MENU,
   BOUTON_REGLES
} Bouton;

#define FICHIER_POLICE "assets/fonts/DejaVuSans.ttf"

/* Charge les polices de caractères. Renvoie false si le fichier est introuvable. */
bool initialiserAffichage(void);

/* Libère les polices avant de quitter. */
void libererAffichage(void);

/* Renvoie le bouton qui se trouve sous le pixel (x, y), ou AUCUN_BOUTON. */
Bouton boutonSous(int x, int y);

/* Écran d'accueil : le nom tapé, le niveau choisi (1, 2 ou 3) et le bouton Jouer. */
void dessinerAccueil(const char *nom, int niveau);

/* Écran d'accueil : renvoie le niveau (1, 2 ou 3) dont le bouton est sous (x, y), sinon 0. */
int niveauSous(int x, int y);

/* Écran d'accueil : renvoie true si (x, y) est sur le bouton Jouer. */
bool boutonJouerSous(int x, int y);

/* Écran d'accueil : renvoie true si (x, y) est sur le bouton « Comment jouer ». */
bool boutonReglesSous(int x, int y);

/* Écran « Comment jouer » : les règles, les tuyaux et les malus dessinés, le bouton Retour.
   modeMemoire : affiche aussi la ligne du mode mémoire. */
void dessinerRegles(bool modeMemoire);

/* Écran « Comment jouer » : renvoie true si (x, y) est sur le bouton Retour. */
bool boutonRetourSous(int x, int y);

/* Dessine tout l'écran : grille, case swap, textes, et le message de fin s'il y en a un.
   voirTout : mode test (Espace). pause : le jeu est en pause (P). */
void dessinerJeu(const Partie *partie, bool voirTout, bool pause);

#endif