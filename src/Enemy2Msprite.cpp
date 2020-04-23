#include "Enemy2Msprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include <iostream>
#include <string>
#include <iomanip>
#include "sprite.h"
#include "explodingSprite.h"
#include "viewport.h"
#include <cmath>

float distance(float x1, float y1, float x2, float y2) 
{
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

void Enemy2Msprite::advanceFrame(Uint32 ticks) 
{
	timeSinceLastFrame += ticks;
	unsigned int mid = numberOfFrames/2;
	if (timeSinceLastFrame > frameInterval) 
	{
        if(velocityX() > 0)
        {
            currentFrame = (currentFrame+1) % mid;
        }
        else 
        {
            if(currentFrame > mid)
            {
                currentFrame =  (currentFrame+1) % numberOfFrames;
            }
            if(currentFrame < mid)
            {
                currentFrame = (currentFrame + mid + 1) % numberOfFrames;
            }
        }
        timeSinceLastFrame = 0;
	}
}


Enemy2Msprite::Enemy2Msprite( const std::string& name, const Vector2f &pos, const Vector2f &vel, int w, int h) :
  Drawable(name, pos, vel),
  io(IOManager::getInstance()),
  explosion(NULL),
  frames( FrameFactory::getInstance().getFrames(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("Sky/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("Sky/height")),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  bExplode(false),
  flag(0),
  playerPos(),
  attackDistance(Gamedata::getInstance().getXmlInt(getName()+"/attackDistance")),
  playerWidth(w),
  playerHeight(h),
  currentMode(NORMAL) 
{ }

Enemy2Msprite::Enemy2Msprite(const Enemy2Msprite& s) :
  Drawable(s), 
  io(IOManager::getInstance()),
  explosion(NULL),
  frames(s.frames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
  bExplode(false),
  flag(0),
  playerPos(s.playerPos),
  attackDistance(s.attackDistance),
  playerWidth(s.playerWidth),
  playerHeight(s.playerHeight),
  currentMode() 
  { }
Enemy2Msprite::~Enemy2Msprite()
{
	if(explosion)
	{
		delete explosion;
		explosion = NULL;
	}
}
/*void Enemy2Msprite::shoot()
{
    float bulletSpeed = 30;
    Vector2f vel = getVelocity();
    float x;
    float y = Y()+ frameHeight/4 + 0;
    if(vel[0] >= 0) {
      x = X() + frameWidth - 40;
      vel[0] += bulletSpeed;
    }
    else {
      x=X() - frameWidth + 40;;
      vel[0] -= bulletSpeed;
    }
    vel[1] *=0;
  
    bullet.shoot(Vector2f(x,y), vel);
}*/

void Enemy2Msprite::explode()
{
	if(explosion) return;
	explosion = new ExplodingSprite(Sprite(getName(), getPosition(), getVelocity(), frames[currentFrame]));
	bExplode = true;
}	
void Enemy2Msprite::draw() const 
{ 
	if (explosion)
	{
		explosion->draw();
		return;
	}
	//bullet.draw();
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  
  frames[currentFrame]->draw(x, y);
  if(flag == 1)
  {
	  int p = 10+ X() - Viewport::getInstance().X();
	  int q = 10+ Y() - Viewport::getInstance().Y();
	  std::stringstream strm;
	  strm << "SM";
	  
	  io.printMessageBlackAt( strm.str(), p+15, q);
  }
  
}

void Enemy2Msprite::resetPosition()
{
  flag = 1;
  Vector2f position(Gamedata::getInstance().getRandFloat(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/x"), 
  Gamedata::getInstance().getXmlInt(getName()+"/endLoc/x")), 
  Gamedata::getInstance().getRandFloat(Gamedata::getInstance().getXmlInt(getName()+"/startLoc/y"), 
  Gamedata::getInstance().getXmlInt(getName()+"/endLoc/y")));
  setPosition(position);
}

void Enemy2Msprite::goLeft()  
{ 
  if (X() > 0) velocityX( abs(velocityX())); 
}
void Enemy2Msprite::goRight() 
{ 
	velocityX( -fabs(velocityX()) ); 
}
void Enemy2Msprite::goUp()    
{ 
	velocityY( -fabs(velocityY()) );
}
void Enemy2Msprite::goDown()  
{ 
	velocityY( fabs(velocityY()) ); 
}

void Enemy2Msprite::update(Uint32 ticks) 
{ 
  advanceFrame(ticks);
  if (explosion)
  {
	explosion->update(ticks);
	if(explosion->chunkCount() == 0)
	{
		delete explosion;
		explosion = NULL;
	}
	resetPosition();
	return;
  }
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 520) 
  {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight ) 
  {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0)
  {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) 
  {
    velocityX( -abs( velocityX() ) );
 
  }  
  
  //bullet.update(ticks, getPosition());
  if(bExplode == true)
  {
	float x= X()+getFrame()->getWidth()/2;
	float y= Y()+getFrame()->getHeight()/2;
	float ex= playerPos[0]+playerWidth/2;
	float ey= playerPos[1]+playerHeight/2;
	float distanceToEnemy = ::distance( x, y, ex, ey );
		
	if  ( currentMode == NORMAL ) 
	{
		if(distanceToEnemy < attackDistance) currentMode = EVADE;
	}
	else if  ( currentMode == EVADE ) 
	{
		if(distanceToEnemy > attackDistance) currentMode=NORMAL;
		else 
		{
		  if ( x > ex ) goRight();
		  if ( x < ex ) goLeft();
		  if ( y < ey ) goDown();
		  if ( y > ey ) goUp();
		}
	  }
	}
}
