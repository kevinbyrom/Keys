#include <windows.h>
#include <io.h>
#include "GBITMAP.h"
#include "Memory.h"




//////////////////////////////
//			                //
// CONSTRUCTOR / DESTRUCTOR //
//			                //
//////////////////////////////


GBITMAP::GBITMAP()
	{
	m_BPP		= 0;
	m_Width		= 0;
	m_Height	= 0;
	m_Pitch		= 0;

	m_pBits8	= NULL;
	m_pBits16	= NULL;
	}




GBITMAP::~GBITMAP()
	{
	if (m_pBits8 != NULL)
		free(m_pBits8);
	}





/////////////////////////////////////
//						           //
// Creation / Destruction routines //
//						           //
/////////////////////////////////////


BOOL GBITMAP::Create(U32 Width, U32 Height, U8 BPP, U16 DefColor)
	{

	// If a bitmap has already been created, destroy it first //

	if (m_pBits8 != NULL)
		Destroy();



	// Avoid creation of bitmap if parameters are invalid //

	if (Width == 0 || Height == 0 || BPP == 0)
		return FALSE;



	// Allocate a bitmap //

	m_pBits8	= (U8 *)malloc((Width * Height) * (BPP / 8));
	m_pBits16	= (U16 *)m_pBits8;



	// If allocation failed, then return false //

	if (m_pBits8 == NULL)
		{
		Destroy();
		
		return FALSE;
		}



	// Otherwise, set the properties //

	m_BPP		= BPP;
	m_Width		= Width;
	m_Height	= Height;
	m_Pitch		= Width;



	// Set the default color //

	Fill(DefColor);


	return TRUE;
	}





void GBITMAP::Destroy()
	{
	// Simply deallocate memory used for the bits //

	if (m_pBits8 != NULL)
		free(m_pBits8);

	m_pBits8	= NULL;
	m_pBits16	= NULL;

	m_BPP		= 0;
	m_Width		= 0;
	m_Height	= 0;
	m_Pitch		= 0;
	}





void GBITMAP::AttachToDDSurface(LPDIRECTDRAWSURFACE4 lpSurface)
	{
	m_lpSurface = lpSurface;
	}






//////////////////////
//					//
// Drawing Routines //
//					//
//////////////////////




void GBITMAP::SetPixel(int x, int y, U32 Color)
	{

	// Return if there is not data to write to //

	if (m_pBits8 == NULL)
		return;


	// Call the proper SetPixel routine based on the BPP //

	switch(m_BPP)
		{	
		case 8:
			SetPixel8(x, y, (U8)Color);
			return;


		case 16:
			SetPixel16(x, y, (U16)Color);
			return;
		}
	}




void GBITMAP::SetPixel8(int	x, int y, U8 Color)
	{
	m_pBits8[x + (y * m_Pitch)] = Color;
	}




void GBITMAP::SetPixel16(int x, int y, U16 Color)
	{
	m_pBits16[x + (y * m_Pitch)] = Color;
	}






U32 GBITMAP::GetPixel(int x, int y)
	{

	// Return 0 if there is not data to read from //

	if (m_pBits8 == NULL)
		return 0;


	// Call the proper GetPixel routine based on the BPP //

	switch(m_BPP)
		{
		case 8:
			return (U32)GetPixel8(x, y);


		case 16:
			return (U32)GetPixel16(x, y);
		}


	return 0;
	}




U8 GBITMAP::GetPixel8(int x, int y)
	{
	return m_pBits8[x + (y * m_Pitch)];
	}




U16 GBITMAP::GetPixel16(int x, int y)
	{
	return m_pBits16[x + (y * m_Pitch)];
	}






void GBITMAP::Fill(U32 Color)
	{

	// Return if there is not data to write to //

	if (m_pBits8 == NULL)
		return;


	// Call the proper Fill routine based on the BPP //

	switch(m_BPP)
		{
		case 8:
			Fill8((U8)Color);
			return;


		case 16:
			Fill16((U16)Color);
			return;
		}
	}




void GBITMAP::Fill8(U8 Color)
	{

	// Use the standard memset to fill bytes //

	if (m_Pitch == m_Width)
		memset(m_pBits8, Color, (m_Width * m_Height)); 
	else
		for (U32 y = 0; y < m_Height; y++)
			memset(&m_pBits8[(y * m_Pitch)], Color, m_Width);
	}




