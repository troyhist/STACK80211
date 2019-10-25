/* ======================================================================= */
/* MD: 04.09.2003 */
/* LE: 15.09.2003 */
/* ======================================================================= */
#include "stdlib.h"		/* free, malloc */
#include "string.h"		/* strcat */
#include "stdio.h"		/* sprintf */
#include "c_vector.h"
/* ======================================================================= */
/* engine */
/* ======================================================================= */
int VectorExpandBitsUC	(void* pDest, void* pSrc, int nSrcSize)
	{
	/* ------------------------------------------------------------------- */
	/* simplified version (05.09.2003) */
	/* ------------------------------------------------------------------- */
	int i, j;
	int nDestIdx=0;
	unsigned char *pX=pSrc;
	unsigned char *pY=pDest;
	/* ------------------------------------------------------------------- */
	for (i=0; i<nSrcSize; i++)
		for (j=0; j<8; j++)
			pY[nDestIdx++]=1 && (pX[i] & (1<<j));
	/* ------------------------------------------------------------------- */
	return(0);
	}
/* ======================================================================= */
int VectorCollapseBitsUC(void* pDest, void* pSrc, int nSrcSize)
	{
	/* ------------------------------------------------------------------- */
	/* simplified version (05.09.2003, 16.09.2003) */
	/* ------------------------------------------------------------------- */
	int i, j;
	int nDestSize;
	unsigned char *pX=pSrc;
	unsigned char *pY=pDest;
	/* ------------------------------------------------------------------- */
	nDestSize=(nSrcSize+7)/8;
	for (i=0; i<nDestSize; i++) pY[i]=0;	/* (16.09.2003) */
	for (i=0; i<nDestSize; i++)
		for (j=0; j<8; j++)
			pY[i]|=(pX[i*8 + j] << j);
	/* ------------------------------------------------------------------- */
	return(0);
	}
/* ======================================================================= */
/* complex functions */
/* ======================================================================= */
int VectorExpandBits_	(tagDATABUF* pDest, tagDATABUF* pSrc)
	{
/* -----------------------------------------------------------------------
// Description: expands binary representation of given data array
//   Parametrs: ...
//      Return: 0 - ok, (-1) - error
// -----------------------------------------------------------------------
//   MD: 04.08.2003
//   LE: 05.09.2003
// TODO: test it
// ----------------------------------------------------------------------- */
	if (0==pSrc->m_bValid || pSrc->m_nSize<=0 || NULL==pSrc->m_pData)
		{ pDest->m_bValid=0; return(0); }
	/* ------------------------------------------------------------------- */
	if (pDest->m_bValid && pDest->m_nSize < (pSrc->m_nSize*8))
		{ free(pDest->m_pData); pDest->m_bValid=0; }
	/* ------------------------------------------------------------------- */
	if(0==pDest->m_bValid)
		{
		pDest->m_bValid=1;
		pDest->m_nSize=pSrc->m_nSize*8;
		pDest->m_pData=malloc(pDest->m_nSize);
		}
	/* ------------------------------------------------------------------- */
	VectorExpandBitsUC(pDest->m_pData, pSrc->m_pData, pSrc->m_nSize);
	/* ------------------------------------------------------------------- */
	return(0);
	}
/* ======================================================================= */
int VectorCollapseBits_	(tagDATABUF* pDest, tagDATABUF* pSrc)
	{
/* -----------------------------------------------------------------------
// Description: collapse binary sequence into a data array
//   Parametrs: ...
//      Return: 0 - ok, (-1) - error
// -----------------------------------------------------------------------
//   MD: 04.08.2003
//   LE: 05.09.2003
// TODO: test it
// ----------------------------------------------------------------------- */
	if (0==pSrc->m_bValid || pSrc->m_nSize<=0 || NULL==pSrc->m_pData)
		{ pDest->m_bValid=0; return(0); }
	/* ------------------------------------------------------------------- */
	if (pDest->m_bValid && (pDest->m_nSize*8 < pSrc->m_nSize))
		{ free(pDest->m_pData); pDest->m_bValid=0; }
	/* ------------------------------------------------------------------- */
	if(0==pDest->m_bValid)
		{
		pDest->m_bValid=1;
		pDest->m_nSize=(pSrc->m_nSize+7)/8;
		pDest->m_pData=malloc(pDest->m_nSize);
		}
	/* ------------------------------------------------------------------- */
	VectorCollapseBitsUC(pDest->m_pData, pSrc->m_pData, pSrc->m_nSize);
	/* ------------------------------------------------------------------- */
	return(0);
	}
