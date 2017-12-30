#include <vector>
#include <cstdlib>
#include <ctime>
#include <map>
#include <cassert>
#include "MJcollection.h"
#include "Shuffler.h"
#include "MJplayer.h"
#include "MJAIplayer.h"
// 加進去會編譯錯檢查一下
// #include "MJgame.h"
// 不能加這行ㄅ，加了會互相 include 會 error
#include "MJstage.h"
using namespace std;


void swapInt(int& a, int& b) {
	int c = a;
	a = b;
	b = c;
	return;
}


//============ MJstage Class Methods =================
MJstage::MJstage() {
	cout << "Call MJstage constructor." << endl;

	Shuffler s;
	MJtile mjtiles[144];
	s.init();
	s.fill(mjtiles);
	mjcol = MJcollection(mjtiles);

	for (int i = 0; i < 4; i++) {
		_players.push_back(MJplayer());
	}
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
		playerToPos[i] = pos[i];
		posToPlayer[pos[i]] = i;
		cout << "Set _players[" << i << "]'s position to " << pos[i] << endl;
		cout << "Set playerToPos[" << i << "] to " << pos[i] << endl;
		cout << "Set posToPlayer[" << pos[i] << "] to " << i << endl;
	}
}


void MJstage::pickBookmaker(void) {
	srand(time(NULL));
	_bookmaker = rand() % 4;
	cout << "_players[" << _bookmaker << "] is the bookmaker" << endl;
}


void MJstage::getTiles(void) {
	cout << "Do getTiles:" << endl;

	MJtile mjtiles_for_player[4][16];
	for (int k = 0; k < 4; k++) {
		for (int i = 0; i < 4; i++) {
			// 從 mjcol 從前面取走 4 張給 _players[i]
			for (int j = k * 4; j < (k + 1) * 4; j++) {
				mjtiles_for_player[i][j] = mjcol.drawfronttile();
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		_players[i].Set_Hand(mjtiles_for_player[i], 16);
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
	cout << "Do initiate:" << endl;
	for (int i = 0; i < 4; i++) {
		_players[i].initiate(mjcol);
		cout << "_players[" << i << "]'s hand is: " << endl;
		_players[i].Print_Hand();
	}
}


void MJstage::mainGame(void) {
	// greedy algorithm
	// 應該把 _players 的資料型態改成 MJAIPlayer？

	int currentPos;
	int currentPlayer;
	int actiontype[4] = {0, 0, 0, 0};
	int actionparameter[4] = {0, 0, 0, 0};
	// 出牌順序：逆時針 (pos 4, 3, 2, 1, 4, etc.)

	currentPlayer = _bookmaker;
	currentPos = playerToPos[_bookmaker];

	// currentPlayer 丟牌
	// 先當作莊家丟出的牌，給莊家決定要丟哪一張牌
	MJtile dummy;

	// 這裡是隨邊丟一個假的 dummy 進去，反正 strategy 裡面不會去用到 dummy 是嘛？
	_players[_bookmaker].strategy(currentPlayer, dummy, actiontype[_bookmaker], actionparameter[_bookmaker]);
	if (actiontype[_bookmaker] == -1) {
		// 莊家自摸
		// TODO
	}

	assert(actiontype[_bookmaker] == 6);
	MJtile t = _players[_bookmaker].play(actionparameter[_bookmaker]);

	// 正式開局！
	// while 為何不從丟牌開始，感覺比較符合真實情況？
	while (mjcol.size() != 0) {
		// 其他三家要傳進那張丟出來的牌看能不能有 strategy
		for (int i = 0; i < 4; i++) {
			if (i != currentPlayer) {
				_players[i].strategy(currentPlayer, t, actiontype[i], actionparameter[i]);
			}
		}


		// Checking Actions: hu (-1)
		for (int i = 0; i < 4; i++) {
			if (actiontype[i] == -1) {
				// someone hu
				// 這樣兩人同時胡會只胡 index 小的喔
				//TODO
				break;
			}
		}

		// Checking Actions: minggone (3)
		for (int i = 0; i < 4; i++) {
			if (actiontype[i] == 3) {
				// someone minggone
				// 這樣兩人同時槓會只槓 index 小的
				//TODO
				break;
			}
		}

		// Checking Actions: pong (2)
		for (int i = 0; i < 4; i++) {
			if (actiontype[i] == 2) {
				// someone pong
				// 這樣兩人同時碰會只碰 index 小的
				//TODO
				break;
			}
		}

		// Checking Actions: eat (1)
		for (int i = 0; i < 4; i++) {
			if (actiontype[i] == -1) {
				// someone eat，可用 actionparameter 判斷怎麼吃
				// 這樣兩人同時吃會只吃 index 小的
				//TODO
				break;
			}
		}

		// Assign actions on players

		// 換下一個人
		(currentPos == 1) ? (currentPos = 4) : (currentPos -= 1);
		currentPlayer = posToPlayer[currentPos];

		
		// 下一位出牌
		_players[currentPlayer].strategy(currentPlayer, dummy, actiontype[currentPlayer], actionparameter[currentPlayer]);
		// actiontype must == 6, play a tile
		if (actiontype[currentPlayer] == -1) {
			// 自摸
			//TODO
			break;
		}
		assert(actiontype[currentPlayer] == 6);
		MJtile t = _players[currentPlayer].play(actionparameter[currentPlayer]);
	}

	return;
}

