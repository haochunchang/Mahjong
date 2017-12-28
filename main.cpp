#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <Windows.h>

#include "MJtile.h"
#include "Shuffler.h"
#include "MJhand.h"
#include "MJcollection.h"

using std::cout;
using std::cin;
using std::endl;

// test 先不用看
void test(void);

void testhu(void);
void testMinggone(void);
void testBugone(void);
void testAngone(void);
void testInitEatPong(void);

int main(){
    testhu();
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

    return 0;
}
void testhu(void){
    Shuffler s;
    MJtile mjtiles[144];
    s.init();
    s.fill(mjtiles);
    MJcollection mjcol = MJcollection(mjtiles);
    
    // 第一組
    // 二條碰，南碰，一二三萬，四五六萬，四筒對，四五條，input t 為六條
    int hu_1[16] = {117, 118, 119, 49, 50, 52, 66, 67, 68, 85, 86, 101, 102, 103, 118, 119};
    int input_1 = 135;

    // 將 MJhand 照上面 ID 設定
    for(int i=0; i<16; i++){
        mjtiles[i] = MJtile(hu_1[i]);
    }
    MJhand myhand = MJhand(mjtiles, 16);
    myhand.set_faceup_len(3);
    MJtile t = MJtile(input_1);
    // 印出
    cout << myhand;
    cout << t;

    bool canhu = myhand.canhu(t);
    cout << canhu;
    return;
}

