#include <iostream> 
#include <algorithm>
#include <cassert>
#include <vector>
#include "MJhand.h"
#include "MJtile.h"
#include "MJcollection.h"

using namespace std;

/*
// 		function		|	status	|	comment
// --------------------------------------------------------
// MJcompare			|	done	|	utility func.
// tile_in_hand			|	done	|	utility func.
// swap					|	done	|	utility func.
// MJhand::MJhand		|	done	|
// MJhand::~MJhand		|	done	|
// MJhand::faceup_len	|	done	|
// MJhand::total_len	|	done	|
// MJhand::stage		|	done	|
// MJhand::set_faceup_len| 	done	|
// MJhand::set_stage	|	done	|
// MJhand::caneat		|	done	|
// MJhand::canpong		|	done	|
// MJhand::canminggone	|	done	|
// MJhand::cananggone	|	done	|
// MJhand::canbuggone	|	done	|
// MJhand::canhu		|	done    |

// 		function		|	status	|	comment
// --------------------------------------------------------
// MJhand::arrange		|	done	|
// MJhand::draw			|	done	|
// MJhand::play			|	done	|
// MJhand::faceup		|	done	|
// MJhand::applique		|	done	|
// MJhand::initial		|	done	|
// MJhand::showhandtoothers
// MJhand::eat 			|	done	|
// MJhand::pong			|	done	|
// MJhand::minggone		|	done	|
// MJhand::angone		|	done	|
// MJhand::buggone		|	done	|
// MJhand::huother		|   done	|
// MJhand::huown		|	done	|
// MJhand::gethuhand    |           |
// MJhand::getTotalLen	|	done	|	for test
// MJhand::getFaceupLen	|	done	|	for test
// MJhand::getLastTile	|	done	|	for test
*/
//===================== Utility Functions ===============================
bool MJcompare(const MJtile& i, const MJtile& j) { 
	// Utility function used in arrange()
  	// compare suit, if the same, compare rank, from small to big
  	if(i.suit() < j.suit()) {
  		return true;
  	} else if(i.suit() == j.suit()) {
    	// if flower, compare id
        if (i.flower() != 0 && j.flower() != 0) {
            if (i.getTileId() < j.getTileId()) { return true; }
            else { return false; }
        }
        // compare rank
    	if(i.rank() < j.rank()) {
      		return true;
    	} else {
      		return false;
    	}
    } else {
      	return false;
    }
}

bool tile_in_hand(const MJtile* tiles, int face_up, int total_len, int suit, int rank){
	for(int i=face_up; i<total_len; i++){
		if(tiles[i].fromsuitrank(suit, rank))
			return true;
	}
	return false;
}

void swap(MJtile& tile1, MJtile& tile2){
	MJtile temp = tile1;
	tile1 = tile2;
	tile2 = temp;
}


//=================== Class Methods =====================================================
MJhand::MJhand(){
	// Set all the private member to 0 and all the MJtile with MJtile()
	_total_len = 0;
	_faceup_len = 0;
	_stage = 0;
	for(int i=0; i<30; i++) {
    	_tiles[i] = MJtile();
    }
}
MJhand::MJhand(MJtile* t, int n){
  	_total_len = n;
	_faceup_len = 0;
	_stage = 0;
	for(int i=0; i<30; i++) {
      	if(i<n) {
      		_tiles[i].setfromId(t[i].getTileId());
      	} else {
      		_tiles[i] = MJtile();
      	}
    }
}
MJhand::~MJhand(){
}

int MJhand::faceup_len() const {
	return _faceup_len;
}

void MJhand::set_faceup_len(int f) {
	_faceup_len = f;
}

int MJhand::total_len() const {
	return _total_len;
}

bool MJhand::stage() const {
	return _stage;
}

void MJhand::set_stage(int s) {
	_stage = s;
}

int MJhand::getTotalLen(void){
	return _total_len;
}

int MJhand::getFaceupLen(void){
	return _faceup_len;
}