void GBITMAP::Fill16(U16 Color)
	{

	// Use the WORD memset to fill WORDS //

	if (m_Pitch == m_Width)
		memset16(m_pBits16, Color, (m_Width * m_Height));
	else
		for (U32 y = 0; y < m_Height; y++)
			memset16(&m_pBits16[(y * m_Pitch)], Color, m_Width);
	}






void GBITMAP::DrawLine(int x1, int y1, int x2, int y2, U32 Color)
	{
	// Return if there is not data to write to //

	if (m_pBits8 == NULL)
		return;


	// Call the proper Line routine based on the BPP //

	switch(m_BPP)
		{
		case 8:
			if (x1 == x2)
				DrawVLine8(x1, y1, y2, (U8)Color);
			else if (y1 == y2)
				DrawHLine8(x1, y1, y2, (U8)Color);
			else
				DrawLine8(x1, y1, x2, y2, (U8)Color);
			return;


		case 16:
			if (x1 == x2)
				DrawVLine16(x1, y1, y2, (U16)Color);
			else if (y1 == y2)
				DrawHLine16(x1, y1, y2, (U16)Color);
			else
				DrawLine16(x1, y1, x2, y2, (U16)Color);
			return;
		}
	}




void GBITMAP::DrawLine8(int x1, int y1, int x2, int y2, U8 Color)
	{
	int y_unit, x_unit;
	int y_diff;
	int x_diff;
	int error_term;
	int length;
	int i;



	// Calculate the difference between the y coordinates //

	y_diff = y2 - y1;


	if (y_diff < 0)
		{
		y_diff = -y_diff;
		y_unit = -1;
		}
	else
		{
		y_unit = 1;
		}


	// Calculate the difference between the x coordinates //

	x_diff = x2 - x1;


	if (x_diff < 0)
		{
		x_diff = -x_diff;
		x_unit = -1;
		}
	else
		{
		x_unit = 1;
		}


	// Initialize the error term //

	error_term = 0;


	// If the difference is bigger in x dimension... //

	if (x_diff > y_diff)
		{
		length = x_diff + 1;

		for (i = 0; i < length; i++)
			{
			if (x1 >= 0 && x1 < (int)m_Width && y1 >= 0 && y1 < (int)m_Height)
				SetPixel8(x1, y1, Color);


			// Move the x position //

			x1 += x_unit;


			// Check the error term //

			error_term += y_diff;

			if (error_term > x_diff)
				{
				error_term	-= x_diff;
				y1			+= y_unit;
				}
			}
		}
	else
		{
		length = y_diff + 1;

		for (i = 0; i < length; i++)
			{
			if (x1 >= 0 && x1 < (int)m_Width && y1 >= 0 && y1 < (int)m_Height)
				SetPixel8(x1, y1, Color);


			// Move the y position //

			y1 += y_unit;


			// Check the error term //

			error_term += x_diff;

			if (error_term > x_diff)
				{
				error_term	-= y_diff;
				x1			+= x_unit;
				}
			}
		}
	}





void GBITMAP::DrawLine16(int x1, int y1, int x2, int y2, U16 Color)
	{
	int y_unit, x_unit;
	int y_diff;
	int x_diff;
	int error_term;
	int length;
	int i;



	// Calculate the difference between the y coordinates //

	y_diff = y2 - y1;


	if (y_diff < 0)
		{
		y_diff = -y_diff;
		y_unit = -1;
		}
	else
		{
		y_unit = 1;
		}


	// Calculate the difference between the x coordinates //

	x_diff = x2 - x1;


	if (x_diff < 0)
		{
		x_diff = -x_diff;
		x_unit = -1;
		}
	else
		{
		x_unit = 1;
		}


	// Initialize the error term //

	error_term = 0;


	// If the difference is bigger in x dimension... //

	if (x_diff > y_diff)
		{
		length = x_diff + 1;

		for (i = 0; i < length; i++)
			{
			if (x1 >= 0 && x1 < (int)m_Width && y1 >= 0 && y1 < (int)m_Height)
				SetPixel16(x1, y1, Color);


			// Move the x position //

			x1 += x_unit;


			// Check the error term //

			error_term += y_diff;

			if (error_term > x_diff)
				{
				error_term	-= x_diff;
				y1			+= y_unit;
				}
			}
		}
	else
		{
		length = y_diff + 1;

		for (i = 0; i < length; i++)
			{
			if (x1 >= 0 && x1 < (int)m_Width && y1 >= 0 && y1 < (int)m_Height)
				SetPixel16(x1, y1, Color);


			// Move the y position //

			y1 += y_unit;


			// Check the error term //

			error_term += x_diff;

			if (error_term > x_diff)
				{
				error_term	-= y_diff;
				x1			+= x_unit;
				}
			}
		}
	}




