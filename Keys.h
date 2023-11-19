#ifndef __KEYS_H
#define __KEYS_H



#include "CDX2.h"
#include "Hero.h"



#define MAX_TILES				10



class CKeys : public CDXGame
	{
	public:

		virtual void Opening();
		virtual void Closing();
	};



extern CKeys		Game;
extern CDXTile		* Tile[MAX_TILES];

extern CHero		Hero;



#endif