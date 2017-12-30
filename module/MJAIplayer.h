#ifndef MJAIPLAYER
#define MJAIPLAYER
#include <iostream>
#include "MJplayer.h"
using namespace std;

class MJGreedyAIplayer: public MJplayer {
	// There are two cases that strategy is called:
	//      1. Other player played a tile
	//      2. One must decide which tile to play
	// actiontype: hu=-1 nothing=0 eat=1 pong=2 minggone=3 angone=4 bugone=5
	// actiontype: play=6, actionparameter: index in hand (start from faceup_len)
	void strategy(int position, MJtile t, int &actiontype, int &actionparameter) {
		// Naive and greedy strategy
		// 上家出的牌才能吃
		if (previousPlayer[_position] == position) {
			if (_hand.caneat(t)) {
				// eat 為可以吃的方式
				int eat = _hand.caneat(t);
				actiontype = 1;
				actionparameter = eat;
				return;
			}
		}

		if (position != _position) { // other players playing
			// check if canhu
			if (_hand.canhu(t)) {
				actiontype = -1;
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
		} else { // my own turn before playing
			// Decide which tile to play
			actiontype = 6;
			actionparameter = _hand.faceup_len();
			// 你直接用 _hand[i] 喔XD 我幫你 overload 好了
			for (int i = _hand.faceup_len() + 1; i < _hand.total_len(); i++) {
				int suit = _hand[i].suit();
				int rank = _hand[i].rank();
				// 尋找落單的牌，跟左右兩張都不一樣
				if (!(_hand[i + 1].fromsuitrank(suit, rank)) && !(_hand[i - 1].fromsuitrank(suit, rank))) {
					actionparameter = i;
					return;
				}
			}
		}
		return;
	};
};

class MJCustomAIplayer: public MJplayer {
	void strategy(int position, MJtile t, int &actiontype, int &actionparameter) {
		// Use information from getinfo to decide
		//TODO
	};
};

#endif
