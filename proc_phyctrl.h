/* =======================================================================
// procedures of PHY CTRL ver. 1.0
// structs and constants definition
// -----------------------------------------------------------------------
// MD: 02.09.2003
// LE: 02.09.2003
// -----------------------------------------------------------------------
// Compiler: MS VC 6.0
// =======================================================================
// procphyctrl_exp.h: implementation of service functions (header)
// ======================================================================= */
#ifndef __PROCPHYCTRL__
#define __PROCPHYCTRL__
/* ======================================================================= */
#include "proc_phyctrl_exp.h"	/* BITSEQ, etc. */
#include "proc_phy_exp.h"		/* SYMBOL, tagFLDPSDU */
#include "emul_cfg.h"			/* tagEMULCFG (10.09.2003) */
#include "coder_intface.h"		/* tagCODERIF (22.09.2003) */
/* ======================================================================= */
#define MAX_FLDDATALEN	(32832)	/* ref:  17.3.5.4 pad bits */
/* ======================================================================= */
/* field DATA (bit sequence) (05.09.2003) */
/* ======================================================================= */
typedef struct
	{
	int m_nSize;							// # of elements
	unsigned char m_pData[MAX_FLDDATALEN];	// ptr to data
	} tagFLDDATA;
/* ======================================================================= */
/* PHY_CTRL layer (02.09.2003) */
/* ======================================================================= */
typedef struct
	{
	int			m_bValid;		/* "valid" flag */
	tagSIGNAL	m_tHDR;			/* HDR */
	tagFLDPSDU	m_tPSDU;		/* PSDU */
	tagFLDDATA	m_tDATA;		/* DATA as a binary sequence */
	tagCODERIF	m_tCIF;			/* interface to coder DLL (22.09.2003) */
	} tagLPC_ST;
/* ======================================================================= */
/* "class" functions (04.09.2003) */
/* ======================================================================= */
void PCTRL		(tagLPC_ST* pLPC);		/* constructor */
void PCTRL_		(tagLPC_ST* pLPC);		/* destructor (24.09.2003) */
int  PCInit		(tagLPC_ST* pLPC);		/* init */
int  PCIsInited	(tagLPC_ST* pLPC);		/* Is inited (24.09.2003) */
void PCSanity	(tagLPC_ST* pLPC, char* sFuncName);		/* init */
/* ======================================================================= */
/* PHY_CTRL LAYER functions (02.09.2003) */
/* ======================================================================= */
/* TX SERVICE PART */
/* ----------------------------------------------------------------------- */
int PCFillTxBuf			(tagLPC_ST* pLPC, tagEMULCFG* pCFG, void* pDataTx, int nSize);
/* ----------------------------------------------------------------------- */
/* TX PART */
/* ----------------------------------------------------------------------- */
int PCConvHdrToBitSeq	(tagLPC_ST* pLPC, tagBITSEQ* pBits, tagSIGNAL* pHdr);
int PCConvPSDUToData	(tagLPC_ST* pLPC, int nSymb, int nNdbps);
int PCCreateHdr			(tagLPC_ST* pLPC, tagSIGNAL* pHdr, int nDataRate, int nLen);
int PCDecodeFieldRate	(tagLPC_ST* pLPC, int nEncodedRate);
int PCEncodeFieldRate	(tagLPC_ST* pLPC, int nDataRate);
int PCEncodeOfdmSymb	(tagLPC_ST* pLPC, tagSYMBOL* pSmpls, tagBITSEQ* pBits, int nRate);
int PCGeneratePreSymb	(tagLPC_ST* pLPC, tagSYMBOL* pSmpls, int nNum);
int PCGetNdbpsByDataRate(tagLPC_ST* pLPC, int nDataRate);
int PCGetPSDUDataLen	(tagLPC_ST* pLPC);
int PCGetSymbolBits		(tagLPC_ST* pLPC, tagBITSEQ* pBits, int nNum, int nNdbps);
/* ----------------------------------------------------------------------- */
/* RX PART */
/* ----------------------------------------------------------------------- */
int PCAddBitsToRxBuf	(tagLPC_ST* pLPC, tagBITSEQ* pBits, int nNum, int nNdbps);
int PCDetectHdr			(tagLPC_ST* pLPC, tagSIGNAL* pHdr, tagBITSEQ* pBits);
int PCDecodeOfdmSymb	(tagLPC_ST* pLPC, tagBITSEQ* pBits, tagSYMBOL* pSmpls, int nRate);
int PCExtractPSDU		(tagLPC_ST* pLPC, int nSize);
int PCIsHdrParityOk		(tagLPC_ST* pLPC, tagSIGNAL* pHdr);
int PCTreatShortSeq		(tagLPC_ST* pLPC, tagSYMBOL* pP1, tagSYMBOL* pP2);
int PCTreatLongSeq		(tagLPC_ST* pLPC, tagSYMBOL* pP3, tagSYMBOL* pP4);
/* ======================================================================= */
#endif /* __PROCPHYCTRL__ */
/* ======================================================================= */
