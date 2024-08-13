#include "main.h"

bool init_fonts(GAME game);


bool init_game(GAME game)
{
    char buf[MSL];

    LOG("Initializing GUI...\n");

    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");    
    SDL_setenv("SDL_VIDEO_CENTERED", "1", 1);


    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) 
    {
        sprintf(buf, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        LOG(buf);
        return false;
    }


    //Initialize SDL PNG stuff
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        sprintf(buf, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        LOG(buf);
        return false;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1)
    {
        sprintf(buf, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        LOG(buf);
        return false;
    }

    //Initializing game
    game.state = GAME_PLAYING;
    game.camera->x = 20;
    game.camera->y = 20;
    
    if (!init_fonts(game))
        return false;


    return true;
}



bool init_fonts(GAME game)
{
    game.font = TTF_OpenFont("../fonts/dalelands.ttf", 12);

    if (game.font)
        return true;
    else
    {
        LOG("Unable to load game fonts");
        return false;
    }    
}