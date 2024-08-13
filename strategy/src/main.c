#include "main.h"
#include "gfx.h"

GAME game;

SDL_Rect tile_rect;
SDL_Rect map_rect;
SDL_Rect * tile_surf;
SDL_Rect * map_surf;

SpriteSheet* terrain;


void draw_map(GAME game);
void set_map(MAP * map);
void generate_perlin_noise_map(MAP * map);

void gui_cleanup(GAME game, SpriteSheet * terrain);
SpriteSheet* load_sprite_sheet(SDL_Renderer* renderer, const char* path);

int SEED = 0;


int main(int argc, char* argv[]) 
{
    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);        
    //Uint32 lastTime = SDL_GetTicks();
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;   
    
    srand(time(NULL));

    terrain = load_sprite_sheet(game.renderer, "../img/terrain/tileset16x16_1.png");

    if (argc > 0)  
    {
        if (argv[1])        
        {
            SEED = atoi(argv[1]);   
            if (SEED > 256 || SEED < 0)
                SEED = rand() % 256;
        }        
    }

    if (!SEED)
        SEED = rand() % 256;

    if (!init_game(game))
    {
        LOG("Failed to initialize game!\n");
        return 1;
    }

    if (argc > 0)  
    {
        if (argv[1])        
        {
            SEED = atoi(argv[1]);   
            if (SEED > 256 || SEED < 0)
                SEED = rand() % 256;
        }        
    }

    if (!SEED)
        SEED = rand() % 256;
    
       
    SDL_RenderClear(game.renderer);

    //Generate map elevations, tiles, caves, rivers, etc.
    generate_perlin_noise_map(game.map);

    set_map(game.map);                    
    draw_map(game);        
    

    tile_rect.x = (tile_rect.x > 0 ? tile_rect.x * TILE_WIDTH : 0);
    tile_rect.y = (tile_rect.y > 0 ? tile_rect.y * TILE_HEIGHT : 0);            
    tile_rect.w = TILE_WIDTH;
    tile_rect.h = TILE_HEIGHT;            
    SDL_BlitSurface( tile_surf, NULL, map_surf, NULL );    

    //Main game loop
    while (game.state != GAME_EXIT)
    {
        Uint32 frameStart = SDL_GetTicks();
        SDL_Event e;    
        bool player_moved = false;

        //Check for input        
        while (SDL_PollEvent(&e)) 
        {
            if (e.type == SDL_QUIT)
            {
                game.state = GAME_EXIT;
                break;
            }   
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_q)
                    game.state = GAME_EXIT;
            }                             
            //else if (e.type == SDL_MOUSEBUTTONDOWN)            
                //handle_mouse_click(&e);
        
            //player_moved = handle_player_movement(SDL_GetKeyboardState(NULL));            
                                    
            //update_game_time(game);
            draw_map(game);            
                                                

            // Frame rate control
            int frameTime = SDL_GetTicks() - frameStart;
            if (frameDelay > frameTime)            
                SDL_Delay(frameDelay - frameTime);                        
        }
    }

	gui_cleanup(game, terrain);
    return 0;
}




