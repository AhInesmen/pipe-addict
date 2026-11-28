/*
 * Pipe-Addict : le bon tuyau - Projet C ING3, ECE Paris 2026-2027, groupe DU01
 * Fichier affichage.c : dessin de l'accueil, des règles, de la grille, des tuyaux, des textes et des boutons
 *
 * Utilisation de l'IA (Claude, Anthropic) dans ce fichier : environ 80 %
 * Code Allegro fourni et expliqué par Claude ; textes modifiés par l'équipe.
 * Exemples de prompts :
 *  - « passons à la prochaine étape » (fenêtre graphique Allegro)
 *  - « les cases cachées doivent afficher un point d'interrogation »
 *  - « un affichage du menu de comment se déroule le jeu », « Refait le text en utilisant de l'impératif »
 *  - « je pense qu'il faut un temps avant le décompte du fluide dans la case » (remplissage progressif)
 * Vérification : code relu, compilé et testé par l'équipe (tests en console et parties jouées).
 */

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "affichage.h"
#include "tuyau.h"
#include "grille.h" // estDansGrille
#include "fluide.h" // DELAI_DEPART, DELAI_CASE, PAUSE_BOMBE (pour l'écran des règles)
#include "jeu.h"    // TEMPS_LIMITE

/* Les polices sont des variables du module : « static » les rend visibles
   seulement dans affichage.c. On les charge une fois au début, pas à chaque dessin. */
static ALLEGRO_FONT *police = NULL;       // texte normal
static ALLEGRO_FONT *policeGrande = NULL; // « ? » et messages de fin

bool initialiserAffichage(void)
{
    al_init_font_addon();
    al_init_ttf_addon();
    police = al_load_ttf_font(FICHIER_POLICE, 16, 0);
    policeGrande = al_load_ttf_font(FICHIER_POLICE, 40, 0);
    return police != NULL && policeGrande != NULL;
}

void libererAffichage(void)
{
    if (police)
        al_destroy_font(police);
    if (policeGrande)
        al_destroy_font(policeGrande);
}

/* Arrondit un temps en secondes vers le haut : 4,2 s s'affiche « 5 s ». */
static int secondesAffichees(float temps)
{
    return (int)(temps + 0.99f);
}

/* Le haut de chaque bouton, en pixels : un bouton tous les 60 pixels. */
static float boutonY(Bouton bouton)
{
    return 220 + (bouton - BOUTON_NOUVEAU) * 60;
}

Bouton boutonSous(int x, int y)
{
    for (Bouton b = BOUTON_NOUVEAU; b <= BOUTON_REGLES; b++)
    {
        float haut = boutonY(b);
        if (x >= BOUTON_X && x <= BOUTON_X + BOUTON_LARGEUR && y >= haut && y <= haut + BOUTON_HAUTEUR)
            return b;
    }
    return AUCUN_BOUTON;
}

/* Dessine le tuyau d'une case révélée, dont le coin en haut à gauche est (x, y).
   Pour chaque côté où le tuyau a une ouverture, on trace un trait épais
   du centre de la case jusqu'au milieu de ce côté.
   remplissage (de 0 à 1) : la part du chemin déjà parcourue par le fluide dans cette case
   (1 = case entièrement remplie). Ne sert que si la case est remplie. */
