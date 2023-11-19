#include <windows.h>
#include <stdio.h>
#include "CDX2.h"



CDXScreen	* Screen	= NULL;
CDXInput	* Input		= NULL;
CDXSound	* Sound		= NULL;



BOOL CDXGame::Init(HINSTANCE hInst, WNDPROC wndproc, LPCTSTR wndname, int width, int height, int bpp, BOOL fullscreen, int fps)
	{

	/////////////////////////////////
	//							   //
	// Used to initialize the game //
	//							   //
	/////////////////////////////////



	// Set the hInstance variable and wnd name //

	Instance	= hInst;
	WndProc		= wndproc;

	strcpy(WndName, wndname);
	


	// Create the main window //

	if (!CreateMainWindow(width, height, fullscreen))
		return FALSE;


	// Create the screen //

	Screen = new CDXScreen();


    // Set the video mode //

	if (fullscreen)
		{
		if(FAILED(Screen->CreateFullScreen(Wnd, width, height, bpp)))
			{
			CDXError(Screen , "Could not set the video mode");
			return FALSE;
			}
		}
	else
		{
		if(FAILED(Screen->CreateWindowed(Wnd, width, height)))
			{
			CDXError(Screen , "Could not set the video mode");
			return FALSE;
			}
		}


   	// Create the input device //

	Input = new CDXInput;

    if(Input == NULL)
		{
        CDXError(Screen, "Could not create CDXInput object!");
		return FALSE;
		}


	if(FAILED(Input->Create(hInst, Wnd)))
		{
        CDXError(Screen, "Could not create CDXInput object!");
		return FALSE;
		}


	// Create the sound device //

	Sound = new CDXSound;

	if (FAILED(Sound->Create(Wnd, FALSE)))
		{
        CDXError(Screen, "Could not create CDXSound object!");
		return FALSE;
		}


	// Set the flags //

	CurrStage			= NULL;
	Exiting				= FALSE;
	InTransition		= FALSE;
	FPS					= fps;


	// Get the initial tick count //

	LastTickCount	= GetTickCount();

#ifdef _DEBUG
	LastTCount = GetTickCount();
	FPSCount	 = 0;
#endif


	return TRUE;
	}





void CDXGame::ShutDown()
	{

	//////////////////////////////////////////////
	//											//
	// Used to handle de-initiation of the game //
	//											//
	//////////////////////////////////////////////


	SAFEDELETE(Sound);
	SAFEDELETE(Input);
	SAFEDELETE(Screen);
	}





int CDXGame::CreateMainWindow(int width, int height, BOOL fullscreen)
	{

	/////////////////////////////////////////////////
	//											   //
	// Used to create the main window for the game //
	//											   //
	/////////////////////////////////////////////////



	WNDCLASSEX wndclass;

	memset(&wndclass, 0, sizeof(WNDCLASSEX));

	wndclass.cbSize			= sizeof(WNDCLASSEX);
	wndclass.style			= CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 0;
	wndclass.lpfnWndProc	= WndProc;
	wndclass.hInstance		= Instance;
	wndclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground	= NULL;
	wndclass.lpszMenuName	= NULL;
	wndclass.lpszClassName	= WINDOW_CLASS_NAME;

	if (!RegisterClassEx(&wndclass))
		return FALSE;


	// Create the main window //

	if (fullscreen)
		{
		Wnd = CreateWindowEx(WS_EX_TOPMOST, WINDOW_CLASS_NAME, WndName, WS_POPUP | WS_VISIBLE, 
								 CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
								 NULL, NULL, Instance, NULL);
		}
	else
		{
		Wnd = CreateWindowEx(0, WINDOW_CLASS_NAME, WndName, WS_BORDER | WS_CAPTION | WS_POPUP | WS_VISIBLE, 
								 (GetSystemMetrics(SM_CXSCREEN) / 2) - width, (GetSystemMetrics(SM_CYSCREEN) / 2) - height, 
								 (width * 2) + (GetSystemMetrics(SM_CXBORDER) * 2), (height * 2) + (GetSystemMetrics(SM_CYBORDER) * 2) + GetSystemMetrics(SM_CYCAPTION),
								 NULL, NULL, Instance, NULL);
		}

	
	return (Wnd != NULL);
	}






void CDXGame::SetStage(CDXStage * stage)
	{

	////////////////////////////////
	//							  //
	// Used to set the next stage //
	//							  //
	////////////////////////////////



	// Call the closing for the current stage //

	if (CurrStage)
		CurrStage->Closing(stage);


	// Set the new stage variable //

	CurrStage = stage;
	

	// Transfer the game pointer to the stage //

	if (CurrStage)
		{
		CurrStage->Opening();
		}
	}





void CDXGame::Loop()
	{

	////////////////////////
	//					  //
	// The main game loop //
	//					  //
	////////////////////////


	MSG msg;
	DWORD tickcount;
	DWORD oneframe = (DWORD)(1000 / FPS);



	// Call the virtual Opening function //

	Opening();



	// Check for exiting flag //

	while(!Exiting)
		{

		// Get the current tick count //

		tickcount = GetTickCount();



		// Check for window messages //

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{ 
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
			}



		// Process all the timers //

		//TIMER::DoLogic();



		// Compare the tick count for maintaining FPS //

		if (tickcount < LastTickCount + oneframe)
			continue;

		LastTickCount = tickcount;



		// Display the Actual FPS in debug mode 

	#ifdef _DEBUG

		FPSCount++;

		if (tickcount >= LastTCount + 1000)
			{
//			sprintf(FPSText, "%d\r\n", FPSCount);
//			OutputDebugString(FPSText);

			LastFPSCount	= FPSCount;
			FPSCount		= 0;
			LastTCount		= tickcount;
			}
	#endif


		// Poll the input //

		Input->Update();


		// Call the stage logic functions //

		if (CurrStage)
			{
			if (InTransition)
				{
				Transition->Loop();
				}
			else
				{
				CurrStage->Loop();
				}
			}


		// Check if transition is done //

		if (InTransition)
			if (Transition->IsDone())
				{
				InTransition	= FALSE;
				Transition		= NULL;
				}
			


		// Update the screen //

		Screen->Flip();
		}


	// Shut down the engine //

	if (CurrStage)
		CurrStage->Closing(NULL);


	Closing();
	ShutDown();
	}





void CDXGame::SetTransition(CDXTransition * transition)
	{

	////////////////////////////////
	//							  //
	// Used to set the transition //
	//							  //
	////////////////////////////////
		

	if (transition)
		{
		InTransition = TRUE;
		transition->Start();
		}


	Transition = transition;
	}

