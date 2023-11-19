#include <windows.h>
#include "keys.h"
#include "LevelStage.h"


CKeys		Game;
CDXTile		* Tile[MAX_TILES];

CHero		Hero;

CLevelStage MainStage;



void CKeys::Opening()
	{
	Screen->LoadPalette("HERO1.BMP");

	SetStage(&MainStage);	
	}



void CKeys::Closing()
	{
	}



