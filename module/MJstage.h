#ifndef MJSTAGE
#define MJSTAGE 
#include <iostream>
#include <vector>
#include "MJcollection.h" 
#include "MJplayer.h"
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
	MJstage(void);
	void pickSeat(void);
	void pickBookmaker(void);
	void getTiles(void);
	void applique(void);
	void arrange(void);
	void drawTile(void);
	void playTile(void);
	void eatPongGone(void);
	void ting(void);
	void hu(void);
	void flowBureau(void);
	void countTai(void);

private:
	vector<MJplayer*> _players;
};

MJstage::MJstage(){
	for(int i=0; i<4; i++){
		MJplayer player[i] = MJplayer();
		_players.push_back(&player);
	}
}

#endif
