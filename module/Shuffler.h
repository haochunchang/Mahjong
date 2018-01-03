#ifndef SHUFFLER_H
#define SHUFFLER_H

#include "MJtile.h"
#include <ctime>
#include <cstdlib>
// #include <Windows.h>


class Shuffler{
private:
	MJtile rand_mj[144];
public:
	Shuffler() {}

	~Shuffler() {}

	void init() {
		// random seed
		srand((time(NULL)));
		// srand(GetTickCount());
		// generate 144 MJtile in order
		for(int i=0; i<144; i++){
			rand_mj[i] = MJtile(i+1);
		}

		// random swap in order
		
		for(int j=0; j<144; j++){
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
		for(int i=0; i<144; i++){
			mj[i] = rand_mj[i];
		}
	}

};

#endif
