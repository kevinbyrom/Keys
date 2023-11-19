#include <windows.h>
#include "Editor.h"
#include "DirectX.h"


///////////////
//			 //
// Init code //
//			 //
///////////////

int KEYS_EDITOR::Init()
	{
	if (!InitDX())
		return 0;

	m_SurfaceBmp.AttachToDDSurface(lpBack);
	m_BufferBmp.Create(320, 240, 8);
	m_TileViewBmp.Create(320, 192, 8);	// 20 x 12 tiles

	LoadTiles();

	return 1;
	}





///////////////
//			 //
// Exit code //
//			 //
///////////////

void KEYS_EDITOR::ShutDown()
	{
	m_BufferBmp.Destroy();
	m_TileViewBmp.Destroy();

	UnloadTiles();

	ShutDownDX();
	}





void KEYS_EDITOR::DoLogic()
	{
	int x,y;

	m_TileViewBmp.Fill(0);

	for (y = 0; y < 24; y++)
		{
		m_TileViewBmp.DrawHLine8(0, 319, y * 8, 1);
		m_TileViewBmp.DrawHLine8(0, 319, (y * 8) + 7, 1);
		}

	for (x = 0; x < 40; x++)
		{
		m_TileViewBmp.DrawVLine8(x * 8, 0, 191, 1);
		m_TileViewBmp.DrawVLine8((x * 8) + 7, 0, 191, 1);
		}

	for (int i = 0; i < 20; i++)
		m_Tile[0].Blit(&m_TileViewBmp, i * 8, 0);


	m_TileViewBmp.DrawRect8(0, 0, 319, 191, 15);
	
	m_BufferBmp.Blit(&m_TileViewBmp, 0, 0, 320, 192, 0, 0);

	m_SurfaceBmp.LockDDSurface();
	m_SurfaceBmp.Blit(&m_BufferBmp, 0, 0, 320, 240, 0, 0);
	m_SurfaceBmp.UnlockDDSurface();

	lpPrimary->Flip(NULL, 0);
	}





void KEYS_EDITOR::InputLogic()
	{
	}





void KEYS_EDITOR::GraphicLogic()
	{
	}





void KEYS_EDITOR::LoadTiles()
	{
	GBITMAP TileBmp;
	int x, y;
	PALETTEENTRY pal[256];



	m_NumTiles = 0;


	// Load the tile bitmap //

	TileBmp.LoadBMP("TILE.BMP");


	// Grab the tile sprites //

	for (y = 0; y < 4; y++)
		for (x = 0; x < 13; x++)
			{
			m_Tile[x + (y * 13)].Grab(&TileBmp, (x * 16) + (x + 1), (y * 16) + (y + 1), 8, 8, 255);
			m_NumTiles++;
			}


	// Set the palette //

	memcpy(pal, TileBmp.GetPalette(), sizeof(PALETTEENTRY) * 256);
	lpPalette->SetEntries(0, 0, 255, pal);


	// Unload the tile bitmap  //

	TileBmp.Destroy();
	}



void KEYS_EDITOR::LoadSprites()
	{
	}



void KEYS_EDITOR::UnloadTiles()
	{
	for (int i = 0; i < m_NumTiles; i++)
		m_Tile[i].Destroy();

	m_NumTiles = 0;
	}



void KEYS_EDITOR::UnloadSprites()
	{
	}