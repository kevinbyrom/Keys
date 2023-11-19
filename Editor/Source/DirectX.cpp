#include <windows.h>
#include <stdio.h>
#include <io.h>
#include "DirectX.h"
#include "WinMain.h"



#define BITMAP_ID			0x4D42
#define MAX_COLORS_PALETTE	256


LPDIRECTDRAW4				lpdd		= NULL;
LPDIRECTDRAWSURFACE4		lpPrimary	= NULL,
							lpBack		= NULL;

LPDIRECTDRAWPALETTE			lpPalette	= NULL;




int InitDX()
	{
	LPDIRECTDRAW lptempdd;
	DDSURFACEDESC2 ddsd;
	PALETTEENTRY GrayPal[256];


	// Create the direct draw object //

	if (FAILED(DirectDrawCreate(NULL, &lptempdd, NULL)))
		return FALSE;


	// Get the latest interface //

	if (FAILED(lptempdd->QueryInterface(IID_IDirectDraw4, (LPVOID *)&lpdd)))
		{
		lptempdd->Release();
		return FALSE;
		}


	// Set the cooperation level //

	lptempdd->Release();

	if (FAILED(lpdd->SetCooperativeLevel(MainWnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE)))
		return FALSE;



	// Set the display mode //

	if (FAILED(lpdd->SetDisplayMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, 0, 0)))
		return FALSE;



	// Create the main palette//

	for (int pal = 1; pal < 255; pal++)
		{
		GrayPal[pal].peRed		= pal;
		GrayPal[pal].peGreen	= pal;
		GrayPal[pal].peBlue		= pal;
		GrayPal[pal].peFlags	= PC_NOCOLLAPSE;
		}


	if (FAILED(lpdd->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256 | DDPCAPS_INITIALIZE, GrayPal, &lpPalette, NULL)))
		return FALSE;



	// Now create the primary surface //

	memset(&ddsd, 0, sizeof(ddsd));

	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags			= DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.dwBackBufferCount	= 1;
	ddsd.ddsCaps.dwCaps		= DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;

	if (FAILED(lpdd->CreateSurface(&ddsd, &lpPrimary, NULL)))
		return FALSE;
		


	// Set the palette to the primary surface //

	if (FAILED(lpPrimary->SetPalette(lpPalette)))
		return FALSE;



	// Create the back surface //

	ddsd.ddsCaps.dwCaps	= DDSCAPS_BACKBUFFER;

	if (FAILED(lpPrimary->GetAttachedSurface(&ddsd.ddsCaps, &lpBack)))
		return FALSE;


	return TRUE;
	}




void ShutDownDX()
	{
	if (lpdd)
		lpdd->RestoreDisplayMode();
		
	if (lpPrimary)
		lpPrimary->Release();

	if (lpdd)
		lpdd->Release();
	}




int CreateOffScreenSurface(LPDIRECTDRAWSURFACE4 * lplpSurface, int width, int height, int colorkey)
	{
	DDSURFACEDESC2 ddsd;
	DDCOLORKEY ddcolorkey;

	
	memset(&ddsd, 0, sizeof(ddsd));

	ddsd.dwSize			= sizeof(ddsd);
	ddsd.dwFlags		= DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_CKSRCBLT;
	ddsd.dwWidth		= width;
	ddsd.dwHeight		= height;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;


	if (FAILED(lpdd->CreateSurface(&ddsd, lplpSurface, NULL)))
		{
		OutputDebugString("COULD NOT CREATE THE SURFACE\n\r");
		return FALSE;
		}


	if (colorkey > -1)
		{
		ddcolorkey.dwColorSpaceLowValue		= colorkey;
		ddcolorkey.dwColorSpaceHighValue	= colorkey;

		(*lplpSurface)->SetColorKey(DDCKEY_SRCBLT, &ddcolorkey);
		}


	return TRUE;
	}





