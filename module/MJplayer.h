#ifndef MJPLAYER 
#define MJPLAYER 
#include <map>
#include <iostream>
#include "MJhand.h"
using namespace std;
class MJplayer{
	public :
		MJplayer();
		MJplayer(int, int, MJtile*, int); // position, money, tiles, num_tiles
		~MJplayer();
		int Get_Pos() const;
		int Get_Mon() const;
		void Print_Hand() const;
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
