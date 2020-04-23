#ifndef HUD__H
#define HUD__H
#include <vector>
#include <SDL.h>

class Hud {
public:
Hud();
~Hud();
void drawhud(float,float) const;
void gameover_lost(int) const;
void gameover_win(int) const;
void objectpool_hud(int,int) const;

private:
SDL_Surface* screen;
const Uint32 RED;
Hud(const Hud&);
Hud& operator=(const Hud&);
};
#endif