static void dessinerTuyau(const Case *c, float x, float y, float remplissage)
{
    float centreX = x + TAILLE_CASE / 2;
    float centreY = y + TAILLE_CASE / 2;
    float demi = TAILLE_CASE / 2; // distance du centre jusqu'à un côté

    ALLEGRO_COLOR gris = al_map_rgb(80, 80, 95);
    ALLEGRO_COLOR bleu = al_map_rgb(50, 140, 230);

    /* 1. Le tuyau vide, en gris */
    for (Direction d = HAUT; d <= GAUCHE; d++)
    {
        if (lireConnexion(c->contenu, d) != AUCUNE) // ouverture de ce côté ?
        {
            float bordX = centreX + decalageColonne(d) * demi;
            float bordY = centreY + decalageLigne(d) * demi;
            al_draw_line(centreX, centreY, bordX, bordY, gris, 16);
        }
    }
    al_draw_filled_circle(centreX, centreY, 8, gris); // arrondit le coude au centre

    /* 2. Le fluide, en bleu, par-dessus : il entre par un côté, rejoint le centre
          (première moitié du remplissage), puis repart vers la sortie (seconde moitié). */
    if (c->remplie)
    {
        Direction entree = c->coteEntree;
        Direction sortie = lireConnexion(c->contenu, entree);

        float partEntree = remplissage < 0.5f ? remplissage * 2 : 1; // de 0 à 1
        float bordX = centreX + decalageColonne(entree) * demi;
        float bordY = centreY + decalageLigne(entree) * demi;
        al_draw_line(bordX, bordY, bordX + (centreX - bordX) * partEntree, bordY + (centreY - bordY) * partEntree,
                     bleu, 16);

        if (remplissage >= 0.5f)
        {
            float partSortie = (remplissage - 0.5f) * 2; // de 0 à 1
            al_draw_filled_circle(centreX, centreY, 8, bleu);
            al_draw_line(centreX, centreY, centreX + decalageColonne(sortie) * demi * partSortie,
                         centreY + decalageLigne(sortie) * demi * partSortie, bleu, 16);
        }
    }

    /* Les malus : un rond par-dessus la croix */
    if (c->contenu == BOMBE)
        al_draw_filled_circle(centreX, centreY, 16, al_map_rgb(20, 20, 20)); // noir
    else if (c->contenu == DECHARGE)
        al_draw_filled_circle(centreX, centreY, 16, al_map_rgb(240, 200, 40)); // jaune
}

/* La part de la case (i, j) que le fluide a déjà remplie, de 0 à 1.
   Seule la case où se trouve le fluide se remplit petit à petit ; les autres cases remplies sont pleines. */
static float remplissageCase(const Partie *partie, int i, int j)
{
    const Fluide *fluide = &partie->fluide;
    if (partie->etat != EN_COURS || i != fluide->ligne || j != fluide->colonne)
        return 1;

    float r = 1 - fluide->attente / fluide->dureeAttente; // 0 quand il arrive, 1 quand il repart
    if (r < 0)
        r = 0;
    if (r > 1)
        r = 1;
    return r;
}

/* La case où le fluide aurait dû entrer ensuite : en cas de défaite, c'est elle qui l'a bloqué.
   Les deux résultats sont rendus par pointeurs. */
static void caseSuivante(const Partie *partie, int *ligne, int *colonne)
{
    *ligne = partie->fluide.ligne + decalageLigne(partie->fluide.sortie);
    *colonne = partie->fluide.colonne + decalageColonne(partie->fluide.sortie);
}

/* Défaite (hors temps écoulé) : on montre en rouge ce qui a bloqué le fluide,
   la case suivante si elle est dans la grille, sinon le bord de la grille qu'il a touché. */
static void dessinerBlocage(const Partie *partie)
{
    if (partie->etat != DEFAITE || partie->tempsRestant <= 0)
        return;

    ALLEGRO_COLOR rouge = al_map_rgb(230, 60, 60);
    int ligne, colonne;
    caseSuivante(partie, &ligne, &colonne);

    if (estDansGrille(ligne, colonne)) // la case suivante bloque : on l'encadre
    {
        float x = GRILLE_X + colonne * TAILLE_CASE;
        float y = GRILLE_Y + ligne * TAILLE_CASE;
        al_draw_rectangle(x + 4, y + 4, x + TAILLE_CASE - 4, y + TAILLE_CASE - 4, rouge, 6);
    }
    else // le fluide sort par le bord : un trait rouge sur ce côté de sa dernière case
    {
        float x = GRILLE_X + partie->fluide.colonne * TAILLE_CASE;
        float y = GRILLE_Y + partie->fluide.ligne * TAILLE_CASE;
        Direction d = partie->fluide.sortie;
        float x1 = (d == DROITE) ? x + TAILLE_CASE : x;
        float y1 = (d == BAS) ? y + TAILLE_CASE : y;
        float x2 = (d == GAUCHE) ? x : x + TAILLE_CASE;
        float y2 = (d == HAUT) ? y : y + TAILLE_CASE;
        al_draw_line(x1, y1, x2, y2, rouge, 10);
    }
}

