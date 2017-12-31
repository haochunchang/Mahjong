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
		// 目前策略：
		// 先找落單的東西北中發白，還沒寫
		// 再找 rank 是 1 或 9，如果落單先打，定義是非 1, 1... 或 1, 2... 或 1, 3...
		// 再從第一張開始找任何 rank 是落單的，目前是從第二張開始找QQ 還要改
		// 都不是前述狀況的話打第一張牌


		// 這時手中應該會比 total len 多一張牌，所以 arrange 時不會排到最後這張
		_hand.set_total_len(_hand.total_len() + 1);
		_hand.arrange();
		_hand.set_total_len(_hand.total_len() - 1);
		cout << "In decidePlay function. After arrange, the hand is:" << endl;
		cout << _hand;


		// ***** 處理落單的東南西北中發白 *****		未完成
		// cout << "First, check suit is 4." << endl;
		for (int i = _hand.faceup_len(); i <= _hand.total_len(); i++) {
			int suit = _hand[i].suit();
			int rank = _hand[i].rank();
			if (suit == 4) {
				// TODO
			}
		}

		// ***** 處理落單的 1 和 9 *****
		// 檢查第一張牌，如果是 rank 1，則下一張 rank 要是 1 or 2 or 3
		// cout << "Next, check rank is 1 or 9." << endl;
		int i = _hand.faceup_len();
		int suit = _hand[i].suit();
		int rank = _hand[i].rank();
		if ((suit == 1 || suit == 2 || suit == 3) && (rank == 1)) {
			// cout << "The very first tile is of rank 1." << endl;
			if (!(_hand[i + 1].fromsuitrank(suit, 1)) && !(_hand[i + 1].fromsuitrank(suit, 2))
			        && !(_hand[i + 1].fromsuitrank(suit, 3))) return i;
		}
		// 檢查最後一張，如果是 rank 9，則下一張 rank 要是 7 or 8 or 9
		i = _hand.total_len();
		suit = _hand[i].suit();
		rank = _hand[i].rank();
		if ((suit == 1 || suit == 2 || suit == 3) && (rank == 9)) {
			// cout << "The very last tile is of rank 9." << endl;
			if (!(_hand[i - 1].fromsuitrank(suit, 8)) && !(_hand[i - 1].fromsuitrank(suit, 9))
			        && !(_hand[i - 1].fromsuitrank(suit, 7))) return i;
		}
		// 檢查中間其他張
		for (i = _hand.faceup_len() + 1; i < _hand.total_len(); i++) {
			suit = _hand[i].suit();
			rank = _hand[i].rank();
			// cout << "Check the tile of suit " << suit << " and rank " << rank << endl;
			// 處理 rank 是 1，至少要前面是 1，或後面是 1 or 2 or 3
			if ((suit == 1 || suit == 2 || suit == 3) && (rank == 1)) {
				bool previousTile = _hand[i - 1].fromsuitrank(suit, 1);
				bool nextTile = _hand[i + 1].fromsuitrank(suit, 1) ||
				                _hand[i + 1].fromsuitrank(suit, 2) ||
				                _hand[i + 1].fromsuitrank(suit, 3);
				if (!previousTile && !nextTile) return i;
			}

			// 處理 rank 是 9，至少要前面是 7 or 8 or 9，或後面是 9
			if ((suit == 1 || suit == 2 || suit == 3) && (rank == 9)) {
				bool previousTile = _hand[i - 1].fromsuitrank(suit, 7) ||
				                    _hand[i - 1].fromsuitrank(suit, 8) ||
				                    _hand[i - 1].fromsuitrank(suit, 9);
				bool nextTile = _hand[i + 1].fromsuitrank(suit, 9);
				if (!previousTile && !nextTile) return i;
			}
		}


		// ***** 處理其他落單的牌 *****
		// 從 faceup_len + 1 到 total_len - 1 找落單的，即左右兩邊都沒有跟自己一樣或 rank 加減 1, 2 的
		// cout << "Next find the lonely tile." << endl;
		// 檢查第一張牌
		i = _hand.faceup_len();
		suit = _hand[i].suit();
		rank = _hand[i].rank();
		if ((suit == 1 || suit == 2 || suit == 3)) {
			if (!(_hand[i + 1].fromsuitrank(suit, rank)) &&
			        !(_hand[i + 1].fromsuitrank(suit, rank + 1)) &&
			        !(_hand[i + 1].fromsuitrank(suit, rank + 2))) return i;
		}
		// 檢查最後一張
		i = _hand.total_len();
		suit = _hand[i].suit();
		rank = _hand[i].rank();
		if ((suit == 1 || suit == 2 || suit == 3)) {
			if (!(_hand[i - 1].fromsuitrank(suit, rank)) &&
			        !(_hand[i - 1].fromsuitrank(suit, rank - 1)) &&
			        !(_hand[i - 1].fromsuitrank(suit, rank - 2))) return i;
		}
		// 檢查中間其他張
		for (int i = _hand.faceup_len() + 1; i < _hand.total_len(); i++) {
			// cout << "in index " << i << endl;
			int suit = _hand[i].suit();
			int rank = _hand[i].rank();
			// cout << "suit is " << suit << " and rank is " << rank << endl;
			if ((suit == 1 || suit == 2 || suit == 3)) {
				bool previousTile = _hand[i - 1].fromsuitrank(suit, rank - 2) ||
				                    _hand[i - 1].fromsuitrank(suit, rank - 1) ||
				                    _hand[i - 1].fromsuitrank(suit, rank);
				// cout << "previousTile is rank or rank - 1? ";
				// (previousTile) ? (cout << "true" << endl) : (cout << "false" << endl);
				bool nextTile = _hand[i + 1].fromsuitrank(suit, rank) ||
				                _hand[i + 1].fromsuitrank(suit, rank + 1) ||
				                _hand[i + 1].fromsuitrank(suit, rank + 2);
				// cout << "nextTile is rank or rank - 1? ";
				// (nextTile) ? (cout << "true" << endl) : (cout << "false" << endl);
				if (!previousTile && !nextTile) return i;
			}
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