void GBITMAP::DrawHLine8(int x1, int x2, int y, U8 Color)
	{
	int xswap;
	int width;


	// If y or x1 and x2 is out of bitmap limits, just return //

	if (y < 0 || y >= (int)m_Height || (x1 < 0 && x2 < 0) || (x1 >= (int)m_Width && x2 >= (int)m_Width))
		return;


	// If x2 is less than x1, swap the values //

	if (x2 < x1)
		{
		xswap	= x1;
		x1		= x2;
		x2		= xswap;
		}


	// Be sure that x1 and x2 are within bitmap limits //

	if (x1 < 0)
		x1 = 0;

	if (x1 >= (int)m_Width)
		x1 = (int)m_Width;

	if (x2 < 0)
		x2 = 0;

	if (x2 >= (int)m_Width)
		x2 = (int)m_Width;


	// Calculate the line width //

	width = (x2 - x1) + 1;


	// Draw the line using memset //

	memset(&m_pBits8[x1 + (y * m_Pitch)], Color, width);
	}




void GBITMAP::DrawHLine16(int x1, int x2, int y, U16 Color)
	{
	int xswap;
	int width;


	// If y or x1 and x2 is out of bitmap limits, just return //

	if (y < 0 || y >= (int)m_Height || (x1 < 0 && x2 < 0) || (x1 >= (int)m_Width && x2 >= (int)m_Width))
		return;


	// If x2 is less than x1, swap the values //

	if (x2 < x1)
		{
		xswap	= x1;
		x1		= x2;
		x2		= xswap;
		}


	// Be sure that x1 and x2 are within bitmap limits //

	if (x1 < 0)
		x1 = 0;

	if (x1 >= (int)m_Width)
		x1 = (int)m_Width;

	if (x2 < 0)
		x2 = 0;

	if (x2 >= (int)m_Width)
		x2 = (int)m_Width;


	// Calculate the line width //

	width = (x2 - x1) + 1;


	// Draw the line using memset //

	memset16(&m_pBits16[x1 + (y * m_Pitch)], Color, width);
	}




void GBITMAP::DrawVLine8(int x1, int y1, int y2, U8 Color)
	{
	int yswap;
	int height;
	int ypos;


	// If x or y1 and y2 is out of bitmap limits, just return //

	if (x1 < 0 || x1 >= (int)m_Width || (y1 < 0 && y2 < 0) || (y1 >= (int)m_Height && y2 >= (int)m_Height))
		return;


	// If y2 is less than y1, swap the values //

	if (y2 < y1)
		{
		yswap	= y1;
		y1		= y2;
		y2		= yswap;
		}


	// Be sure that y1 and y2 are within bitmap limits //

	if (y1 < 0)
		y1 = 0;

	if (y1 >= (int)m_Height)
		y1 = (int)m_Height;

	if (y2 < 0)
		y2 = 0;

	if (y2 >= (int)m_Height)
		y2 = (int)m_Height;


	// Calculate the line height //

	height = (y2 - y1) + 1;


	// Draw each pixel in order vertically //

	for (ypos = 0; ypos < height; ypos++)
		SetPixel8(x1, y1 + ypos, Color);
	}




void GBITMAP::DrawVLine16(int x1, int y1, int y2, U16 Color)
	{
	int yswap;
	int height;
	int ypos;


	// If x or y1 and y2 is out of bitmap limits, just return //

	if (x1 < 0 || x1 >= (int)m_Width || (y1 < 0 && y2 < 0) || (y1 >= (int)m_Height && y2 >= (int)m_Height))
		return;


	// If y2 is less than y1, swap the values //

	if (y2 < y1)
		{
		yswap	= y1;
		y1		= y2;
		y2		= yswap;
		}


	// Be sure that y1 and y2 are within bitmap limits //

	if (y1 < 0)
		y1 = 0;

	if (y1 >= (int)m_Height)
		y1 = (int)m_Height;

	if (y2 < 0)
		y2 = 0;

	if (y2 >= (int)m_Height)
		y2 = (int)m_Height;


	// Calculate the line height //

	height = (y2 - y1) + 1;


	// Draw each pixel in order vertically //

	for (ypos = 0; ypos < height; ypos++)
		SetPixel16(x1, y1 + ypos, Color);
	}




