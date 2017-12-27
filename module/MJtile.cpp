// int _suit;//1~4 
// int _rank;//1~9 
// int _flowernum;//1~4 
// int _tileId; //1~144

// sample output
// .__
// | 1|
// | W|
//  --
//  __
// | 2|
// | W|
//  --
// Different tile.


#include<iostream>
#include "MJtile.h"

using std::endl;
using std::cout;
using std::string;

ostream& operator << (ostream&, const MJtile& mj){
    int rank_temp = mj._rank;
    string rank = std::to_string(rank_temp);
    string suit;
    switch(mj._suit){
        case 1: suit = " O";
                break;
        case 2: suit = " I";
                break;
        case 3: suit = " W";
                break;
    }
    if(mj._suit==4){
        if(mj._rank==1){
            suit = "Ea";
            rank = " ";
        }
        else if(mj._rank==2){
            suit = "So";
            rank = " ";
        }
        else if(mj._rank==3){
            suit = "We";
            rank = " ";
        }
        else if(mj._rank==4){
            suit = "No";
            rank = " ";
        }
        else if(mj._rank==5){
            suit = "Mi";
            rank = " ";
        }
        else if(mj._rank==6){
            suit = "Fa";
            rank = " ";
        }
        else if(mj._rank==7){
            suit = "  ";
            rank = " ";
        }
        else if(mj._tileId==125){
            suit = "Me";
            rank = "1";
        }
        else if(mj._tileId==126){
            suit = "La";
            rank = "2";
        }
        else if(mj._tileId==127){
            suit = "Zu";
            rank = "3";
        }
        else if(mj._tileId==128){
            suit = "Ju";
            rank = "4";
        }
        else if(mj._tileId==141){
            suit = "Sp";
            rank = "1";
        }
        else if(mj._tileId==142){
            suit = "Su";
            rank = "2";
        }
        else if(mj._tileId==143){
            suit = "Au";
            rank = "3";
        }
        else if(mj._tileId==144){
            suit = "Wi";
            rank = "4";
        } else{
            assert(false);    
        }
    }

    cout << " __ " << endl;
    cout << "| " << rank << "|" << endl;
    cout << "|" << suit[0] << suit[1] << "|" << endl;
    cout << " -- " << endl;
    return cout;
}

MJtile::MJtile(){
    _tileId = 0;
    _suit = 0;
    _rank = 0;
    _flowernum = 0;
}

MJtile::MJtile(int tileId){
    _tileId = tileId;

    int suit = tileId % 16;
    if(suit==0){
        _suit = 4;
    } else if(suit <= 4){
        _suit = 1;
    } else if(suit <= 8){
        _suit = 2;
    } else if(suit <= 12){
        _suit = 3;
    } else{
        _suit = 4;
    }

    if(tileId % 16 == 0){
        _rank = tileId / 16;
    } else{
        _rank = tileId / 16 + 1;
    }
    _flowernum = 0;

    if(tileId == 125 || tileId == 141)
        _flowernum = 1;
    if(tileId == 126 || tileId == 142)
        _flowernum = 2;
    if(tileId == 127 || tileId == 143)
        _flowernum = 3;
    if(tileId == 128 || tileId == 144)
        _flowernum = 4;

}

MJtile::~MJtile() {}

void MJtile::setfromId(int tileId){
    _tileId = tileId;

    int suit = tileId % 16;
    if(suit==0){
        _suit = 4;
    } else if(suit <= 4){
        _suit = 1;
    } else if(suit <= 8){
        _suit = 2;
    } else if(suit <= 12){
        _suit = 3;
    } else{
        _suit = 4;
    }

    if(tileId % 16 == 0){
        _rank = tileId / 16;
    } else{
        _rank = tileId / 16 + 1;
    }

    _flowernum = 0;

    if(tileId == 125 || tileId == 141)
        _flowernum = 1;
    if(tileId == 126 || tileId == 142)
        _flowernum = 2;
    if(tileId == 127 || tileId == 143)
        _flowernum = 3;
    if(tileId == 128 || tileId == 144)
        _flowernum = 4;
}

int MJtile::suit() const{
    return _suit;
}
/*get suit, 筒子=1, 條子=2, 萬子=3, 風牌花牌=4 */
int MJtile::rank() const{
    return _rank;
}
/*get rank, 1~9*/
int MJtile::flower() const{
    return _flowernum;
}
/*get flower number, if not flower return 0*/
int MJtile::getTileId() const{
    return _tileId;
}
/*get tile id*/
bool MJtile::operator == (const MJtile& mj2) const{
    return (_suit==mj2._suit && _rank==mj2._rank);
}
/*compare same kind of tile, don't need to worry of flower tile*/
bool MJtile::operator != (const MJtile& mj2) const{
    return !(_suit==mj2._suit && _rank==mj2._rank);
}
/*compare not the same kind of tile, don't need to worry of flower tile*/
bool MJtile::fromsuitrank(int suit, int rank) const{
    return (_suit == suit && _rank == rank);
}
/*return true if equal to given suit and rank, don't need to worry of flower tile*/
