#ifndef MJSTAGE
#define MJSTAGE
#include <vector>
#include <cstdlib>
#include <ctime>
#include "MJcollection.h"
#include "shuffler.h"
#include "MJplayer.h"
#include "MJgame.h"
using namespace std;

// In TA's dependency graph
// MJstage includes
// 1. Dealing phase
// 2. Stretagy phase
// 3. Check cangong phase
// 4. comparison phase
// 5. shooting phase

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
	void pickBookmaker(void);	// done
	void getTiles(void);		// done
	void initiate(void);		// done

	// 以下寫成一個 mainGame 就好？
	void drawTile(void);
	void playTile(void);
	void eatPongGone(void);
	void ting(void);
	void hu(void);
	void flowBureau(void);
	
	void countTai(void);

private:
	vector<MJplayer> _players;
	// It's a vector by TA originally
	int _bookmaker; // player index (0 - 3), not position (1 - 4)
	MJcollection mjcol;
};

MJstage::MJstage() {
	cout << "Call MJstage constructor." << endl;
	// 在想要不要等有牌的資訊再來 init MJplayer

	Shuffler s;
	MJtile mjtiles[144];
	s.init();
	s.fill(mjtiles);
	mjcol = MJcollection(mjtiles);

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
		int change_with = rand() % 4;
		swapInt(pos[i], pos[change_with]);
	}
	// cout << "pos[0] = " << pos[0] << endl;
	// cout << "pos[1] = " << pos[1] << endl;
	// cout << "pos[2] = " << pos[2] << endl;
	// cout << "pos[3] = " << pos[3] << endl;

	// now can set position for the 4 _players
	for (int i = 0; i < 4; i++) {
		_players[i].Set_Pos(pos[i]);
		cout << "Set _players[" << i << "]'s position to " << pos[i] << endl;
	}
}


void MJstage::pickBookmaker(void) {
	srand(time(NULL));
	_bookmaker = rand() % 4;
	cout << "_players[" << _bookmaker << "] is the bookmaker" << endl;
}


void MJstage::getTiles(void) {
	cout << "Do getTiles:" << endl;

	for (int i = 0; i < 4; i++) {
		// 從 mjcol 從前面取走 16 張給 _players[i]
		MJtile mjtiles_for_player[16];
		for (int i = 0; i < 16; i++) {
			mjtiles_for_player[i] = mjcol.drawfronttile();
		}
		_players[i].Set_Hand(mjtiles_for_player, 16);
	}
	cout << "_players[" << _bookmaker << "] draw 17th tile" << endl;
	_players[_bookmaker].draw(mjcol);

	for (int i = 0; i < 4; i++) {
		cout << "_players[" << i << "]'s hand is: " << endl;
		_players[i].Print_Hand();
	}

	return;
}


void MJstage::initiate(void) {
	// 小問題：因為莊家會拿多一張，在 arrange 時沒有處理到最後那張
	cout << "Do initiate:" << endl;
	for (int i = 0; i < 4; i++) {
		_players[i].initiate(mjcol);
		cout << "_players[" << i << "]'s hand is: " << endl;
		_players[i].Print_Hand();
	}
}


#endif
