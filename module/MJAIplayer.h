#ifndef MJAIPLAYER
#define MJAIPLAYER
#include <iostream>
#include "MJplayer.h"
using namespace std;

class MJGreedyAIplayer: public MJplayer {
public:
	MJGreedyAIplayer() : MJplayer() {
		// cout << "Call MJGreedyAIplayer constructor." << endl;
	}
	MJGreedyAIplayer(int money) : MJplayer(money) {
		// cout << "Call MJGreedyAIplayer constructor with money." << endl;
		// cout << "this->Get_Mon(): " << this->Get_Mon() << endl;
	}

	// There are two cases that strategy is called:
	//      1. Other player played a tile
	//      2. One must decide which tile to play
	// actiontype: nothing=0 eat=1 pong=2 minggone=3 angone=4 bugone=5 hu=7 play=8
	// actionparameter: huown=1 huother=2 play=number
	void strategy(int position, MJtile t, int &actiontype, int &actionparameter) {
		// Naive and greedy strategy
		// cout << "Call MJGreedyAIplayer::strategy." << endl;
		// 上家出的牌才能吃
		if (previousPlayer[_position] == position) {
			// cout << "Previous players playing." << endl;
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
			// cout << "Other players playing." << endl;
			// check if canhu
			// cout << "Check if canhu." << endl;
			if (_hand.canhu(t)) {
				actiontype = 7;
				actionparameter = 2;
				return;
			}
			// check if canminggone
			// cout << "Check if canminggone." << endl;
			if (_hand.canminggone(t)) {
				actiontype = 3;
				return;
			}
			// check if canpong
			// cout << "Check if canpong." << endl;
			if (_hand.canpong(t)) {
				actiontype = 2;
				return;
			}
		} else {
			// 自己 call 這個函數時
			// cout << "Myself playing." << endl;
			// 剛從牌底抽牌，決定要不要補槓、暗槓
			if (_hand.canhu(t)) {
				actiontype = 7;
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
			// 以上動作皆沒有，出牌囉
			actiontype = 8;
			int index = this->decidePlay();
			int number = index - _hand.faceup_len() + 1;
			actionparameter = number;
		}
		return;
	};

	int singleSuit4(void) {
		if (_hand.total_len() - _hand.faceup_len() >= 5) {
			extern bool greedyAIPlayer_decidePlay_checkPoint;
			// ***** 處理落單的東南西北中發白 *****
			if (greedyAIPlayer_decidePlay_checkPoint) cout << "Check: suit is 4." << endl;
			// 看第一張，如果下一張跟自己不一樣就回傳這張
			int i = _hand.faceup_len();
			int suit = _hand[i].suit();
			int rank = _hand[i].rank();
			if (suit == 4 && !(_hand[i + 1].fromsuitrank(suit, rank))) return i;
			// 看最後一張，如果前一張跟自己不一樣就回傳這張
			i = _hand.total_len();
			suit = _hand[i].suit();
			rank = _hand[i].rank();
			if (suit == 4 && !(_hand[i - 1].fromsuitrank(suit, rank))) return i;
			// 看中間的牌，如果前一張跟自己不一樣且後一張跟自己不一樣才會回傳
			for (int i = _hand.faceup_len() + 1; i < _hand.total_len(); i++) {
				suit = _hand[i].suit();
				rank = _hand[i].rank();
				if (suit == 4) {
					bool previousTile = _hand[i - 1].fromsuitrank(suit, rank);
					bool nextTile = _hand[i + 1].fromsuitrank(suit, rank);
					if (!previousTile && !nextTile) return i;
				}
			}
		}
		return -1;
	}

	int singleRank19(void) {
		if (_hand.total_len() - _hand.faceup_len() >= 5) {
			extern bool greedyAIPlayer_decidePlay_checkPoint;
			// ***** 處理落單的 1 和 9 *****
			// 檢查第一張牌，如果是 rank 1，則下一張 rank 要是 1 or 2 or 3
			if (greedyAIPlayer_decidePlay_checkPoint) cout << "Check: rank is 1 or 9." << endl;
			int i = _hand.faceup_len();
			int suit = _hand[i].suit();
			int rank = _hand[i].rank();
			if ((suit == 1 || suit == 2 || suit == 3) && (rank == 1)) {
				// cout << "The very first tile is of rank 1." << endl;
				if (!(_hand[i + 1].fromsuitrank(suit, 1)) && !(_hand[i + 1].fromsuitrank(suit, 2))
				        && !(_hand[i + 1].fromsuitrank(suit, 3))) return i;
			}
			// 檢查最後一張，如果是 rank 9，則前一張 rank 要是 7 or 8 or 9
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
		}
		return -1;
	}

	int singleTile(void) {
		if (_hand.total_len() - _hand.faceup_len() >= 5) {
			extern bool greedyAIPlayer_decidePlay_checkPoint;
			// ***** 處理其他落單的牌 *****
			if (greedyAIPlayer_decidePlay_checkPoint) cout << "Check: lonely tile." << endl;
			// 從 faceup_len + 1 到 total_len - 1 找落單的，即左右兩邊都沒有跟自己一樣或 rank 加減 1, 2 的
			// cout << "Next find the lonely tile." << endl;
			// 檢查第一張牌
			int i = _hand.faceup_len();
			int suit = _hand[i].suit();
			int rank = _hand[i].rank();
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
		}
		return -1;
	}

	int pairSuit4(void) {
		if (_hand.total_len() - _hand.faceup_len() >= 5) {
			extern bool greedyAIPlayer_decidePlay_checkPoint;
			// ***** 處理只有兩個 suit 4 (Fa Fa) 這種狀況 *****
			if (greedyAIPlayer_decidePlay_checkPoint) cout << "Check: Fa Fa condition." << endl;
			// 先直接看最後兩張是不是這種狀況
			int i = _hand.total_len() - 1;
			if (_hand[i].suit() == 4 && _hand[i + 1] == 4) {
				if (_hand[i].rank() == _hand[i + 1].rank())
					return i;
			}
			// 從 faceup 開始一次看兩張，且第三張要不一樣
			for (i = _hand.faceup_len(); i < _hand.total_len() - 1; i++) {
				if (_hand[i].suit() == 4 && _hand[i + 1] == 4) {
					if (_hand[i].rank() == _hand[i + 1].rank())
						if (!_hand[i + 2].fromsuitrank(_hand[i].suit(), _hand[i].rank()))
							return i;
				}
			}
		}
		return -1;
	}

	int gapSuit123Rank19(void) {
		if (_hand.total_len() - _hand.faceup_len() >= 5) {
			extern bool greedyAIPlayer_decidePlay_checkPoint;
			// ***** 處理只有 7W 9W 或 1W 3W ，應先丟 9W 或 1W *****
			if (greedyAIPlayer_decidePlay_checkPoint) cout << "Check: 7W 9W or 1W 3W condition." << endl;
			// 先 1W 3W 這種
			// 看第一張
			int i = _hand.faceup_len();
			if (_hand[i].suit() != 4 && _hand[i].rank() == 1) {
				if (_hand[i].suit() == _hand[i + 1].suit() && _hand[i + 1].rank() == 3) {
					return i;
				}
			}
			for (i = _hand.faceup_len() + 1; i < _hand.total_len(); i++) {
				if (_hand[i].suit() != 4 && _hand[i].rank() == 1) {
					// 前一張不能是同一張，後一張是 3W
					if (!_hand[i].fromsuitrank(_hand[i - 1].suit(), _hand[i - 1].rank())) {
						if (_hand[i].suit() == _hand[i + 1].suit() && _hand[i + 1].rank() == 3) {
							return i;
						}
					}
				}
			}
			// 再 7W 9W 這種
			// 看最後一張
			i = _hand.total_len();
			if (_hand[i].suit() != 4 && _hand[i].rank() == 9) {
				if (_hand[i].suit() == _hand[i - 1].suit() && _hand[i - 1].rank() == 7) {
					return i;
				}
			}
			// 從前面
			for (i = _hand.faceup_len() + 1; i < _hand.total_len(); i++) {
				if (_hand[i].suit() != 4 && _hand[i].rank() == 9) {
					// 後一張不能是同一張，前一張必須是 7W
					if (!_hand[i].fromsuitrank(_hand[i + 1].suit(), _hand[i + 1].rank())) {
						if (_hand[i].suit() == _hand[i - 1].suit() && _hand[i - 1].rank() == 7) {
							return i;
						}
					}
				}
			}
		}
		return -1;
	}

	int gapSuit123(void) {
		if (_hand.total_len() - _hand.faceup_len() >= 5) {
			extern bool greedyAIPlayer_decidePlay_checkPoint;
			// ***** 處理如果只有 6W 8W 應先丟，因為只能等 7W *****
			if (greedyAIPlayer_decidePlay_checkPoint) cout << "Check: 6W 8W condition." << endl;
			// 先直接看最後兩張是不是這種狀況
			int i = _hand.total_len();
			if (_hand[i].suit() != 4 && _hand[i].suit() == _hand[i - 1].suit()) {
				// 不用寫最後一張是 1，有的話在前面就應該回傳了
				// 看最後一張 8W，前一張是 6W 就回傳，但 6W 前面有 6W 或 5W 則
				if (_hand[i - 1].rank() == _hand[i].rank() - 2) return i;
			}
			// 再看第一張
			i = _hand.faceup_len();
			if (_hand[i].suit() != 4 && _hand[i].suit() == _hand[i + 1].suit()) {
				if (_hand[i + 1].rank() == _hand[i].rank() + 2) return i;
			}

			// 再看中間其他張
			for (i = _hand.faceup_len(); i < _hand.total_len(); i++) {
				if (_hand[i].suit() != 4 && _hand[i].suit() == _hand[i + 1].suit()) {
					// 下一張如果是自己 +2
					// 再檢查自己的前面是否是自己或自己 -1，不是的話才回傳
					if (_hand[i + 1].rank() == _hand[i].rank() + 2) {
						if (_hand[i - 1].rank() != _hand[i].rank() && _hand[i - 1].rank() != _hand[i].rank() - 1)
							return i;
					}
				}
			}
		}
		return -1;
	}

	int pairSuit123(void) {
		if (_hand.total_len() - _hand.faceup_len() >= 5) {
			extern bool greedyAIPlayer_decidePlay_checkPoint;
			// ***** 處理 6W 6W 這種，但要先確定是不是 6W 6W 6W *****
			if (greedyAIPlayer_decidePlay_checkPoint) cout << "Check: 6W 6W condition." << endl;
			// 先看最後兩個
			int i = _hand.total_len();
			if (_hand[i].suit() != 4) {
				// 如果跟前一個一樣
				if (_hand[i].fromsuitrank(_hand[i - 1].suit(), _hand[i - 1].rank())) {
					// 還要跟前二個不一樣
					if (!_hand[i].fromsuitrank(_hand[i - 2].suit(), _hand[i - 2].rank())) {
						return i;
					}
				}
			}
			// 從第一個開始看
			for (i = _hand.faceup_len(); i < _hand.total_len() - 1; i++) {
				if (_hand[i].suit() != 4) {
					// 跟後一個一樣
					if (_hand[i].fromsuitrank(_hand[i + 1].suit(), _hand[i + 1].rank())) {
						// 跟後二個不一樣
						if (!_hand[i].fromsuitrank(_hand[i + 2].suit(), _hand[i + 2].rank())) {
							return i;
						}
					}
				}
			}
		}
		return -1;
	}

	int contiSuit123Rank19(void) {
		if (_hand.total_len() - _hand.faceup_len() >= 5) {
			extern bool greedyAIPlayer_decidePlay_checkPoint;
			bool condition[10];
			// ***** 然後才丟 8W 9W 或 1W 2W 這種，但要先確定不是 789 或 123 *****
			// 先 1W 2W
			if (greedyAIPlayer_decidePlay_checkPoint) cout << "Check: 1W 2W or 8W 9W condition." << endl;
			// 看第一張
			int i = _hand.faceup_len();
			// 條件一：suit 不為 4 ，rank 是 1
			condition[1] = _hand[i].suit() != 4 && _hand[i].rank() == 1;
			// 條件二：第二張是 2W
			condition[2] = _hand[i].suit() == _hand[i + 1].suit() && _hand[i + 1].rank() == 2;
			// 條件三：第三張不是 3W
			condition[3] = !(_hand[i].suit() == _hand[i + 2].suit() && _hand[i + 2].rank() == 3);
			if (condition[1] && condition[2] && condition[3]) return i;

			// 看倒數第二張
			i = _hand.total_len() - 1;
			// 條件一：suit 不為 4 ，rank 是 1
			condition[1] = _hand[i].suit() != 4 && _hand[i].rank() == 1;
			// 條件二：下一張是 2W
			condition[2] = _hand[i].suit() == _hand[i + 1].suit() && _hand[i + 1].rank() == 2;
			if (condition[1] && condition[2]) return i;

			// 看其他張
			for (i = _hand.faceup_len() + 1; i < _hand.total_len() - 1; i++) {
				// 條件一：suit 不為 4 ，rank 是 1
				condition[1] = _hand[i].suit() != 4 && _hand[i].rank() == 1;
				// 條件二：前一張跟自己不一樣
				condition[2] = !_hand[i].fromsuitrank(_hand[i - 1].suit(), _hand[i - 1].rank());
				// 條件三：第二張是 2W
				condition[3] = _hand[i].suit() == _hand[i + 1].suit() && _hand[i + 1].rank() == 2;
				// 條件四：第三張不是 3W
				condition[4] = !(_hand[i].suit() == _hand[i + 2].suit() && _hand[i + 2].rank() == 3);
				if (condition[1] && condition[2] && condition[3] && condition[4]) return i;
			}
			// cout << "finish 1W 2W" << endl;

			// 再 8W 9W 這種
			// 看最後一張
			i = _hand.total_len();
			// 條件一：suit 不為 4 ，rank 是 9
			condition[1] = _hand[i].suit() != 4 && _hand[i].rank() == 9;
			// 條件二：倒數第二張是 8W
			condition[2] = _hand[i].suit() == _hand[i - 1].suit() && _hand[i - 1].rank() == 8;
			// 條件三：倒數第三張不是 7W
			condition[3] = !(_hand[i].suit() == _hand[i - 2].suit() && _hand[i - 2].rank() == 7);
			if (condition[1] && condition[2] && condition[3]) return i;

			// 看第二張
			i = _hand.faceup_len() + 1;
			// 條件一：suit 不為 4 ，rank 是 9
			condition[1] = _hand[i].suit() != 4 && _hand[i].rank() == 9;
			// 條件二：前一張是 8W
			condition[2] = _hand[i].suit() == _hand[i + 1].suit() && _hand[i + 1].rank() == 8;
			if (condition[1] && condition[2]) return i;

			// 看其他張
			for (i = _hand.total_len() - 1; i > _hand.faceup_len() + 1; i--) {
				// 條件一：suit 不為 4 ，rank 是 1
				condition[1] = _hand[i].suit() != 4 && _hand[i].rank() == 9;
				// 條件二：後一張跟自己不一樣
				condition[2] = !_hand[i].fromsuitrank(_hand[i + 1].suit(), _hand[i + 1].rank());
				// 條件三：前一張是 8W
				condition[3] = _hand[i].suit() == _hand[i - 1].suit() && _hand[i - 1].rank() == 8;
				// 條件四：前二張不是 7W
				condition[4] = !(_hand[i].suit() == _hand[i - 2].suit() && _hand[i - 2].rank() == 7);
				if (condition[1] && condition[2] && condition[3] && condition[4]) return i;
			}
		}
		return -1;
	}

	int contiSuit123(void) {
		if (_hand.total_len() - _hand.faceup_len() >= 5) {
			extern bool greedyAIPlayer_decidePlay_checkPoint;
			bool condition[10];
			// ***** 處理 3W 4W 這種，但要先確定不是 234 或 345 *****

			if (greedyAIPlayer_decidePlay_checkPoint) cout << "Check: 3W 4W condition." << endl;
			// 先最後兩張
			int i = _hand.total_len() - 1;
			// 條件一：suit 不為 4
			condition[1] = !(_hand[i].suit() == 4);
			// 條件二：下一張是 4W
			condition[2] = _hand[i].suit() == _hand[i + 1].suit() && _hand[i + 1].rank() == _hand[i].rank() + 1;
			// 條件三：前一張不是 2W
			condition[3] = !(_hand[i].suit() == _hand[i - 1].suit() && _hand[i - 1].rank() == _hand[i].rank() - 1);
			// 條件四：前一張不是 3W
			condition[4] = !(_hand[i].suit() == _hand[i - 1].suit() && _hand[i - 1].rank() == _hand[i].rank());
			if (condition[1] && condition[2] && condition[3] && condition[4]) return i;

			// 處理第一張
			i = _hand.faceup_len();
			// 條件一：suit 不為 4
			condition[1] = !(_hand[i].suit() == 4);
			// 條件二：下一張是 4W
			condition[2] = _hand[i].suit() == _hand[i + 1].suit() && _hand[i + 1].rank() == _hand[i].rank() + 1;
			// 條件三：下二張不是 4W
			condition[3] = !(_hand[i].suit() == _hand[i + 2].suit() && _hand[i + 2].rank() == _hand[i].rank() + 1);
			// 條件四：下二張不是 5W
			condition[4] = !(_hand[i].suit() == _hand[i + 2].suit() && _hand[i + 2].rank() == _hand[i].rank() + 2);
			if (condition[1] && condition[2] && condition[3] && condition[4]) return i;

			// 處理其他張
			for (i = _hand.faceup_len() + 1; i < _hand.total_len() - 1; i++) {
				// 條件一：不是 suit 4
				condition[1] = !(_hand[i].suit() == 4);
				// 條件二：下一張是自己 4W
				condition[2] = _hand[i].suit() == _hand[i + 1].suit() && _hand[i + 1].rank() == _hand[i].rank() + 1;
				// 條件三：前一張不是 2W
				condition[3] = !(_hand[i].suit() == _hand[i - 1].suit() && _hand[i - 1].rank() == _hand[i].rank() - 1);
				// 條件四：前一張不是 3W
				condition[4] = !(_hand[i].suit() == _hand[i - 1].suit() && _hand[i - 1].rank() == _hand[i].rank());
				// 條件五：下二張不是 4W
				condition[5] = !(_hand[i].suit() == _hand[i + 2].suit() && _hand[i + 2].rank() == _hand[i].rank() + 1);
				// 條件六：下二張不是 5W
				condition[6] = !(_hand[i].suit() == _hand[i + 2].suit() && _hand[i + 2].rank() == _hand[i].rank() + 2);
				if (condition[1] && condition[2] && condition[3] && condition[4] && condition[5] && condition[6]) return i;
			}
		}
		return -1;
	}

	int decidePlay(void) {
		extern bool greedyAIPlayer_decidePlay_checkPoint;
		// 目前策略：
		// 以下策略請在手牌數量 >= 5 時做，目前 <5 仍無策略
		// 先找落單的東西北中發白 (singleSuit4, id = 1)
		// 再找 rank 是 1 或 9，如果落單先打，定義是非 1, 1... 或 1, 2... 或 1, 3... (singleRank19, id = 2)
		// 再從第一張開始找任何 rank 是落單的 (singleTile, id = 3)
		// 只有 Fa Fa 應先丟，因為只能等 Fa，但應先確定手牌是不是 Fa Fa Fa 以免丟掉 (pairSuit4, id = 4)
		// 如果只有 7W 9W 或 1W 3W 先丟 9W 或 1W (gapSuit123Rank19, id = 5)
		// 如果只有 6W 8W 應先丟，因為只能等 7W (gapSuit123, id = 6)
		// 然後丟 6W 6W 這種，但要先確定是不是 6W 6W 6W (pairSuit123, id = 7)
		// 然後才丟 8W 9W 或 1W 2W 這種，但要先確定不是 789 或 123 (contiSuit123Rank19, id = 8)
		// 然後 3W 4W 這種，但要先確定不是 234 或 345 (contiSuit123, id = 9)
		// 都不是前述狀況的話打第一張牌

		// 這時手中應該會比 total len 多一張牌，所以 arrange 時不會排到最後這張
		_hand.set_total_len(_hand.total_len() + 1);
		_hand.arrange();
		_hand.set_total_len(_hand.total_len() - 1);
		int i = -1;
		int suit, rank;

		for (int j = 0; j < functionOrder.length(); j++) {
			char functionToCall = functionOrder.at(j);
			// cout << functionToCall << endl;
			if (functionToCall == '1') i = singleSuit4();
			if (functionToCall == '2') i = singleRank19();
			if (functionToCall == '3') i = singleTile();
			if (functionToCall == '4') i = pairSuit4();
			if (functionToCall == '5') i = gapSuit123Rank19();
			if (functionToCall == '6') i = gapSuit123();
			if (functionToCall == '7') i = pairSuit123();
			if (functionToCall == '8') i = contiSuit123Rank19();
			if (functionToCall == '9') i = contiSuit123();
			// cout << i << endl;
			if (i != -1) return i;
		}

		// i = singleSuit4();
		// if (i != -1) return i;

		// i = singleRank19();
		// if (i != -1) return i;

		// i = singleTile();
		// if (i != -1) return i;

		// i = pairSuit4();
		// if (i != -1) return i;

		// i = gapSuit123Rank19();
		// if (i != -1) return i;

		// i = gapSuit123();
		// if (i != -1) return i;

		// i = pairSuit123();
		// if (i != -1) return i;

		// i = contiSuit123Rank19();
		// if (i != -1) return i;

		// i = contiSuit123();
		// if (i != -1) return i;

		if (greedyAIPlayer_decidePlay_checkPoint) cout << "Not the above method. Return first tile." << endl;
		// 沒其他判斷方式了，就 return 第一張牌吧
		return _hand.faceup_len();
	}

	bool is_human(void) {
		return false;
	}

	string getFunctionOrder(void) {
		return functionOrder;
	}
	
private:
	string functionOrder = "123456789";
};


class MJCustomAIplayer: public MJplayer {
public:
	MJCustomAIplayer() : MJplayer() {
		// cout << "Call MJCustomAIplayer constructor." << endl;
	}
	MJCustomAIplayer(int money) : MJplayer(money) {
		// cout << "Call MJCustomAIplayer constructor with money." << endl;
	}

