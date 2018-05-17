#include "structs.h"

#ifndef PROTOTYPES
#define PROTOTYPES

#include "structs.h"

/* Catalogue des prototypes des fonctions utilisées.
On le comprétera au fur et à mesure. */

extern void update_player(t_input *input);
extern void set_start_x(int valeur);
extern void set_start_y(int valeur);
extern int get_max_y(void);
extern int get_max_x(void);
extern void clean_player(void);
extern void cleanup(void);
extern void draw_player(void);
extern int get_begin_x(void);
extern int get_begin_y(void);
extern int get_max_x(void);
extern int get_begin_y(void);
extern t_gameObject *get_player(void);
extern int get_player_direction(void);
extern int get_player_x(void);
extern int get_player_y(void);
extern int get_start_x(void);
extern int get_start_y(void);
extern void initialize_player(void);
extern void init_player_sprites(void);
extern void delay(unsigned int frame_limit);
extern void draw_game(void);
extern void gestion_inputs(t_input *input);
extern void get_input(t_input *input);
extern SDL_Renderer *get_renderer(void);
extern void init(char *);
extern void clean_maps(void);
extern void draw_image(SDL_Texture *, int, int);
extern void draw_map(int);
extern void init_maps(void);
extern void load_game(void);
extern SDL_Texture *load_image(char *name);
extern SDL_Texture *get_background(void);
extern void change_level(void);
extern void draw_tile (SDL_Texture *image, int dest_x, int dest_y, int src_x, int src_y);
extern int get_level(void);
extern void load_map(char *name);
extern void set_nombre_de_vie(int valeur);
extern void set_nombre_detoiles(int valeur);
extern void set_valeur_du_niveau(int valeur);

#endif