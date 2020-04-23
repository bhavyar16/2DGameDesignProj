#ifndef ENEMY2MSPRITE__H
#define ENEMY2MSPRITE__H
#include <string>
#include <vector>
#include "drawable.h"
#include "ioManager.h"

class ExplodingSprite; 
class Enemy2Msprite : public Drawable {
public:
  Enemy2Msprite(const std::string&,const Vector2f&,const Vector2f&, int, int);
  Enemy2Msprite(const Enemy2Msprite&);
  virtual ~Enemy2Msprite(); 
  void setPlayerPos(const Vector2f& p) { playerPos = p; }
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  unsigned int getChunks();
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }
  void goLeft();
  void goRight();
  void goUp();
  void goDown();
  void explode();
  void resetPosition();
  bool getExplode(){return bExplode;}

protected:
  enum MODE {NORMAL, EVADE};
  IOManager& io;
  ExplodingSprite* explosion;
  const std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;
  bool bExplode;
  int flag;
  Vector2f playerPos;
  const float attackDistance;
  int playerWidth;
  int playerHeight;
  MODE currentMode;
  void advanceFrame(Uint32 ticks);
  void flipImage(Uint32 ticks);
  Enemy2Msprite& operator=(const Enemy2Msprite& rhs);
};
#endif
