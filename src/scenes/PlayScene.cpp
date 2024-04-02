#include "app/App.h"
#include "PlayScene.h"
#include "defines.h"

#include <stdio.h>


void PlayScene::updateUFO()
{
  if (ufo)
  {
    ufo->update();
    if (!ufo->isDead()) return;

    delete ufo;
    ufo = nullptr;
    ufoClock.reset(5000);
  }
  else
  {
    ufoClock.update();
    if (!ufoClock.hasTimedOut()) return;

    ufo = new UFO();
  }
}


PlayScene::PlayScene()
{
  state = STARTING;
  delayer.reset(2500);

  ufo = nullptr;
  ufoClock.reset(5000);

  cannon = nullptr;
  cannonLives = 3;
  cannonShootingClock.reset(1500);
}

PlayScene::~PlayScene()
{
  
}


void PlayScene::processEvent()
{
}

void PlayScene::update()
{
  switch (state)
  {
  case STARTING:
    horde.update();

    delayer.update();
    if (delayer.hasTimedOut())
    {
      state = PLAYING;
      cannon = new Cannon();
    }

    break; 
  case PLAYING:
    updatePlayerShooting();
    updateUFO();

    horde.update();

    cannon->update();
    if (cannon->checkAndProcessHit({WIDTH - 5 * TILE, HEIGHT - 5 * TILE, 20, 8}))
    {
      state = DELAYING;
    }

    updateDynamicCollections();

    break;
  case DELAYING:
    if (cannon->isDead())
    {
      delayer.update();
      if (!delayer.hasTimedOut()) break;
      
      state = PLAYING;
      delete cannon;
      cannon = new Cannon();      
    }
    else
    {
      // not dead but may be dead after update
      cannon->update();
      if (!cannon->isDead()) break;

      delayer.reset(2000); // wait 3 seconds after cannon death
      cannonLives--;
    }

    break;
  default:
    assert(0 && "Not implemented");
  }
}

void PlayScene::render()
{
  for (Bunker& bunker : bunkers)
  {
    bunker.render();
  }

  horde.render();

  if (ufo) ufo->render();
  if (cannon) cannon->render();

  for (const Shot& shot : shots)
  {
    shot.render();
  }

  for (const Explosion& explosion : explosions)
  {
    explosion.render();
  }

  const int cannonsY = HEIGHT - 2 * TILE;
  for (int i = 0; i < cannonLives - 1; i++)
  {
    app->renderClip(3 * TILE + 16 * i, cannonsY, {0, 8, 16, 8});
  }

  char livesFmt[2];
  livesFmt[0] = (char)cannonLives + '0';
  app->renderText(TILE, HEIGHT - 2 * TILE, livesFmt);
}
