#include "Internal.h"

struct Horde horde;

void InitHorde()
{
    horde.clipIndexOffset = 1;

    for (int i = 0; i < 11; i++)
    {
        // first row
        horde.invaders[i].x = HORDE_X_INIT + 16 * i;
        horde.invaders[i].y = HORDE_Y_INIT;
        // second
        horde.invaders[11 + i].x = HORDE_X_INIT + 16 * i;
        horde.invaders[11 + i].y = HORDE_Y_INIT + 16;
        // third
        horde.invaders[22 + i].x = HORDE_X_INIT + 16 * i;
        horde.invaders[22 + i].y = HORDE_Y_INIT + 32;
        // fourth
        horde.invaders[33 + i].x = HORDE_X_INIT + 16 * i;
        horde.invaders[33 + i].y = HORDE_Y_INIT + 48;
        // fifth
        horde.invaders[44 + i].x = HORDE_X_INIT + 16 * i;
        horde.invaders[44 + i].y = HORDE_Y_INIT + 64;
    }

    // All invaders start dead to play an animation before starting gameplay
    for (int i = 0; i < HORDE_INVADER_COUNT; i++)
        horde.invaders[i].dead = true;

    horde.moveRight = true;
    horde.moveCount = 0;
    horde.moveTimer = (t1mer_t){
        .has_timed_out = false,
        .time = 0,
        .timeout = HORDE_MOVE_TIMEOUT_INIT
    };
}

bool HasHordeReachedBounds()
{
    for (int i = 0; i < HORDE_INVADER_COUNT; i++)
    {
        if (horde.invaders[i].dead)
            continue;

        const int x = horde.invaders[i].x;
        if (x <= 10 || x >= WORLD_WIDTH - 22)
            return true;
    }

    return false;
}

void MoveHorde()
{
    timer_update(&horde.moveTimer);
    // can't move yet
    if (!horde.moveTimer.has_timed_out)
        return;

    // should move down
    if (HasHordeReachedBounds())
    {
        for (int i = 0; i < HORDE_INVADER_COUNT; i++)
            horde.invaders[i].y += 8;

        horde.moveCount++;
        // accelerate
        if (horde.moveTimer.timeout > 16 && horde.moveCount <= 9)
            horde.moveTimer.timeout -= 16;
        horde.moveRight = !horde.moveRight; // change horizontal movement
    }

    const int offset = horde.moveRight ? 2 : -2;
    for (int i = 0; i < HORDE_INVADER_COUNT; i++)
        horde.invaders[i].x += offset;

    horde.clipIndexOffset = !horde.clipIndexOffset;
}

void RenderHorde()
{
    for (int i = 0; i < HORDE_INVADER_COUNT; i++)
    {
        // ignore dead invaders
        if (horde.invaders[i].dead)
            continue;

        /*{  0,  0, 24,  8 }, // tourist
        { 24,  0, 24,  8 }, // tourist explosion

        {  0,  8, 16,  8 }, // cannon
        { 16,  8, 16,  8 }, // cannon explosion 0
        { 32,  8, 16,  8 }, // cannon explosion 1

        { 39, 18,  1,  4 }, // cannon shot
        { 36 ,24,  8,  8 }, // cannon shot explosion

        {  0, 16, 12,  8 }, // invader 0 0
        { 12, 16, 12,  8 }, // invader 0 1
        {  0, 24, 12,  8 }, // invader 1 0
        { 12, 24, 12,  8 }, // invader 1 1
        {  0, 32, 12,  8 }, // invader 2 0
        { 12, 32, 12,  8 }, // invader 2 1
        {  0, 40, 13,  8 }, // invader explosion
        
        //{}, // invader shot 1
        //{}, // invader shot 2
        //{}, // invader shot 3

        //{}, // invader shot explosion*/

        SDL_Rect clip = {horde.clipIndexOffset ? 0 : 12, 16, 12, 8};
        switch (i / 11)
        {
        case 0: // first row
            clip_render(
                &clip,
                ATLAS_INDEX,
                WORLD_WIDTH,
                WORLD_HEIGHT,
                horde.invaders[i].x,
                horde.invaders[i].y
            ); 
            break;
        case 1: case 2: // second or third
            clip.y = 24;
            clip_render(
                &clip,
                ATLAS_INDEX,
                WORLD_WIDTH,
                WORLD_HEIGHT,
                horde.invaders[i].x,
                horde.invaders[i].y
            );
            break;
        case 3: case 4: // fourth or fifth
            clip.y = 32;
            clip_render(
                &clip,
                ATLAS_INDEX,
                WORLD_WIDTH,
                WORLD_HEIGHT,
                horde.invaders[i].x,
                horde.invaders[i].y
            );
            break;
        }
    }
}