void GBITMAP::DrawRect(int x1, int y1, int x2, int y2, U32 Color)
	{
	// Return if there is not data to write to //

	if (m_pBits8 == NULL)
		return;


	// Call the proper Draw Rect routine based on the BPP //

	switch(m_BPP)
		{
		case 8:
			DrawRect8(x1, y1, x2, y2, (U8)Color);
			return;


		case 16:
			DrawRect16(x1, y1, x2, y2, (U16)Color);
			return;
		}
	}




void GBITMAP::DrawRect8(int x1, int y1, int x2, int y2, U8 Color)
	{
	DrawHLine8(x1, x2, y1, Color);
	DrawHLine8(x1, x2, y2, Color);
	DrawVLine8(x1, y1, y2, Color);
	DrawVLine8(x2, y1, y2, Color);
	}




void GBITMAP::DrawRect16(int x1, int y1, int x2, int y2, U16 Color)
	{
	DrawHLine16(x1, x2, y1, Color);
	DrawHLine16(x1, x2, y2, Color);
	DrawVLine16(x1, y1, y2, Color);
	DrawVLine16(x2, y1, y2, Color);
	}




void GBITMAP::DrawFillRect(int x1, int y1, int x2, int y2, U32 Color)
	{
	// Return if there is not data to write to //

	if (m_pBits8 == NULL)
		return;


	// Call the proper Draw Rect routine based on the BPP //

	switch(m_BPP)
		{
		case 8:
			DrawFillRect8(x1, y1, x2, y2, (U8)Color);
			return;


		case 16:
			DrawFillRect16(x1, y1, x2, y2, (U16)Color);
			return;
		}
	}




void GBITMAP::DrawFillRect8(int x1, int y1, int x2, int y2, U8 Color)
	{
	int y;
	int swap;
	

	if (y1 > y2)
		{
		swap	= y1;
		y2		= swap;
		y1		= y2;
		}
	

	for (y = y1; y <= y2; y++)
		DrawHLine8(x1, x2, y, Color);
	}




void GBITMAP::DrawFillRect16(int x1, int y1, int x2, int y2, U16 Color)
	{
	int y;
	int swap;
	

	if (y1 > y2)
		{
		swap	= y1;
		y2		= swap;
		y1		= y2;
		}
	

	for (y = y1; y <= y2; y++)
		DrawHLine16(x1, x2, y, Color);
	}




///////////////////////
//					 //
// Blitting routines //
//					 //
///////////////////////




void GBITMAP::Blit(GBITMAP * pSrc, int x, int y, int width, int height, int sx, int sy)
	{
	U32 BPPSrc, BPPDest;



	// If one of the bitmaps are invalid, just return //

	if (pSrc == NULL)
		return;



	// Do adjustments to be sure that the bitmap is clipped //

	if (x < 0)
		{
		sx		+= -x;
		width	+= x;
		x		= 0;
		}


	if (y < 0)
		{
		sy		+= -y;
		height	+= y;
		y		= 0;
		}


	if (x + width >= (int)m_Width)
		{
		width = (int)m_Width - x;
		}


	if (y + height >= (int)m_Height)
		{
		height = (int)m_Height - y;
		}


	if (sx + width >= (int)pSrc->GetWidth())
		{
		width = (int)pSrc->GetWidth() - sx;
		}


	if (sy + height >= (int)pSrc->GetHeight())
		{
		height = (int)pSrc->GetHeight() - sy;
		}



	// Compare the BPP of the two bitmaps to find what draw sprite //
	// function to call											   //

	BPPSrc	= pSrc->GetBPP();
	BPPDest	= GetBPP();


	switch(BPPSrc)
		{
		case 8:
			if (BPPDest == 8)
				Blit8(pSrc, x, y, width, height, sx, sy);
			else if (BPPDest == 16)
				Blit8To16(pSrc, x, y, width, height, sx, sy);		
			return;


		case 16:
			if (BPPDest == 8)
				Blit16To8(pSrc, x, y, width, height, sx, sy);
			else if (BPPDest == 16)
				Blit16(pSrc, x, y, width, height, sx, sy);
			return;
		}
	}




