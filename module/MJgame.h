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
#include "MJAIplayer.h"
#include "MJstage.h"

int MAX = numeric_limits<int>::max();

class MJgame {
    friend ostream& operator << (ostream&, const MJhand&);
public:
    MJgame();
    MJgame(int, int, int, int);
    ~MJgame();
    void start();
    void end();

private:
    int rounds;             // 圈數
    int valueofpoints;      // 每台多少錢
    vector<int> hu_count;   // count of hu

    MJstage stage;
};

void clear_screen()
{
#ifdef WINDOWS
    std::system("cls");
#else
    // Assume POSIX
    std::system("clear");
#endif
}

MJgame::MJgame() {
    clear_screen();
    cout << endl;
    cout << "************************************" << endl;
    cout << "***** Welcome to Taiwan Mahjong ****" << endl;
    cout << "************************************" << endl;
    cout << endl;
    rounds = 0;
    valueofpoints = 1;

    cout << "Start Game Setting..." << endl;
    int human = -1;
    int times = 0;
    int isAIgreedy = -1;
    int money = 0;

    // set human number
    while ((human > 4 || human < 0) && times < 5) {
        fprintf(stdout, "How many human players? (0-4)  ");
        fscanf(stdin, "%d", &human);
        if (human < 0) {
            times++;
            if (times == 5) {
                fprintf(stdout, "I am out of patience...\n");
                exit(1);
            }
            fprintf(stdout, "Too few human players, please input 0-4.\n\n");
        } else if (human > 4) {
            times++;
            if (times == 5) {
                fprintf(stdout, "I am out of patience...\n");
                exit(1);
            }
            fprintf(stdout, "Too many human players, please input 0-4.\n\n");
        }
    }


    // set AI players number
    times = 0;
    if (human != 4) {
        fprintf(stdout, "There will be %d AI players.\n\nWhat kind of AI do you want?\n", 4 - human);
        while (!(isAIgreedy == 0 || isAIgreedy == 1) && times < 5) {
            fprintf(stdout, "(1: GreedyAI, 0: CustomAI)  ");
            fscanf(stdin, "%d", &isAIgreedy);
            if (!(isAIgreedy == 0 || isAIgreedy == 1)) {
                times++;
                if (times == 5) {
                    fprintf(stdout, "I am out of patience...\n");
                    exit(1);
                }
                fprintf(stdout, "Invalid input. Please input 0 or 1.\n\n");
            }
        }
    }


    // set rounds
    fprintf(stdout, "\nHow many rounds do you want to play? (default: 1)  ");
    fscanf(stdin, "%d", &rounds);
    if (rounds <= 0 || rounds > MAX) {
        cout << "Invalid input. Automatically set rounds to 1.\n";
        rounds = 1;
    }

    // set money
    fprintf(stdout, "How much money do every player has? (default: 10000)  ");
    fscanf(stdin, "%d", &money);
    if (money <= 0 || money > MAX) {
        cout << "Invalid input. Automatically set money to 10000.\n";
        money = 10000;
    }

    // 原來的錯誤寫法
    // MJstage stage(human, isAIgreedy, money);
    stage = MJstage(human, isAIgreedy, money);
    cout << "Press any key to continue...";
    cin.sync();
    cin.get();
    cout << endl;
};

MJgame::MJgame(int human, int isAIgreedy, int round_in, int money) {
    clear_screen();
    cout << endl;
    cout << "************************************" << endl;
    cout << "***** Welcome to Taiwan Mahjong ****" << endl;
    cout << "************************************" << endl;
    cout << endl;

    cout << "  Set\thuman = " << human << endl;
    cout << "\tisAIgreedy = " << isAIgreedy << endl;
    cout << "\trounds = " << round_in << endl;
    cout << "\tmoney = " << money << endl;
    rounds = round_in;
    // 原來的錯誤寫法
    // MJstage stage(human, isAIgreedy, money);
    stage = MJstage(human, isAIgreedy, money);

    cout << "\nPress any key to continue...";
    cin.sync();
    cin.get();
    cout << endl;
}


MJgame::~MJgame() {
};


void MJgame::start() {
    stage.pickSeat();
    cin.get();
    // cout << endl;

    stage.pickBookmaker();
    cin.get();
    // cout << endl;

    int init_book = stage.getBookmaker();
    int num_rounds = 0;
    int winner = -1;
    vector<int> hu(4, 0);

    for (int i = 0; i < rounds; i++) {
        while (true) {
            stage.getTiles();
            cout << endl;

            stage.initiate();
            cin.get();
            // cout << endl;

            winner = stage.mainGame(num_rounds);
            if (winner != -1) hu[winner] += 1;
            if (winner != stage.getBookmaker() && winner != -1) {
                stage.nextBookmaker();
                if (stage.getBookmaker() == init_book) break;
            }
            winner = -1;
            num_rounds = 0;
            stage.clear();
        }
    }

    for (int i = 0; i < hu.size(); i++) {
        hu_count.push_back(hu[i]);
    }
    return;
}

void MJgame::end() {
    cout << "Game End." << endl;
    cout << "===== Final Result =====" << endl;
    for (int i = 0; i < 4; i++) {
        fprintf(stdout, "Player %d: $ %d, #hu: %d\n", i, stage.get_money(i), hu_count[i]);
    }
    cout << "========================" << endl;
    // cin.get();
    return;
}
#endif
