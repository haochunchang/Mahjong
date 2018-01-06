#include <iostream>
#include <map>
#include <vector>
#include "MJplayer.h"
#include "MJhand.h"
#include "MJcollection.h"

using namespace std;

MJplayer::MJplayer() {
    // cout << "Call MJplayer constructor." << endl;
    _position = 0;
    _money = 0;
    _hand = MJhand();

    // 1 的上家是 4，2 的上家是 1，3 的上家是 2，4 的上家是 3
    previousPlayer[1] = 4;
    previousPlayer[2] = 1;
    previousPlayer[3] = 2;
    previousPlayer[4] = 3;

    // 1 的對家是 3，3 的對家是 1，2 的對家是 4，4 的對家是 2
    oppositePlayer[1] = 3;
    oppositePlayer[3] = 1;
    oppositePlayer[2] = 4;
    oppositePlayer[4] = 2;
}

MJplayer::MJplayer(int pos, int money, MJtile* t, int n) {
    cout << "Call MJplayer constructor with input value." << endl;
    _position = pos;
    _money = money;
    _hand = MJhand(t, n);

    // 1 的上家是 4，2 的上家是 1，3 的上家是 2，4 的上家是 3
    previousPlayer[1] = 4;
    previousPlayer[2] = 1;
    previousPlayer[3] = 2;
    previousPlayer[4] = 3;

    // 1 的對家是 3，3 的對家是 1，2 的對家是 4，4 的對家是 2
    oppositePlayer[1] = 3;
    oppositePlayer[3] = 1;
    oppositePlayer[2] = 4;
    oppositePlayer[4] = 2;
}


MJplayer::~MJplayer() {
}


void MJplayer::Set_Pos(int position) {
    _position = position;
    return;
}


int MJplayer::Get_Pos() const {
    return _position;
}


void MJplayer::Set_Mon(int money) {
    _money = money;
    return;
}


int MJplayer::Get_Mon() const {
    return _money;
}


int MJplayer::faceup_len() const {
    return _hand.faceup_len();
}

void MJplayer::Set_Hand(MJtile* mjtiles, int number) {
    _hand = MJhand(mjtiles, number);
    return;
}


void MJplayer::Print_Hand() const {
    cout << _hand;
    return;
}


void MJplayer::initiate(MJcollection& mjcol) {
    _hand.initial(mjcol);
    return;
}


void MJplayer::draw(MJcollection& mjcol) {
    _hand.draw(mjcol);
    return;
}


MJtile MJplayer::play(int index) {
    return _hand.play(index);
}

// actiontype: nothing=0 eat=1 pong=2 minggone=3 angone=4 bugone=5 hu=7
void MJplayer::act(int type, int param, MJtile& t, MJcollection& mjcol) {
    switch (type) {
    case 7:
        if (param == 2)
            _hand.huother(t);
        if (param == 1)
            _hand.huown();
        break;
    case 1:
        _hand.eat(t, param);
        break;
    case 2:
        _hand.pong(t);
        break;
    case 3:
        _hand.minggone(t, mjcol);
        break;
    default:
        return;
    }
    return;
}


void MJplayer::strategy(int position, MJtile t, int &actiontype, int &actionparameter) {
    // call after every play
    // This one is for human player
    int method = 0;
    map<int, string> action_map;
    action_map[0] = "play";
    action_map[1] = "eat";
    action_map[2] = "pong";
    action_map[3] = "minggone";
    action_map[5] = "bugone";
    action_map[7] = "hu";
    vector<bool> avail(8, false); // Indicator of available actions
    avail[0] = true;
    // if 現在出牌的人是上家, check if caneat
    if (previousPlayer[_position] == position) {
        cout << "check if caneat: ";
        if (_hand.caneat(t)) { 
            avail[1] = true; 
            method = _hand.caneat(t);
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
        }
    }
    // check if canpong
    cout << "check if canpong: ";
    if (_hand.canpong(t)) {
        avail[2] = true;    
    }

    // check if canminggone
    cout << "check if canminggone: ";
    if (_hand.canminggone(t)) {
        avail[3] = true;    
    }

    // check if canbugone
    // not sure if angone is needed
    cout << "check if canbugone: ";
    if (_hand.canbugone(t)) {
        avail[5] = true;    
    }

    // check if canhu
    cout << "check if canhu: ";
    if (_hand.canhu(t)) {
        avail[7] = true;
    }

    // Prompt user to choose from available actions
    for (int i = 7; i >= 0; i--) {
        fprintf(stdout, "You can do the following actions:\n");
        if (avail[i]) {
            fprintf(stdout, "%d: %s\n", i, action_map[i].c_str());    
        }
    }
    int a = 0;
    fprintf(stdout, "Please choose one of the actions above:\n");
    fscanf(stdin, "%d", &a);
    if (a > 7 || a < 0 || !avail[a]) {
        fprintf(stdout, "Invalid action: (0~7) and available\n");
        fprintf(stdout, "Default: play a tile\n");
        a = 0;
    }
    if (a == 0) {
        actiontype = 8;
        int index = this->decidePlay();
	    int number = index - this->faceup_len() + 1;
        actionparameter = number;
    } else if (a == 1) {
        actiontype = a;
        actionparameter = method;
    } else {
        actiontype = a;    
    }
    return;
}


int MJplayer::decidePlay(void) {
    int pos = 0;
    cout << "Which tile do you want to play?" << endl;
    cout << _hand;
    cin >> pos;
    return pos;
}


void MJplayer::getinfo(int position, int type, MJtile* ts, int tiles_num) {
    // type: eat=1 pong=2 minggone=3 angone=4 bugone=5 applique=6
    // call after any type above
    return;
}


