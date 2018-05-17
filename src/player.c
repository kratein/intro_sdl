#include "../include/prototypes.h"

int _level;
t_gameObject _player;
SDL_Texture *_player_sprite_sheet;

// Renvoie le GameObject player
t_gameObject *get_player(void)
{
    return &_player;
}

// Renvoie les coordonnées x du héros
int get_player_x(void)
{
    return _player.x;
}

// Renvoie les coordonnées y du héros
int get_player_y(void)
{
    return _player.y;
}

// Change la valeur des coordonnées x du héros
void set_player_x(int valeur)
{
    _player.x = valeur;
}

//Change la valeur des coordonnées y du héros
void set_player_y(int valeur)
{
    _player.y = valeur;
}

//Renvoie le numéro du niveau en cours
int get_level(void)
{
    return (_level);
}

//Change le numéro du niveau en cours 
void set_valeur_du_niveau(int valeur)
{
    _level = valeur;
}

//Charge la spriteshieet de notre héros
// au début du jeu
void init_player_sprites(void)
{
    _player_sprite_sheet = load_image("graphics/rabidja.png");
}

//Libère le sprite du héros à la fin du jeu
void clean_player(void)
{
    if (_player_sprite_sheet != NULL)
    {
        SDL_DestroyTexture(_player_sprite_sheet);
        _player_sprite_sheet = NULL;
    }
}

void initialize_player(void)
{
    //PV à 3
    _player.life = 3;

    //Timer d'invincibilité à 0
    _player.invincible_timer = 0;

    //Indique l'état et la direction de notre héros
    _player.direction = RIGHT;
    _player.etat = IDLE;

    //Indique le numéro de la frame où commencer
    _player.frame_number = 0;

    //...la valeur de son chrono ou timer
    _player.frame_timer = TIME_BETWEEN_2_FRAMES_PLAYER;

    //...et son nombre de frames max 
    _player.frame_max = 8;

    _player.x = get_begin_x();
    _player.y = get_begin_y();

    // Hauteur et largeur de notree héros
    _player.w = PLAYER_WIDTH;
    _player.h = PLAYER_HEIGTH;

    //Variables nécessaires au fonctionnement de la gestion des collisions
    _player.timer_mort = 0;
    _player.on_ground = 0;
}

void draw_player(void)
{
    // Gestion du timer
    // Si notre timer arrive à zéro
    if (_player.frame_timer <= 0)
    {
        //On le réinitialise
        _player.frame_timer = TIME_BETWEEN_2_FRAMES_PLAYER;

        // et on incrémente notre variable qui compte les frames de 1 pour apsser à la suivante
        _player.frame_number++;

        //Mais si on dépasse al frame amx, il faut revenir à la première :
        if (_player.frame_number >= _player.frame_max)
        {
            _player.frame_number = 0;
        }
    }
    //Sinon, on décrémente notre timer
    else
    {
        _player.frame_timer--;
    }
    //Ensuite, on peut passer la min à notre fonction
    //rectangle de destination à dessiner

    SDL_Rect dest;

    //On soustrait des coordonnées de notre héros, ceux du début de l amap, pour qu'il colle au scrolling :
    dest.x = _player.x - get_start_x();
    dest.y = _player.y - get_start_y();
    dest.w = _player.w;
    dest.h = _player.h;

    //Rectangle source
    SDL_Rect src;

    //Pour connaitre le x de la bonne frame à dessiner, il suffit de multiplier
    // la largeur du sprite par le numéro de la frame à afficher -> 0 = 0; 1 = 40; 2 = 80
    src.x = _player.frame_number * _player.w;
    src.w = _player.w;
    src.h = _player.h;

    //On calcul le y de la bonne frame à dessiner, selon la valeur de l'état du héros :
    //Aucun mouvement (Idle) = 0, marche (walk) = 1, etc..
    //Tout cela en accord avec notre spritesheet, of course
    src.y = _player.etat * _player.h;

    //Gestion du flip
    if (_player.direction == LEFT)
    {
        SDL_RenderCopyEx(get_renderer(), _player_sprite_sheet, &src, &dest, 0, 0, SDL_FLIP_HORIZONTAL);
    }
    else
    {
        SDL_RenderCopyEx(get_renderer(), _player_sprite_sheet, &src, &dest, 0, 0, SDL_FLIP_NONE);
    }
}

