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
#include "MJstage.h"

class MJgame {
    friend ostream& operator << (ostream&, const MJhand&);
public:
    MJgame();
    ~MJgame();

    void start();
    void setting();
    void end();
private:
    int rounds;
    int valueofpoints;
    MJstage stage;
};

MJgame::MJgame() {
    cout << endl;
    cout << "************************************" << endl;
    cout << "******Welcome to Taiwan Mahjong*****" << endl;
    cout << "************************************" << endl;
    cout << endl;
    rounds = 0;
    valueofpoints = 1;
};

MJgame::~MJgame() {
};

void MJgame::setting() {
    int human = -1;
    while (human > 4 || human < 0) {
        fprintf(stdout, "How many human players? (0~4)\n");
        fscanf(stdin, "%d", &human);
        if (human < 0) {
            fprintf(stdout, "Too few human players, please input 0~4\n");
        } else if (human > 4) {
            fprintf(stdout, "Too many human players, please input 0~4\n");
        }
    }

    int isAIgreedy = 1;
    if (human != 4) {
        fprintf(stdout, "There will be %d AI players\n What kind of AI do you want?\n", 4 - human);
        fprintf(stdout, "1: GreedyAI, 0: CustomAI\n");
        fscanf(stdin, "%d", &isAIgreedy);
    }
    stage = MJstage(human, isAIgreedy);
    return;
}

void MJgame::start() {
    stage.pickSeat();
    cout << endl;
    stage.pickBookmaker();
    cout << endl;
    stage.getTiles();
    cout << endl;
    stage.initiate();
    cout << endl;
    stage.mainGame(rounds);
    return;
}

void MJgame::end() {
    cout << "Game End." << endl;
    cout << "Press any button to exit.." << endl;
    cin.get();
    return;
}
#endif
