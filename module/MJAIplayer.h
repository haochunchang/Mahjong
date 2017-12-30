#ifndef MJAIPLAYER
#define MJAIPLAYER
#include <iostream>
#include "MJplayer.h"
using namespace std;

class MJGreedyAIplayer: public MJplayer {
public:
	MJGreedyAIplayer() {
		// cout << "Call MJGreedyAIplayer constructor." << endl;
	}
	// There are two cases that strategy is called:
	//      1. Other player played a tile
	//      2. One must decide which tile to play
	// actiontype: hu=-1 nothing=0 eat=1 pong=2 minggone=3 angone=4 bugone=5
	// actionparameter: huown=1 huother=2
	void strategy(int position, MJtile t, int &actiontype, int &actionparameter) {
		// Naive and greedy strategy
		// 上家出的牌才能吃
		if (previousPlayer[_position] == position) {
			if (_hand.caneat(t)) {
				// eat 為可以吃的方式
				int method = _hand.caneat(t);
				actiontype = 1;
				switch (method) {
				case 3:
					method = 1;
					break;
				case 5:
					method = 1;
					break;
				case 6:
					method = 2;
					break;
				case 7:
					method = 1;
					break;
				}
				actionparameter = method;

				return;
			}
		}

		if (position != _position) { // other players playing
			// check if canhu
			if (_hand.canhu(t)) {
				actiontype = -1;
				actionparameter = 2;
				return;
			}
			// check if canminggone
			if (_hand.canminggone(t)) {
				actiontype = 3;
				return;
			}
			// check if canpong
			if (_hand.canpong(t)) {
				actiontype = 2;
				return;
			}
		} else {
			// 自己 call 這個函數時
			// 剛從牌底抽牌，決定要不要補槓、暗槓
			if (_hand.canhu(t)) {
				actiontype = -1;
				actionparameter = 1;
				return;
			}
			if (_hand.canbugone(t)) {
				actiontype = 5;
				return;
			}
			if (_hand.canangone(t)) {
				actiontype = 4;
				return;
			}
		}
		return;
	};

	int decidePlay(void) {
		// 這時手中應該會多一張牌，所以 arrange 時不會排到最後這張
		_hand.set_total_len(_hand.total_len() + 1);
		_hand.arrange();

		_hand.set_total_len(_hand.total_len() - 1);
		cout << "In decidePlay function. After arrange, the hand is:" << endl;
		cout << _hand;
		// 先找 suit 是 1, 2, 3;  rank 是 1 或 9 且落單（旁邊不是 1、2 或 8、9）的
		// e.g. 1W, 1W, ... 不算落單、..., 8W, 9W 不算落單，1W, 3W, ... 才是落單
		// 現在總牌數應該比 total len 多一
		cout << "First find rank is 1 or 9." << endl;
		for (int i = _hand.faceup_len(); i <= _hand.total_len(); i++) {
			int suit = _hand[i].suit();
			int rank = _hand[i].rank();
			cout << "suit is " << suit << " and rank is " << rank << endl;
			// 處理 rank 是 1 
			if ((suit == 1 || suit == 2 || suit == 3) && (rank == 1)) {
				// 已經是最後一張了
				if (i == _hand.total_len()) return i;
				else {
					// 後一張 rank 是 1 或 2 的話過關，不是 1 或 2 就打這張牌
					if (!(_hand[i + 1].fromsuitrank(suit, 1)) && !(_hand[i + 1].fromsuitrank(suit, 2))) return i;
				}
			}
			// 處理 rank 是 9
			if ((suit == 1 || suit == 2 || suit == 3) && (rank == 9)) {
				// 是第一張
				if (i == _hand.faceup_len()) return i;
				else {
					// 前一張 rank 是 8 或 9 的話過關，不是 8 或 9 就打這張牌
					if (!(_hand[i - 1].fromsuitrank(suit, 8)) && !(_hand[i - 1].fromsuitrank(suit, 9))) return i;
				}
			}
		}

		// 從 faceup_len + 1 到 total_len - 1 找落單的，即左右兩邊都沒有跟自己一樣或 rank 加減 1 的
		cout << "Second find the lonely tile." << endl;
		for (int i = _hand.faceup_len() + 1; i < _hand.total_len(); i++) {
			cout << "in index " << i << endl;
			int suit = _hand[i].suit();
			int rank = _hand[i].rank();
			cout << "suit is " << suit << " and rank is " << rank << endl;
			bool previousTile = _hand[i - 1].fromsuitrank(suit, rank - 1) || _hand[i - 1].fromsuitrank(suit, rank);
			cout << "previousTile is rank or rank - 1? ";
			(previousTile) ? (cout << "true" << endl) : (cout << "false" << endl);
			bool nextTile = _hand[i + 1].fromsuitrank(suit, rank) || _hand[i + 1].fromsuitrank(suit, rank + 1);
			cout << "nextTile is rank or rank - 1? ";
			(nextTile) ? (cout << "true" << endl) : (cout << "false" << endl);
			if (!previousTile && !nextTile) return i;
			cout << endl;
		}

		// 沒其他判斷方式了，就 return 第一張牌吧
		return _hand.faceup_len();
	}
};


class MJCustomAIplayer: public MJplayer {
	void strategy(int position, MJtile t, int &actiontype, int &actionparameter) {
		// Use information from getinfo to decide
		//TODO
	};
};


#endif
