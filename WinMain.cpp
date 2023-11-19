#include <windows.h>
#include <stdio.h>
#include <time.h>
#include "WinMain.h"
#include "Keys.h"




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
	{
	if (!Game.Init(hInstance, WndProc, "GAMELIB", 320, 200, 8, FALSE, 30))
		return 0;

	Game.Loop();

	return (0);
	}





LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
	switch(msg)
		{
		case WM_CREATE:
			ShowCursor(FALSE);
			return 1;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}


	return DefWindowProc(hWnd, msg, wParam, lParam);
	}
