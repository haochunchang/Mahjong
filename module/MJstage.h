#ifndef MJSTAGE
#define MJSTAGE
#include <vector>
#include <cstdlib>
#include <ctime>
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

class MJstage {
public:
	MJstage(void);				// 還不確定要怎寫
	void pickSeat(void);		// done
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
	vector<MJplayer> _players;
	// It's a vector bt TA originally
	int bookmaker; // 莊家
};

MJstage::MJstage() {
	cout << "Call MJstage constructor." << endl;
	// 在想要不要等有牌的資訊再來 init MJplayer
	for (int i = 0; i < 4; i++) {
		_players.push_back(MJplayer());
	}
}


void swapInt(int& a, int& b) {
	int c = a;
	a = b;
	b = c;
	return;
}


void MJstage::pickSeat(void) {
	// east = 1 ,south = 2 , west = 3 , north = 4
	srand(time(NULL));
	int pos[4] = {1, 2, 3, 4};
	for (int i = 0; i < 4; i++) {
		int change_with = rand() % 4 + 1;
		swapInt(pos[i], pos[change_with]);
	}
	// cout << "pos[0] = " << pos[0] << endl;
	// cout << "pos[1] = " << pos[1] << endl;
	// cout << "pos[2] = " << pos[2] << endl;
	// cout << "pos[3] = " << pos[3] << endl;

	// now can set position for the 4 _players
	for(int i=0; i<4; i++){
		_players[i].Set_Pos(pos[i]);
	}
}




#endif