	bool defensive = false; // 如果開局牌太爛就優先跟打牌
	void initiate(MJcollection& mjcol) {
		_hand.initial(mjcol);
		int count_lonely = 0;

		int i = _hand.faceup_len();
		int suit = _hand[i].suit();
		int rank = _hand[i].rank();
		if (suit != 4) {
			if (!(_hand[i + 1].fromsuitrank(suit, rank)) &&
			        !(_hand[i + 1].fromsuitrank(suit, rank + 1)) &&
			        !(_hand[i + 1].fromsuitrank(suit, rank + 2))) count_lonely++;
		} else {
			if (!_hand[i + 1].fromsuitrank(suit, rank)) { count_lonely++; }
		}
		for (int i = _hand.faceup_len() + 1; i < _hand.total_len(); i++) {
			suit = _hand[i].suit();
			rank = _hand[i].rank();
			if ((suit == 1 || suit == 2 || suit == 3)) {
				bool previousTile = _hand[i - 1].fromsuitrank(suit, rank - 2) ||
				                    _hand[i - 1].fromsuitrank(suit, rank - 1) ||
				                    _hand[i - 1].fromsuitrank(suit, rank);
				bool nextTile = _hand[i + 1].fromsuitrank(suit, rank) ||
				                _hand[i + 1].fromsuitrank(suit, rank + 1) ||
				                _hand[i + 1].fromsuitrank(suit, rank + 2);
				if (!previousTile && !nextTile) count_lonely++;
			} else { // suit == 4
				if (!_hand[i + 1].fromsuitrank(suit, rank)) { count_lonely++; }
			}
		}
		if (count_lonely > 7) { defensive = true; }
		return;
	}

