/******************************************************************************
   FileName  [MJgame.h]
   Author    [HsuehYa Huang]
   Synopsis  [define a game]
******************************************************************************/
#ifndef MJGAME
#define MJGAME

#include <iostream>
#include <vector>
#include <limits>
using namespace std;
#include "MJplayer.h"
#include "MJstage.h"

int MAX = numeric_limits<int>::max();

class MJgame {
    friend ostream& operator << (ostream&, const MJhand&);
public:
    MJgame();
    ~MJgame();

    void start();
    //void setting();
    void end();
private:
    int rounds; // 圈數
    int valueofpoints; // 每台多少錢
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
    
    cout << "Start Game Setting..." << endl;
    int human = -1;
    int times = 0;
    while ((human > 4 || human < 0) && times < 5) {
        fprintf(stdout, "How many human players? (0~4)\n");
        fscanf(stdin, "%d", &human);
        if (human < 0) {
            fprintf(stdout, "Too few human players, please input 0~4\n");
            times++;
        } else if (human > 4) {
            fprintf(stdout, "Too many human players, please input 0~4\n");
            times++;
        }
    }
    if (times == 5) { fprintf(stdout, "I am out of patience...\n"); return; }
    int isAIgreedy = 1;
    if (human != 4) {
        fprintf(stdout, "There will be %d AI players\n What kind of AI do you want?\n", 4 - human);
        fprintf(stdout, "1: GreedyAI, 0: CustomAI\n");
        fscanf(stdin, "%d", &isAIgreedy);
    }

    int money = 0;
    fprintf(stdout, "How many rounds do you want to play? (default: 1)\n");
    fscanf(stdin, "%d", &rounds);
    if (rounds <= 0 || rounds > MAX) { rounds = 1; }
    fprintf(stdout, "How much money do every player has? (default: 10000)\n");
    fscanf(stdin, "%d", &money);
    if (money <= 0 || money > MAX) { money = 10000; }

    MJstage stage(human, isAIgreedy, money);
    return;
};

MJgame::~MJgame() {
};


void MJgame::start() {
    stage.pickSeat();
    cout << endl;
    stage.pickBookmaker();
    cout << endl;
    int num_rounds = 0;
    int winner = -1;
    for (int i = 0; i < rounds*4; i++) {
        stage.getTiles();
        cout << endl;
        stage.initiate();
        cout << endl;
        winner = stage.mainGame(num_rounds);
        if (winner != stage.getBookmaker()){    
            stage.nextBookmaker();
        }
        winner = -1;
        num_rounds = 0;
        stage.clear();
    }
    return;
}

void MJgame::end() {
    cout << "Game End." << endl;
    cout << "===== Final Result =====" << endl;
    for (int i = 0; i < 4; i++) {
        fprintf(stdout, "Player %d: $ %d\n", i, stage.get_money(i));    
    }
    cout << "============" << endl;
    // cin.get();
    return;
}
#endif
