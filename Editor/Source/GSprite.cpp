#include <windows.h>
#include "GSprite.h"
#include "Memory.h"



//////////////////////////////
//							//
// Constructor / Destructor //
//							//
//////////////////////////////



GSPRITE::GSPRITE()
	{
	m_Width			= 0;
	m_Height		= 0;
	m_BPP			= 0;
	m_TransClr8		= 0;
	m_TransClr16	= 0;

	m_pBits8		= NULL;
	m_pBits16		= NULL;
	}




GSPRITE::~GSPRITE()
	{
	if (m_pBits8 != NULL)
		free(m_pBits8);
	}




////////////////////////
//	                  //
// Acquiring routines //
//	                  //
////////////////////////



void GSPRITE::Grab(GBITMAP * pSrc, int x, int y, int width, int height, U32 transclr)
	{
	U8  * pSrcBits8;
	U16 * pSrcBits16;
	U32 SrcPitch;

	
	pSrcBits8	= pSrc->GetBits8();
	pSrcBits16	= (U16 *)pSrcBits8;
	SrcPitch	= pSrc->GetPitch();

	m_BPP		= pSrc->GetBPP();
	m_Width		= width;
	m_Height	= height;


	m_pBits8	= (U8 *)malloc((m_Width * m_Height) * (m_BPP / 8));
	m_pBits16	= (U16 *)m_pBits8;


	if (m_BPP == 8)
		m_TransClr8		= (U8)transclr;
	else if (m_BPP == 16)
		m_TransClr16	= (U16)transclr;



	// Read each row //

	for (U32 ypos = 0; ypos < m_Height; ypos++)
		{
		switch (m_BPP)
			{
			case 8:
				memcpy(&m_pBits8[ypos * m_Width], &pSrcBits8[x + ((y + ypos) * SrcPitch)], m_Width);
				break;

			case 16:
				memcpy16(&m_pBits16[ypos * m_Width], &pSrcBits16[x + ((y + ypos) * SrcPitch)], m_Width);
				break;
			}
		}
	}




void GSPRITE::Destroy()
	{
	if (m_pBits8 != NULL)
		free(m_pBits8);


	m_Width			= 0;
	m_Height		= 0;
	m_BPP			= 0;
	m_TransClr8		= 0;
	m_TransClr16	= 0;

	m_pBits8		= NULL;
	m_pBits16		= NULL;
	}




///////////////////////
//					 //
// Blitting routines //
//					 //
///////////////////////




void GSPRITE::Blit(GBITMAP * pDest, int x, int y)
	{
	int sx,
		sy;

	U32	width, 
		height;
	
	
	// Set default variables //

	sx		= 0;
	sy		= 0;
	width	= m_Width;
	height	= m_Height;


	// Perform clipping if necessary //

	if (x < 0)
		{
		x = 0;
		sx = -x;
		width -= sx;
		}


	if (y < 0)
		{
		y = 0;
		sy = -y;
		height -= sy;
		}


	if (x + width >= pDest->GetWidth())
		width = pDest->GetWidth() - x;


	if (y + height >= pDest->GetHeight())
		height = pDest->GetHeight() - y;


	// Return if the sprite is out of the bitmap range //

	if (width <= 0 || height <= 0)
		return;
		

	// Call the proper Blit function based on the BPP //

	switch(m_BPP)
		{
		case 8:
			Blit8(pDest, x, y, width, height, sx, sy);
			return;


		case 16:
			Blit16(pDest, x, y, width, height, sx, sy);
			return;
		}
	}




void GSPRITE::Blit8(GBITMAP * pDest, int x, int y, int width, int height, int sx, int sy)
	{
	U8 * pCopyBits8;

	U8 * pDestBits8,
	   * pCopyDestBits8;

	U32 DestPitch;
	U32 ypos;


	// If the sprite has not been initialized, or the destination //
	// bitmap is invalid, just return							  //

	if (m_pBits8 == NULL || pDest == NULL)
		return;


	// Get the destination pointer and bits //

	DestPitch		= pDest->GetPitch();
	pDestBits8		= pDest->GetBits8();


	// Blit the sprite to the destination bitmap //

	pCopyBits8		= &m_pBits8[sx + (sy * m_Width)];
	pCopyDestBits8  = &pDestBits8[x + (y * DestPitch)];


	for (ypos = 0; ypos < height; ypos++)
		{
		memcpy(pCopyDestBits8, pCopyBits8, width);

		pCopyBits8		+= m_Width;
		pCopyDestBits8	+= DestPitch;
		}
	}




