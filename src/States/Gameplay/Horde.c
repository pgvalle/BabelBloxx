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
    horde.moveTimer = (Timer){
        .reachedTimeout = false,
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
        if (x <= 10 || x >= APP_VSCREEN_WIDTH - 22)
            return true;
    }

    return false;
}

void MoveHorde()
{
    UpdateTimer(&horde.moveTimer);
    // can't move yet
    if (!horde.moveTimer.reachedTimeout)
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

        const int x = horde.invaders[i].x;
        const int y = horde.invaders[i].y;

        switch (i / 11)
        {
        case 0: // first row
            RenderClip(x, y, CLIP_INVADER00 + horde.clipIndexOffset);
            break;
        case 1: case 2: // second or third
            RenderClip(x, y, CLIP_INVADER10 + horde.clipIndexOffset);
            break;
        case 3: case 4: // fourth or fifth
            RenderClip(x, y, CLIP_INVADER20 + horde.clipIndexOffset);
            break;
        }
    }
}