	void strategy(int position, MJtile t, int &actiontype, int &actionparameter) {
		// Use information from getinfo to decide
		vector<bool> condition(4, false);
		vector<bool> avail(9, false); // Indicator of available actions
		avail = this->get_avail_actions(position, t);

		// type: eat=1 pong=2 minggone=3 angone=4 bugone=5 applique=6
		int s = t.suit();
		int r = t.rank();
		// hu
		if (avail[7]) { actiontype = 7; return; }
		// minggone & pong
		if (avail[3]) {
			// can minggone -> can pong
			// pong first
			// but minggone first if there are pairs of rank-1, rank-2, rank+1, rank+2
			for (int i = _hand.faceup_len(); i < _hand.total_len() - 1; i++) {
				condition[0] = (_hand[i].fromsuitrank(s, r - 2) && _hand[i + 1].fromsuitrank(s, r - 2));
				condition[1] = (_hand[i].fromsuitrank(s, r - 1) && _hand[i + 1].fromsuitrank(s, r - 1));
				condition[2] = (_hand[i].fromsuitrank(s, r + 1) && _hand[i + 1].fromsuitrank(s, r + 1));
				condition[3] = (_hand[i].fromsuitrank(s, r + 2) && _hand[i + 1].fromsuitrank(s, r + 2));
				if (condition[0] || condition[1] || condition[2] || condition[3]) {
					actiontype = 3;
					return;
				}
			}
			actiontype = 2;
			return;

		} else if (avail[2]) {
			// can only pong
			actiontype = 2;
			return;
		}
		// eat
		int method = 0;
		if (avail[1]) {
			actiontype = 1;
			method = _hand.caneat(t);
			switch (method) {
			case 3:
				actionparameter = 1;
				break;
			case 5:
				actionparameter = 1;
				break;
			case 6:
				actionparameter = 2;
				break;
			case 7:
				actionparameter = 1;
				break;
			}
		}
		// play
		if (avail[8]) {
			actiontype = 8;
			actionparameter = this->decidePlay();
			return;
		}
		// nothing
		return;
	};


