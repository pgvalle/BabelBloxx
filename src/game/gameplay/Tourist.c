#include "Internal.h"
#include <stdlib.h>

struct Tourist tourist;

void InitTourist()
{
    tourist.x = 0;
    tourist.moveRight = false;

    tourist.dead = false;

    tourist.spawned = false;
    tourist.spawnTimer = (timer_t){
        .has_timed_out = false,
        .time = 0,
        .timeout = 5000
    };
}

void UpdateTourist()
{
    if (tourist.dead && tourist.spawned) // spawned and dead
    {
        tourist.dead = false; // not dead anymore. Didn't even spawn yet
        // reset spawn status
        tourist.spawned = false;
        tourist.spawnTimer = (timer_t){
            .has_timed_out = false,
            .time = 0,
            .timeout = rand() % 20000 + 10000
        };
    }
    else if (tourist.spawned) // spawned and not dead yet
    {
        const float offset = tourist.moveRight ? TOURIST_VEL : -TOURIST_VEL;
        tourist.x += offset;
        if (tourist.x <= 8.f || tourist.x >= APP_VSCREEN_WIDTH - 32.f)
        {
            // reset spawn status
            tourist.spawned = false;
            tourist.spawnTimer = (timer_t){
                .has_timed_out = false,
                .time = 0,
                .timeout = rand() % 20000 + 10000
            };
        }
    }
    else // neither spawned nor dead
    {
        timer_update(&tourist.spawnTimer);
        if (tourist.spawnTimer.has_timed_out) // spawn
        {
            tourist.moveRight = rand() % 2;
            tourist.x = tourist.moveRight ? 8.f : (APP_VSCREEN_WIDTH - 32.f);
            tourist.spawned = true;
        }
    }
}

void RenderTourist()
{
    if (!tourist.dead && tourist.spawned)
        clip_render((int)roundf(tourist.x), TOURIST_Y, CLIP_TOURIST);
}
