#include <iostream>
#include <string>
#include <iomanip>
#include "Hud.h"
#include "aaline.h"
#include "ioManager.h"
#include "multibullets.h"

Hud::Hud():
	screen(IOManager::getInstance().getScreen()),
	RED(SDL_MapRGB(screen->format, 0xff, 0xff, 0))
	{}
Hud::~Hud(){}
void Hud::drawhud(float secs, float fpss) const {
	IOManager& io = IOManager::getInstance();
    io.printMessageValueAt("Seconds: ", secs, Gamedata::getInstance().getXmlInt("Hud/xval"), Gamedata::getInstance().getXmlInt("Hud/yval"));
    io.printMessageValueAt("Frames per sec(fps) = ",fpss, Gamedata::getInstance().getXmlInt("Hud/xval"), 
    Gamedata::getInstance().getXmlInt("Hud/yval")+15);
    io.printMessageAt("Let's save Daisie", Gamedata::getInstance().getXmlInt("Hud/xval"), 
    Gamedata::getInstance().getXmlInt("Hud/yval")+30);
    io.printMessageAt("Gameplay : ", Gamedata::getInstance().getXmlInt("Hud/xval"), Gamedata::getInstance().getXmlInt("Hud/yval")+45);
    io.printMessageAt("W : Move Up", Gamedata::getInstance().getXmlInt("Hud/xval")+90, Gamedata::getInstance().getXmlInt("Hud/yval")+60);
    io.printMessageAt("A : Move left      D: Move right ", Gamedata::getInstance().getXmlInt("Hud/xval")+5, 
    Gamedata::getInstance().getXmlInt("Hud/yval")+75);
    io.printMessageAt("S : Move Down", Gamedata::getInstance().getXmlInt("Hud/xval")+90, Gamedata::getInstance().getXmlInt("Hud/yval")+90);
    io.printMessageAt("Spacebar : Shoot", Gamedata::getInstance().getXmlInt("Hud/xval")+5, Gamedata::getInstance().getXmlInt("Hud/yval")+105);
    io.printMessageAt("Press F2 to open Bullets HUD  ", Gamedata::getInstance().getXmlInt("Hud/xval")+5, Gamedata::getInstance().getXmlInt("Hud/yval")+120);
    io.printMessageAt("Press n to Reset Health bar  ", Gamedata::getInstance().getXmlInt("Hud/xval")+5, Gamedata::getInstance().getXmlInt("Hud/yval")+140);
    io.printMessageAt("Press r to Restart Game  ", Gamedata::getInstance().getXmlInt("Hud/xval")+5, Gamedata::getInstance().getXmlInt("Hud/yval")+160);
	io.printMessageAt("Press g for Godmode  ", Gamedata::getInstance().getXmlInt("Hud/xval")+5, Gamedata::getInstance().getXmlInt("Hud/yval")+180);
	io.printMessageAt("Press m to toggle Music  ", Gamedata::getInstance().getXmlInt("Hud/xval")+5, Gamedata::getInstance().getXmlInt("Hud/yval")+200);
	
     Sint16 y1 = 20;
     y1 += 0;
     Sint16 x1 = 10, x2 = 310;
     
     Draw_AALine(screen, x1,y1, x2,y1, 2.0f,RED);
	 y1= 260;
     Draw_AALine(screen, x1,y1, x2,y1, 2.0f,RED);
     Sint16 y2;
     x1 = 10, y1 = 260, y2 = y1-240;
     Draw_AALine(screen, x1,y1, x1,y2, 2.0f, RED);
     x1 = 310, y1 = 260, y2 = y1-240;
     Draw_AALine(screen, x1,y1, x1,y2, 2.0f, RED);
     Draw_AALine(screen, 162, 22, 162, 262, 298, 128, 128, 128, 0xf0/2);  
     SDL_Flip(screen);
}
void Hud::objectpool_hud(int bulletlistsize, int freelistsize) const
{
	IOManager& io = IOManager::getInstance();
	io.printMessageValueAt("bulletlist = ", bulletlistsize, Gamedata::getInstance().getXmlInt("Hud/xval")+540, Gamedata::getInstance().getXmlInt("Hud/yval")+15);
    io.printMessageValueAt("freelist = ", freelistsize, Gamedata::getInstance().getXmlInt("Hud/xval")+540, Gamedata::getInstance().getXmlInt("Hud/yval")+35);
	Draw_AALine(screen, 540, 30, 750, 30, 2.0f,RED);
	
    Draw_AALine(screen, 750, 30, 750, 80, 2.0f,RED);
     
    Draw_AALine(screen, 750, 80, 540, 80, 2.0f, RED);
     
    Draw_AALine(screen, 540, 80, 540, 30, 2.0f, RED);
     
    Draw_AALine(screen, 647, 32, 647, 82, 210, 128, 128, 128, 0xf0/2);  
     
    SDL_Flip(screen);
}
void Hud::gameover_lost(int score) const 
{
	IOManager& io = IOManager::getInstance();
	io.printMessageValueAt("You lost with a score of : ",score, Gamedata::getInstance().getXmlInt("Hud/xval")+230, Gamedata::getInstance().getXmlInt("Hud/yval")+180);
	io.printMessageAt("Press R to Restart or ESC to Quit", Gamedata::getInstance().getXmlInt("Hud/xval")+230, Gamedata::getInstance().getXmlInt("Hud/yval")+210);
    Draw_AALine(screen, 240, 190, 580, 190, 2.0f,RED);
	
    Draw_AALine(screen, 580, 190, 580, 250, 2.0f,RED);
     
    Draw_AALine(screen, 580, 250, 240, 250, 2.0f, RED);
     
    Draw_AALine(screen, 240, 250, 240, 190, 2.0f, RED);
     
    Draw_AALine(screen, 412, 192, 412, 252, 340, 128, 128, 128, 0xf0/2);  
     
    SDL_Flip(screen);
	
}
void Hud::gameover_win(int score) const 
{
	IOManager& io = IOManager::getInstance();
	io.printMessageValueAt("You won with a score of :",score, Gamedata::getInstance().getXmlInt("Hud/xval")+230, Gamedata::getInstance().getXmlInt("Hud/yval")+180);
	io.printMessageAt("Press R to Restart or ESC to Quit", Gamedata::getInstance().getXmlInt("Hud/xval")+230, Gamedata::getInstance().getXmlInt("Hud/yval")+210);
    Draw_AALine(screen, 240, 190, 580, 190, 2.0f,RED);
	
    Draw_AALine(screen, 580, 190, 580, 250, 2.0f,RED);
     
    Draw_AALine(screen, 580, 250, 240, 250, 2.0f, RED);
     
    Draw_AALine(screen, 240, 250, 240, 190, 2.0f, RED);
     
    Draw_AALine(screen, 412, 192, 412, 252, 340, 128, 128, 128, 0xf0/2);  
     
    SDL_Flip(screen);
	
}
