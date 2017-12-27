/****************************************************************************** 
   FileName  [MJgame.h]
   Author    [HsuehYa Huang]
   Synopsis  [define a game] 
******************************************************************************/ 
#ifndef MJGAME
#define MJGAME 

#include <iostream> 
#include <vector>
using namespace std;
#include "MJplayer.h"

class MJgame{
	friend ostream& operator << (ostream&, const MJhand&);
public:
	MJgame();
	~MJgame();
	
	void start();
	void setting();
	void end();
private:
	vector<MJplayer*> _players;
	int rounds;//圈數 一個風為一圈 
	int valueofpoints;//一台多少錢 
}; 

MJgame::MJgame(){
	cout<<endl;
	cout<<"************************************"<<endl;
	cout<<"******Welcome to Taiwan Mahjong*****"<<endl;
	cout<<"************************************"<<endl;
	cout<<endl;
}
MJgame::~MJgame(){
}
void MJgame::setting() {
}

void MJgame::start() {

}

void MJgame::end() {
	cout << "Game End." << endl;
	cout << "Press any button to exit.." << endl;
	cin.get();
}
#endif
