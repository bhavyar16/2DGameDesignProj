#ifndef MULTIBULLET__H
#define MULTIBULLET__H
#include <iostream>
#include <list>
#include "twowayMsprite.h"
#include <string>
class CollisionStrategy;

class multibullet : public TwowayMSprite {
public:
  using TwowayMSprite::update;
  using TwowayMSprite::collidedWith;
  multibullet(const multibullet&);
  virtual ~multibullet();
  multibullet(const std::string&, const Vector2f&, const Vector2f&);
  void update(Uint32 ticks, const Vector2f& pPos);
  virtual bool collidedWith(const Drawable* d);
  bool goneTooFar() const { return tooFar; }
  void reset() 
  {
    tooFar = false;
  }
  void setStrategy(CollisionStrategy* newStrategy);
private:
  
  float maxDistance;
  bool tooFar;
  CollisionStrategy* strategy;
  multibullet& operator=(const multibullet&); // (Item 6)
};

#endif
