#ifndef __EDITOR_H
#define __EDITOR_H


#include "GBITMAP.H"
#include "GSPRITE.H"


#define MAX_TILES			100
#define MAX_SPRITES			100



class KEYS_EDITOR
	{
	public:

		int Init();
		void ShutDown();
		
		void DoLogic();
		void InputLogic();
		void GraphicLogic();
		 

	protected:

		GBITMAP m_SurfaceBmp,
				m_BufferBmp,
				m_TileViewBmp;

		GSPRITE m_Tile[MAX_TILES];
		GSPRITE m_Sprite[MAX_SPRITES];

		int m_NumTiles;
		int m_NumSprites;

		void LoadTiles();
		void LoadSprites();

		void UnloadTiles();
		void UnloadSprites();
	};



#endif