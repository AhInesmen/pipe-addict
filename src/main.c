/*
 * Pipe-Addict : le bon tuyau - Projet C ING3, ECE Paris 2026-2027, groupe DU01
 * Fichier main.c : fenêtre Allegro, chronomètre et boucle d'événements (souris, clavier, chrono)
 *
 * Utilisation de l'IA (Claude, Anthropic) dans ce fichier : environ 50 %
 * Boucle d'événements fournie et expliquée par Claude ; elle reprend la logique de la
 *  version console écrite par l'équipe (révéler, échanger, faire couler le fluide).
 * Exemples de prompts :
 *  - « Demande l'utilisateur son nom »
 *  - « je veux rajouter la fonctionnalité de pouvoir revoir les règles du jeu pendant la partie »
 *  - « refaisons la dernière étape pour pouvoir utiliser le projet sur windows »
 * Vérification : code relu, compilé et testé par l'équipe (tests en console et parties jouées).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "types.h"
#include "grille.h"
#include "jeu.h"
#include "affichage.h"

#define FPS 60.0f // le chronomètre fait 60 tics par seconde

/* Idée 5 : quand on révèle une case, les autres cases non remplies se recachent.
   Mettre false pour revenir à la règle du sujet (les cases révélées restent visibles). */
#define MODE_MEMOIRE true

