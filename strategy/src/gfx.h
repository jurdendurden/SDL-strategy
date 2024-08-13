#include <SDL2/SDL_image.h>

#define TERRAIN_COLS 8
#define TERRAIN_ROWS 14

typedef struct 
{
    SDL_Texture* texture;
    SDL_Rect sprites[TERRAIN_ROWS][TERRAIN_COLS];
} SpriteSheet;

SpriteSheet* load_sprite_sheet(SDL_Renderer* renderer, const char* path);
void free_sprite_sheet(SpriteSheet* sheet);
SDL_Texture* load_texture(SDL_Renderer* renderer, const char* path);