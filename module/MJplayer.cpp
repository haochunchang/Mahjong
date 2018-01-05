#include <iostream>
#include <map>
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

    // if 現在出牌的人是上家, check if caneat
    if (previousPlayer[_position] == position) {
        cout << "check if caneat: ";
        (_hand.caneat(t)) ? (cout << "true" << endl) : (cout << "false" << endl);
    }
    // check if canpong
    cout << "check if canpong: ";
    (_hand.canpong(t)) ? (cout << "true" << endl) : (cout << "false" << endl);

    // check if canminggone
    cout << "check if canminggone: ";
    (_hand.canminggone(t)) ? (cout << "true" << endl) : (cout << "false" << endl);

    // check if canbugone
    // not sure if angone is needed
    cout << "check if canbugone: ";
    (_hand.canbugone(t)) ? (cout << "true" << endl) : (cout << "false" << endl);

    // check if canhu
    cout << "check if canhu: ";
    (_hand.canhu(t)) ? (cout << "true" << endl) : (cout << "false" << endl);

    return;
}


int MJplayer::decidePlay(void) {
    int pos = 0;
    cout << _hand;
    cout << "Which tile do you want to play?" << endl;
    cin >> pos;
    return pos;
}


void MJplayer::getinfo(int position, int type, MJtile* ts, int tiles_num) {
    // type: eat=1 pong=2 minggone=3 angone=4 bugone=5 applique=6
    // call after any type above
    return;
}


