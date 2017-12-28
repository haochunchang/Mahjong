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

	void strategy(int position, MJtile t, int &actiontype, int &actionparameter);
	void getinfo(int position, int type, MJtile* ts, int tiles_num);
	//type: eat=1 pong=2 minggone=3 angone=4 bugone=5 applique=6

private :
	MJhand _hand;
	int _position;//east = 1 ,south = 2 , west = 3 , north = 4
	int _money;
	map<int, int> previousPerson;
	map<int, int> oppositePerson;
};
#endif
