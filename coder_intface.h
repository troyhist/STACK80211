/* =======================================================================
// coder interface ver. 1.0 (pure C)
// provides comfortable interface to coder DLL library
// -----------------------------------------------------------------------
// MD: 22.09.2003
// LE: 22.09.2003
// -----------------------------------------------------------------------
//   Compiler: MS VC 6.0
// Aplication: WIN32
//  Edited by: Serguei Kolevatov
// =======================================================================
// coder_intface.h: implementation of service functions (header)
// ======================================================================= */
#ifndef __CODER_INTFACE__
#define __CODER_INTFACE__
/* ======================================================================= */
#include <stdlib.h>
#include <string.h>
/* ----------------------------------------------------------------------- */
#include "proc_phyctrl_exp.h"	/* BITSEQ, */
#include "proc_phy_exp.h"		/* SYMBOL, etc. */
/* ----------------------------------------------------------------------- */
#include "..\\prot_ifc\\include\\tx_11a_data.h"
#include "..\\prot_ifc\\include\\rx_11a_data.h"
/* ======================================================================= */
/* (22.09.2003) */
/* ======================================================================= */
typedef struct
	{
	/* ------------------------------------------------------------------- */
	int m_bInited;				// init flag
	/* ------------------------------------------------------------------- */
	Prmtx_11a_data  *prmTx;
	Prmrx_11a_data  *prmRx;
	/* ------------------------------------------------------------------- */
	tx_11a_data	  *txMod;
	rx_11a_data	  *rxMod;
	/* ------------------------------------------------------------------- */
	} tagCODERIF;
/* ======================================================================= */
/* "class" functions */
/* ======================================================================= */
void CIF			(tagCODERIF* pCIF);
void CIF_			(tagCODERIF* pCIF);
int  CIFInit		(tagCODERIF* pCIF);
int  CIFIsInited	(tagCODERIF* pCIF);	/* (24.09.2003) */
/* ======================================================================= */
/* public functions */
/* ======================================================================= */
int CIFConvDataRateToRateSelector	(tagCODERIF* pCIF, int nDataRate);
int CIFDecodeOFDMSymb	(tagCODERIF* pCIF, tagBITSEQ* pBits, tagSYMBOL* pSymb, int nRate);
int CIFEncodeOFDMSymb	(tagCODERIF* pCIF, tagSYMBOL* pSymb, tagBITSEQ* pBits, int nRate);
int CIFGetVitPathLen	(tagCODERIF* pCIF);
/* ======================================================================= */
#endif /* __CODER_INTFACE__ */
/* ======================================================================= */
