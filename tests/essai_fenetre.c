/*
 * Pipe-Addict : le bon tuyau - Projet C ING3, ECE Paris 2026-2027, groupe DU01
 * Fichier essai_fenetre.c : premier essai Allegro (fenêtre, carré, clics), pour apprendre
 *
 * Utilisation de l'IA (Claude, Anthropic) dans ce fichier : environ 100 %
 * Exemple de prompt :
 *  - « passons à la prochaine étape » (ouvrir une fenêtre graphique avec Allegro)
 * Vérification : compilé et lancé par l'équipe.
 */
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

/* Essai : ouvrir une fenêtre, y dessiner un carré, afficher les clics dans le terminal. */
int main(void)
{
    /* 1. Démarrer Allegro et ses modules */
    if (!al_init())
    {
        fprintf(stderr, "Allegro n'a pas pu démarrer.\n");
        return 1;
    }
    al_init_primitives_addon(); // pour dessiner des formes (rectangles, lignes, cercles)
    al_install_mouse();         // pour recevoir les clics

    /* 2. Ouvrir la fenêtre : 800 pixels de large, 600 de haut */
    ALLEGRO_DISPLAY *fenetre = al_create_display(800, 600);
    if (!fenetre)
    {
        fprintf(stderr, "Impossible d'ouvrir la fenêtre.\n");
        return 1;
    }
    al_set_window_title(fenetre, "Pipe-Addict");

    /* 3. La file d'événements : Allegro y range tout ce qui se passe */
    ALLEGRO_EVENT_QUEUE *file = al_create_event_queue();
    al_register_event_source(file, al_get_display_event_source(fenetre)); // la croix de la fenêtre
    al_register_event_source(file, al_get_mouse_event_source());          // la souris

    /* 4. La boucle : dessiner, puis attendre le prochain événement */
    bool continuer = true;
    while (continuer)
    {
        al_clear_to_color(al_map_rgb(30, 30, 40));                              // fond gris foncé
        al_draw_filled_rectangle(100, 100, 200, 200, al_map_rgb(230, 140, 50)); // carré orange
        al_draw_rectangle(100, 100, 200, 200, al_map_rgb(255, 255, 255), 3);    // son contour blanc
        al_flip_display();                                                      // montrer le dessin

        ALLEGRO_EVENT evenement;
        al_wait_for_event(file, &evenement); // le programme dort jusqu'au prochain événement

        if (evenement.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            continuer = false;
        else if (evenement.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            printf("Clic en x = %d, y = %d\n", evenement.mouse.x, evenement.mouse.y);
    }

    /* 5. Tout libérer avant de quitter */
    al_destroy_event_queue(file);
    al_destroy_display(fenetre);
    return 0;
}
