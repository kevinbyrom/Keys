#ifndef __GSPRITE_H
#define __GSPRITE_H


#include "DataTypes.h"
#include "GBitmap.h"


class GSPRITE
	{
	public:
		
		GSPRITE();
		~GSPRITE();


		// Acquiring routines //

		void Grab(GBITMAP * pSrc, int x, int y, int width, int height, U32 transclr);
		void Destroy();
		

		// Blitting routines //

		void Blit(GBITMAP * pDest, int x, int y);
		void Blit8(GBITMAP * pDest, int x, int y, int width, int height, int sx, int sy);
		void Blit16(GBITMAP * pDest, int x, int y, int width, int height, int sx, int sy);


		// Drawing routines //

		void Draw(GBITMAP * pDest, int x, int y);
		void Draw8(GBITMAP * pDest, int x, int y, int width, int height, int sx, int sy);
		void Draw16(GBITMAP * pDest, int x, int y, int width, int height, int sx, int sy);


	protected:

		U8	m_BPP;
		U32 m_Width,
			m_Height;

		U8	* m_pBits8;
		U16 * m_pBits16;
		
		U8  m_TransClr8;
		U16 m_TransClr16;
	};



#endif