MJtile MJhand::getLastTile(void){
	// stage == 0 return total_len-1
	// stage == 1 return total_len
	return _tiles[_total_len+_stage-1];
}
//======= can xxx =================================
int MJhand::caneat(const MJtile& t){
	int suit = t.suit();
	int rank = t.rank();
	int return_value = 0;
	if(suit==1 || suit==2 || suit==3){
		// (001)
		if(tile_in_hand(_tiles, _faceup_len, _total_len, suit, rank-1) && 
            tile_in_hand(_tiles, _faceup_len, _total_len, suit, rank-2)){
			return_value += 1;
		}
		// (010)
		if(tile_in_hand(_tiles, _faceup_len, _total_len, suit, rank-1) && 
            tile_in_hand(_tiles, _faceup_len, _total_len, suit, rank+1)){
			return_value += 2;
		}
		// (100)
		if(tile_in_hand(_tiles, _faceup_len, _total_len, suit, rank+1) && 
            tile_in_hand(_tiles, _faceup_len, _total_len, suit, rank+2)){
			return_value += 4;
		}
	}
	return return_value;
}

bool MJhand::canpong(const MJtile& t){
	int count = 0;
	// 計算有幾個一樣的 tiles
	for(int i=_faceup_len; i<_total_len; i++){
		if(_tiles[i].fromsuitrank(t.suit(), t.rank())){
			count++;
		}
	}
	if(count>=2){
		return true;
	} else {
		return false;
	}
}

bool MJhand::canminggone(const MJtile& t){
	int count = 0;
	// 計算有幾個一樣的 tiles
	for(int i=_faceup_len; i<_total_len; i++){
		if(_tiles[i].fromsuitrank(t.suit(), t.rank())){
			count++;
		}
	}
	if(count==3){
		return true;
	} else {
		return false;
	}
}

bool MJhand::canangone(const MJtile& t){
	for(int i=_faceup_len; i<_total_len-2; i++){
		if(_tiles[i].fromsuitrank(t.suit(), t.rank()) && _tiles[i+1].fromsuitrank(t.suit(), t.rank()) && 
           _tiles[i+2].fromsuitrank(t.suit(), t.rank())){
           	// cout << t.suit() << " " << t.rank() << endl;
            // cout << "i is " << i << endl;
           	// cout << _tiles[i].suit() << " " << _tiles[i].rank() << endl;
           	// cout << _tiles[i+1].suit() << " " << _tiles[i+1].rank() << endl;
           	// cout << _tiles[i+2].suit() << " " << _tiles[i+2].rank() << endl;
			return true;
		}
	}
  	return false;
}

bool MJhand::canbugone(const MJtile& t){
	// i, i+1, i+2 should be the same as t
	if(_faceup_len<3) return false;
	for(int i=0; i<_faceup_len-2; i++){
		if(_tiles[i].rank()==t.rank() && _tiles[i+1].rank()==t.rank() && _tiles[i+2].rank()==t.rank() && _tiles[i].suit()==t.suit() && _tiles[i+1].suit()==t.suit() && _tiles[i+2].suit()==t.suit()){
			return true;
		}
	}
	return false;
}





typedef vector<vector<int> > table;

table getCombination(vector<MJtile> tv){
	table combination_table;
	// 碰
	for(int i=0; i<tv.size()-2; i++){
		int suit = tv[i].suit();
		int rank = tv[i].rank();
		if(tv[i+1].fromsuitrank(suit, rank) && tv[i+2].fromsuitrank(suit, rank)){
			vector<int> combination = {i+2, i+1, i};
			combination_table.push_back(combination);
		}
	}
	// 槓
	for(int i=0; i<tv.size()-3; i++){
		int suit = tv[i].suit();
		int rank = tv[i].rank();
		if(tv[i+1].fromsuitrank(suit, rank) && tv[i+2].fromsuitrank(suit, rank) && tv[i+3].fromsuitrank(suit, rank)){
			vector<int> combination = {i+3, i+2, i+1, i};
			combination_table.push_back(combination);
		}
	}
	// 吃
	for(int suit=1; suit<=3; suit++){
		for(int first_rank=1; first_rank<=7; first_rank++){
			// first_rank 在 tv 裡面 && first_rank+1 在 tv 裡面 && first_rank+2
			vector<int> combination;
			for(int i=0; i<tv.size(); i++){
				if(tv[i].fromsuitrank(suit, first_rank+2)){
					combination.push_back(i);
					break;
				}
			}
			for(int i=0; i<tv.size(); i++){
				if(tv[i].fromsuitrank(suit, first_rank+1)){
					combination.push_back(i);
					break;
				}
			}
			for(int i=0; i<tv.size(); i++){
				if(tv[i].fromsuitrank(suit, first_rank)){
					combination.push_back(i);
					break;
				}
			}
			if(combination.size()==3){
				combination_table.push_back(combination);
			}
		}
	}

	return combination_table;
}


