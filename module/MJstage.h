#ifndef MJSTAGE
#define MJSTAGE 
#include <iostream>
#include "MJgame.h" 
using namespace std;

// All the rules refer to the following webpage
// http://www.dragona.com.tw/mahjong-rule/

// pickSeat
// pickBookmaker
// getTiles
// applique
// arrange
// drawTile
// playTile
// eatPongGone
// ting
// hu
// flowBureau
// countTai

class MJstage{
public:
	void pickSeat(void){}
	void pickBookmaker(void){}
	void getTiles(void){}
	void applique(void){}
	void arrange(void){}


private:
	MJplayer player[4];
};
#endif
