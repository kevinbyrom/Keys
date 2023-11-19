#ifndef __DirectX_H
#define __DirectX_H


#include "ddraw.h"


#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	240
#define SCREEN_BPP		8



// DIRECT DRAW OBJECTS 

extern LPDIRECTDRAW4			lpdd;
extern LPDIRECTDRAWSURFACE4		lpPrimary,
								lpBack;

extern LPDIRECTDRAWPALETTE	lpPalette;



int InitDX();
void ShutDownDX();
int CreateOffScreenSurface(LPDIRECTDRAWSURFACE4 * lplpSurface, int width, int height, int colorkey = -1);



#endif