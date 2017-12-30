#ifndef MJPLAYER
#define MJPLAYER
#include <map>
#include <iostream>
#include "MJhand.h"
using namespace std;
class MJplayer {
public :
	MJplayer();
	MJplayer(int, int, MJtile*, int); // position, money, tiles, num_tiles
	~MJplayer();

	void Set_Pos(int);
	int Get_Pos() const;

	void Set_Mon(int);
	int Get_Mon() const;

	void Set_Hand(MJtile*, int); // tiles, num_tiles
	void Print_Hand() const;
	void initiate(MJcollection&);
	void draw(MJcollection&);
    MJtile play(int);
    void act(int, int);

	// actiontype: hu=-1 nothing=0 eat=1 pong=2 minggone=3 angone=4 bugone=5
	void strategy(int position, MJtile t, int &actiontype, int &actionparameter);
	// call after every play
	int decidePlay(void);
    void getinfo(int position, int type, MJtile* ts, int tiles_num);
	// type: eat=1 pong=2 minggone=3 angone=4 bugone=5 applique=6
	// call after any type above

protected :
	MJhand _hand;
	int _position;//east = 1 ,south = 2 , west = 3 , north = 4
	int _money;
	map<int, int> previousPlayer;
	map<int, int> oppositePlayer;
};
#endif
