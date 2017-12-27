/****************************************************************************** 
   FileName  [MJhand.h]
   Author    [HsuehYa Huang]
   Synopsis  [define a player's hand] 
******************************************************************************/ 
#ifndef MJHAND
#define MJHAND

#include <iostream> 
using namespace std;

#include "MJtile.h" 
#include "MJcollection.h" 
class MJhand{
	friend ostream& operator << (ostream&, const MJhand&);
public:
	MJhand();
	MJhand(MJtile*, int);
	~MJhand();
	
	int total_len() const;
	int faceup_len() const;
	void set_faceup_len(int f);
	void set_stage(bool s);
	bool stage() const;
	
	int caneat(const MJtile&);
	bool canpong(const MJtile&);
	bool canminggone(const MJtile&);
	bool canangone(const MJtile&);
	bool canbugone(const MJtile&);
	bool canhu(const MJtile&);

	void arrange();
	void draw(MJcollection&);
	MJtile play(int);
	void faceup(int);
	void applique(int, MJcollection&);
	void initial(MJcollection&);
	
	void eat(const MJtile&, int);
	void pong(const MJtile&);
	void minggone(const MJtile&, MJcollection&);
	void angone(int, MJcollection&);
	void bugone(int, MJcollection&);
	void huother(const MJtile&);
	void huown();

	int getTotalLen(void);
	int getFaceupLen(void);
	MJtile getLastTile(void);
	//bool hu(const MJtile&);
	
	
private:
	int _total_len;
	int _faceup_len;
	int _stage;//0 normal stage, 1 draw stage
	MJtile _tiles[30];//total 17 tiles, at most 30 = 17+8+5
}; 
#endif
