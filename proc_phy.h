/* =======================================================================
// PROC_PHY function library ver. 1.0
// i/f to PHY layer procedures
// -----------------------------------------------------------------------
// MD: 05.09.2003
// LE: 05.09.2003
// -----------------------------------------------------------------------
// Compiler: MS VC 6.0
// =======================================================================
// proc_phy.h: implementation of service functions
// ======================================================================= */
#ifndef __PROC_PHY__
#define __PROC_PHY__
/* ======================================================================= */
#include "proc_phyctrl_exp.h"
#include "proc_phy_exp.h"
#include "..\\MDM_ACCESS\\mdm_cli.h"		/* (10.09.2003) */
/* ======================================================================= */
/* PHY layer (i/f to PHY part) (04.09.2003, 09.09.2003) */
/* ======================================================================= */
typedef struct
	{
	int			m_bValid;		/* "valid" flag */
	int			m_bTxActive;	/* flag "Tx mode is active" */
	int			m_bRxMdmActive;	/* flag "Rx/MDM is active" (09.09.2003) */
	int			m_nSymb;		/* number of OFDM symbols to Tx */
	int			m_nRate;		/* Data Rate */
	tagSYMBOL	m_tSymbSmpls;	/* Received OFDM symbol samples */
	tagMDM_SAMPLES	m_tMdmSmpls;	/* Received medium samples samples (09.09.2003) */
	} tagLPHY_ST;
/* ======================================================================= */
/* "class" functions (05.09.2003) */
/* ======================================================================= */
void PHY			(tagLPHY_ST*pLPHY);		/* constructor */
void PHY_			(tagLPHY_ST*pLPHY);		/* destructor (24.09.2003) */
int  PHYInit		(tagLPHY_ST*pLPHY);		/* init */
int  PHYIsInited	(tagLPHY_ST*pLPHY);		/* Is init (24.09.2003) */
void PHYSanity		(tagLPHY_ST*pLPHY, char* sFuncName); /* sanity */
/* ======================================================================= */
/* PHY LAYER functions (05.09.2003) */
/* ======================================================================= */
/* TX PART */
/* ----------------------------------------------------------------------- */
int PHYTxFillBuf	(tagLPHY_ST*pLPHY, tagSYMBOL* pSmpls);
int PHYTxRequired	(tagLPHY_ST*pLPHY, int nSymb, int nDataRate);
int PHYTxSendSymb	(tagLPHY_ST*pLPHY, tagCMDM_CLI* pMDM_CLI);
int PHYTxStop		(tagLPHY_ST*pLPHY);
/* ----------------------------------------------------------------------- */
/* RX PART */
/* ----------------------------------------------------------------------- */
int PHYRxRecvOfdmSymb(tagLPHY_ST*pLPHY, tagSYMBOL* pSmpls);
int PHYRxRecvSmpls	(tagLPHY_ST*pLPHY, tagMDM_SAMPLES* pSmpls);
/* ----------------------------------------------------------------------- */
/* PHY_EVENT_GEN */
/* ----------------------------------------------------------------------- */
int PHYAddSmplsToSymb	(tagLPHY_ST*pLPHY, tagMDM_SAMPLES* pSmpls, int nNum);
int PHYIsPhyMdmAct		(tagLPHY_ST*pLPHY);
int PHYIsTxReq			(tagLPHY_ST*pLPHY, double *pSymb, int* pDataRate);
/* ======================================================================= */
#endif /* __PROC_PHY__ */
/* ======================================================================= */