/* Les 36 cases, l'entrée et la sortie. */
static void dessinerGrille(const Partie *partie, bool voirTout)
{
    ALLEGRO_COLOR cachee = al_map_rgb(90, 90, 110);    // gris : case pas encore révélée
    ALLEGRO_COLOR revelee = al_map_rgb(235, 225, 200); // beige : case révélée
    ALLEGRO_COLOR bord = al_map_rgb(30, 30, 40);       // trait entre les cases
    ALLEGRO_COLOR repere = al_map_rgb(230, 140, 50);   // orange : entrée et sortie
    ALLEGRO_COLOR pointInterrogation = al_map_rgb(200, 200, 215);

    for (int i = 0; i < TAILLE; i++)
    {
        for (int j = 0; j < TAILLE; j++)
        {
            /* Coin en haut à gauche de la case (i, j), en pixels */
            float x = GRILLE_X + j * TAILLE_CASE;
            float y = GRILLE_Y + i * TAILLE_CASE;

            const Case *c = &partie->grille.cases[i][j];
            if (c->revelee || voirTout) // en mode test, on montre tout
            {
                al_draw_filled_rectangle(x, y, x + TAILLE_CASE, y + TAILLE_CASE, revelee);
                dessinerTuyau(c, x, y, remplissageCase(partie, i, j));
            }
            else
            {
                al_draw_filled_rectangle(x, y, x + TAILLE_CASE, y + TAILLE_CASE, cachee);
                /* Le « ? », centré dans la case : on remonte de la moitié de la hauteur du texte */
                float hauteur = al_get_font_line_height(policeGrande);
                al_draw_text(policeGrande, pointInterrogation, x + TAILLE_CASE / 2,
                             y + (TAILLE_CASE - hauteur) / 2, ALLEGRO_ALIGN_CENTRE, "?");
            }

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

    dessinerBlocage(partie);
}

/* La case swap, encadrée en orange, avec son titre au-dessus. */
static void dessinerSwap(const Partie *partie)
{
    /* Une case « pour l'affichage » : le contenu de la case swap, révélée, pas remplie, sans entrée */
    Case swap = {partie->swap, true, false, AUCUNE};

    al_draw_text(police, al_map_rgb(230, 230, 240), SWAP_X + TAILLE_CASE / 2, SWAP_Y - 24,
                 ALLEGRO_ALIGN_CENTRE, "Swap");
    al_draw_filled_rectangle(SWAP_X, SWAP_Y, SWAP_X + TAILLE_CASE, SWAP_Y + TAILLE_CASE,
                             al_map_rgb(235, 225, 200));
    dessinerTuyau(&swap, SWAP_X, SWAP_Y, 1);
    al_draw_rectangle(SWAP_X, SWAP_Y, SWAP_X + TAILLE_CASE, SWAP_Y + TAILLE_CASE,
                      al_map_rgb(230, 140, 50), 4);
}

/* En haut : le temps restant à gauche, l'état du fluide à droite.
   En bas : le rappel des commandes. */
static void dessinerTextes(const Partie *partie, bool pause)
{
    ALLEGRO_COLOR blanc = al_map_rgb(230, 230, 240);
    ALLEGRO_COLOR rouge = al_map_rgb(240, 90, 90);
    ALLEGRO_COLOR jaune = al_map_rgb(240, 200, 40);
    char texte[64];

    /* Le temps restant, en rouge dans les 10 dernières secondes */
    int temps = secondesAffichees(partie->tempsRestant);
    snprintf(texte, sizeof texte, "Temps restant : %d s", temps);
    al_draw_text(police, temps <= 10 ? rouge : blanc, GRILLE_X, 20, ALLEGRO_ALIGN_LEFT, texte);

    /* L'état du fluide */
    if (partie->etat == EN_COURS)
    {
        if (pause)
            al_draw_text(police, jaune, GRILLE_X + TAILLE * TAILLE_CASE, 20, ALLEGRO_ALIGN_RIGHT,
                         "PAUSE (P pour reprendre)");
        else if (partie->fluide.colonne == -1) // le fluide n'est pas encore entré dans la grille
        {
            snprintf(texte, sizeof texte, "Départ du fluide dans %d s",
                     secondesAffichees(partie->fluide.attente));
            al_draw_text(police, jaune, GRILLE_X + TAILLE * TAILLE_CASE, 20, ALLEGRO_ALIGN_RIGHT, texte);
        }
        else
        {
            /* Le fluide est dans la grille : on regarde la case où il se trouve */
            Contenu ici = partie->grille.cases[partie->fluide.ligne][partie->fluide.colonne].contenu;
            if (ici == BOMBE)
            {
                snprintf(texte, sizeof texte, "Bombe ! Le fluide repart dans %d s",
                         secondesAffichees(partie->fluide.attente));
                al_draw_text(police, rouge, GRILLE_X + TAILLE * TAILLE_CASE, 20, ALLEGRO_ALIGN_RIGHT, texte);
            }
            else if (ici == DECHARGE)
                al_draw_text(police, jaune, GRILLE_X + TAILLE * TAILLE_CASE, 20, ALLEGRO_ALIGN_RIGHT,
                             "Décharge : le fluide accélère !");
        }
    }

    al_draw_text(police, al_map_rgb(150, 150, 165), FENETRE_LARGEUR / 2, FENETRE_HAUTEUR - 36, ALLEGRO_ALIGN_CENTRE,
                 "Clic : révéler, puis échanger   |   Espace : tout voir   |   P : pause");
}

/* Un bouton : un rectangle arrondi avec son texte au milieu.
   actif = false : le bouton est grisé (il ne sert à rien pour l'instant). */
static void dessinerBouton(Bouton bouton, const char *texte, bool actif)
{
    float haut = boutonY(bouton);
    ALLEGRO_COLOR fond = actif ? al_map_rgb(60, 60, 80) : al_map_rgb(45, 45, 55);
    ALLEGRO_COLOR couleurTexte = actif ? al_map_rgb(240, 240, 250) : al_map_rgb(110, 110, 120);

    al_draw_filled_rounded_rectangle(BOUTON_X, haut, BOUTON_X + BOUTON_LARGEUR, haut + BOUTON_HAUTEUR,
                                     8, 8, fond);
    if (actif)
        al_draw_rounded_rectangle(BOUTON_X, haut, BOUTON_X + BOUTON_LARGEUR, haut + BOUTON_HAUTEUR,
                                  8, 8, al_map_rgb(230, 140, 50), 2);
    float hauteurTexte = al_get_font_line_height(police);
    al_draw_text(police, couleurTexte, BOUTON_X + BOUTON_LARGEUR / 2,
                 haut + (BOUTON_HAUTEUR - hauteurTexte) / 2, ALLEGRO_ALIGN_CENTRE, texte);
}

/* La colonne de gauche : les compteurs, puis les boutons. */
static void dessinerColonneGauche(const Partie *partie)
{
    ALLEGRO_COLOR blanc = al_map_rgb(230, 230, 240);
    char texte[64];

    al_draw_text(police, al_map_rgb(230, 140, 50), BOUTON_X, 70, ALLEGRO_ALIGN_LEFT, partie->joueur);
    snprintf(texte, sizeof texte, "Niveau %d", partie->niveau);
    al_draw_text(police, blanc, BOUTON_X, 94, ALLEGRO_ALIGN_LEFT, texte);
    snprintf(texte, sizeof texte, "Partie n° %d", partie->numeroPartie);
    al_draw_text(police, blanc, BOUTON_X, 118, ALLEGRO_ALIGN_LEFT, texte);
    snprintf(texte, sizeof texte, "Gagnées : %d", partie->victoires);
    al_draw_text(police, blanc, BOUTON_X, 142, ALLEGRO_ALIGN_LEFT, texte);

    dessinerBouton(BOUTON_NOUVEAU, "Nouveau", true);
    dessinerBouton(BOUTON_REFAIRE, "Refaire", true);
    dessinerBouton(BOUTON_TERMINER, "Terminer", partie->etat == EN_COURS);
    dessinerBouton(BOUTON_MENU, "Menu", true);
    dessinerBouton(BOUTON_REGLES, "Règles", true);
}

/* Le message de fin, dans une bande SOUS la grille : toute la grille reste visible,
   avec en rouge ce qui a bloqué le fluide. */
static void dessinerMessageFin(const Partie *partie)
{
    if (partie->etat == EN_COURS)
        return;

    const char *titre;
    const char *raison;
    ALLEGRO_COLOR couleur;
    if (partie->etat == VICTOIRE)
    {
        titre = "Victoire !";
        raison = "Le fluide a atteint la sortie.";
        couleur = al_map_rgb(90, 210, 120);
    }
    else
    {
        titre = "Perdu...";
        couleur = al_map_rgb(240, 90, 90);
        int ligne, colonne;
        caseSuivante(partie, &ligne, &colonne);
        if (partie->tempsRestant <= 0)
            raison = "Le temps est écoulé.";
        else if (!estDansGrille(ligne, colonne))
            raison = "Le fluide est sorti par le bord.";
        else if (partie->grille.cases[ligne][colonne].remplie)
            raison = "Le fluide est repassé par une case remplie.";
        else
            raison = "Cul-de-sac : voir la case en rouge.";
    }

    /* La bande : de la gauche de la grille jusqu'au bord droit de la fenêtre */
    float haut = GRILLE_Y + TAILLE * TAILLE_CASE + 8;
    al_draw_filled_rounded_rectangle(GRILLE_X - 10, haut, FENETRE_LARGEUR - 30, haut + 64, 10, 10,
                                     al_map_rgb(45, 45, 60));
    al_draw_rounded_rectangle(GRILLE_X - 10, haut, FENETRE_LARGEUR - 30, haut + 64, 10, 10, couleur, 2);

    /* Le titre en grand à gauche, puis la raison et la consigne à sa droite */
    al_draw_text(policeGrande, couleur, GRILLE_X + 10, haut + 8, ALLEGRO_ALIGN_LEFT, titre);
    float xTexte = GRILLE_X + 10 + al_get_text_width(policeGrande, titre) + 24;
    al_draw_text(police, al_map_rgb(230, 230, 240), xTexte, haut + 10, ALLEGRO_ALIGN_LEFT, raison);
    al_draw_text(police, al_map_rgb(150, 150, 165), xTexte, haut + 36, ALLEGRO_ALIGN_LEFT,
                 "Nouveau ou Refaire pour rejouer");
}

void dessinerJeu(const Partie *partie, bool voirTout, bool pause)
{
    al_clear_to_color(al_map_rgb(30, 30, 40)); // fond gris foncé
    dessinerGrille(partie, voirTout);
    dessinerSwap(partie);
    dessinerColonneGauche(partie);
    dessinerTextes(partie, pause);
    dessinerMessageFin(partie);
}

/* ---------- Écran d'accueil ---------- */

#define NIVEAU_Y 350 // haut des trois boutons de niveau
#define NIVEAU_LARGEUR 110
#define JOUER_Y 450  // haut du bouton Jouer
#define REGLES_Y 500 // haut du bouton « Comment jouer »
#define RETOUR_Y 530 // haut du bouton Retour, sur l'écran des règles
#define JOUER_LARGEUR 160

/* Le bord gauche du bouton du niveau n (1, 2 ou 3) : trois boutons centrés, espacés de 10 pixels. */
static float niveauX(int n)
{
    return FENETRE_LARGEUR / 2 - 1.5f * NIVEAU_LARGEUR - 10 + (n - 1) * (NIVEAU_LARGEUR + 10);
}

int niveauSous(int x, int y)
{
    for (int n = 1; n <= 3; n++)
    {
        if (x >= niveauX(n) && x <= niveauX(n) + NIVEAU_LARGEUR && y >= NIVEAU_Y && y <= NIVEAU_Y + BOUTON_HAUTEUR)
            return n;
    }
    return 0;
}

/* true si (x, y) est dans un bouton centré horizontalement, dont le haut est à « haut ». */
static bool dansBoutonCentre(int x, int y, float haut)
{
    float gauche = FENETRE_LARGEUR / 2 - JOUER_LARGEUR / 2;
    return x >= gauche && x <= gauche + JOUER_LARGEUR && y >= haut && y <= haut + BOUTON_HAUTEUR;
}

/* Un bouton centré horizontalement, avec son texte (même style que Jouer). */
static void dessinerBoutonCentre(float haut, const char *texte, bool actif)
{
    float gauche = FENETRE_LARGEUR / 2 - JOUER_LARGEUR / 2;
    al_draw_filled_rounded_rectangle(gauche, haut, gauche + JOUER_LARGEUR, haut + BOUTON_HAUTEUR, 8, 8,
                                     actif ? al_map_rgb(60, 60, 80) : al_map_rgb(45, 45, 55));
    if (actif)
        al_draw_rounded_rectangle(gauche, haut, gauche + JOUER_LARGEUR, haut + BOUTON_HAUTEUR, 8, 8,
                                  al_map_rgb(230, 140, 50), 2);
    al_draw_text(police, actif ? al_map_rgb(230, 230, 240) : al_map_rgb(110, 110, 120), FENETRE_LARGEUR / 2,
                 haut + 10, ALLEGRO_ALIGN_CENTRE, texte);
}

bool boutonJouerSous(int x, int y)
{
    return dansBoutonCentre(x, y, JOUER_Y);
}

bool boutonReglesSous(int x, int y)
{
    return dansBoutonCentre(x, y, REGLES_Y);
}

bool boutonRetourSous(int x, int y)
{
    return dansBoutonCentre(x, y, RETOUR_Y);
}

void dessinerAccueil(const char *nom, int niveau)
{
    ALLEGRO_COLOR orange = al_map_rgb(230, 140, 50);
    ALLEGRO_COLOR blanc = al_map_rgb(230, 230, 240);
    ALLEGRO_COLOR grisClair = al_map_rgb(150, 150, 165);
    float centreX = FENETRE_LARGEUR / 2;
    char texte[64];

    al_clear_to_color(al_map_rgb(30, 30, 40));
    al_draw_text(policeGrande, orange, centreX, 70, ALLEGRO_ALIGN_CENTRE, "Pipe-Addict");
    al_draw_text(police, grisClair, centreX, 125, ALLEGRO_ALIGN_CENTRE, "Le bon tuyau");

    /* Le nom : une zone de texte avec un « _ » qui montre où s'écrit la prochaine lettre */
    al_draw_text(police, blanc, centreX, 200, ALLEGRO_ALIGN_CENTRE, "Pseudo :");
    al_draw_filled_rounded_rectangle(centreX - 150, 228, centreX + 150, 272, 8, 8, al_map_rgb(235, 225, 200));
    snprintf(texte, sizeof texte, "%s_", nom);
    al_draw_text(police, al_map_rgb(30, 30, 40), centreX, 240, ALLEGRO_ALIGN_CENTRE, texte);

    /* Les trois niveaux : le niveau choisi est en orange */
    al_draw_text(police, blanc, centreX, 318, ALLEGRO_ALIGN_CENTRE, "Niveau :");
    const char *noms[3] = {"Facile", "Moyen", "Difficile"};
    for (int n = 1; n <= 3; n++)
    {
        float x = niveauX(n);
        bool choisi = (n == niveau);
        al_draw_filled_rounded_rectangle(x, NIVEAU_Y, x + NIVEAU_LARGEUR, NIVEAU_Y + BOUTON_HAUTEUR, 8, 8,
                                         choisi ? orange : al_map_rgb(60, 60, 80));
        al_draw_text(police, choisi ? al_map_rgb(30, 30, 40) : blanc, x + NIVEAU_LARGEUR / 2, NIVEAU_Y + 10,
                     ALLEGRO_ALIGN_CENTRE, noms[n - 1]);
    }
    snprintf(texte, sizeof texte, "%d malus sur la grille", 2 * niveau);
    al_draw_text(police, grisClair, centreX, NIVEAU_Y + 50, ALLEGRO_ALIGN_CENTRE, texte);

    /* Le bouton Jouer, grisé tant que le nom est vide, puis le bouton des règles */
    dessinerBoutonCentre(JOUER_Y, "Jouer", nom[0] != '\0');
    dessinerBoutonCentre(REGLES_Y, "Comment jouer", true);

    al_draw_text(police, grisClair, centreX, FENETRE_HAUTEUR - 40, ALLEGRO_ALIGN_CENTRE,
                 "Écris ton pseudo, choisis un niveau, puis clique sur Jouer ou appuie sur Entrée");
}

/* ---------- Écran « Comment jouer » ---------- */

void dessinerRegles(bool modeMemoire)
{
    ALLEGRO_COLOR orange = al_map_rgb(230, 140, 50);
    ALLEGRO_COLOR blanc = al_map_rgb(230, 230, 240);
    ALLEGRO_COLOR grisClair = al_map_rgb(150, 150, 165);
    char texte[128];

    al_clear_to_color(al_map_rgb(30, 30, 40));
    al_draw_text(policeGrande, orange, FENETRE_LARGEUR / 2, 15, ALLEGRO_ALIGN_CENTRE, "Comment jouer");

    /* Les 8 contenus possibles d'une case, dessinés côte à côte avec dessinerTuyau */
    float largeurRangee = 8 * TAILLE_CASE + 7 * 5; // 8 cases espacées de 5 pixels
    float x0 = (FENETRE_LARGEUR - largeurRangee) / 2;
    for (Contenu contenu = COUDE_HAUT_DROITE; contenu <= DECHARGE; contenu++)
    {
        float x = x0 + contenu * (TAILLE_CASE + 5);
        Case exemple = {contenu, true, false, AUCUNE};
        al_draw_filled_rectangle(x, 75, x + TAILLE_CASE, 75 + TAILLE_CASE, al_map_rgb(235, 225, 200));
        dessinerTuyau(&exemple, x, 75, 1);
    }
    al_draw_text(police, grisClair, FENETRE_LARGEUR / 2, 160, ALLEGRO_ALIGN_CENTRE,
                 "Repère les 6 tuyaux, la bombe (noire) et la décharge (jaune).");

    /* Les règles, une ligne après l'autre : y avance de 22 pixels à chaque ligne */
    float x = 40;
    float y = 195;
    al_draw_text(police, orange, x, y, 0, "Relie l'entrée (à gauche) à la sortie (à droite) avant que le fluide ne t'échappe !");
    y += 30;
    al_draw_text(police, blanc, x, y, 0, "- Clique sur une case « ? » pour la révéler.");
    y += 22;
    al_draw_text(police, blanc, x, y, 0, "- Clique encore dessus pour l'échanger avec la case Swap. Les malus ne bougent pas !");
    y += 22;
    snprintf(texte, sizeof texte, "- Fais vite : le fluide part dans %d s, puis avance d'une case toutes les %d s.",
             (int)DELAI_DEPART, (int)DELAI_CASE);
    al_draw_text(police, blanc, x, y, 0, texte);
    y += 22;
    al_draw_text(police, blanc, x, y, 0, "- Suis les tuyaux : le fluide entre par une ouverture et ressort par l'autre.");
    y += 22;
    al_draw_text(police, blanc, x, y, 0, "- Méfie-toi des décharges : le fluide les traverse tout droit et file 2 fois plus vite.");
    y += 22;
    snprintf(texte, sizeof texte, "- Évite les bombes : elles recachent les cases non remplies et figent le fluide %d s.",
             (int)PAUSE_BOMBE);
    al_draw_text(police, blanc, x, y, 0, texte);
    y += 22;
    if (modeMemoire)
    {
        al_draw_text(police, blanc, x, y, 0, "- Mémorise bien : dès que tu révèles une case, les autres se recachent.");
        y += 22;
    }
    y += 8;
    al_draw_text(police, orange, x, y, 0, "Amène le fluide jusqu'à la sortie pour gagner !");
    y += 22;
    snprintf(texte, sizeof texte, "Gare aux culs-de-sac, aux bords de la grille et au chrono de %d s.",
             (int)TEMPS_LIMITE);
    al_draw_text(police, orange, x, y, 0, texte);
    y += 30;
    al_draw_text(police, grisClair, x, y, 0, "Utilise les boutons Nouveau, Refaire, Terminer, Menu et Règles.");
    y += 22;
    al_draw_text(police, grisClair, x, y, 0, "Appuie sur Espace pour tout voir, sur P pour faire une pause.");
    y += 22;
    al_draw_text(police, grisClair, x, y, 0, "Pendant une partie, le chrono s'arrête tant que tu lis les règles.");

    dessinerBoutonCentre(RETOUR_Y, "Retour", true);
    al_draw_text(police, grisClair, FENETRE_LARGEUR / 2, 575, ALLEGRO_ALIGN_CENTRE, "ou appuie sur Échap");
}