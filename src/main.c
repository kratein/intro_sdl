#include <SDL2/SDL.h>
#include <stdio.h>
#include "../include/prototypes.h"

t_input _input;

int main()
{
    unsigned int frame_limit = SDL_GetTicks() + 16;
    int go;

    // Initialisation de la SDL
    init("Rabidja 3 - SDL 2 -");

    // Chargement des ressources
    load_game();

    //On initialise le joueur
    initialize_player();

    // Appelle la fonction cleanup à la fin du programme
    atexit(cleanup);
    go = 1;

    // Boucle infinie, principale, du jeu
    while (go == 1)
    {
        // Gestion des inputs clavier
        gestion_inputs(&_input);

        // On met à jour le jeu, en commençant par le joueur
        update_player(&_input);

        // On dessine tout
        draw_game();

        // Gestion des 60 fps (1000ms/60 = 16.6 -> 16)
        delay(frame_limit);
        frame_limit = SDL_GetTicks() + 16;
   }

   // On quitte
   exit(0);
}