#include <vector>
#include <cstdlib>
#include <ctime>
#include <map>
#include <cassert>
#include "MJcollection.h"
#include "Shuffler.h"
#include "MJplayer.h"
#include "MJAIplayer.h"
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
		_players.push_back(MJGreedyAIplayer());
	}
}


MJstage::MJstage(int n_human, int AIkind) {
	cout << "Call MJstage constructor." << endl;

	Shuffler s;
	MJtile mjtiles[144];
	s.init();
	s.fill(mjtiles);
	mjcol = MJcollection(mjtiles);

	for (int i = 0; i < n_human; i++) {
		_players.push_back(MJplayer());
	}
    for (int i = 0; i < 4-n_human; i++) {
        if (AIkind == 1) {
            _players.push_back(MJGreedyAIplayer());    
        } else {
            _players.push_back(MJcustomAIplayer());    
        }
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

	/*
	// 這裡要判斷開局時莊家有沒有胡，跟之後摸一張牌、別人出牌時的寫法應該會不一樣（有一張傳入的牌）
	// 在initiate就有莊家就已經摸一張牌了，所以只要判斷有沒有胡？
    // 補充：因為開局只可能是胡，之後摸一張牌還可能是補槓暗槓
	// 我覺得是可以直接不寫因為機率太低了

	MJtile dummy;
	_players[_bookmaker].strategy(currentPlayer, dummy, actiontype[_bookmaker], actionparameter[_bookmaker]);
	if (actiontype[_bookmaker] == -1) {
		// 莊家自摸
		// TODO
	}
	assert(actiontype[_bookmaker] == 6);
	*/

	// 判定莊家沒胡之後莊家丟一張牌
	int index = _players[_bookmaker].decidePlay();
	MJtile t = _players[_bookmaker].play(index);

	// 正式開局！
	// while 為何不從丟牌開始，感覺比較符合真實情況？
	while (mjcol.size() != 0) {
		// 其他三家要傳進那張丟出來的牌看能不能有 strategy
		for (int i = 0; i < 4; i++) {
			if (i != currentPlayer) {
				_players[i].strategy(currentPlayer, t, actiontype[i], actionparameter[i]);
			}
		}

		// Checking Actions: gone > pong > eat
		int current_action = 0;
		// decide which player's action is executed
		int player_to_act = -1;
		for (int i = 0; i < 4; i++) {
			if (actiontype[i] == -1) {
				// someone huother
				// TODO
				break;
			} else { // 優先順序：gone > pong > eat，同時有人同樣動作就由玩家index小的先？應該要由下家優先
				if (actiontype[i] > current_action) {
					player_to_act = i;
					current_action = actiontype[i];
				}
			}
		}

		// Assign actions on players
		// 下一位出牌
		MJtile dummy;
        if (player_to_act == -1) { // 大家都沒有動作，直接換下一位
			// 原來寫 (currentPos == 4) ? (currentPos = 1) : (currentPos += 1); 是不是有誤？
			(currentPos == 1) ? (currentPos = 4) : (currentPos -= 1);
			currentPlayer = posToPlayer[currentPos];
			_players[currentPlayer].draw(mjcol);
		    _players[currentPlayer].strategy(currentPlayer, dummy, actiontype[currentPlayer], actionparameter[currentPlayer]);
		    if (actiontype[currentPlayer] == -1) {
			    // 自摸, huown
			    //TODO
			    break;
		    }
	        index = _players[currentPlayer].decidePlay();
            t = _players[currentPlayer].play(index);

        } else {
			// 之類的動作：_players[player_to_act].act(current_action);
			currentPos = playerToPos[player_to_act];
			currentPlayer = player_to_act;
		    // 有吃碰槓的動作就是直接出一張，不用抽
            _players[currentPlayer].strategy(currentPlayer, dummy, actiontype[currentPlayer], actionparameter[currentPlayer]);
		    if (actiontype[currentPlayer] == -1) {
			    // 自摸, huown
			    //TODO
			    break;
		    }
	        index = _players[currentPlayer].decidePlay();
            t = _players[currentPlayer].play(index);
        }
    }
	return;
}

