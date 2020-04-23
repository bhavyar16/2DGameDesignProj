#include <iostream>
#include <string>
#include <iomanip>
#include "multisprite.h"
#include "playerMsprite.h"
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"
#include "aaline.h"
#include "Hud.h"
#include <cmath>
#include <algorithm>

int flag = 0;
int flag1 = 0;

class ScaledSpriteCompare 
{
public:
  bool operator()(const ScaledSprite* lhs, const ScaledSprite* rhs) {
    return lhs->getScale() < rhs->getScale();
  }
};

Manager::~Manager() 
{ 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  SDL_FreeSurface(orbSurface);
  for (unsigned i = 0; i < sprites.size(); ++i) {
    delete sprites[i];
  }
  for (unsigned i = 0; i < orbs.size(); ++i) {
    delete orbs[i];
  } 
  delete player;
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ), 
  screen( io.getScreen() ),
  orbSurface( io.loadAndSet(Gamedata::getInstance().getXmlStr("redorb/file"), 
              Gamedata::getInstance().getXmlBool("redorb/transparency")) ),
  Sky("Sky", Gamedata::getInstance().getXmlInt("Sky/factor") ),
  Buildings("Buildings", Gamedata::getInstance().getXmlInt("Buildings/factor") ),
  Pyramid("Pyramid", Gamedata::getInstance().getXmlInt("Pyramid/factor") ),
  Land("Land", Gamedata::getInstance().getXmlInt("Land/factor") ),
  player(new playerMsprite("Aladin")),
  viewport( Viewport::getInstance() ),
  isDiplayHud(false),
  hud1(),
  bar(),
  numberOfSmart(0),
  currentSprite(0),
  sprites(),
  orbs(),
  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  sound(),
  count(0),
  isObjPoolDiplayHud(false),
  toggleGodMode(0),
  lose(0),
  win(0)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) 
  {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
  makeOrbs();
  printOrbs();
  makeEnemy();
  sprites.push_back( new Sprite("genie") );
  sprites.push_back( new Sprite("jasmine") );
  
  viewport.setObjectToTrack(sprites[currentSprite]);
}

void Manager::makeEnemy()
{
	std::string name = "Enemy";
	unsigned numberofEnemy = Gamedata::getInstance().getXmlInt("Enemy/numberofEnemy");
	Enemy2Msprite *enemies = NULL;
	Vector2f vel(Gamedata::getInstance().getXmlFloat("Enemy/speedX"),Gamedata::getInstance().getXmlFloat("Enemy/speedY"));
	for(unsigned int i=0; i < numberofEnemy; i++)
	{
		Vector2f pos(Gamedata::getInstance().getRandFloat(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),Gamedata::getInstance().getXmlInt(name+"/endLoc/x")),
		Gamedata::getInstance().getRandFloat(Gamedata::getInstance().getXmlInt(name+"/endLoc/x"),Gamedata::getInstance().getXmlInt(name+"/endLoc/y")));
		
		enemies = new Enemy2Msprite(name,pos,vel, player->getFrame()->getWidth()/2, 
                        player->getFrame()->getHeight()/2);
		sprites.push_back(enemies);	
		static_cast<playerMsprite*>(player)->attach(static_cast<Enemy2Msprite*>(enemies));
	}	
}

void Manager::makeOrbs() 
{
  unsigned numberOfOrbs = Gamedata::getInstance().getXmlInt("numberOfOrbs");
  orbs.reserve( numberOfOrbs );
  for (unsigned i = 0; i < numberOfOrbs; ++i) 
  {
    orbs.push_back( new ScaledSprite("redorb", orbSurface) );
  }
  sort(orbs.begin(), orbs.end(), ScaledSpriteCompare());
}

void Manager::printOrbs() const 
{
  for (unsigned i = 0; i < orbs.size(); ++i) 
  {
	  //do nothing
  }
}

void Manager::draw() const 
{
  Sky.draw();
  for (unsigned j = 0; j < (orbs.size()/3); ++j) 
  {
    orbs[j]->draw();
  }
  Pyramid.draw();
  Buildings.draw();
  Land.draw(); 
  for (unsigned j = orbs.size()/3; j < orbs.size()/2; ++j) 
  {
    orbs[j]->draw();
  }
  
  for (unsigned j = orbs.size()/2; j < (orbs.size()); ++j) 
  {
    orbs[j]->draw();
  }

  for (unsigned i = 0; i < sprites.size(); ++i) 
  {
    sprites[i]->draw();
  }
	player->draw();
  
  if (lose)
  {
	  hud1.gameover_lost(player->getGameScore());
  }
  if (win)
  {
	  hud1.gameover_win(player->getGameScore());
  }
  if(isDiplayHud || clock.getSeconds() < 4)
  {
	hud1.drawhud(clock.getSeconds(),clock.getfps());

  }
  if(isObjPoolDiplayHud)
  {
	  hud1.objectpool_hud(player->getbullet().getBulletlistCount(), player->getbullet().getFreelistCount());
  }
  
  io.printMessageAt(title, 10, 450);
  io.printMessageValueAt("Score : ", player->getGameScore() , 360, 80);
  io.printMessageAt("Ready to Shoot", 360, 60);
  if(toggleGodMode ==1)
  {
	  io.printMessageAt("God Mode", 360, 100);
  }
  
  viewport.draw();
  bar.draw();
  SDL_Flip(screen);
}

