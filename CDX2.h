#ifndef __CDX2_H
#define __CDX2_H



#include "cdx.h"
#include "cdxscreen.h"
#include "cdxinput.h"
#include "cdxtile.h"
#include "cdxlayer.h"
#include "cdxsound.h"
#include "cdxmidi.h"


/////////////
// DEFINES //
/////////////


#define INIT_GUID
#define WINDOW_CLASS_NAME		"GAMELIBCLASS"



////////////////
// PROTOTYPES //
////////////////

class CDXGame;
class CDXStage;
class CDXTransition;



///////////////
// EXTERNALS //
///////////////

extern CDXScreen	* Screen;
extern CDXInput		* Input;
extern CDXSound		* Sound;


//////////////////
// GAME OBJECTS //
//////////////////



class CDXGame
	{
	public:

		/////////////////////////////
		// Init / DeInit functions //
		/////////////////////////////

		BOOL Init(HINSTANCE hInst, WNDPROC wproc, const char * wndname, int width = 320, int height = 240, int bpp = 8, int fullscreen = 1, int fps = 30);
		void ShutDown();

		void End()								{ Exiting = TRUE; }
		BOOL IsExiting()						{ return Exiting; }


		///////////////////////////////////
		// Overridable Virtual Functions //
		///////////////////////////////////
		
		virtual void Loop();
		virtual void Opening(){}
		virtual void Closing(){}


		////////////////////////////////////////
		// State and Transition set functions //
		////////////////////////////////////////

		void SetStage(CDXStage * stage);
		void SetTransition(CDXTransition * transition);


		///////////////////////////////
		// Property attain functions //
		///////////////////////////////

		HINSTANCE	GetInstance()				{ return Instance; }
		HWND		GetWindow()					{ return Wnd; }


		//////////////////////
		// Palette Routines //
		//////////////////////

		//void SetPalette(PALETTEENTRY * pPal)	{ m_pDisplay->SetPalette(pPal); }
		//PALETTEENTRY * GetPalette()				{ return m_pDisplay->GetPalette(); }
		

		////////////////////
		// Debug Routines //
		////////////////////

		int GetFPSCount()						{ return LastFPSCount; }



	protected:

		///////////////////////
		// Window Properties //
		///////////////////////

		HINSTANCE	Instance;
		HWND		Wnd;
		char		WndName[256];
		WNDPROC		WndProc;

		CDXStage		* CurrStage;
		CDXTransition	* Transition;


		/////////////////
		// Game States //
		/////////////////

		BOOL	Exiting;
		BOOL	InTransition;
		

		////////////////////////////////
		// Frames Per Second Counters //
		////////////////////////////////

		int		FPS;
		DWORD	LastTickCount;


		////////////////////////
		// Debug FPS Counters //
		////////////////////////

		int FPSCount;
		int LastFPSCount;
		DWORD LastTCount;

		virtual int CreateMainWindow(int Width, int Height, BOOL FullScreen);
	};





class CDXStage
	{
	public:

		///////////////////
		// Overrideables //
		///////////////////

		virtual void Opening()						{}
		virtual void Closing(CDXStage * stage)		{}
		virtual void Loop()							{}


	protected:

		int State;
	};




class CDXTransition
	{
	public:

		CDXTransition()								{ Done = TRUE; }

		virtual void Start()						{ Done = FALSE; }
		virtual void Loop()							{}
		
		int IsDone()								{ return Done; }


	protected:

		int Done;
	};



#endif
