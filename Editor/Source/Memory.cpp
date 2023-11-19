#include <windows.h>
#include "Memory.h"



void memset16(void * pMemory, short Value16, short Size)
	{
	short * pMemory16;


	pMemory16 = (short *)pMemory;


	for (short i = 0; i < Size; i++)
		pMemory16[i] = Value16;
	}



void memset32(void * pMemory, int Value32, int Size)
	{
	int * pMemory32;


	pMemory32 = (int *)pMemory;


	for (int i = 0; i < Size; i++)
		pMemory32[i] = Value32;

	/*__asm
		{
		CLD					// clear direction flag 
		MOV EDI, pMemory	// move pointer into EDI
		MOV ECX, Size		// ECX hold loop count
		MOV EAX, Value32	// EAX hold value
		REP STOSD			// Perform fill
		}*/
	}



void memcpy16(void * pDest, const void * pSrc, int Size)
	{
	memcpy(pDest, pSrc, (Size << 1));
	}



void memcpy32(void * pDest, const void * pSrc, int Size)
	{
	memcpy(pDest, pSrc, (Size << 2));
	}



void memcpy16back(short * pDest, short * pSrc, int Size)
	{
	/*__asm
		{
		MOV		EAX, pSrc
		MOV		ECX, Size
		MOV		EDX, pDest

LOOP1:

		SUB		ECX, 1
		TEST	ECX, ECX
		JE		END1

		MOV		BX, [EAX + ECX * 2]
		MOV		EDX, BX
		ADD		EDX, 2
		JMP		LOOP1

END1: 
		}*/
	}