	int decidePlay(void) {
		// We have current count of playout tiles.
		_hand.set_total_len(_hand.total_len() + 1);
		_hand.arrange();
		_hand.set_total_len(_hand.total_len() - 1);

		// 記錄每張手牌在場上已經出現的次數
		vector<int> hand_occur(30, 0); // hand_occur[i] = i_th tile in hand playout counts
		for (int i = _hand.faceup_len(); i < _hand.total_len() + 1; i++) {
			hand_occur[i] = out[_hand[i].suit() - 1][_hand[i].rank() - 1];
		}
		// 記錄手牌中哪張出現最多次
		int max = 0;
		int max_tile = 0;
		for (int i = _hand.faceup_len(); i < _hand.total_len() + 1; i++) {
			if (hand_occur[i] > max) {
				max = hand_occur[i];
				max_tile = i;
			}
		}

		if (defensive) {
			// 打前一張一樣的牌>打出現過最多次的牌
			for (int i = _hand.faceup_len(); i < _hand.total_len() + 1; i++) {
				if (_hand[i].fromsuitrank(previousTile.suit(), previousTile.rank())) {
					return i;
				}
			}
			return max_tile;
		}

		//大部分應該都跟Greedy策略類似
		//TODO

		return max_tile;
	}


	vector<bool> get_avail_actions(int position, MJtile t) {
		vector<bool> avail(9, false); // Indicator of available actions
		// if 現在出牌的人是上家, check if caneat
		avail[0] = true;
		if (previousPlayer[_position] == position) {
			//cout << "check if caneat: ";
			if (_hand.caneat(t)) {
				avail[1] = true;
			}
		}
		// check if canpong
		if (_hand.canpong(t)) {
			avail[2] = true;
		}

		// check if canminggone
		if (_hand.canminggone(t)) {
			avail[3] = true;
		}

		// check if canbugone
		// not sure if angone is needed
		if (_hand.canbugone(t)) {
			avail[5] = true;
		}

		// check if canhu
		if (_hand.canhu(t)) {
			avail[7] = true;
		}

		// your turn, you can play
		if (position == _position) {
			avail[8] = true;
			// draw stage, you must play
			if (_hand.stage() == 1) {
				avail[0] = false;
			}
		}
		return avail;
	};


	bool is_human(void) {
		return false;
	}
};

#endif
