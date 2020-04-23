#ifndef MULTIBULLETS__H
#define MULTIBULLETS__H
#include <iostream>
#include <list>
#include "multibullet.h"

class multibullets
{
public:
    
    multibullets(const std::string&);
    ~multibullets();
    multibullets(const multibullets&);
	void draw() const;
	void update(Uint32 ticks, const Vector2f& pPos);
	void shoot(const Vector2f& pos, const Vector2f& vel);
	bool collidedWith(const Drawable* obj);
	bool canShoot() const { return !bulletlist.empty();}
  unsigned int getBulletlistCount() const {return bulletlist.size();}
  unsigned int getFreelistCount() const {return freelist.size();}
  
  void reset();
private:
    std::string name;
	std::list<multibullet> bulletlist;
	std::list<multibullet> freelist;
};

#endif