bool hu_recursion(vector<MJtile>& tv){
	// cout << "print tv in this layer" << endl;
	// cout << "tv.size() = " << tv.size() << endl;
	// for(int i=0; i<tv.size(); i++)
	// 	cout << tv[i] << endl;

	// 剩兩張且兩張一樣的話
	if(tv.size()==2){
		// cout << tv[0] << tv[1];
		return (tv[0].suit()==tv[1].suit() && tv[0].rank()==tv[1].rank());
	}
	table combination_table = getCombination(tv);

	// if(combination_table.size()==0) cout << "\ncombination_table.size() == 0\n";
	// else{
	// 	cout <<  "\ncombination_table: " << endl;
	// 	for(int i=0; i<combination_table.size(); i++){
	// 		cout << '[';
	// 		for(int j=0; j<combination_table[i].size(); j++){
	// 			cout << combination_table[i][j] << " ";
	// 		}
	// 		cout << "]\n";
	// 	}
	// }

	for(table::iterator it = combination_table.begin(); it!=combination_table.end(); it++){
		// *it 為 index 的 vector，例如 [3, 2, 1] 或 [7, 6, 5, 4]
		// 用一個 for 按 *it 去掉 tv，存到 tv_minus_combination
		vector<MJtile> tv_minus_combination = tv;
		for(vector<int>::iterator it2 = (*it).begin(); (it2) != (*it).end(); it2++){
			tv_minus_combination.erase(tv_minus_combination.begin() + (*it2));
		}
		
		if(hu_recursion(tv_minus_combination)) return true;
	}

	return false;
}


bool MJhand::canhu(const MJtile& t){
	// Eliminate flowers 
    vector<MJtile> tv(_tiles+_faceup_len, _tiles+_total_len);
    tv.push_back(t);
    sort(tv.begin(), tv.end(), MJcompare);

    int flower_ind = -1;
    for(int i=0; i<tv.size(); i++){
        if(tv[i].flower() != 0) {
            flower_ind = i;
            // cout << flower_ind << ' ';
            break;
        }
    }
    if(flower_ind != -1) {
        int tmp = tv.size();
        for (int i=flower_ind; i<tmp; i++){
            tv.pop_back();
        }
    }
    // cout << "after processing flower\n";
    // for(int i=0; i<tv.size(); i++){
    // 	cout << tv[i];
    // }
    return hu_recursion(tv);
}


// ====== Actions ============
void MJhand::arrange(){	
	sort(begin(_tiles)+_faceup_len, end(_tiles)-(30-_total_len), MJcompare);
}

void MJhand::draw(MJcollection& mjcol){
	// 待改寫
	// mjtiles: 牌堆, frontind: 牌堆頭(抽牌), backind:牌堆尾(補花)
  	// Draw a tile from the frontind of mjtiles and set it to the last index of the hand. 
  	// _stage = 1;
    // int last = _total_len;
  	// _tiles[last] = mjtiles[frontind];
    // frontind++; // tile drawn, so # of mjtiles decrease by 1
  	
    // If the drawn tile is a flower(花牌), recursively call applique(補花) until the tile isn’t a flower.
  	// if(_tiles[last].flower() != 0) {
  	// applique(last, mjtiles, frontind, backind);
    // }

    // 12/14 rewrite
    _stage = 1;
    _tiles[_total_len] = mjcol.drawfronttile();
    if(_tiles[_total_len].flower() != 0)
    	applique(_total_len, mjcol);
}