void Manager::makeFrame() 
{
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

void Manager::switchSprite() 
{
  viewport.setObjectToTrack(player);
}

bool Manager::checkForCollisions() 
{ 
	bool bExploding = false;
  for(unsigned int i=0; i < sprites.size(); i++)
  {
	  if (sprites[i]->getName()=="Enemy" && player->getbullet().collidedWith(sprites[i]))
	  {
		 sprites[i]->explode();
		 sound.playSound(1);
		 count++;
		 bExploding = true;
		 int score = player->getGameScore();
		 ++score;
		 player->setGameScore(score);
	  }

  }

  return bExploding;
}

void Manager::update() 
{
  ++clock;
  viewport.setObjectToTrack(player);
  Uint32 ticks = clock.getElapsedTicks();
  
  static unsigned int lastSeconds = clock.getSeconds();
  if ( clock.getSeconds() - lastSeconds == 5 ) 
  {
    lastSeconds = clock.getSeconds();
  }
  
  if ( checkForCollisions() ) 
  {
    io.printMessageAt("One Down! Many to go", 360, 60);
    
  }
  else 
  {
    io.printMessageAt("Ready to Shoot", 360, 60);
  }
  for (unsigned int i = 0; i < sprites.size(); ++i) 
  {
	Enemy2Msprite* emSprite = dynamic_cast<Enemy2Msprite*>(sprites[i]);
	if(emSprite)
		emSprite->setPlayerPos(player->getPosition());  
    sprites[i]->update(ticks);
  }
  for (unsigned j = 0; j < orbs.size(); ++j) 
  {
    orbs[j]->update(ticks);
  }
  if ( makeVideo && frameCount < frameMax ) 
  {
    makeFrame();
  }
  
  unsigned numberofEnemy = Gamedata::getInstance().getXmlInt("Enemy/numberofEnemy");
  for(unsigned int i=0; i < numberofEnemy; i++)
  {
	  if (player->collidedWith(sprites[i]))
	  {
		  if (bar.getLen() == 1)
		  {
			  player->explode();  
			  lose = 1;
		  }   
		 bar.update();
		 bar.draw();
		 SDL_Flip(screen);
	  } 
  }
 if (player->check_win())
 {
	 win = 1;
	 clock.pause();
 }
 if(lose == 1)
 { 
	clock.pause();
	sound.playSound(2);		
 }  
 if(toggleGodMode ==1)
 {
	 bar.reset();
 }
 
  player->update(ticks);
  Sky.update();
  Pyramid.update();
  Buildings.update();
  Land.update();
  viewport.update(); // always update viewport last
}

void Manager::play() 
{
  SDL_Event event;
  bool done = false;
  bool keyCatch = false;
  
  while ( not done )
  {
    while ( SDL_PollEvent(&event) ) 
    {
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
          break;
        }
        if ( keystate[SDLK_t] ) {
          switchSprite();
        }
        if ( keystate[SDLK_SPACE] ) {
		  player->shoot();
		  if (!keyCatch) {
            keyCatch = true;
            io.clearString();
            io.buildString(event);
            sound.playSound(0);
            break;
          }
        }
        if (keystate[SDLK_n] ) {
			bar.reset();
		}
        if ( keystate[SDLK_i] ) {
          clock.toggleSloMo();
        }
        if ( keystate[SDLK_p] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
        if (keystate[SDLK_m]) 
        {
			if (!keyCatch) {
            keyCatch = true;
            io.clearString();
            sound.toggleMusic();
          }
          break;
		}
        if (keystate[SDLK_r]) 
        {
		   clock.start();
           bar.reset();
           bar.setLen(0);
           std::vector<Drawable*>::iterator iter = sprites.begin(); 	
	       while(iter != sprites.end())
	       {
			   if((*iter)->getName() == "Enemy")
			   { 			
				  delete *iter; 			
				  iter = sprites.erase(iter); 			
				} 
				else iter++; 			
		   }
		   lose = 0;
		   win = 0; 
		   makeEnemy();
		   sprites.push_back( new Sprite("genie") );
		   sprites.push_back( new Sprite("jasmine") );
           player->reset();
           flag = 0;
           flag1 = 0;
           toggleGodMode =0;
           break;
        }
        if(keystate[SDLK_F1]) {
			 if(!isDiplayHud)
			 { 
				isDiplayHud = true;
			 }
			 else
			 {  
				isDiplayHud = false;
		     }
		}
		
		if(keystate[SDLK_F2])
		{
			if(!isObjPoolDiplayHud)
			{
				isObjPoolDiplayHud = true;
			}
			else
			{
				isObjPoolDiplayHud = false;
			}
		}
		if(keystate[SDLK_d])
        {
			flag = 1;
		}
		if(keystate[SDLK_s])
		{
			flag1 = 1;
		}
		if(keystate[SDLK_a])
		{
			flag = -1;
		}
		if(keystate[SDLK_w])
		{
			flag1 = -1;
		}
		if(keystate[SDLK_d] && keystate[SDLK_a])
		{
			flag = 0;
		}
		if ( keystate[SDLK_g] ) 
		{
		  if(!toggleGodMode)
		  {
			  toggleGodMode = true;
		  }
		  else toggleGodMode = false;
        }
         
		player->setCollisionStrategy(2);
      }
      if(event.type == SDL_KEYUP) 
      {
		 flag = 0; 
		 flag1 = 0; 
		 keyCatch = false;
	  }
    }
    draw();	
    update();
  }
}
