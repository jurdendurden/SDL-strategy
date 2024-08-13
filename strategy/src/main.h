#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ncurses.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>


//Type defines
#if !defined byte
    typedef uint8_t byte;
#endif

//Utility macros
#define NULL_STR(str)               (strlen(str) < 1 || str[0] == '\0' || !strcmp(str, "(null)") || !strcmp(str, "recycled")) 
#define LOG(str)                    (add_log(str))

//Max string length
#define MSL                     512


//Map defines
#define SCREEN_WIDTH            1024
#define SCREEN_HEIGHT           768

#define TILE_WIDTH              32
#define TILE_HEIGHT             32

#define MAP_WIDTH               (SCREEN_WIDTH / TILE_WIDTH) * 32
#define MAP_HEIGHT              (SCREEN_HEIGHT / TILE_HEIGHT) * 32

#define VIEW_RANGE              5

#define EQUATOR                 MAP_HEIGHT / 2
#define NEAR_EQUATOR(y)         ((y < EQUATOR + (MAP_HEIGHT / 10)) && (y > EQUATOR - (MAP_HEIGHT / 10)))
#define NEAR_POLES(y)           ((y > EQUATOR + (MAP_HEIGHT / 3)) && (y < EQUATOR - (MAP_HEIGHT / 3)))
#define NEAR_N_POLE(y)          (y < EQUATOR - (MAP_HEIGHT / 3))
#define NEAR_S_POLE(y)          (y > EQUATOR + (MAP_HEIGHT / 3))


//Game States
#define GAME_EXIT               0
#define GAME_PLAYING            1
#define GAME_PAUSED             2


//Directions
#define NORTH                   0
#define EAST                    1
#define SOUTH                   2
#define WEST                    3


typedef struct 
{
    int         x;
    int         y;
} COORDS; 

typedef struct 
{
    int         x;
    int         y;
} CAMERA; 

typedef struct
{          
    int             height;
    int             width;    
    int             tiles[MAP_WIDTH][MAP_HEIGHT];               //which tile on tile_table
    double          elevation[MAP_WIDTH][MAP_HEIGHT];           //for perlin generator           

} MAP;

typedef struct
{
    int troop_count;
    int troop_type;

    int x;
    int y;
    SDL_Texture * texture;
} ARMY;

typedef struct 
{
    char * name;

    int population;
    ARMY * armies;
} CITY;

typedef struct 
{
    char * name;
    
    int gold;
    int lumber;
    int stone;
    int food;

    int max_storage;
    CITY * cities;

    int x;
    int y;

} PLAYER;

typedef struct 
{
    int state;    

    SDL_Renderer * renderer;
    TTF_Font * font;
    SDL_Window * main;    
    MAP * map;
    PLAYER * player;
    CAMERA * camera;

    SDL_Texture * tile_textures[112];
    //SDL_Texture * unit_textures[

} GAME;


//External functions (used globally)
void add_log(char * message);
bool init_game(GAME game);


//Global variables
extern int SEED;