MJtile MJhand::play(int index){
    
    if (index >= _total_len+_stage-_faceup_len+1 || index < 1) {	
  	    return MJtile(0);
    }
    // play the selected index tile(count from _faceup_id) 
  	MJtile playout = _tiles[_faceup_len+index-1];
  
  	// and forward all the tiles in the array one place front
  	for(int i=_faceup_len+index; i<_total_len+_stage; i++) {
  		_tiles[i-1] = _tiles[i];
    }
    _tiles[_total_len+_stage-1] = MJtile();
    
    // change state to 0 (normal stage)
  	_stage = 0;
    arrange();	
  	return playout;
}
void MJhand::faceup(int index){
	// Turn the selected index tiles(start counting from _faceup_len) face up, then arrange the hand
  	// Faceup: swap it to the first face-down tiles, and increment _faceup_len
  	swap(_tiles[_faceup_len], _tiles[_faceup_len+index-1]);
  
  	_faceup_len++;
  	arrange();
}
void MJhand::applique(int index, MJcollection& mjcol){
	// (補花) check if card at index (count from index 0) is a flower, 
  	// if yes then put the flower tile out and draw a new tile, _total_len+=1
    // while (_tiles[index].flower() != 0) {
        // swap(_tiles, _faceup_len, index);
        // _faceup_len++;

        // _tiles[_total_len+_stage] = mjtiles[backind];
    //     backind--;
    //     _total_len++;

    //     index = _total_len+_stage-1;
    // }

	// 12/14 rewrite
    // flower go to position _faceup_len and faceup it.
    if(_tiles[index].flower()==0)
    	return;
    else{
	    swap(_tiles[_faceup_len], _tiles[index]);
	    _faceup_len++;
	    // draw tile from mjcollection
		_tiles[_total_len+_stage] = mjcol.drawbacktile();
		_total_len++;
		if(_tiles[_total_len+_stage-1].flower() != 0)
			applique(_total_len+_stage-1, mjcol);
	}
    arrange();
    return;
}

void MJhand::initial(MJcollection& mjcol){
	// 待改寫
	// after the dealing of tiles, applique every flower in the hand, then arrange the hand.
  	// arrange();
    //  for(int i=_faceup_len; i<_total_len+_stage; i++) {
    //    	if(_tiles[i].flower() != 0) {
    //    		applique(i, mjtiles, frontind, backind);
    //    	}
  	// }
  	// arrange();

  	// 12/14 rewrite
	arrange();
  	for(int i=0; i<_total_len+_stage; i++){
  		// if this tile is flower
  		if(_tiles[i].flower() != 0)
  			applique(i, mjcol);
  	}
  	arrange();
}

void MJhand::showhandtoothers() {
    // print the hand on the screen. 
    // Only the faceup tiles will be shown. 
    // Needs the gap between faceup and facedown tiles.   
    //TODO
}

