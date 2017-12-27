/****************************************************************************** 
   FileName  [MJtile.h]
   Author    [HsuehYa Huang]
   Synopsis  [define a single tile] 
******************************************************************************/ 
#ifndef MJTILE
#define MJTILE

#include <iostream> 
#include <cassert> 
using namespace std; 
class MJtile{
	friend ostream& operator << (ostream&, const MJtile&);
public:
	MJtile();
	MJtile(int id);
	~MJtile();
	void setfromId(int);
	/*set mjtile from id */
	int suit() const;
	/*get suit, 筒子=1, 條子=2, 萬子=3, 風牌花牌=4 */
	int rank() const;
	/*get rank, 1~9*/
	int flower() const; 
	/*get flower number, if not flower return 0*/
	int getTileId() const;
	/*get tile id*/ 
	bool operator == (const MJtile&) const;
	/*compare same kind of tile, don't need to worry of flower tile*/ 
	bool operator != (const MJtile&) const;
	/*compare not the same kind of tile, don't need to worry of flower tile*/ 
	bool fromsuitrank(int, int) const;
	/*return true if equal to given suit and rank, don't need to worry of flower tile*/ 
private:
	int _suit;//1~4 
	int _rank;//1~9 
	int _flowernum;//1~4 
	int _tileId; //1~144
}; 
#endif
