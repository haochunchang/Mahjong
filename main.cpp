#include <iostream>
#include <ctime>
#include <cstdlib>
//#include <Windows.h>

#include "module/MJtile.h"
#include "module/Shuffler.h"
#include "module/MJhand.h"
#include "module/MJcollection.h"
#include "module/MJplayer.h"
#include "module/MJgame.h"

using std::cout;
using std::cin;
using std::endl;

void mainGame(void);
void testhu(void);
void testMinggone(void);
void testBugone(void);
void testAngone(void);
void testInitEatPong(void);

int main() {
	// testhu();

	// while (true) {
	// 	mainGame();
	// }
	mainGame();
	return 0;
}



// testInitEatPongMinggone();

// while(true){
//     testMinggone();
//     Sleep(1);
// }

// while(true){
//     testBugone();
//     Sleep(1);
//  }

// while(true){
//     testAngone();
//     Sleep(1);
// }

void mainGame(void) {
	MJgame mygame(0, 1, 1, 10000);
	// mygame.setting();
	mygame.start();
	mygame.end();
	return;
}

void testhu(void) {
	Shuffler s;
	MJtile mjtiles[144];
	s.init();
	s.fill(mjtiles);
	MJcollection mjcol = MJcollection(mjtiles);

	/*
	// 第一組
	// 二條碰 / 南碰，一二三萬，四五六萬，四筒對，四五條，input t 為六條
	int hu[] = {117, 118, 119, 49, 50, 52, 66, 67, 68, 85, 86, 101, 102, 103, 118, 119};
	int input = 135;
	int faceup_len = 3;
	*/

	// 第二組（mainGame 時發現問題）
	// 秋，二條碰 / 一條碰，三條槓，七條對，九條，發碰，input t 為八條
	int hu[] = {143, 21, 22, 23, 5, 6, 7, 37, 38, 39, 40, 101, 102, 136, 93, 94, 95};
	int input = 117;
	int faceup_len = 4;


	// 將 MJhand 照上面 ID 設定
	int hu_length = sizeof(hu) / sizeof(hu[0]);

	for (int i = 0; i < hu_length; i++) {
		mjtiles[i] = MJtile(hu[i]);
	}
	MJhand myhand = MJhand(mjtiles, hu_length);
	myhand.set_faceup_len(faceup_len);
	MJtile t = MJtile(input);
	// 印出
	cout << myhand;
	cout << t;

	bool canhu = myhand.canhu(t);
	canhu ? (cout << "canhu!\n") : (cout << "cannot hu :(\n");
	return;
}

void testInitEatPongMinggone(void) {
	static MJtile t;
init:
	Shuffler s;
	MJtile mjtiles[144];
	s.init();
	s.fill(mjtiles);
	MJcollection mjcol = MJcollection(mjtiles);

	// 前 16 個拿去當 MJhand，index 16 ~ 143 當剩下 collection
	MJhand myhand = MJhand(mjtiles, 16);
	for (int i = 0; i < 16; i++) {
		mjcol.drawfronttile();
	}

	cout << "\n\nInitial: \n";
	myhand.initial(mjcol);
	cout << myhand;

stage0:
	do {
		// simulate the opponent
		t = mjcol.drawbacktile();
		cout << "\n\nThe opponent play tile\n" << t;

		cout << "Test caneat: ";
		(myhand.caneat(t)) ? (cout << "true\n") : (cout << "false\n");
		cout << "Test canpong: ";
		(myhand.canpong(t)) ? (cout << "true\n") : (cout << "false\n");
		cout << "Test canminggone: ";
		(myhand.canminggone(t)) ? (cout << "true\n") : (cout << "false\n");
	} while (!myhand.caneat(t) && !myhand.canpong(t) && !myhand.canminggone(t));
	cout << "\"e\" to eat, \"p\" to pong, and \"g\" to minggone: ";
	char action;
	cin >> action;
	int method;
	if (action == 'e') {
		cout << "1: (001)   2: (010)    3: (100)\n";
		cin >> method;
		myhand.eat(t, method); cout << "\nAfter eat:\n";
	} else if (action == 'p') {
		myhand.pong(t); cout << "\nAfter pong:\n";
	} else if (action == 'g') {
		myhand.minggone(t, mjcol); cout << "\nAfter minggone:\n";
	} else {
		cout << "\nDo nothing.\n";
	}

	cout << myhand;

stage1:
	if (myhand.stage() == 1) {
		cout << "Test canangone: ";
		(myhand.canangone(myhand.getLastTile())) ? (cout << "true\n") : (cout << "false\n");
		cout << "Test canbugone: ";
		(myhand.canbugone(myhand.getLastTile())) ? (cout << "true\n") : (cout << "false\n");

		cout << "\nWhich tile would you want to play? ";
		int index;
		cin >> index;
		myhand.play(index);
		cout << myhand;
	}
	cout << "Press enter to continue.\n";
	cin.sync();
	cin.get();
	goto stage0;
	return;
}








