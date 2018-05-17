#ifndef DEF_STRUCTS
#define DEF_STRUCTS

#include "defs.h"

/* Structures qui seront utilisé pour gérer le jeu */

// Structure pour gérer l'input
typedef struct s_input
{
    int left, right, up, down, jump, attack, enter, erase, pause;
} t_input;


// Structure pour gérer la map à afficher
typedef struct s_map
{
    SDL_Texture *background;
    SDL_Texture *tile_set, *tile_set_b;

    // Numéro du tileset à utiliser
    int tileset_affiche;

    // Coordonnées de départ du héros, lorsqu'il commence le niveau 
    int beginx, beginy;

    // Coordonnées de début, lorsqu'on doit dessiner la map
    int start_x, start_y;

    // Coordonnées max de fin de la map
    int max_x, max_y;

    // Tableau à double dimension représentant la map de tiles
    int tile[MAX_MAP_Y][MAX_MAP_X];

    // Deuxième couche de tiles
    int tile2[MAX_MAP_Y][MAX_MAP_X];

    // Troisième couche de tiles
    int tile3[MAX_MAP_Y][MAX_MAP_X];

    // Timer et numéro du teliset à afficher pour animer la map
    int map_timer, tileset_number;
} t_map;

// Structure pour gérer nos sprites
typedef struct s_gameObject
{
    // Points de vie/santé + chrono d'invincibilité
    int life, invincible_timer;

    // Coordonnées du sprite
    int x, y;

    // Largeur, hauteur du sprite
    int h, w;

    // Checkpoint pour le héros (actif ou non)
    int check_point_actif;

    // + coordonnées de respawn (réapparition)
    int respawn_x, respawn_y;

    // Variables utiles pour l'animation :
    // Numéro de la frame en cours+ timer
    int frame_number, frame_timer, frame_max;

    // Nombre max de frames, état du sprite et direction
    // Dans laquelle il se déplace (gauche/droite)
    int etat, direction;

    // Variables utiles pour la gestion des collisions :
    // Est-il sur le sol, chrono une fois mort
    int on_ground, timer_mort;

    // Vecteurs de déplacement temporaires avant détection
    // des collisions avec la map
    float dir_x, dir_y;
    // Sauvegarde des coordonnées de départ 
    int save_x, save_y;

    // Variable pour le double saut
    int jump;
    
} t_gameObject;

#endif