void MJhand::eat(const MJtile& t, int method){
    // 助教給的 1:(001), 2:(010), 3:(100)
    // caneat 的
    // case 0: cout << "false";
	// case 1: cout << "(001)";
	// case 2: cout << "(010)";
	// case 3: cout << "(001) (010)";
	// case 4: cout << "(100)";
	// case 5: cout << "(001) (100)";
	// case 6: cout << "(010) (100)";
	// case 7: cout << "(001) (010) (100)";
	if(caneat(t)==0) return;
	if(caneat(t)==1) goto method1;
	if(caneat(t)==2) goto method2;
	if(caneat(t)==4) goto method3;
	if(caneat(t)==3){
		if(method==1) goto method1;
		if(method==2) goto method2;
		if(method==3) return;
	}
	if(caneat(t)==5){
		if(method==1) goto method1;
		if(method==2) return;
		if(method==3) goto method3;
	}
	if(caneat(t)==6){
		if(method==1) return;
		if(method==2) goto method2;
		if(method==3) goto method3;
	}
	if(caneat(t)==7){
		if(method==1) goto method1;
		if(method==2) goto method2;
		if(method==3) goto method3;
	}
	method1: // (001)
		// search item for suit==t.suit() and rank==t.rank()-2, swap to _faceup_len
		for(int i=_faceup_len; i<_total_len; i++){
			if(_tiles[i].suit()==t.suit() && _tiles[i].rank()==t.rank()-2){
				swap(_tiles[i], _tiles[_faceup_len]);
				break;
			}
		}
		// search item for suit==t.suit() and rank==t.rank()-1, swap to _faceup_len+2
		for(int i=_faceup_len; i<_total_len; i++){
			if(_tiles[i].suit()==t.suit() && _tiles[i].rank()==t.rank()-1){
				swap(_tiles[i], _tiles[_faceup_len+2]);
				break;
			}
		}
		// _faceup_len+1 to total_len, t to _faceup_len+1
		_tiles[_total_len] = _tiles[_faceup_len+1];
		_tiles[_faceup_len+1] = t;
		_faceup_len += 3;

		_total_len++;
        arrange();
        _total_len--;
        _stage = 1;
		return;
		
	method2: //(010)
		// search item for suit==t.suit() and rank==t.rank()-1, swap to _faceup_len
		for(int i=_faceup_len; i<_total_len; i++){
			if(_tiles[i].suit()==t.suit() && _tiles[i].rank()==t.rank()-1){
				swap(_tiles[i], _tiles[_faceup_len]);
				break;
			}
		}
		// search item for suit==t.suit() and rank==t.rank()+1, swap to _faceup_len+2
		for(int i=_faceup_len; i<_total_len; i++){
			if(_tiles[i].suit()==t.suit() && _tiles[i].rank()==t.rank()+1){
				swap(_tiles[i], _tiles[_faceup_len+2]);
				break;
			}
		}
		// _faceup_len+1 to total_len, t to _faceup_len+1
		_tiles[_total_len] = _tiles[_faceup_len+1];
		_tiles[_faceup_len+1] = t;
		_faceup_len += 3;

		_total_len++;
        arrange();
        _total_len--;
        _stage = 1;
		return;

	method3: // (001)
		// search item for suit==t.suit() and rank==t.rank()-1, swap to _faceup_len
		for(int i=_faceup_len; i<_total_len; i++){
			if(_tiles[i].suit()==t.suit() && _tiles[i].rank()==t.rank()+1){
				swap(_tiles[i], _tiles[_faceup_len]);
				break;
			}
		}
		// search item for suit==t.suit() and rank==t.rank()+1, swap to _faceup_len+2
		for(int i=_faceup_len; i<_total_len; i++){
			if(_tiles[i].suit()==t.suit() && _tiles[i].rank()==t.rank()+2){
				swap(_tiles[i], _tiles[_faceup_len+2]);
				break;
			}
		}
		// _faceup_len+1 to total_len, t to _faceup_len+1
		_tiles[_total_len] = _tiles[_faceup_len+1];
		_tiles[_faceup_len+1] = t;
		_faceup_len += 3;


		_total_len++;
        arrange();
        _total_len--;
        _stage = 1;
		return;
}


void MJhand::pong(const MJtile& t){
	// 驗證是可以碰的，以免不能碰又要使用這個函式
	if(canpong(t)){
		for(int i=_faceup_len; i<_total_len; i++){
			// 如果這張手中的牌 == 傳進來的 t
			if(_tiles[i].fromsuitrank(t.suit(), t.rank())){
				// 這張跟 _tiles[_faceup_len] 換
				swap(_tiles[i], _tiles[_faceup_len]);
				// 下一張跟 _tiles[_faceup_len + 1] 換
				swap(_tiles[i+1], _tiles[_faceup_len+1]);
				// _tiles[_faceup_len + 2] 移到最後
				_tiles[_total_len] = _tiles[_faceup_len + 2];
				// 原來的位置放新進來的 t
				_tiles[_faceup_len + 2] = t;
                swap(_tiles[_faceup_len+1], _tiles[_faceup_len+2]);
				_faceup_len += 3;

                _total_len++;
                arrange();
                _total_len--;
                _stage = 1;
                return;
			}
		}
	} else {
		return;
	}
}

