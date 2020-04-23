#include "twowayMsprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include <iostream>
#include <string>
#include <iomanip>
#include "sprite.h"
#include "explodingSprite.h"

extern int flag;
extern int flag1;
void TwowayMSprite::advanceFrame(Uint32 ticks) 
{
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

TwowayMSprite::TwowayMSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  explosion(NULL),
  frames( FrameFactory::getInstance().getFrames(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("Sky/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("Sky/height")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight())
{ }

TwowayMSprite::TwowayMSprite(const TwowayMSprite& s) :
  Drawable(s), 
  explosion(NULL),
  frames(s.frames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight )
  { }

TwowayMSprite::TwowayMSprite(const string& n, const Vector2f& pos, const Vector2f& vel):
  Drawable(n, pos, vel),
  explosion(NULL), 
  frames( FrameFactory::getInstance().getFrames(n) ),
  worldWidth(Gamedata::getInstance().getXmlInt("Sky/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("Sky/height")),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(n+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(n+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight())
{ }

void TwowayMSprite::explode(){
	if(explosion) return;
	explosion = new ExplodingSprite(Sprite(getName(), getPosition(), getVelocity(), frames[currentFrame]));
}	
void TwowayMSprite::draw() const { 
	if (explosion){
		explosion->draw();
		return;
	}
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
}

void TwowayMSprite::update(Uint32 ticks) 
{ 
 advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  
  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  
  if ( Y() > worldHeight-frameHeight) {
        velocityY( -abs( velocityY() ) );
    }
    if ( X() > worldWidth-frameWidth) {
        velocityX( -abs( velocityX() ) );
    }  
}