void testBugone(void) {
	Shuffler s;
	MJtile mjtiles[144];
	s.init();
	s.fill(mjtiles);
	MJcollection mjcol = MJcollection(mjtiles);

	// 前 16 個拿去當 MJhand，index 16 ~ 143 當剩下 collection
	MJhand myhand = MJhand(mjtiles, 16);
	for (int i = 0; i < 16; i++) {
		mjcol.drawfronttile();
	}

	cout << "\n\nInitial: \n";
	myhand.initial(mjcol);
	cout << myhand;

	//srand(GetTickCount());
	srand((time(NULL)));
	MJtile t = MJtile(rand() % 144 + 1);
	cout << "input tile: \n";
	cout << t;

	cout << "\ncanpong: ";
	(myhand.canpong(t)) ? (cout << "true\n") : (cout << "false\n");

	if (myhand.canpong(t)) {
		myhand.pong(t);
		cout << "\nAfter pong: \n";
		cout << myhand;
		myhand.play(1);
		myhand.draw(mjcol);
		cout << "After play(1) and draw a new tile:\n" << myhand;
		// cin.get();

		// cout << "The last tile is " << endl;
		// cout << myhand.getLastTile();
		// cout << "\ncanbugone: ";
		// (myhand.canbugone(myhand.getLastTile())) ? (cout << "true\n") : (cout << "false\n");
		if (myhand.canbugone(myhand.getLastTile())) {
			int index = myhand.getTotalLen() - myhand.getFaceupLen() + 1;
			myhand.bugone(index, mjcol);
			cout << "\nAfetr bugone: \n";
			cout << myhand;
			std::cin.get();
		}
	}
}




void testAngone(void) {
	Shuffler s;
	MJtile mjtiles[144];
	s.init();
	s.fill(mjtiles);
	MJcollection mjcol = MJcollection(mjtiles);

	// 前 16 個拿去當 MJhand，index 16 ~ 143 當剩下 collection
	MJhand myhand = MJhand(mjtiles, 16);
	for (int i = 0; i < 16; i++) {
		mjcol.drawfronttile();
	}

	cout << "\n\nInitial: \n";
	myhand.initial(mjcol);
	cout << myhand;

	//srand(GetTickCount());
	srand((time(NULL)));
	myhand.draw(mjcol);
	cout << "After draw: \n" << myhand;
	cout << "The last tile is\n" << myhand.getLastTile();

	cout << "\ncanangone: ";
	(myhand.canangone(myhand.getLastTile())) ? (cout << "true\n") : (cout << "false\n");

	if (myhand.canangone(myhand.getLastTile())) {
		int index = myhand.getTotalLen() - myhand.getFaceupLen() + 1;
		myhand.angone(index, mjcol);
		cout << "\nAfetr angone: \n";
		cout << myhand;
		int a;
		cin >> a;
	}
}







void testMinggone() {
	Shuffler s;
	MJtile mjtiles[144];
	s.init();
	s.fill(mjtiles);
	MJcollection mjcol = MJcollection(mjtiles);

	// 前 16 個拿去當 MJhand，index 16 ~ 143 當剩下 collection
	MJhand myhand = MJhand(mjtiles, 16);
	for (int i = 0; i < 16; i++) {
		mjcol.drawfronttile();
	}

	cout << "\n\nInitial: \n";
	myhand.initial(mjcol);
	cout << myhand;

	//srand(GetTickCount());
	srand((time(NULL)));
	MJtile t = MJtile(rand() % 144 + 1);
	cout << "input tile: \n";
	cout << t;

	cout << "\ncanminggone: ";
	(myhand.canminggone(t)) ? (cout << "true\n") : (cout << "false\n");

	if (myhand.canminggone(t)) {
		myhand.minggone(t, mjcol);
		cout << "After minggone: \n" << myhand;
		cin.get();
	}
}