/*
這是補花，參考用
if(_tiles[index].flower()==0)
	return;
else{
    swap(_tiles[_faceup_len], _tiles[index]);
    _faceup_len++;
    // draw tile from mjcollection
	_tiles[_total_len+_stage] = mjcol.drawbacktile();
	_total_len++;
	if(_tiles[_total_len+_stage-1].flower() != 0)
		applique(_total_len+_stage-1, mjcol);
}	
arrange();

這是 draw 參考用
_stage = 1;
_tiles[_total_len] = mjcol.drawfronttile();
if(_tiles[_total_len].flower() != 0)
	applique(_total_len, mjcol);


*/
void MJhand::minggone(const MJtile& t, MJcollection& mjcol){
	if(canminggone(t)){
		// find the tile with the same suit and rank
		for(int i=_faceup_len; i<_total_len-2; i++){
			if(_tiles[i].fromsuitrank(t.suit(), t.rank()) && _tiles[i+1].fromsuitrank(t.suit(), t.rank()) && 
           _tiles[i+2].fromsuitrank(t.suit(), t.rank())){
				// 先把 i 到 i+2 hold 住
				MJtile hold[4];
				hold[0] = _tiles[i]; hold[1] = t;
				hold[2] = _tiles[i+1]; hold[3] = _tiles[i+2];

				// i+3 以後到 total_len -1 都要往後推一格
				// faceup_len 到 i-1 都要往後推 4 格
				// hold 填入 faceup_len 到 faceup_len+3
				for(int j=_total_len; j>i+3; j--){
					_tiles[j] = _tiles[j-1];
				}
				for(int j=i+3; j>_faceup_len; j--){
					_tiles[j] = _tiles[j-4];
				}
				_tiles[_faceup_len] = hold[0];
				_tiles[_faceup_len+1] = hold[1];
				_tiles[_faceup_len+2] = hold[2];
				_tiles[_faceup_len+3] = hold[3];

				
				// 原來寫法，在 faceup_len = 0 時出問題，可能暗槓也要改
				// i swap to faceup_len
				// swap(_tiles[i], _tiles[_faceup_len]);
				// i+1 swap to faceup_len+2
				// swap(_tiles[i+1], _tiles[_faceup_len+2]);
				// i+2 swap to faceup_len+3
				// swap(_tiles[i+2], _tiles[_faceup_len+3]);
				// faceup_len+1 to total_len
				// _tiles[_total_len] = _tiles[_faceup_len+1];
				// t to faceup_len+1
				// _tiles[_faceup_len+1] = t;

				_faceup_len += 4;
				_total_len++;
                arrange();

                // cout << "\nIn minggone, before draw:\n" << *this;

                // draw
                _stage = 1;
              	_tiles[_total_len] = mjcol.drawbacktile();
              	// cout << "\nThe tile I've draw is \n" << _tiles[_total_len];
              	if(_tiles[_total_len].flower() != 0){
                	applique(_total_len, mjcol);
                }

				break;
			}
		}
	}
}


void MJhand::angone(int index, MJcollection& mjcol){
	MJtile t = _tiles[_faceup_len+index-1];
  	if(canangone(t)){
    	for(int i=_faceup_len; i<_total_len-2; i++){
    		if(_tiles[i].fromsuitrank(t.suit(), t.rank()) && _tiles[i+1].fromsuitrank(t.suit(), t.rank()) && 
           		_tiles[i+2].fromsuitrank(t.suit(), t.rank())){
                // turn the four same tiles in your hand face up
                // i swap to faceup_len
				swap(_tiles[i], _tiles[_faceup_len]);
				// i+1 swap to faceup_len+2
				swap(_tiles[i+1], _tiles[_faceup_len+2]);
				// i+2 swap to faceup_len+3
				swap(_tiles[i+2], _tiles[_faceup_len+3]);
				// faceup_len+1 swap to _faceup_len+index-1
				swap(_tiles[_faceup_len+index-1], _tiles[_faceup_len+1]);
				_faceup_len += 4;
              	_total_len++;
                arrange();
              
              	// draw
              	_tiles[_total_len] = mjcol.drawbacktile();
              	if(_tiles[_total_len].flower() != 0){
                	applique(_total_len, mjcol);
                }
                _stage = 1;
              	break;
            }
        }
  }
  return;
}

