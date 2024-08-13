#include "main.h"
#include "gfx.h"



SpriteSheet* load_sprite_sheet(SDL_Renderer* renderer, const char* path) 
{
    SpriteSheet* sheet = malloc(sizeof(SpriteSheet));
    if (!sheet) 
    {
        LOG("Failed to allocate memory for SpriteSheet\n");
        return NULL;
    }

    sheet->texture = load_texture(renderer, path);
    if (!sheet->texture)
    {
        free(sheet);
        return NULL;
    }

    for (int row = 0; row < TERRAIN_ROWS; row++) 
    {
        for (int col = 0; col < TERRAIN_COLS; col++) 
        {
            sheet->sprites[row][col].x = col * TILE_WIDTH;
            sheet->sprites[row][col].y = row * TILE_HEIGHT;
            sheet->sprites[row][col].w = TILE_WIDTH;
            sheet->sprites[row][col].h = TILE_HEIGHT;
        }
    }

    return sheet;
}

void free_sprite_sheet(SpriteSheet* sheet) 
{
    if (sheet) 
    {
        if (sheet->texture) 
        {
            SDL_DestroyTexture(sheet->texture);
        }
        free(sheet);
    }
}

SDL_Texture* load_texture(SDL_Renderer* renderer, const char* path) 
{
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) 
    {
        char buf[MSL];
        sprintf(buf, "Failed to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
        LOG(buf);
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) 
    {
        char buf[MSL];
        sprintf(buf, "Failed to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
        LOG(buf);
    }

    SDL_FreeSurface(surface);
    return texture;
}


void gui_cleanup(GAME game, SpriteSheet * terrain)
{

    SDL_DestroyRenderer(game.renderer);
    SDL_DestroyWindow(game.main);
    free_sprite_sheet(terrain);
    TTF_CloseFont(game.font);

    TTF_Quit();
    SDL_Quit();
}