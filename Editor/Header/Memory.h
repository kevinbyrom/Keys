#ifndef __Memory_H
#define __Memory_H


void memset16(void * pMemory, short Value16, short Size);
void memset32(void * pMemory, int Value32, int Size);

void memcpy16(void * pDest, const void * pSrc, int Size);
void memcpy32(void * pDest, const void * pSrc, int Size);


#endif