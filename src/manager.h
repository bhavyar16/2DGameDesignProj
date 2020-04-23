#ifndef MANAGER__H
#define MANAGER__H
#include <vector>
#include <SDL.h>
#include "ioManager.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "Hud.h"
#include "scaledSprite.h"
#include "health.h"
#include "sound.h"

class playerMsprite;

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void switchSprite();
private:
  const bool env;
  IOManager& io;
  Clock clock;

  SDL_Surface * const screen;
  SDL_Surface * const orbSurface;
  World Sky;
  World Buildings;
  World Pyramid;
  World Land;
  playerMsprite *player;
  Viewport& viewport;
  bool isDiplayHud;
  Hud hud1;
  Health bar;
  unsigned int numberOfSmart;
  int currentSprite;
  std::vector<Drawable*> sprites;
  std::vector<ScaledSprite*> orbs;
  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;
  SDLSound sound;
  int count;
  bool isObjPoolDiplayHud;
  bool toggleGodMode;
  int lose;
  int win;
  void draw() const;
  bool checkForCollisions();
  void update();
  void makeEnemy();
  void makeOrbs();
  void printOrbs() const;

  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
};
#endif
