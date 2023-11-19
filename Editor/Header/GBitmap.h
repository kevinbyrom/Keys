#ifndef __GBITMAP_H
#define __GBITMAP_H


#include "DDraw.h"
#include "DataTypes.h"


#define BITMAP_ID			0x4D42
#define MAX_COLORS_PALETTE	256


class GBITMAP
	{
	public:

		GBITMAP();
		~GBITMAP();


		U8 GetBPP()						{ return m_BPP; }
		U32 GetWidth()					{ return m_Width; }
		U32 GetHeight()					{ return m_Height; }
		U32 GetPitch()					{ return m_Pitch; }
		U8 * GetBits8()					{ return m_pBits8; }
		U16 * GetBits16()				{ return m_pBits16; }
		PALETTEENTRY * GetPalette()		{ return m_Palette; }

		// Creation / Destruction routines //

		BOOL Create(U32 Width, U32 Height, U8 BPP, U16 DefColor = 0);
		void Destroy();

		void AttachToDDSurface(LPDIRECTDRAWSURFACE4 lpSurface);


		// Drawing routines //

		void SetPixel(int x, int y, U32 Color);
		void SetPixel8(int x, int y, U8 Color);
		void SetPixel16(int x, int y, U16 Color);

		U32 GetPixel(int x, int y);
		U8  GetPixel8(int x, int y);
		U16 GetPixel16(int x, int y);

		void Fill(U32 Color);
		void Fill8(U8 Color);
		void Fill16(U16 Color);

		void DrawLine(int x1, int y1, int x2, int y2, U32 Color);
		void DrawLine8(int x1, int y1, int x2, int y2, U8 Color);
		void DrawLine16(int x1, int y1, int x2, int y2, U16 Color);
		void DrawHLine8(int x1, int x2, int y, U8 Color);
		void DrawHLine16(int x1, int x2, int y, U16 Color);
		void DrawVLine8(int x1, int y1, int y2, U8 Color);
		void DrawVLine16(int x1, int y1, int y2, U16 Color);
		
		void DrawRect(int x1, int y1, int x2, int y2, U32 Color);
		void DrawRect8(int x1, int y1, int x2, int y2, U8 Color);
		void DrawRect16(int x1, int y1, int x2, int y2, U16 Color);

		void DrawFillRect(int x1, int y1, int x2, int y2, U32 Color);
		void DrawFillRect8(int x1, int y1, int x2, int y2, U8 Color);
		void DrawFillRect16(int x1, int y1, int x2, int y2, U16 Color);


		// Blitting routines //

		void Blit(GBITMAP * pSrc, int x, int y, int width, int height, int sx, int sy);
		void Blit8(GBITMAP * pSrc, int x, int y, int width, int height, int sx, int sy);
		void Blit16(GBITMAP * pSrc, int x, int y, int width, int height, int sx, int sy);
		void Blit8To16(GBITMAP * pSrc, int x, int y, int width, int height, int sx, int sy);
		void Blit16To8(GBITMAP * pSrc, int x, int y, int width, int height, int sx, int sy);


		// Manipulation routines //

		void FlipVertical();


		// Surface routines //
		
		BOOL LockDDSurface();
		BOOL UnlockDDSurface();


		// Save and loading routines //

		BOOL SaveBMP(LPCTSTR lpstrFileName);
		BOOL LoadBMP(LPCTSTR lpstrFileName);
		BOOL SavePCX(LPCTSTR lpstrFileName);
		BOOL LoadPCX(LPCTSTR lpstrFileName);

		BOOL SaveBMP(HFILE hFile);
		BOOL LoadBMP(HFILE hFile);
		BOOL SavePCX(HFILE hFile);
		BOOL LoadPCX(HFILE hFile);

	protected:

		U8	m_BPP;						// Bits Per Pixel
		U32	m_Width,					// Width of bitmap
			m_Height,					// Height of bitmap
			m_Pitch;					// Pitch of bitmap

		U8  * m_pBits8;					// Actual bit data (BYTE)
		U16 * m_pBits16;				// WORD pointer to the actual bit data

		PALETTEENTRY m_Palette[256];	// Palette, if palettized 


		LPDIRECTDRAWSURFACE4 m_lpSurface;
	};



#endif
