#include <iostream>
#include <cmath>
#include "multibullets.h"

multibullets::multibullets(const std::string &n) :
  name(n),
  bulletlist(),
  freelist()
{
}
 
multibullets::multibullets(const multibullets& b) :
  name(b.name),
  bulletlist(b.bulletlist),
  freelist(b.freelist)
{
}
 
multibullets::~multibullets() { 
	
  bulletlist.clear();
  freelist.clear();
}

void multibullets::shoot(const Vector2f& pos, const Vector2f& vel)
{
	if(freelist.empty())
	{
		multibullet bullet(name, pos, vel);
		bulletlist.push_back(bullet);
	}
	else
	{
		multibullet b = freelist.front();
		freelist.pop_front();
		b.reset();
		b.setPosition(pos);
		b.setVelocity(vel);
		bulletlist.push_back(b);
	}
}

void multibullets::draw() const { 
  // Override draw; use the draw in Chunk, which derives from Sprite.
  // So the draw we're using is actually in Sprite
  std::list<multibullet>::const_iterator ptr = bulletlist.begin();
    //std::cout << "bulletList Size" << bulletlist.size() << std::endl;
  while (ptr != bulletlist.end()) 
  {
    (*ptr).draw();
    ++ptr;
  }
}

void multibullets::update(Uint32 ticks, const Vector2f& pPos) 
{ 
  std::list<multibullet>::iterator ptr = bulletlist.begin();
  while (ptr != bulletlist.end()) 
  {
    (*ptr).update(ticks, pPos);
    if ((*ptr).goneTooFar())
     { 
	   // Check to see if we should free a chunk
        freelist.push_back(*ptr);
        ptr = bulletlist.erase(ptr);
    }   
    else ++ptr;
  }
}

void multibullets::reset(){
  std::list<multibullet>::iterator ptr = bulletlist.begin();
  while(ptr != bulletlist.end()){
     freelist.push_back(*ptr);
     ptr = bulletlist.erase(ptr);
 }
}

bool multibullets::collidedWith(const Drawable* obj)
{
   std::list<multibullet>::iterator ptr = bulletlist.begin();
   while(ptr != bulletlist.end()){   
     if((*ptr).collidedWith(obj)){
         freelist.push_back(*ptr);
         ptr = bulletlist.erase(ptr);
         return true;
     }
     ++ptr;
   }          
   return false;
}
 