void GBITMAP::Blit8(GBITMAP * pSrc, int x, int y, int width, int height, int sx, int sy)
	{
	U8 * pSrcBits8;
	U8 * pCopySrcBits8;
	U8 * pCopyDestBits8;
	U32 SrcPitch;
	

	// Get a pointer to the source bits //

	pSrcBits8 = pSrc->GetBits8();


	// If there are no source bits, just return //

	if (pSrcBits8 == NULL || m_pBits8 == NULL)
		return;


	// Get the source pitch //

	SrcPitch = pSrc->GetPitch();
	

	// Copy the bits over, row by row //

	pCopySrcBits8	= &pSrcBits8[sx + (sy * SrcPitch)];
	pCopyDestBits8	= &m_pBits8[x + (y * m_Pitch)];


	for (int ypos = 0; ypos < height; ypos++)
		{
		memcpy(pCopyDestBits8, pCopySrcBits8, width);

		pCopySrcBits8	+= SrcPitch;
		pCopyDestBits8	+= m_Pitch;
		}
	}




void GBITMAP::Blit16(GBITMAP * pSrc, int x, int y, int width, int height, int sx, int sy)
	{
	U16 * pSrcBits16;
	U16 * pCopySrcBits16;
	U16 * pCopyDestBits16;
	U32 SrcPitch;
	

	// Get a pointer to the source bits //

	pSrcBits16 = pSrc->GetBits16();


	// If there are no source bits, just return //

	if (pSrcBits16 == NULL || m_pBits16 == NULL)
		return;


	// Get the source pitch //

	SrcPitch = pSrc->GetPitch();
	

	// Copy the bits over, row by row //

	pCopySrcBits16	= &pSrcBits16[sx];
	pCopyDestBits16	= &m_pBits16[x];


	for (int ypos = 0; ypos < height; ypos++)
		{
		memcpy16(pCopyDestBits16, pCopySrcBits16, width);

		pCopySrcBits16	+= SrcPitch;
		pCopyDestBits16	+= m_Pitch;
		}
	}




void GBITMAP::Blit8To16(GBITMAP * pSrc, int x, int y, int width, int height, int sx, int sy)
	{
	}




void GBITMAP::Blit16To8(GBITMAP * pSrc, int x, int y, int width, int height, int sx, int sy)
	{
	}





///////////////////////////
//						 //
// Manipulation routines //
//						 //
///////////////////////////



void GBITMAP::FlipVertical()
	{
	U8 * pBuffer = NULL; 
	int Size;
	U32 index;     


	// allocate the temporary buffer //
	
	Size = (m_Width * m_Height) * (m_BPP / 8);

	if (!(pBuffer = (U8 *)malloc(Size)))
		return;


	// Copy image to work area //
	
	for (index = 0; index < m_Height; index++)
		memcpy(&pBuffer[index * m_Width], &m_pBits8[index * m_Pitch], m_Width);


	// Flip vertically //
	
	for (index = 0; index < m_Height; index++)
		memcpy(&m_pBits8[index * m_Pitch], &pBuffer[(m_Height - 1 - index) * m_Width], m_Width);


	// Release the memory //
	
	free(pBuffer);
	}






//////////////////////
//					//
// Surface routines //
//					//
//////////////////////




BOOL GBITMAP::LockDDSurface()
	{
	DDSURFACEDESC2 ddsd;


	// If there is no surface attached, return false //

	if (m_lpSurface == NULL)
		return FALSE;


	// Lock the surface //

	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	m_lpSurface->Lock(NULL, &ddsd, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL); 


	// Get the surface properties //
	
	m_Pitch		= ddsd.lPitch;
	m_Width		= ddsd.dwWidth;
	m_Height	= ddsd.dwHeight;
	m_BPP		= (U8)ddsd.ddpfPixelFormat.dwRGBBitCount;


	// Get pointer to surface //

	m_pBits8	= (U8 *)ddsd.lpSurface;
	m_pBits16	= (U16 *)ddsd.lpSurface;


	return TRUE;
	}




