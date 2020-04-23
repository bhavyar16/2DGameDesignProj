#include "player.h"

Player::~Player() {
  for (unsigned i = 0; i < strategies.size(); ++i) {
    delete strategies[i];
  }
}

Player::Player(const std::string& name) : 
  player(name), 
  playerWidth( player.getFrame()->getWidth() ), 
  playerHeight( player.getFrame()->getHeight() ),
  velocity(player.getVelocity()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  strategies(),
  strategy( NULL )
{
  strategies.push_back( new MidPointCollisionStrategy );
  strategies.push_back( new RectangularCollisionStrategy );
  strategies.push_back( new PerPixelCollisionStrategy );
  strategy = strategies[0];
}

void Player::right() { 
  if ( player.X() < worldWidth-playerWidth) {
    player.velocityX(velocity[0]);
  }
} 
void Player::left()  { 
  if ( player.X() > 0) {
    player.velocityX(-velocity[0]);
  }
} 
void Player::up()    { 
  if ( player.Y() > 0) {
    player.velocityY(-velocity[1]);
  }
} 
void Player::down()  { 
  if ( player.Y() < worldHeight-playerHeight) {
    player.velocityY(velocity[1]);
  }
} 