int main(void)
{
    srand(time(NULL));

    /* 1. Démarrer Allegro et ses modules */
    if (!al_init())
    {
        fprintf(stderr, "Allegro n'a pas pu demarrer.\n");
        return 1;
    }
    /* La police est cherchée dans assets/fonts/, à partir du dossier courant (CodeBlocks,
       terminal lancé depuis pipe-addict). Si elle n'y est pas (double-clic sous Windows,
       lancement depuis un autre dossier), on se place dans le dossier du programme. */
    if (!al_filename_exists(FICHIER_POLICE))
    {
        ALLEGRO_PATH *dossier = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
        if (dossier)
        {
            al_change_directory(al_path_cstr(dossier, ALLEGRO_NATIVE_PATH_SEP));
            al_destroy_path(dossier);
        }
    }

    al_init_primitives_addon();
    al_install_mouse();
    al_install_keyboard();

    /* 2. Ouvrir la fenêtre et charger les polices */
    ALLEGRO_DISPLAY *fenetre = al_create_display(FENETRE_LARGEUR, FENETRE_HAUTEUR);
    if (!fenetre)
    {
        fprintf(stderr, "Impossible d'ouvrir la fenetre.\n");
        return 1;
    }
    al_set_window_title(fenetre, "Pipe-Addict");
    if (!initialiserAffichage())
    {
        fprintf(stderr, "Police introuvable (%s) : le dossier assets doit etre a cote du programme.\n",
                FICHIER_POLICE);
        return 1;
    }

    /* 3. Le chronomètre et la file d'événements */
    ALLEGRO_TIMER *chrono = al_create_timer(1.0 / FPS); // un tic tous les 1/60 de seconde
    ALLEGRO_EVENT_QUEUE *file = al_create_event_queue();
    al_register_event_source(file, al_get_display_event_source(fenetre));
    al_register_event_source(file, al_get_mouse_event_source());
    al_register_event_source(file, al_get_keyboard_event_source());
    al_register_event_source(file, al_get_timer_event_source(chrono));

    /* 4. Les compteurs ; la partie elle-même sera préparée en quittant l'accueil */
    Partie partie;
    partie.numeroPartie = 0;
    partie.victoires = 0;

    /* 5. La boucle : traiter chaque événement, puis redessiner si besoin */
    al_start_timer(chrono);
    bool continuer = true;
    bool redessiner = true;
    bool voirTout = false;      // mode test : Espace montre ou recache toute la grille
    bool pause = false;         // P met le jeu en pause, ou le relance
    bool accueil = true;        // on commence par l'écran d'accueil
    bool regles = false;        // écran « Comment jouer », ouvert depuis l'accueil ou pendant le jeu
    char nom[NOM_MAX + 1] = ""; // le nom tapé à l'accueil (vide au départ)
    int niveau = 2;             // le niveau choisi à l'accueil

    while (continuer)
    {
        ALLEGRO_EVENT evenement;
        al_wait_for_event(file, &evenement);

        if (evenement.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            continuer = false;

        else if (evenement.type == ALLEGRO_EVENT_TIMER) // un tic du chronomètre
        {
            if (!pause && !accueil && !regles)    // le temps s'arrête pendant la lecture des règles
                mettreAJour(&partie, 1.0f / FPS); // 1/60 de seconde s'est écoulé
            redessiner = true;
        }

        /* ----- Écran « Comment jouer » : Retour ou Échap ramène là où on était
                 (à l'accueil, ou à la partie si on l'a ouvert pendant le jeu) ----- */
        else if (regles)
        {
            if (evenement.type == ALLEGRO_EVENT_KEY_CHAR && evenement.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                regles = false;
            else if (evenement.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && boutonRetourSous(evenement.mouse.x, evenement.mouse.y))
                regles = false;
            redessiner = true;
        }

        /* ----- Écran d'accueil : taper son nom, choisir le niveau ----- */
        else if (accueil)
        {
            bool lancer = false;
            int longueur = strlen(nom);

            if (evenement.type == ALLEGRO_EVENT_KEY_CHAR) // une touche tapée (avec sa lettre)
            {
                int touche = evenement.keyboard.keycode;
                int lettre = evenement.keyboard.unichar; // le caractère tapé

                if (touche == ALLEGRO_KEY_BACKSPACE && longueur > 0)
                    nom[longueur - 1] = '\0'; // on efface la dernière lettre
                else if ((touche == ALLEGRO_KEY_ENTER || touche == ALLEGRO_KEY_PAD_ENTER) && longueur > 0)
                    lancer = true;
                else if (lettre >= 32 && lettre <= 126 && longueur < NOM_MAX) // lettres, chiffres, espace
                {
                    nom[longueur] = (char)lettre;
                    nom[longueur + 1] = '\0'; // une chaîne de caractères finit toujours par '\0'
                }
            }
            else if (evenement.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                int choix = niveauSous(evenement.mouse.x, evenement.mouse.y);
                if (choix != 0)
                    niveau = choix;
                else if (boutonJouerSous(evenement.mouse.x, evenement.mouse.y) && longueur > 0)
                    lancer = true;
                else if (boutonReglesSous(evenement.mouse.x, evenement.mouse.y))
                    regles = true; // ouvrir l'écran « Comment jouer »
            }

            if (lancer) // on quitte l'accueil : la partie commence
            {
                strcpy(partie.joueur, nom);
                nouvellePartie(&partie, niveau);
                partie.numeroPartie++;
                accueil = false;
                pause = false;
            }
            redessiner = true;
        }

        else if (evenement.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (evenement.keyboard.keycode == ALLEGRO_KEY_SPACE)
                voirTout = !voirTout; // true devient false, false devient true
            else if (evenement.keyboard.keycode == ALLEGRO_KEY_P && partie.etat == EN_COURS)
                pause = !pause;
            redessiner = true;
        }

        else if (evenement.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            int x = evenement.mouse.x;
            int y = evenement.mouse.y;
            Bouton bouton = boutonSous(x, y);

            if (bouton == BOUTON_NOUVEAU) // nouvelle grille, nouvelles entrée et sortie
            {
                nouvellePartie(&partie, partie.niveau);
                partie.numeroPartie++;
                pause = false;
            }
            else if (bouton == BOUTON_REFAIRE) // même grille, tout recaché
            {
                refairePartie(&partie);
                partie.numeroPartie++;
                pause = false;
            }
            else if (bouton == BOUTON_TERMINER && partie.etat == EN_COURS) // le fluide va au bout
                terminerPartie(&partie);
            else if (bouton == BOUTON_MENU) // retour à l'accueil pour changer de nom ou de niveau
                accueil = true;
            else if (bouton == BOUTON_REGLES) // revoir les règles, pendant ou après une partie
                regles = true;

            /* Clic dans la grille, seulement pendant la partie et hors pause (sinon on pourrait tricher).
               Clic à gauche ou au-dessus de la grille : on l'ignore (sinon la division donnerait 0 à tort) */
            else if (partie.etat == EN_COURS && !pause && x >= GRILLE_X && y >= GRILLE_Y)
            {
                /* Le calcul inverse : du pixel à la case */
                int ligne = (y - GRILLE_Y) / TAILLE_CASE;
                int colonne = (x - GRILLE_X) / TAILLE_CASE;

                if (estDansGrille(ligne, colonne))
                {
                    if (partie.grille.cases[ligne][colonne].revelee == false && !voirTout)
                    {
                        if (MODE_MEMOIRE)
                            masquerCasesNonRemplies(&partie); // idée 5 : on recache les autres
                        revelerCase(&partie, ligne, colonne); // 1er clic : on révèle
                    }
                    else
                    {
                        /* 2e clic, ou mode test : la case est déjà visible, on échange directement */
                        revelerCase(&partie, ligne, colonne);
                        if (!swapperCase(&partie, ligne, colonne))
                            printf("Echange impossible : malus ou case deja remplie.\n"); // sans accents : la console Windows les affiche mal
                    }
                }
            }
            redessiner = true;
        }

        /* On ne redessine que s'il y a du nouveau et qu'aucun événement n'attend */
        if (redessiner && al_is_event_queue_empty(file))
        {
            if (regles)
                dessinerRegles(MODE_MEMOIRE);
            else if (accueil)
                dessinerAccueil(nom, niveau);
            else
                dessinerJeu(&partie, voirTout, pause);
            al_flip_display();
            redessiner = false;
        }
    }

    /* 6. Tout libérer avant de quitter */
    libererAffichage();
    al_destroy_event_queue(file);
    al_destroy_timer(chrono);
    al_destroy_display(fenetre);
    return 0;
}