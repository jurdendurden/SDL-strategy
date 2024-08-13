#include "main.h"

//Draw the map
void draw_map(GAME game)
{           
    if (game.camera == NULL) 
    {
        LOG("Error: plr_camera is NULL in draw_map function\n");
        return;
    }

    SDL_Rect dest_rect;
    dest_rect.w = TILE_WIDTH;
    dest_rect.h = TILE_HEIGHT;

    SDL_RenderClear(game.renderer);

    int start_x = game.camera->x / TILE_WIDTH;
    int start_y = game.camera->y / TILE_HEIGHT;
    int end_x = start_x + (SCREEN_WIDTH / TILE_WIDTH);
    int end_y = start_y + (SCREEN_HEIGHT / TILE_HEIGHT);

    for (int x = start_x; x < end_x && x < MAP_WIDTH; x++)
    {
        for (int y = start_y; y < end_y && y < MAP_HEIGHT; y++)
        {
            dest_rect.x = (x - start_x) * TILE_WIDTH;
            dest_rect.y = (y - start_y) * TILE_HEIGHT;

            int tile_type = game.map->tiles[x][y];
                                    
            SDL_RenderCopy(game.renderer, game.tile_textures[tile_type], NULL, &dest_rect);
        }
    }

    
    SDL_Rect fullscreen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(game.renderer, &fullscreen);

    SDL_RenderPresent(game.renderer);

    // Reset render target to the default
    SDL_SetRenderTarget(game.renderer, NULL);
            

    SDL_RenderPresent(game.renderer);


    last_camera_x = game.camera->x;
    last_camera_y = game.camera->y;
}

void set_map(MAP * map)
{
    int x = 0;
    int y = 0;

    for (x = 0; x < MAP_WIDTH; x++)        
    {
        for (y = 0; y < MAP_HEIGHT; y++)                            
            map->tiles[x][y] = get_tile(map->elevation[x][y], x, y);       
    }
    //add_rivers(map);    
    //add_caves(map);    
}

int get_tile(GAME game, double value, int x, int y)
{
    if (value > 0.73)        
        game.map->tiles[x][y] = TILE_MOUNTAIN;    
    else if (value > 0.65)
    {
        if (/*!mountain_near(game.map, x, y, 2)*/ rnd_num(1,100) < 80)        
            game.map->tiles[x][y] = TILE_HILL;                    
        else
        {
            if (y < EQUATOR - 180 || y > EQUATOR + 180)            
            {
                if (rnd_num(1,100) < 35)
                    game.map->tiles[x][y] = TILE_FWOODS;                            
                else            
                {
                    if (rnd_num(1,100) < 80)
                        game.map->tiles[x][y] = TILE_FFOREST;                
                    else
                        game.map->tiles[x][y] = TILE_FOREST;                
                }
            }
            else
            {
                game.map->tiles[x][y] = TILE_FOREST;
            }     
        }            
    }
    else if (value > 0.54)            
    {
        if (NEAR_EQUATOR(y))        
            game.map->tiles[x][y] = TILE_WOODS;
        else if (y < EQUATOR - 180 || y > EQUATOR + 180)            
        {
            if (rnd_num(1,100) < 35)
                game.map->tiles[x][y] = TILE_FWOODS;                            
            else            
                game.map->tiles[x][y] = TILE_FFOREST;                
        }                 
        else        
            game.map->tiles[x][y] = TILE_FOREST;
    }
    else if (value > 0.45)
    {
        //Close to equator
        if (NEAR_EQUATOR(y))
        {
            switch (rnd_num(1,100))
            {                
                case 1 ... 7:
                    game.map->tiles[x][y] = TILE_FIELD;                    
                    break;
                default:
                case 8 ... 30:
                    game.map->tiles[x][y] = TILE_DESERT;                    
                    break;                
                case 31 ... 100:
                    game.map->tiles[x][y] = TILE_SWAMP;                                     
                    break;                
            }
        }
        //Towards the poles
        else if (NEAR_POLES(y))
        {            
            game.map->tiles[x][y] = TILE_TUNDRA;            
        }
        else
        {
            if (rnd_num(1,100) < 80)
            {
                game.map->tiles[x][y] = TILE_FFOREST;                
            }
            else
            {
                game.map->tiles[x][y] = TILE_FIELD;                
            }
        }        
    }
    else if (value > 0.35)
    {        
        if (!NEAR_POLES(y))
        {
            if (water_near(game.map, x, y, 5))     
            {
                if (rnd_num(1,100) < 60)       
                    game.map->tiles[x][y] = TILE_SWAMP;
                else
                    game.map->tiles[x][y] = TILE_JUNGLE;
            }   
        }
        else
        {
            game.map->tiles[x][y] = TILE_TUNDRA;
        }
    }
    else if ((value > 0.31) && /*(water_near(game.map, x, y, 1)) &&*/ (!water_locked(game.map, x, y)))    
        game.map->tiles[x][y] = TILE_SHORELINE;
    else if (value > 0.27)
        game.map->tiles[x][y] = TILE_SHALLOWS;
    else
    {
        if (!land_near(game.map, x, y, 2))
            game.map->tiles[x][y] = TILE_DEEP_WATER;
        else
            game.map->tiles[x][y] = TILE_WATER;  
    }
        
    return (game.map->tiles[x][y]);    
}