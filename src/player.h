#include <string>
#include <vector>
#include "sprite.h"
#include "gamedata.h"
#include "collisionStrategy.h"

class Player {
public:
  Player(const std::string& name);
  virtual ~Player();

  virtual void update(Uint32 ticks) { player.update(ticks); }
  virtual void draw() const { 
    player.draw(); 
    strategy->draw();
  }
  const Drawable* getSprite() const { return &player; }
  Vector2f getPosition() const { return player.getPosition(); }
  float getX() const { return player.X(); }
  float getY() const { return player.Y(); }

  /*bool collidedWith(const Drawable* d) const {
    return strategy->execute(player, *d);
  }*/
  void setCollisionStrategy(int index) {
    strategy = strategies[index];
  }

  void stop() { 
    player.velocityX(0); 
    player.velocityY(0); 
  }
  void right();
  void left();
  void up();
  void down();

private:
  Sprite player;
  float playerWidth;
  float playerHeight;
  Vector2f velocity;
  float worldWidth;
  float worldHeight;
  std::vector<CollisionStrategy*> strategies;
  CollisionStrategy * strategy;
  Player(const Player&);
  Player& operator=(const Player&);
};