/* ======================================================================= */
int VectorCopy (tagDATABUF* pDest, tagDATABUF* pSrc)
	{
/* -----------------------------------------------------------------------
// Description: copies given vector, allocates memory if no memory reserved
//   Parametrs: ...
//      Return: 0 - ok, (-1) - error
// -----------------------------------------------------------------------
//   MD: 04.09.2003
//   LE: 05.09.2003
// TODO: test it
// ----------------------------------------------------------------------- */
	int i;
	unsigned char *pX, *pY;
	/* ------------------------------------------------------------------- */
	if (0==pSrc->m_bValid || pSrc->m_nSize<=0 || NULL==pSrc->m_pData)
		{ pDest->m_bValid=0; return(0); }
	/* ------------------------------------------------------------------- */
	if (pDest->m_bValid && pDest->m_nSize<pSrc->m_nSize)
		{ free(pDest->m_pData); pDest->m_bValid=0; }
	/* ------------------------------------------------------------------- */
	if(0==pDest->m_bValid)
		{
		pDest->m_bValid=1;
		pDest->m_nSize=pSrc->m_nSize;
		pDest->m_pData=malloc(pDest->m_nSize);
		}
	/* ------------------------------------------------------------------- */
	pX=(unsigned char*)pSrc->m_pData;
	pY=(unsigned char*)pDest->m_pData;
	/* ------------------------------------------------------------------- */
	for (i=0; i<pDest->m_nSize; i++) pY[i]=pX[i];
	return(0);
	}
/* ======================================================================= */
/* dumping functions (15.09.2003) */
/* ======================================================================= */
char* VectorDumpCMPLX	(char* sBuf, tagCMPLX* pVector, int nLen)
	{
	/* ------------------------------------------------------------------- */
	/* 15.09.2003 */
	/* ------------------------------------------------------------------- */
	int i;
	char sNumBuf[64];
	/* ------------------------------------------------------------------- */
	*sBuf=0;
	/* ------------------------------------------------------------------- */
	for (i=0; i<nLen; i++)
		{
		sprintf(sNumBuf, "(%4.1lf, %4.1lf)%s", pVector[i].R, pVector[i].I, 
			(i+1<nLen)?"\t":"");
		strcat(sBuf, sNumBuf);
		}
	/* ------------------------------------------------------------------- */
	return(sBuf);
	}
/* ======================================================================= */
char* VectorDumpDBL		(char* sBuf, void* pVector, int nLen)
	{
	/* ------------------------------------------------------------------- */
	/* 15.09.2003 */
	/* ------------------------------------------------------------------- */
	int i;
	int dNumber;
	char sNumBuf[32];
	double* pVectorDBL;
	/* ------------------------------------------------------------------- */
	pVectorDBL=pVector;
	*sBuf=0;
	/* ------------------------------------------------------------------- */
	for (i=0; i<nLen; i++)
		{
		dNumber=(int)pVectorDBL[i];
		sprintf(sNumBuf, "%5.2g%s", dNumber, (i+1<nLen)?" ":"");
		strcat(sBuf, sNumBuf);
		}
	/* ------------------------------------------------------------------- */
	return(sBuf);
	}
/* ======================================================================= */
char* VectorDumpUC		(char* sBuf, void* pVector, int nLen)
	{
	/* ------------------------------------------------------------------- */
	/* 15.09.2003 */
	/* ------------------------------------------------------------------- */
	int i;
	int nNumber;
	char sNumBuf[8];
	unsigned char* pVectorUC;
	/* ------------------------------------------------------------------- */
	pVectorUC=pVector;
	*sBuf=0;
	/* ------------------------------------------------------------------- */
	for (i=0; i<nLen; i++)
		{
		nNumber=(int)pVectorUC[i];
		sprintf(sNumBuf, "%02x%s", nNumber, (i+1<nLen)?" ":"");
		strcat(sBuf, sNumBuf);
		}
	/* ------------------------------------------------------------------- */
	return(sBuf);
	}
/* ======================================================================= */
char* VectorDumpUC_BITS		(char* sBuf, void* pVector, int nLen)
	{
	/* ------------------------------------------------------------------- */
	/* 16.09.2003 */
	/* ------------------------------------------------------------------- */
	int i;
	int nNumber, nOctetcCounter;
	char sNumBuf[16];
	unsigned char* pVectorUC;
	/* ------------------------------------------------------------------- */
	pVectorUC=pVector;
	*sBuf=0;
	nOctetcCounter=0;
	/* ------------------------------------------------------------------- */
	for (i=0; i<nLen; i++)
		{
		nOctetcCounter++;
		nNumber=(int)pVectorUC[i];
		sprintf(sNumBuf, "%d%s", nNumber, (0==nOctetcCounter%8)?" ":"");
		strcat(sBuf, sNumBuf);
		}
	/* ------------------------------------------------------------------- */
	return(sBuf);
	}
/* ======================================================================= */
