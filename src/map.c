#include "../include/prototypes.h"

t_map map;

void init_maps(void)
{
    // Charge l'image de fond
    map.background = load_image("graphics/background.png");

    // On initialise le timer 
    map.map_timer = TIME_BETWEEN_2_FRAMES * 3;
    map.tileset_number = 0;
}

SDL_Texture *get_background(void)
{
    return map.background;
}

void clean_maps(void)
{
    // Libère la texture du background
    if (map.background != NULL)
    {
        SDL_DestroyTexture(map.background);
        map.background = NULL;
    }

    // Libère les textures des tilesets
    if (map.tile_set != NULL)
    {
        SDL_DestroyTexture(map.tile_set);
        map.tile_set = NULL;
    }
    if (map.tile_set_b != NULL)
    {
        SDL_DestroyTexture(map.tile_set_b);
        map.tile_set_b = NULL;
    }
}

void load_map(char *name)
{
    int x, y;
    FILE *fp;
    
    fp = fopen(name, "rb");

    // Si on ne peut pas ouvrir le fichier, on quitte
    if (fp == NULL)
    {
        printf("Failed to open map %s\n", name);
        exit(1);
    }

    // Lit les données du fichier dans la map

    // Lit les coordonnées de début du joueur
    fscanf(fp, "%d", &map.beginx);
    fscanf(fp, "%d", &map.beginy);

    // Lit le nombre du tileset
    fscanf(fp, "%d", &map.tileset_affiche);
    map.max_x = map.max_y = 0;

    for (y = 0; y < MAX_MAP_Y; y++)
    {
        for (x = 0; x < MAX_MAP_X; x++)
        {
            // On lit le numéro de la tile et on le copie dans notre tableau
            fscanf(fp, "%d", &map.tile[y][x]);

            // Permet de déterminer la taille de la map
            if (map.tile[y][x])
            {
                if (x > map.max_x)
                {
                    map.max_x = x;
                }
                if (y > map.max_y)
                {
                    map.max_y = y;
                }
            }
        }
    }
    // Deuxième couche de tiles
    for (y = 0; y < MAX_MAP_Y; y++)
    {
        for (x = 0; x < MAX_MAP_X; x++)
        {
            // On lit le numéro de la tile et on le copie dans notre tableau
            fscanf(fp, "%d", &map.tile2[y][x]);
        }
    }

    // Troisième couche de tiles
    for (y = 0; y < MAX_MAP_Y; y++)
    {
        for (x = 0; x < MAX_MAP_X; x++)
        {
            // On lit le numéro de la tile et on le copie dans notre tableau
            fscanf(fp, "%d", &map.tile3[y][x]);
        }
    }

    /* max_x et max_y sont les coordonnées de fin de la map.
    On les trouve dès qu'il n'y a plus que des zéros à la suite.
    Comme ça, on peut faire des maps de tailles différentes avec la même
    structures de fichier */
    map.max_x = (map.max_x + 1) * TILE_SIZE;
    map.max_y = (map.max_y + 1) * TILE_SIZE;
    
    // Et on referme le fichier
    fclose(fp);
}

