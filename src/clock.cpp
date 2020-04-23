#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <queue>
#include "clock.h"
#include "gamedata.h"
#include "ioManager.h"

Clock& Clock::getInstance() {
  static Clock instance;
  if ( SDL_WasInit(SDL_INIT_VIDEO) == 0) {
    throw std::string("Must init SDL before Clock");
  }
  //if ( instance == NULL) instance = new Clock; //not needed because define static clock instance;
  return instance;
}
int fps1;
Clock::Clock() :
  started(false), 
  paused(false), 
  sloMo(false), 
  framesAreCapped(Gamedata::getInstance().getXmlBool("framesAreCapped")), 
  frameCap(Gamedata::getInstance().getXmlInt("frameCap")), 
  frames(0), 
  tickSum(0),
  sumOfAllTicks(0),
  timeAtStart(0), timeAtPause(0),
  currTicks(0), prevTicks(0), ticks(0),
  fps1(0) 
  {
  start();
}

Clock::Clock(const Clock& c) :
  started(c.started), 
  paused(c.paused), 
  sloMo(c.sloMo), 
  framesAreCapped(c.framesAreCapped), 
  frameCap(c.frameCap), 
  frames(c.frames),
  tickSum(c.tickSum),
  sumOfAllTicks(c.sumOfAllTicks),
  timeAtStart(c.timeAtStart), timeAtPause(c.timeAtPause),
  currTicks(c.currTicks), prevTicks(c.prevTicks), ticks(c.ticks),
  fps1(c.fps1)
  {
  start();
}

void Clock::display() const { 
  static unsigned int lastFrames = 0;
  static unsigned int oldFrames = 0;
  static unsigned int seconds = getSeconds();
  //int fps2 = getfps();

  if ( getSeconds() > seconds ) {
    seconds = getSeconds();
    lastFrames = frames - oldFrames;
    oldFrames = frames;
  }
  IOManager::getInstance()
    .printMessageValueAt("seconds: ", seconds, 10, 30);
    IOManager::getInstance()
    .printMessageValueAt("frames in sec: ", lastFrames, 10, 50);
}

void Clock::toggleSloMo() {
  if( started && !sloMo ) {
    timeAtPause = SDL_GetTicks() - timeAtStart;
    sloMo = true;
  }
  else if ( started && sloMo ) {
    timeAtStart = SDL_GetTicks() - timeAtPause;
    sloMo = false;
  }
}

unsigned int Clock::getTicks() const { 
  if (paused) return timeAtPause;
  else if ( sloMo ) return 1;
  else return SDL_GetTicks() - timeAtStart; 
}

unsigned int Clock::getElapsedTicks() { 
  if (paused) return 0;
  else if ( sloMo ) return 1;

  currTicks = getTicks();
  ticks = currTicks-prevTicks;
  unsigned int delay = capFrameRate();
  prevTicks = currTicks + delay;
  ticks += delay;
  sumOfAllTicks += ticks;
  return ticks;
}

unsigned int Clock::capFrameRate() const { 
  if ( !framesAreCapped ) return 0u;
  unsigned int delay = std::max(0.0,1000.0/frameCap+0.5 - ticks);
  SDL_Delay( delay );
  return delay;
}

Clock& Clock::operator++() 
{ 
	int fpsSum =0;
	std::queue<int> fpsq;
  if ( !paused ) 
  {
    ++frames; 
    if(getSeconds()>0)
    {
		const unsigned int currentfps = frames/getSeconds();
		const unsigned int frame_max = Gamedata::getInstance().getXmlInt("frameMax");
		if(fpsq.size() == frame_max)
		{
			int fpsnow = fpsq.front();
			fpsSum -= fpsnow;
			fpsq.pop();
		}
		fpsSum += currentfps;
		fpsq.push(currentfps);
	}
  }
  const int frame_size = fpsq.size();
  if(frame_size > 0) 
  {
	  fps1= fpsSum/frame_size;
  }
  return *this;
}

void Clock::start() { 
  started = true; 
  paused = false; 
  frames = 0;
  prevTicks = 0;
  timeAtPause = timeAtStart = SDL_GetTicks(); 
}

void Clock::pause() {
  if( started && !paused ) {
    timeAtPause = SDL_GetTicks() - timeAtStart;
    paused = true;
  }
}
void Clock::unpause() {
  if( started && paused ) {
    timeAtStart = SDL_GetTicks() - timeAtPause;
    paused = false;
  }
}