void testInitEatPongMinggone(void){
    static MJtile t;
    init:
        Shuffler s;
        MJtile mjtiles[144];
        s.init();
        s.fill(mjtiles);
        MJcollection mjcol = MJcollection(mjtiles);

        // 前 16 個拿去當 MJhand，index 16 ~ 143 當剩下 collection
        MJhand myhand = MJhand(mjtiles, 16);
        for(int i=0; i<16; i++){
            mjcol.drawfronttile();
        }

        cout << "\n\nInitial: \n";
        myhand.initial(mjcol);
        cout << myhand;

    stage0:
        do{
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
        if(action == 'e'){
            cout << "1: (001)   2: (010)    3: (100)\n";
                cin >> method;
                myhand.eat(t, method); cout << "\nAfter eat:\n";
        } else if(action == 'p'){
            myhand.pong(t); cout << "\nAfter pong:\n";
        } else if(action == 'g'){
            myhand.minggone(t, mjcol); cout << "\nAfter minggone:\n";
        } else{
            cout << "\nDo nothing.\n";
        }
        
        cout << myhand;

    stage1:
        if(myhand.stage()==1){
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








void testBugone(void){
    Shuffler s;
    MJtile mjtiles[144];
    s.init();
    s.fill(mjtiles);
    MJcollection mjcol = MJcollection(mjtiles);
    
    // 前 16 個拿去當 MJhand，index 16 ~ 143 當剩下 collection
    MJhand myhand = MJhand(mjtiles, 16);
    for(int i=0; i<16; i++){
        mjcol.drawfronttile();
    }

    cout << "\n\nInitial: \n";
    myhand.initial(mjcol);
    cout << myhand;

    srand(GetTickCount());
    // srand((time(NULL)));
    MJtile t = MJtile(rand()%144+1);
    cout << "input tile: \n";
    cout << t;

    cout << "\ncanpong: ";
    (myhand.canpong(t)) ? (cout << "true\n") : (cout << "false\n");

    if(myhand.canpong(t)){
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
        if(myhand.canbugone(myhand.getLastTile())){
            int index = myhand.getTotalLen()-myhand.getFaceupLen()+1;
            myhand.bugone(index, mjcol);
            cout << "\nAfetr bugone: \n";
            cout << myhand;
            std::cin.get();
        }
    }
}




void testAngone(void){
    Shuffler s;
    MJtile mjtiles[144];
    s.init();
    s.fill(mjtiles);
    MJcollection mjcol = MJcollection(mjtiles);
    
    // 前 16 個拿去當 MJhand，index 16 ~ 143 當剩下 collection
    MJhand myhand = MJhand(mjtiles, 16);
    for(int i=0; i<16; i++){
        mjcol.drawfronttile();
    }

    cout << "\n\nInitial: \n";
    myhand.initial(mjcol);
    cout << myhand;

    srand(GetTickCount());
    // srand((time(NULL)));
    myhand.draw(mjcol);
    cout << "After draw: \n" << myhand;
    cout << "The last tile is\n" << myhand.getLastTile();

    cout << "\ncanangone: ";
    (myhand.canangone(myhand.getLastTile())) ? (cout << "true\n") : (cout << "false\n");

    if(myhand.canangone(myhand.getLastTile())){
        int index = myhand.getTotalLen()-myhand.getFaceupLen()+1;
        myhand.angone(index, mjcol);
        cout << "\nAfetr angone: \n";
        cout << myhand;
        int a;
        cin >> a;
    }
}







void testMinggone(){
    Shuffler s;
    MJtile mjtiles[144];
    s.init();
    s.fill(mjtiles);
    MJcollection mjcol = MJcollection(mjtiles);
    
    // 前 16 個拿去當 MJhand，index 16 ~ 143 當剩下 collection
    MJhand myhand = MJhand(mjtiles, 16);
    for(int i=0; i<16; i++){
        mjcol.drawfronttile();
    }

    cout << "\n\nInitial: \n";
    myhand.initial(mjcol);
    cout << myhand;

    srand(GetTickCount());
    // srand((time(NULL)));
    MJtile t = MJtile(rand()%144+1);
    cout << "input tile: \n";
    cout << t;

    cout << "\ncanminggone: ";
    (myhand.canminggone(t)) ? (cout << "true\n") : (cout << "false\n");

    if(myhand.canminggone(t)){
        myhand.minggone(t, mjcol);
        cout << "After minggone: \n" << myhand; 
        cin.get();
    }
}


// void test() {
//     Shuffler s;
//     MJtile mjtiles[144];
//     s.init();
//     s.fill(mjtiles);
//     MJcollection mjcol = MJcollection(mjtiles);
    
//     // 前 16 個拿去當 MJhand，index 16 ~ 143 當剩下 collection
//     MJhand myhand = MJhand(mjtiles, 16);
//     for(int i=0; i<16; i++){
//         mjcol.drawfronttile();
//     }


//     cout << "\n\nDeclare an MJhand: \n";
//     cout << myhand;

//     cout << "Initial: \n";
//     myhand.initial(mjcol);
//     cout << myhand;
    
//     srand((time(NULL)));
//     MJtile t = MJtile(rand()%144+1);
//     cout << "input tile: \n";
//     cout << t;

//     cout << "\ncaneat: ";
//     switch(myhand.caneat(t)){
//         case 0: cout << "false"; break;
//         case 1: 
//             cout << "(001)"; 
//             myhand.eat(t, 1); 
//             cout << "\nEat with method1 (001)" << endl; 
//             cout << "\n" << myhand; 
//             break;
//         case 2: cout << "(010)"; 
//             myhand.eat(t, 2); 
//             cout << "\nEat with method2 (010)" << endl; 
//             cout << "\n" << myhand; 
//             break;
//         case 3: cout << "(001) (010)"; 
//             myhand.eat(t, 1); 
//             cout << "\nEat with method1 (001)" << endl; 
//             cout << "\n" << myhand; 
//             break;
//         case 4: cout << "(100)"; 
//             myhand.eat(t, 3); 
//             cout << "\nEat with method3 (100)" << endl; 
//             cout << "\n" << myhand; 
//             break;
//         case 5: 
//             cout << "(001) (100)"; 
//             myhand.eat(t, 3); 
//             cout << "\nEat with method3 (100)" << endl; 
//             cout << "\n" << myhand; 
//             break;
//         case 6: 
//             cout << "(010) (100)"; 
//             myhand.eat(t, 3); 
//             cout << "\nEat with method3 (100)" << endl; 
//             cout << "\n" << myhand; 
//             break;
//         case 7: 
//             cout << "(001) (010) (100)"; 
//             myhand.eat(t, 2); 
//             cout << "\nEat with method2 (010)" << endl; 
//             cout << "\n" << myhand; 
//             break;
//     }
//     cout << "\ncanpong: ";
//     (myhand.canpong(t)) ? (cout << "true") : (cout << "false");
//     cout << "\ncanminggone: ";
//     (myhand.canminggone(t)) ? (cout << "true") : (cout << "false");

    // if(myhand.canpong(t)){
    //  myhand.pong(t);
    //  cout << "\n" << myhand;
    // }


    // myhand.draw(mjcol);
    // cout << "\nAfter draw a tile: \n";
    // cout << myhand;

    // myhand.play(1);
    // cout << "\nAfter play a tile: \n";
    // cout << myhand;
// }