void draw_map (int layer)
{
    int x, y, map_x, x1, x2, map_y, y1, y2, x_source, y_source, a;

    /* On initialise map_x à la 1ère colonne qu'on doit blitter.
    Celle-ci correspond au x de la map (en pixels) divisés par la taille d'un tile (32)
    Exemple : si x du début de la map = 1026, on fait 1026 / 32 
    et on sait qu'on doit commencer par affciher la 32ème colonne de tiles de notre map */
    map_x = map.start_x / TILE_SIZE;

    /* Coordonnées de départ pour l'affichage de la map : permet
    de déterminer à quels coordonnées blitter la 1ère colonne de tiles au pixel près
    (par exemple, si la 1ère colonne n'est visible qu'en partie, on devra commencer à blitter 
    hors écran, donc avoir des coordonnées négatives - d'où le -1) */
    x1 = (map.start_x % TILE_SIZE) * -1;

    /* Calcul des coordonnées de la fin de la map : jusqu'où doit-on blitter ?
    Logiquement, on doit aller à x1 (départ) + SCREEN_WIDTH (la largeur de l'écran).
    Mais si on a commencé à blitter en dehors de l'écran la première colonne, il
    va falloir rajouter une autre colonne de tiles sinon on va avoir des pixels blancs. 
    C'est ce que fait : x1 == 0 ? 0 : TILE_SIZE qu'on pourrait traduire par : 
    if (x1 != 0)
    x2 = x1 + SCREEN_WIDTH + TILE_SIZE, mais forcément, c'est plus long*/
    x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);

    // On fait exactement pareil pour calculer y
    map_y = map.start_y / TILE_SIZE;
    y1 = (map.start_y % TILE_SIZE) * -1;
    y2 = y1 + SCREEN_HEIGHT + (y1 == 0 ? 0 : TILE_SIZE);

    // On met en placeun timer pour animer la map 
    if (map.map_timer <= 0)
    {
        if (map.tileset_number == 0)
        {
            map.tileset_number = 1;
            map.map_timer = TIME_BETWEEN_2_FRAMES * 3;
        }
        else
        {
            map.tileset_number = 0;
            map.map_timer = TIME_BETWEEN_2_FRAMES * 3;
        }
    }
    else
    {
        map.map_timer--;
    }

    // Dessine la carte en commençant par startx et starty

    /* On dessine ligne par ligne en commençant par y1 (0) jusqu'à y2 (480)
    A chaque fois, on rajoute TILE_SIZE (donc 32), car on descend d'une ligne
    de tile (qui fait 32 pixels de hauteur)*/

    if (layer == 1)
    {
        for (y = y1; y < y2; y += TILE_SIZE)
        {
            /* A chaque début de ligne, on réinitialise mapX qui
            contient la colonne */
            map_x = map.start_x / TILE_SIZE;

            /* A chaque colonne de tile, on dessine la bonne tile en allant
            de x = 0 à x = 640 */
            for (x = x1; x < x2; x += TILE_SIZE)
            {
                /* Suivant le numéro de notre tile, on découpe le 
                tileset (a = le numéro de la tile) */
                a = map.tile[map_y][map_x];
                
                /* Calcul pour obtenir son y (pour un tileset de 10 tiles
                par ligne, d'où le 10) */
                y_source = a / 10 * TILE_SIZE;

                /* Et son x */
                x_source = a % 10 * TILE_SIZE;

                /* Fonction qui blitte la bonne tile au bon endroit
                suivant le timer */
                if (map.tileset_number == 0)
                {
                    draw_tile(map.tile_set, x, y, x_source, y_source);
                }
                else
                {
                    draw_tile(map.tile_set_b, x, y, x_source, y_source);
                }
                map_x++;
            }
            map_y++;
        }
    }
    else if (layer == 2)
    {
        // Deuxième couche de tiles
        for (y = y1; y < y2; y += TILE_SIZE)
        {
            map_x = map.start_x / TILE_SIZE;
            for (x = x1; x < x2; x += TILE_SIZE)
            {
                /* Suivant le numéro de notre tile, on découpe le 
                tileset (a = le numéro
                de la tile) */
                a = map.tile2[map_y][map_x];

                /* Calcul pour obtenir son y (pour un tileset de 10 tiles
                par ligne d'où le 10) */
                y_source = a / 10 * TILE_SIZE;
                /* Et son x */
                x_source = a % 10 * TILE_SIZE;

                /* Fonction qui blitte la bonne tile au bon endroit
                suivant le timer */
                if (map.tileset_number == 0)
                {
                    draw_tile(map.tile_set, x, y, x_source, y_source);
                }
                else
                {
                    draw_tile(map.tile_set_b, x, y, x_source, y_source);
                }
                map_x++;
            }
            map_y++;
        }
    }
    else if (layer == 3)
    {
        // Troisième couche de tiles
        for (y = y1; y < y2; y += TILE_SIZE)
        {
            map_x = map.start_x / TILE_SIZE;
            for (x = x1; x < x2; x += TILE_SIZE)
            {
                /* Suivant le numéro de notre tile, on découpe le
                tileset (a = le numéro de la tile) */
                a = map.tile3[map_y][map_x];

                /*Calcul pour obtenir son y (pour un tileset de 10 tiles
                par ligne, d'où le 10) */
                y_source = a / 10 * TILE_SIZE;

                /* Et son x */
                x_source = a % 10 * TILE_SIZE;

                /* Fonction qui blitte la bonne tile au bon endroit
                suivant le timer */
                if (map.tileset_number == 0)
                {
                    draw_tile(map.tile_set,x ,y, x_source, y_source);
                }
                else
                {
                    draw_tile(map.tile_set_b, x, y, x_source, y_source);
                }
                map_x++;
            }
            map_y++;
        }
    }
}

void change_level(void)
{
    char file[200];

    /* Charge de la map depuis le fichier */
    sprintf(file, "map/map%d.txt", get_level());
    load_map(file);

    // Charge le tileset
    if (map.tile_set != NULL)
    {
        SDL_DestroyTexture(map.tile_set);
    }
    if (map.tile_set_b != NULL)
    {
        SDL_DestroyTexture(map.tile_set_b);
    }
    sprintf(file, "graphics/tileset%d.png", map.tileset_affiche);
    map.tile_set = load_image(file);
    sprintf(file, "graphics/tileset%db.png", map.tileset_affiche);
    map.tile_set_b = load_image(file);
}

int get_start_x(void)
{
    return map.start_x;
}

void set_start_x(int valeur)
{
    map.start_x = valeur;
}

int get_start_y(void)
{
    return map.start_y;
}

void set_start_y(int valeur)
{
    map.start_y = valeur;
}

int get_max_x(void)
{
    return map.max_x;
}

int get_max_y(void)
{
    return map.max_y;
}

int get_begin_x(void)
{
    return map.beginx;
}

int get_begin_y(void)
{
    return map.beginy;
}