void MJhand::bugone(int index, MJcollection& mjcol){
	// _stage = 0;
	MJtile t = _tiles[_faceup_len+index-1];
	// cout << t;
	// cout << "\nEnter bugone fun\n";
	if(canbugone(t)){
		// cout << "\nEnter bugone if 1\n";
		for(int i=0; i<_faceup_len-2; i++){
			// cout << "\nEnter bugone for " << i;
			if(_tiles[i].rank()==t.rank() && _tiles[i+1].rank()==t.rank() && _tiles[i+2].rank()==t.rank() && _tiles[i].suit()==t.suit() && _tiles[i+1].suit()==t.suit() && _tiles[i+2].suit()==t.suit()){
				// cout << "\nEnter bugone if loop\n";
				// i+3 ~ _faceup_len+index-2 全部往後移
				for(int j=_faceup_len+index-2; j>=i+3; j--){
					_tiles[j+1] = _tiles[j];
					// cout << j+1 << " = " << j << endl;
				}
				// t to i+3
				// cout << "_tiles[" << i+3 << "] = t" << endl;
				_tiles[i+3] = t;
				_faceup_len += 1;
				_total_len += 1;
				// cout << *this;

				// draw
              	_tiles[_total_len] = mjcol.drawbacktile();
              	if(_tiles[_total_len].flower() != 0){
                	applique(_total_len, mjcol);
                }
                _stage = 1;
				break;
			}
		}
	}
}

void MJhand::huother(const MJtile& t){
  	if (canhu(t)){
  		_tiles[_total_len] = t;  	
  		_stage = 2;
  		_total_len++;
		arrange();
  	}
}

void MJhand::huown(){
  	MJtile t = _tiles[_total_len];
  	if (canhu(t)){
  		_stage = 2;
  		_total_len++;
  		arrange();
  	}
}

MJhand* MJhand::gethuhand(){
    return this;
}
// ======== Print out Methods ===================
ostream& operator << (ostream& os, const MJhand& h){
	
	//part 1 " __ "
	for(int i=0;i<h._total_len+h._stage;i++){
		if(i==h.faceup_len())
			os<<"    ";//4 spaces
		if(h._stage && i==h._total_len+h._stage-1)
			os<<"    ";//4 spaces
		os<<" __ ";
	}
	os<<endl;
	
	//part 2 "| N|"
	for(int i=0;i<h._total_len+h._stage;i++){
		if(i==h.faceup_len())
			os<<"    ";//4 spaces
		if(h._stage && i==h._total_len+h._stage-1)
			os<<"    ";//4 spaces
		os<<"|";
		if(h._tiles[i].suit()==4){ 
			if(h._tiles[i].flower())
				os<<" "<<h._tiles[i].flower();
			else 
				os<<"  ";
		} 
		else{
			switch(h._tiles[i].rank()){
				case 1:
					os<<" 1";
					break; 
				case 2:
					os<<" 2";
					break; 
				case 3:
					os<<" 3";
					break; 
				case 4:
					os<<" 4";
					break; 
				case 5:
					os<<" 5";
					break; 
				case 6:
					os<<" 6";
					break; 
				case 7:
					os<<" 7";
					break; 
				case 8:
					os<<" 8";
					break; 
				case 9:
					os<<" 9";
					break; 
				default:
					assert(false); 
			}
		} 
		os<<"|";
	}
	os<<endl;
	
	//part 3 "|XX|"
	for(int i=0;i<h._total_len+h._stage;i++){
		if(i==h.faceup_len())
			os<<"    ";//4 spaces
		if(h._stage && i==h._total_len+h._stage-1)
			os<<"    ";//4 spaces
		os<<"|";
		if(h._tiles[i].suit()==4)
			switch(h._tiles[i].rank()){
				case 1:
					os<<"Ea";
					break; 
				case 2:
					os<<"So";
					break; 
				case 3:
					os<<"We";
					break; 
				case 4:
					os<<"No";
					break; 
				case 5:
					os<<"Mi";
					break; 
				case 6:
					os<<"Fa";
					break; 
				case 7:
					os<<"  ";
					break; 
				case 8:
					switch(h._tiles[i].flower()){
						case 1:
							os<<"Me";
							break; 
						case 2:
							os<<"La";
							break; 
						case 3:
							os<<"Zu";
							break; 
						case 4:
							os<<"Ju";
							break; 
						default:
							assert(false); 
					} 
					break; 
				case 9:
					switch(h._tiles[i].flower()){
						case 1:
							os<<"Sp";
							break; 
						case 2:
							os<<"Su";
							break; 
						case 3:
							os<<"Au";
							break; 
						case 4:
							os<<"Wi";
							break; 
						default:
							assert(false); 
					} 
					break; 
				default:
					assert(false); 
			}
		else{
			switch(h._tiles[i].suit()){
				case 1:
					os<<" O";
					break; 
				case 2:
					os<<" I";
					break; 
				case 3:
					os<<" W";
					break; 
				default:
                    assert(false); 
			}
		} 
		os<<"|";
	}
	os<<endl;
	
	//part 4 " -- "
	for(int i=0;i<h._total_len+h._stage;i++){
		if(i==h.faceup_len())
			os<<"    ";//4 spacse
		if(h._stage && i==h._total_len+h._stage-1)
			os<<"    ";//4 spaces
		os<<" -- ";
	}
	os<<endl; 
	return os;
} 