void center_scrolling_on_player(void)
{
    /*
    Nouveau scolling à sous-boite limite :
    pour éviter les effets de saccadés dus à une caméra qui se centre
    automatiquement et constamment
    sur le joueur, on crée une "boite" imaginaire autour du joueur.
    quand on dépasse un de ses bords, on scrolle.
    mais là encore, au lieu de centrer sur le joueur, on déplace
    simplement la caméra jusqu'à arriver au joueur.
    On a choisi la valeur de 3 pixels pour pouvoir avoir le plaisir 
    d'aller plus vite que cameraman.
    On accélère aussi la vitesse de la caméra en cas de chute rapide
    */
   int c_x_perso = _player.x + _player.w / 2;
   int c_y_perso = _player.y + _player.h /2;
   int x_lim_min = get_start_x() + LIMITE_X;
   int x_lim_max = x_lim_min + LIMITE_W;
   int y_lim_min = get_start_y() + LIMITE_Y;
   int y_lim_max = y_lim_min + LIMITE_H;

   //Si on dépasse par la gauche, on recule la caméra de 3 pixels
   if (c_x_perso > x_lim_min)
   {
       set_start_x(get_start_x() + 3);
   }

   //si on dépasse par la droite, on avance la caméra de 3 pixels
   if (c_x_perso > x_lim_max)
   {
       set_start_x(get_start_x() + 3);
   }

   //Si on arrive au bout de la map à gauche, on stoppe le scrolling
   if (get_start_x() < 0)
   {
       set_start_x(0);
   }
   //Si on arrive au bout de la map à droite, on stoppe le scrolling à la valeur
   // max de la map - la moitité d'un écran
   else if (get_start_x() + SCREEN_WIDTH >= get_max_x())
   {
       set_start_x(get_max_x() - SCREEN_WIDTH);
   }
   //Si on dépasse par le bas, on descend la caméra de 3 pixels
   if (c_y_perso > y_lim_max)
   {
       //sauf si on tombe très vite, auquel cas, on accélère la caméra :
       if (_player.dir_y >= MAX_FALL_SPEED + 1)
       {
           set_start_y(get_start_y() + MAX_FALL_SPEED + 1);
       }
       else
       {
           set_start_y(get_start_y() + 3);
       }
   }
    //si on arrive au bout de la map en haut, on stoppe le scolling
    if (get_start_y() < 0)
    {
        set_start_y(0);
    }

    //si on arrive au bout de la map en bas, on stoppe le scrolling à la 
    //valeur max de la map - la moitié d'un écran
    else if (get_start_y() + SCREEN_HEIGHT >= get_max_y())
    {
        set_start_y(get_max_y() - SCREEN_HEIGHT);
    }
}

void update_player(t_input *input)
{
    //Pour l'instant, on place automatiquement le joueur 
    // sur le sol à 302 pixuel du haut de l'écran, car 
    //On ne gère pas encore les collisions avec le sol

    _player.on_ground = 1;
    _player.y = 302;
    
    //Si on détecte un appui sur la touche fléchée gauche
    if (input->left == 1)
    {
        //On diminue les coordonnées en x du joueur
        _player.x -= PLAYER_SPEED;
        //et on indique qu'il va à gauche
        _player.direction = LEFT;

        //si ce n'était pas son état auparavant et qu'il est bien sur le sol
        if (_player.etat != WALK && _player.on_ground == 1)
        {
            //On enregistre l'anim' de la marche et on l'initialise à 0
            _player.etat = WALK;
            _player.frame_number = 0;
            _player.frame_timer = TIME_BETWEEN_2_FRAMES_PLAYER;
            _player.frame_max = 8;
        }
    }
    //si on détecte un appui sur la touche fléchée droite
    else if (input->right == 1)
    {
        //on augmente les coordonnées en x du joueur
        _player.x += PLAYER_SPEED;
        //et on indique qu'il va à droite
        _player.direction = RIGHT;

        //si ce n'était pas son état auparavant et qu'il est bien sur le sol
        if (_player.etat != WALK && _player.on_ground == 1)
        {
            //on enregistre l'anim' de la marche et on l'initialise à 0
            _player.etat = WALK;
            _player.frame_number = 0;
            _player.frame_timer = TIME_BETWEEN_2_FRAMES_PLAYER;
            _player.frame_max = 8;
        }
    }
    //si on n'appuie sur rien et qu'on est sur le sol, on charge l'animation marquant
    //l'inactivité
    else if (input->right == 0 && input->left == 0 && _player.on_ground == 1)
    {
        //on teste si le joueur n'était pas déjà inactif, pour ne pas recharger l'animation à chaque tour de boucle
        if (_player.etat != IDLE)
        {
            //On enregistre l'anim' de l''inactivité et on l'initialise à 0
            _player.etat = IDLE;
            _player.frame_number = 0;
            _player.frame_timer = TIME_BETWEEN_2_FRAMES_PLAYER;
            _player.frame_max = 8;
        }
    }
    //On gère le scrolling
    center_scrolling_on_player();
}