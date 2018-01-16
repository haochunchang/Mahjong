#ifndef SHUFFLER_H
#define SHUFFLER_H

#include "MJtile.h"
#include <ctime>
#include <cstdlib>

// #ifdef WINDOWS
#include <Windows.h>
// #endif

class Shuffler {
private:
	MJtile rand_mj[144];
	int seed;
public:
	Shuffler() {}

	~Shuffler() {}

	void init() {
		// seed = time(NULL);
		// #ifdef WINDOWS
		seed = GetTickCount(); // in Windows.h
		// #endif
		// cout << seed << endl;
		srand(seed);

		// generate 144 MJtile in order
		for (int i = 0; i < 144; i++) {
			rand_mj[i] = MJtile(i + 1);
		}

		// random swap in order

		for (int j = 0; j < 144; j++) {
			int r = rand() % 144;
			MJtile temp = rand_mj[j];
			rand_mj[j] = rand_mj[r];
			rand_mj[r] = temp;
		}

		// for(int k=0; k<144; k++){
		// 	cout << rand_mj[k].getTileId() << endl;
		// }

	}

	void fill(MJtile mj[]) {
		for (int i = 0; i < 144; i++) {
			mj[i] = rand_mj[i];
		}
	}

	int getSeed(void){
		return seed;
	}

};

#endif
