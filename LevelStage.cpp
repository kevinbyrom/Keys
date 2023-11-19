#include <windows.h>
#include "Keys.h"
#include "LevelStage.h"



void CLevelStage::Opening()
	{
	Tile[0] = new CDXTile;

	Tile[0]->Create(Screen, "Hero1.Bmp", 16, 24, 1, 1, 4, 4, 1);
	Tile[0]->SetColorKey(255); 
	//Hero.SetTile(Tile[0]);
	Hero.Create(Tile[0]);
	}



void CLevelStage::Closing(CDXStage * stage)
	{
	delete Tile[0];
	}



void CLevelStage::Loop()
	{
	// Process the input //

	// If the arrow-left key is down //
	// Move the controlled object left //

	// Else if the arrow-right key is down //
	// Move the controlled object right //

	// If the arrow-left key is up //
	// Move the controlled object left //

	// If the arrow-left key is down //
	// Move the controlled object left //


	if (Input->GetKeyState(CDXKEY_UPARROW))
		Hero.SetPosY(Hero.GetPosY() - 1);
	
	if (Input->GetKeyState(CDXKEY_DOWNARROW))
		Hero.SetPosY(Hero.GetPosY() + 1);
	
	if (Input->GetKeyState(CDXKEY_LEFTARROW))
		Hero.SetPosX(Hero.GetPosX() - 1);
	
	if (Input->GetKeyState(CDXKEY_RIGHTARROW))
		Hero.SetPosX(Hero.GetPosX() + 1);

	if (Input->GetKeyState(CDXKEY_ESCAPE))
		Game.End();



	// Move objects //
	
	

	// Render to the screen //
	
	Screen->Fill(16);
	
	Hero.SetFrame(0);

	Hero.Draw(Screen->GetBack(), 0, 0, CDXBLT_TRANS);

	Screen->WaitForVerticalBlank();
	}