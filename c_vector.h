/* ======================================================================= */
#ifndef __C_VECTOR__
#define __C_VECTOR__
/* ======================================================================= */
/* MD: 04.09.2003 */
/* LE: 04.09.2003 */
/* ======================================================================= */
//#include "c_vector_exp.h"
#include "proc_phy_exp.h"	/* tagCMPLX (15.09.2003) */
/* ======================================================================= */
/* abstract data buffer (02.09.2003) */
/* ======================================================================= */
typedef struct
	{
	int m_bValid;			/* validity */
	int m_nSize;			/* size in bytes */
	void* m_pData;			/* ptr to data */
	} tagDATABUF;
/* ======================================================================= */
/* engine */
/* ======================================================================= */
int VectorExpandBitsUC	(void* pDest, void* pSrc, int nSrcSize);
int VectorCollapseBitsUC(void* pDest, void* pSrc, int nSrcSize);
/* ======================================================================= */
/* complex functions */
/* ======================================================================= */
int VectorExpandBits_	(tagDATABUF* pDest, tagDATABUF* pSrc);
int VectorCollapseBits_	(tagDATABUF* pDest, tagDATABUF* pSrc);
int VectorCopy			(tagDATABUF* pDest, tagDATABUF* pSrc);
/* ======================================================================= */
/* dumping functions (15.09.2003) */
/* ======================================================================= */
char* VectorDumpCMPLX	(char* sBuf, tagCMPLX* pVector, int nLen);
char* VectorDumpDBL		(char* sBuf, void* pVector, int nLen);
char* VectorDumpUC		(char* sBuf, void* pVector, int nLen);
char* VectorDumpUC_BITS	(char* sBuf, void* pVector, int nLen);
/* ======================================================================= */
#endif /* __C_VECTOR__ */
/* ======================================================================= */
