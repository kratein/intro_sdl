#include "../include/prototypes.h"

SDL_Window *_screen;
SDL_Renderer *_renderer;

SDL_Renderer *get_renderer(void)
{
    return (_renderer);
}

void init (char *title)
{
    _screen = SDL_CreateWindow(title, 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    
    // On crée un renderer pour la SDL et on active la synchro verticale : VSYNC
    _renderer = SDL_CreateRenderer(_screen, -1, SDL_RENDERER_PRESENTVSYNC);

    // Si on n'y arrrive pas, on quitte en enregistrant l'erreur dans stdout.txt
    if (_screen == NULL || _renderer == NULL)
    {
        printf("Impossible d'initialiser le mode écran à %d x %d: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
        exit(1);
    }

    // On cache le curseur de la souris
    SDL_ShowCursor(SDL_DISABLE);

    // On initialise SDL_TTF 2 qui gérera l'écriture de texte
    if (TTF_Init() < 0)
    {
        printf("Impossible d'initialiser SDL TTF: %s\n", TTF_GetError());
        exit(1);
    }

    // On initialise SDL_Mixer 2, qui gérera la musique et les effets sonores
    int flags = MIX_INIT_MP3;
    int initted = Mix_Init(flags);
    if ((initted & flags) != flags)
    {
        printf("Mix_Init: Failed to init SDL_Mixer\n");
        printf("Mix_init: %s\n", Mix_GetError());
        exit(1);
    }

    /* Open 44.1Khz, signed 16 bits, system byte order, 
        stereo audio, using 1024 byte chunks */
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
    {
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
        exit(1);
    }

    // Définit le nombre de pistes audio (channels) à mixer
    Mix_AllocateChannels(32);
}

void cleanup()
{
    // libère le sprite du héros
    clean_player();

    // On quitte SDL_Mixer 2 et on décharge la mémoire
    Mix_CloseAudio();
    Mix_Quit();

    // On fait le ménage et on remet les pointeurs à NULL
    SDL_DestroyRenderer(_renderer);
    _renderer = NULL;
    SDL_DestroyWindow(_screen);
    _screen = NULL;

    // Nettoie les sprites de la map
    clean_maps();

    // On quitte SDL_TFF 2
    TTF_Quit();

    // On quitte la SDL
    SDL_Quit();
}

void load_game(void)
{
    // On charge les données pour la map
    init_maps();

    //On charge la feuille de sprites de notre héros
    init_player_sprites();

    // On commence au premier niveau 
    set_valeur_du_niveau(1);
    change_level();
}