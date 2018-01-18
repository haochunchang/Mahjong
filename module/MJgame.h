/*
   FileName  [MJgame.h]
   Author    [HsuehYa Huang]
   Synopsis  [define a game]
*/
#ifndef MJGAME
#define MJGAME

#include <iostream>
#include <vector>
#include <limits>
#include "MJplayer.h"
#include "MJAIplayer.h"
#include "MJstage.h"
#include "Debug.h"
using namespace std;

int MAX = numeric_limits<int>::max();
bool extern gaming_UI;

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
    vector<int> gun_count;  // count of 放槍

    MJstage stage;
};

// void clear_screen()
// {
// #ifdef WINDOWS
//     std::system("cls");
// #else
//     // Assume POSIX
//     std::system("clear");
// #endif
// }

MJgame::MJgame() {
    clear_screen();
    cout << "=====================================" << endl;
    cout << "===== Welcome to Taiwan Mahjong =====" << endl;
    cout << "=====================================" << endl;
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
    extern bool is_holding;
    if (is_holding) cout << "Press any key to continue...";
    hold();
};

MJgame::MJgame(int human, int isAIgreedy, int round_in, int money) {
    clear_screen();
    cout << "=====================================" << endl;
    cout << "===== Welcome to \033[1;91mTaiwan Mahjong\033[0m =====" << endl;
    cout << "=====================================" << endl;
    cout << endl;

    if (gaming_UI) {
        cout << "Auto set \033[1;93m" << human << "\033[0m humans."  << endl;
        cout << "Other players are ";
        if (isAIgreedy) cout << "\033[1;93mgreedy AI\033[0m." << endl;
        else cout << "\033[1;93mcustom AI\033[0m." << endl;
        cout << "Will play \033[1;93m" << round_in << "\033[0m rounds." << endl;
        cout << "Press \033[1;93mEnter\033[0m to continue...";
        cin.sync();
        cin.get();
    } else {
        cout << "  Set\thuman = " << human << endl;
        cout << "\tisAIgreedy = " << isAIgreedy << endl;
        cout << "\trounds = " << round_in << endl;
        cout << "\tmoney = " << money << endl;
    }

    rounds = round_in;
    // 原來的錯誤寫法
    // MJstage stage(human, isAIgreedy, money);
    stage = MJstage(human, isAIgreedy, money);
    extern bool is_holding;
    if (is_holding) cout << "\nPress any key to continue...";
    hold();
}


MJgame::~MJgame() {
};


void MJgame::start() {
    extern bool print_result;

    stage.pickSeat();
    hold();
    // cout << endl;

    stage.pickBookmaker();
    hold();
    // cout << endl;

    int init_book = stage.getBookmaker();
    int num_rounds = 0;
    pair<int, int> w_and_l = make_pair(-1, -1);
    int winner = -1;
    int loser = -1;
    vector<int> hu(4, 0);
    vector<int> put_gun(4, 0);

    for (int i = 0; i < rounds; i++) {
        if(gaming_UI) clear_screen();
        if (print_result) {
            cout << "Enter " << i + 1;
            if (i % 10 == 0) cout << "st";
            else if (i % 10 == 1) cout << "nd";
            else if (i % 10 == 2) cout << "rd";
            else cout << "th";
            cout << " round." << endl;
            cout << "Press \033[1;93mEnter\033[0m to continue...";
            cin.get();
        }
        while (true) {
            stage.getTiles();
            hold();
            stage.initiate();
            hold();
            // cout << endl;

            w_and_l = stage.mainGame(num_rounds);
            winner = w_and_l.first;
            loser = w_and_l.second;
            if (print_result) {
                clear_screen();
                cout << "\n================================" << endl;
                if (winner == -1) {
                    cout << "============= 流局 =============" << endl;
                } else {
                    if (loser == -1) {
                        cout << "====== " << "player[" << winner << "] 自摸^ ^! ======" << endl;
                    } else {
                        cout << "====== " << "player[" << winner << "] 胡牌^ ^! ======" << endl;
                        cout << "====== " << "player[" << loser << "] 放槍QAQ! ======" << endl;
                    }
                }
                cout << "================================" << endl;
                //cin.get();
            }
            if (winner != -1) hu[winner] += 1;
            if (loser != -1) put_gun[loser] += 1;
            if (winner != stage.getBookmaker() && winner != -1) {
                stage.nextBookmaker();
                if (stage.getBookmaker() == init_book) {
                    stage.clear();
                    break;
                }
            }
            winner = -1;
            num_rounds = 0;
            stage.clear();
        }
    }

    for (int i = 0; i < hu.size(); i++) {
        hu_count.push_back(hu[i]);
        gun_count.push_back(put_gun[i]);
    }
    return;
}

void MJgame::end() {
    cout << "\nGame End." << endl;
    cout << "============ Final Result ============" << endl;
    for (int i = 0; i < 4; i++) {
        fprintf(stdout, "Player %d: $ %d, #hu: %d, #gun: %d\n",
                i, stage.get_money(i), hu_count[i], gun_count[i]);
    }
    cout << "======================================" << endl;
    // hold();
    return;
}
#endif