BOOL GBITMAP::UnlockDDSurface()
	{

	// If there is no surface attached, return false //

	if (m_lpSurface == NULL)
		return FALSE;


	// Unlock the surface //

	m_lpSurface->Unlock(NULL);


	// Get rid of the surface properties //

	m_Pitch		= 0;
	m_Width		= 0;
	m_Height	= 0;
	m_BPP		= 0;

	m_pBits8	= NULL;
	m_pBits16	= NULL;


	return TRUE;
	}





///////////////////////////////
//							 //
// Save and loading routines //
//							 //
///////////////////////////////




BOOL GBITMAP::SaveBMP(LPCTSTR lpstrFileName)
	{
	return TRUE;
	}




BOOL GBITMAP::LoadBMP(LPCTSTR lpstrFileName)
	{
	HFILE hFile;
	OFSTRUCT file_data;
	
	// Attempt to open the bitmap file, if failure, return false //

	if ((hFile = OpenFile(lpstrFileName, &file_data, OF_READ)) == -1)
		{
		return FALSE;
		}


	// Continue the loading at another file //

	return LoadBMP(hFile);
	}




BOOL GBITMAP::SavePCX(LPCTSTR lpstrFileName)
	{
	return TRUE;
	}




BOOL GBITMAP::LoadPCX(LPCTSTR lpstrFileName)
	{
	return TRUE;
	}




BOOL GBITMAP::SaveBMP(HFILE hFile)
	{
	return TRUE;
	}




BOOL GBITMAP::LoadBMP(HFILE hFile)
	{
	BITMAPFILEHEADER bmpfileheader;
	BITMAPINFOHEADER bmpinfoheader;
	PALETTEENTRY	 bmppalette[256];

	int	index;
	int tempcolor;



	// Read the bitmap file header //

	_lread(hFile, &bmpfileheader, sizeof(BITMAPFILEHEADER));



	// If the file is not a bitmap then return //

	if (bmpfileheader.bfType != BITMAP_ID)
		{
		_lclose(hFile);

		return FALSE;
		}



	// Read the bitmap info header //

	_lread(hFile, &bmpinfoheader, sizeof(BITMAPINFOHEADER));



	// If reading an 8 bit bitmap, then we need a palette //

	if (bmpinfoheader.biBitCount == 8)
		{
		_lread(hFile, &bmppalette, MAX_COLORS_PALETTE * sizeof(PALETTEENTRY));


		for (index = 0; index < MAX_COLORS_PALETTE; index++)
			{
			tempcolor					= bmppalette[index].peRed;
			m_Palette[index].peRed		= bmppalette[index].peBlue;
			m_Palette[index].peBlue		= tempcolor;
			m_Palette[index].peGreen	= bmppalette[index].peGreen;

			m_Palette[index].peFlags	= PC_NOCOLLAPSE;
			}
		}


	
	// Set the size of the image //

	bmpinfoheader.biSizeImage = (bmpinfoheader.biWidth * bmpinfoheader.biHeight) * (bmpinfoheader.biBitCount / 8);



	// Seek the end of the file //

	_lseek(hFile, -(int)(bmpinfoheader.biSizeImage), SEEK_END);


	if (bmpinfoheader.biBitCount==8 ||
		bmpinfoheader.biBitCount==16 ||
		bmpinfoheader.biBitCount==24)
		{
		// If there is already a bitmap, free it //

		if (m_pBits8 != NULL)
			{
			free(m_pBits8);

			m_pBits16 = NULL;
			}


		// Allocate new room for the bitmap //

		if (!(m_pBits8 = (U8 *)malloc(bmpinfoheader.biSizeImage)))
			{
			_lclose(hFile);

			return FALSE;
			}


		m_pBits16 = (U16 *)m_pBits8;
		

		// Read the actual bit data //

		_lread(hFile, m_pBits8, bmpinfoheader.biSizeImage);		


		// Set the bitmap properties //

		m_Width		= bmpinfoheader.biWidth;
		m_Height	= bmpinfoheader.biHeight;
		m_BPP		= (U8)bmpinfoheader.biBitCount;
		m_Pitch		= bmpinfoheader.biWidth;
		}
	else
		{
		return FALSE;
		}



	// Close the file when done //

	_lclose(hFile);



	// Flip the bitmap //

	FlipVertical();


	return TRUE;
	}




BOOL GBITMAP::SavePCX(HFILE hFile)
	{
	return TRUE;
	}




BOOL GBITMAP::LoadPCX(HFILE hFile)
	{
	return TRUE;
	}