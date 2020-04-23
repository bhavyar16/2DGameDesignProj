#include "playerMsprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include <iostream>
#include <string>
#include <iomanip>
#include "sprite.h"
#include "explodingSprite.h"
#include <cmath>

extern int flag;
extern int flag1;

playerMsprite::~playerMsprite() 
{
  for (unsigned i = 0; i < strategies.size(); ++i) 
  {
    delete strategies[i];
  }
  if(explosion)
  {
	  delete explosion;
	  explosion = NULL;
  }
}
void playerMsprite::advanceFrame(Uint32 ticks) 
{
	timeSinceLastFrame += ticks;
	unsigned int mid = numberOfFrames/2;
	if (timeSinceLastFrame > frameInterval) 
	{
        if(velocityX() > 0)
        {
			if(flag == 0)
            {
				mid = 1;
			}
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
            if(flag == 0)
            {
				currentFrame = 14;
			}
        }
        timeSinceLastFrame = 0;
	}
}

playerMsprite::playerMsprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  explosion(NULL),
  name1("bullet"),
  bullet(name1),
  frames( FrameFactory::getInstance().getFrames(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("Sky/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("Sky/height")),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  gameScore(0),
  strategies(),
  strategy( NULL ),
  observers()  
{ 
  strategies.push_back( new MidPointCollisionStrategy );
  strategies.push_back( new RectangularCollisionStrategy );
  strategies.push_back( new PerPixelCollisionStrategy );
  strategy = strategies[0];
}

playerMsprite::playerMsprite( const std::string& name, const Vector2f &pos, const Vector2f &vel) :
  Drawable(name, pos, vel),
  explosion(NULL),
  name1("bullet"),
  bullet(name1),
  frames( FrameFactory::getInstance().getFrames(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("Sky/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("Sky/height")),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  gameScore(0),
  strategies(),
  strategy( NULL ),
  observers() 
{
	 
  strategies.push_back( new MidPointCollisionStrategy );
  strategies.push_back( new RectangularCollisionStrategy );
  strategies.push_back( new PerPixelCollisionStrategy );
  strategy = strategies[0];
}

playerMsprite::playerMsprite(const std::string& name,
                         const Vector2f& pos, const Vector2f& vel,
                         const std::vector<Frame*>& fms) : 
  Drawable(name, pos, vel), 
  explosion(NULL),
  name1("bullet"),
  bullet(name1),
  frames(fms),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/FrameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(fms[0]->getWidth()),
  frameHeight(fms[0]->getHeight()),
  gameScore(0),
  strategies(),
  strategy( NULL ),
  observers() 
{
  strategies.push_back( new MidPointCollisionStrategy );
  strategies.push_back( new RectangularCollisionStrategy );
  strategies.push_back( new PerPixelCollisionStrategy );
  strategy = strategies[0];
}

playerMsprite::playerMsprite(const playerMsprite& s) :
  Drawable(s), 
  explosion(NULL),
  name1(s.name1),
  bullet(s.bullet),
  frames(s.frames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight ),
  gameScore(s.gameScore),
  strategies(),
  strategy( NULL ),
  observers(s.observers)
  { }

void playerMsprite::reset()
{
	X(0);
	Y(680);
	velocityX(-75);
	velocityY(75);
	gameScore = 0;
	
}
void playerMsprite::shoot()
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
      x = X() - frameWidth + 40;;
      vel[0] -= bulletSpeed;
    }
    vel[1] *=0;
  
    bullet.shoot(Vector2f(x,y), vel);
}

void playerMsprite::explode()
{
	if(explosion) return;
	explosion = new ExplodingSprite(Sprite(getName(), getPosition(), getVelocity(), frames[currentFrame]));
}	
void playerMsprite::draw() const 
{
	strategy->draw(); 
	if (explosion)
	{
		explosion->draw();
		return;
	}
  bullet.draw();
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
}

void playerMsprite::detach( Enemy2Msprite* o ) 
{
  std::list<Enemy2Msprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

bool playerMsprite::check_win()
{
	bool var = false;
	 if(X() > 3100 && X() < 3200)
	  {
		return true;
	  }
	  return var;
	  
}
void playerMsprite::update(Uint32 ticks) 
{ 
  advanceFrame(ticks);
if (explosion){
		explosion->update(ticks);
		if(explosion->chunkCount() == 0){
			delete explosion;
			explosion = NULL;
		}
		return;
  }
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
 // setPosition(getPosition() + incr);
  float x = incr[0] ;
  float y = incr[1];
  if(flag == 1)
  {
	  if(X() < 3110)
	  {
		float t = X() + x;
		X(t);
	  }
  }
  if(flag == -1)
  {
	  if(X() > 0)
	  {
		float t = X() + x;
		X(t);
	  }
  }
  
  if(flag1 == 1)
  {
	  if(Y() < 695)
	  {
		float s = Y() + y;
		Y(s);
	  }
  }
  if(flag1 == -1)
  {
	  if(Y() > 520)
	  {
	 float s = Y() - y;
	 Y(s);
 }
  }
  if (flag1 == 1) {
    velocityY( abs( velocityY() ) );
  }
  if ( flag1 == -1) {
    velocityY( abs( velocityY() ) );
  }

  if (flag == 1)
  {
    velocityX( abs( velocityX() ) );
  }
 else if ( flag == -1) {
      velocityX( -abs( velocityX() ) );
  }    
  
  bullet.update(ticks, getPosition());
}
