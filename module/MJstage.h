#ifndef MJSTAGE
#define MJSTAGE

#include "MJAIplayer.h"

using namespace std;

// In TA's dependency graph
// MJstage includes
// 1. Dealing phase
// 2. Stretagy phase
// 3. Check cangong phase
// 4. comparison phase
// 5. shooting phase

// All the rules refer to the following webpage
// http://www.dragona.com.tw/mahjong-rule/

// void pickSeat(void);
// void pickBookmaker(void);
// void getTiles(void);
// void initiate(void);
// void mainGame(void);
// void countTai(void);

class MJstage {
public:
	MJstage(void);				// 還不確定要怎寫
	MJstage(int, int);
    void pickSeat(void);		// done
	void pickBookmaker(void);	// done
	void getTiles(void);		// done
	void initiate(void);		// done
	void mainGame(int&);
	void countTai(void);

private:
	vector<MJplayer> _players;
	// It's a vector by TA originally
	int _bookmaker;
	// player index (0 - 3), not position (1 - 4)
	MJcollection mjcol;
	map<int, int> playerToPos;
	map<int, int> posToPlayer;
};


#endif
