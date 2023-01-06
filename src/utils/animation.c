#include "animation.h"
#include "stb_ds.h"
#include "../core/app.h"
#include "../core/asset_man.h"
#include <stdarg.h>

static inline animation_frame_t* animation_get_current_frame(const animation_t* anim)
{
    return &anim->frames[anim->current];
}

void animation_init(animation_t* anim, uint16_t texture_id, uint8_t size, ...)
{
    anim->texture_id = texture_id;
    anim->frames = NULL;
    anim->current = 0;

    va_list args;
    va_start(args, size);
    for (int i = 0; i < size; i++)
        arrput(anim->frames, va_arg(args, animation_frame_t));
    va_end(args);
}

void animation_free(animation_t* anim)
{
    anim->texture_id = 0;
    arrfree(anim->frames);
    anim->current = 0;
}

bool animation_is_over(const animation_t* anim)
{
    return anim->current == arrlen(anim->frames);
}

void animation_update(animation_t* anim)
{
    
    timer_update(&animation_get_current_frame(anim)->timer);
    if (animation_get_current_frame(anim)->timer.has_timed_out)
    {
        // only update frame pointer if it is valid
        if (anim->current < arrlen(anim->frames))
            anim->current++;
    }
}

void animation_render(const animation_t* anim, int x, int y)
{
    SDL_RenderCopy(
        app.renderer,
        asset_man_get_texture(anim->texture_id),
        // this is not dangerous. It's all right
        &animation_get_current_frame(anim)->rect,
        NULL
    );
}