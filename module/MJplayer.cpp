#include <iostream>
#include "MJplayer.h"

using namespace std;

MJplayer::MJplayer() {
    _position = 0;
    _money = 0;
}

MJplayer::MJplayer(int pos, int money, MJtile* t, int n) {
    _position = pos;
    _money = money;
    _hand = MJhand(t, n); 
}

MJplayer::~MJplayer() {
}

MJplayer::Get_Pos() const {
    return _position;
}

MJplayer::Get_Mon() const {
    return _money;
}

MJplayer::Print_Hand() {
    cout << _hand << endl;       
}

MJplayer::strategy(int position, MJtile t) {
    //if 現在出牌的人是上家, check if caneat
    // check if canpong, cangone, canhu
}

MJplayer::getinfo(int position, MJtile* ts, int tiles_num) {
     
}


