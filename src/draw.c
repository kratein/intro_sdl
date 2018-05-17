#include "../include/prototypes.h"

void draw_game(void)
{
    // Affiche le fond aux coordonnées (0,0)
    draw_image(get_background(), 0, 0);

    /* Affiche la map de tiles : layer 2 (couche de fond) */
    draw_map(2);

    // Affiche la map de tiles : layer 1 (couche active : sol, etc)
    draw_map(1);

    //Affiche le joueur
    draw_player();
    
    // Affiche la map de tiles : layer 3 (couche de devant)
    draw_map(3);

    // Affiche l'écran
    SDL_RenderPresent(get_renderer());

    // Délai pour laisser respirer le processeur
    SDL_Delay(1);
}

void delay (unsigned int frame_limit)
{
    // Gestion des 60 fps
    unsigned int ticks = SDL_GetTicks();

    if (frame_limit < ticks)
    {
        return;
    }

    if (frame_limit > ticks + 16)
    {
        SDL_Delay(16);
    }
    else
    {
        SDL_Delay(frame_limit - ticks);
    }
}

SDL_Texture *load_image(char *name)
{
    // Charge les images avec SDl Image dans une SDL_Surface
    SDL_Surface *loaded_image = NULL;
    SDL_Texture *texture = NULL;
    loaded_image = IMG_Load(name);
    if (loaded_image != NULL)
    {
        // Conversion de l'imageen texture
        texture = SDL_CreateTextureFromSurface(get_renderer(), loaded_image);

        // On se débarasse du pointeur vers une surface
        SDL_FreeSurface(loaded_image);
        loaded_image = NULL; 
    }
    else
    {
        printf("L'image n'a pas pu être chargée ! SDL_Error : %s\n", SDL_GetError());
    }
    return (texture);
}

void draw_image(SDL_Texture *image, int x, int y)
{
    SDL_Rect dest;

    // Règle le rectangle à dessiner selon la taille de l'image source
    dest.x = x;
    dest.y = y;

    // Dessine l'image entière sur l'écran aux coordonnées x et y
    SDL_QueryTexture(image, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(get_renderer(), image, NULL, &dest);
}

void draw_tile(SDL_Texture *image, int dest_x, int dest_y, int src_x, int src_y)
{
    // Rectangle de destination à dessiner
    SDL_Rect dest;

    dest.x = dest_x;
    dest.y = dest_y;
    dest.w = TILE_SIZE;
    dest.h = TILE_SIZE;

    // Rectangle source
    SDL_Rect src;

    src.x = src_x;
    src.y = src_y;
    src.w = TILE_SIZE;
    src.h = TILE_SIZE;

    // Dessine la tile choisie sur l'écran aux coordonnées x et y
    SDL_RenderCopy(get_renderer(), image, &src, &dest);
}