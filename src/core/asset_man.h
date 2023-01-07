#ifndef ASSET_MAN_H
#define ASSET_MAN_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#ifndef ASSETS_BASEDIR
    #define ASSETS_BASEDIR "./res"
#endif

#define ASSET_MAN_DEFAULT_TEXTURE_W 48
#define ASSET_MAN_DEFAULT_TEXTURE_H 48

typedef enum asset_type_t {
    ASSETTYPE_TEXTURE,
    ASSETTYPE_FONT,
    ASSETTYPE_MUSIC, // A song
    ASSETTYPE_CHUNK  // An audio effect
} asset_type_t;

// id 0 is reserved to default assets (fonts don't have defaults)
// id may repeat for different asset types.
// In case asset loading fails, returns false and assign a "default" resource to that id
// Error codes:
//  -2 = INVALID
//  -1 = OVERWTITING
//   0 = OK
//   1 = FAILED
int asset_man_load(asset_type_t type, uint8_t id, const char* file);
// Type is required because different asset types can have the same id.
void asset_man_free(asset_type_t type, uint8_t id);
// Deletes all assets, but not the default ones
void asset_man_free_all();

SDL_Texture* asset_man_get_texture(uint8_t id);
TTF_Font* asset_man_get_font(uint8_t id);
Mix_Music* asset_man_get_music(uint8_t id);
Mix_Chunk* asset_man_get_chunk(uint8_t id);


#endif // ASSET_MAN_H