void GSPRITE::Blit16(GBITMAP * pDest, int x, int y, int width, int height, int sx, int sy)
	{
	U16 * pCopyBits16;

	U16 * pDestBits16,
	    * pCopyDestBits16;

	U32 DestPitch;
	U32 ypos;


	// If the sprite has not been initialized, or the destination //
	// bitmap is invalid, just return							  //

	if (m_pBits16 == NULL || pDest == NULL)
		return;


	// Get the destination pointer and bits //

	DestPitch		= pDest->GetPitch();
	pDestBits16		= pDest->GetBits16();


	// Blit the sprite to the destination bitmap //

	pCopyBits16		= &m_pBits16[sx + (sy * m_Width)];
	pCopyDestBits16  = &pDestBits16[x + (y * DestPitch)];


	for (ypos = 0; ypos < height; ypos++)
		{
		memcpy16(pCopyDestBits16, pCopyBits16, width);

		pCopyBits16		+= m_Width;
		pCopyDestBits16	+= DestPitch;
		}
	}




//////////////////////
//					//
// Drawing routines //
//					//
//////////////////////




void GSPRITE::Draw(GBITMAP * pDest, int x, int y)
	{
	int sx,
		sy;

	U32	width, 
		height;
	
	
	// Set default variables //

	sx		= 0;
	sy		= 0;
	width	= m_Width;
	height	= m_Height;


	// Perform clipping if necessary //

	if (x < 0)
		{
		x = 0;
		sx = -x;
		width -= sx;
		}


	if (y < 0)
		{
		y = 0;
		sy = -y;
		height -= sy;
		}


	if (x + width >= pDest->GetWidth())
		width = pDest->GetWidth() - x;


	if (y + height >= pDest->GetHeight())
		height = pDest->GetHeight() - y;


	// Return if the sprite is out of the bitmap range //

	if (width <= 0 || height <= 0)
		return;
		

	// Call the proper Draw function based on the BPP //

	switch(m_BPP)
		{
		case 8:
			Draw8(pDest, x, y, width, height, sx, sy);
			return;


		case 16:
			Draw16(pDest, x, y, width, height, sx, sy);
			return;
		}
	}




void GSPRITE::Draw8(GBITMAP * pDest, int x, int y, int width, int height, int sx, int sy)
	{
	U8 * pCopyBits8;

	U8 * pDestBits8,
	   * pCopyDestBits8;

	U32 DestPitch;
	U32 ypos, xpos;


	// If the sprite has not been initialized, or the destination //
	// bitmap is invalid, just return							  //

	if (m_pBits8 == NULL || pDest == NULL)
		return;


	// Get the destination pointer and bits //

	DestPitch		= pDest->GetPitch();
	pDestBits8		= pDest->GetBits8();


	// Now, go through each pixel, checking if the pixel to place is //
	// transparent or not, if not, then place the pixel				 //

	for (ypos = 0; ypos < height; ypos++)
		{
		pCopyBits8		= &m_pBits8[sx + ((sy + ypos) * m_Width)];
		pCopyDestBits8  = &pDestBits8[x + ((y + ypos) * DestPitch)];

		for (xpos = 0; xpos < width; xpos++)
			{
			if (*pCopyBits8 != m_TransClr8)
				*pCopyDestBits8 = *pCopyBits8;

			pCopyBits8++;
			pCopyDestBits8++;
			}
		}
	}




void GSPRITE::Draw16(GBITMAP * pDest, int x, int y, int width, int height, int sx, int sy)
	{
	U16 * pCopyBits16;

	U16 * pDestBits16,
	    * pCopyDestBits16;

	U32 DestPitch;
	U32 ypos, xpos;


	// If the sprite has not been initialized, or the destination //
	// bitmap is invalid, just return							  //

	if (m_pBits16 == NULL || pDest == NULL)
		return;


	// Get the destination pointer and bits //

	DestPitch		= pDest->GetPitch();
	pDestBits16		= pDest->GetBits16();


	// Now, go through each pixel, checking if the pixel to place is //
	// transparent or not, if not, then place the pixel				 //

	for (ypos = 0; ypos < height; ypos++)
		{
		pCopyBits16		= &m_pBits16[sx + ((sy + ypos) * m_Width)];
		pCopyDestBits16 = &pDestBits16[x + ((y + ypos) * DestPitch)];

		for (xpos = 0; xpos < width; xpos++)
			{
			if (*pCopyBits16 != m_TransClr16)
				*pCopyDestBits16 = *pCopyBits16;

			pCopyBits16++;
			pCopyDestBits16++;
			}
		}
	}