/*
ostream& operator << (ostream& os, const MJhand& h){
	
	//part 1 " __ "
	for(int i=0;i<h._total_len+h._stage;i++){
		if(i==h.faceup_len())
			os<<"    ";//4 spaces
		if(h._stage && i==h._total_len+h._stage-1)
			os<<"    ";//4 spaces
		os<<" __ ";
	}
	os<<endl;
	
	//part 2 "| N|"
	for(int i=0;i<h._total_len+h._stage;i++){
		if(i==h.faceup_len())
			os<<"    ";//4 spaces
		if(h._stage && i==h._total_len+h._stage-1)
			os<<"    ";//4 spaces
		os<<"|";
		if(h._tiles[i].suit()==4){ 
			if(h._tiles[i].flower())
				os<<" "<<h._tiles[i].flower();
			else 
				os<<"  ";
		} 
		else{
			switch(h._tiles[i].rank()){
				case 1:
					os<<"一";
					break; 
				case 2:
					os<<"二";
					break; 
				case 3:
					os<<"三";
					break; 
				case 4:
					os<<"四";
					break; 
				case 5:
					os<<"五";
					break; 
				case 6:
					os<<"六";
					break; 
				case 7:
					os<<"七";
					break; 
				case 8:
					os<<"八";
					break; 
				case 9:
					os<<"九";
					break; 
				default:
					assert(false); 
			}
		} 
		os<<"|";
	}
	os<<endl;
	
	//part 3 "|XX|"
	for(int i=0;i<h._total_len+h._stage;i++){
		if(i==h.faceup_len())
			os<<"    ";//4 spaces
		if(h._stage && i==h._total_len+h._stage-1)
			os<<"    ";//4 spaces
		os<<"|";
		if(h._tiles[i].suit()==4)
			switch(h._tiles[i].rank()){
				case 1:
					os<<"東";
					break; 
				case 2:
					os<<"南";
					break; 
				case 3:
					os<<"西";
					break; 
				case 4:
					os<<"北";
					break; 
				case 5:
					os<<"中";
					break; 
				case 6:
					os<<"發";
					break; 
				case 7:
					os<<"  ";
					break; 
				case 8:
					switch(h._tiles[i].flower()){
						case 1:
							os<<"梅";
							break; 
						case 2:
							os<<"蘭";
							break; 
						case 3:
							os<<"竹";
							break; 
						case 4:
							os<<"菊";
							break; 
						default:
							assert(false); 
					} 
					break; 
				case 9:
					switch(h._tiles[i].flower()){
						case 1:
							os<<"春";
							break; 
						case 2:
							os<<"夏";
							break; 
						case 3:
							os<<"秋";
							break; 
						case 4:
							os<<"冬";
							break; 
						default:
							assert(false); 
					} 
					break; 
				default:
					assert(false); 
			}
		else{
			switch(h._tiles[i].suit()){
				case 1:
					os<<"筒";
					break; 
				case 2:
					os<<"條";
					break; 
				case 3:
					os<<"萬";
					break; 
				default:
					assert(false); 
			}
		} 
		os<<"|";
	}
	os<<endl;
	
	//part 4 " -- "
	for(int i=0;i<h._total_len+h._stage;i++){
		if(i==h.faceup_len())
			os<<"    ";//4 spaces
		if(h._stage && i==h._total_len+h._stage-1)
			os<<"    ";//4 spaces
		os<<" ￣ ";
	}
	os<<endl; 
	return os;
} 
*/
