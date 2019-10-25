/* =======================================================================
// PROC_PHY function library ver. 1.0
// i/f of PHY layer procedures
// -----------------------------------------------------------------------
// MD: 05.09.2003
// LE: 08.09.2003
// -----------------------------------------------------------------------
// Compiler: MS VC 6.0
// =======================================================================
// proc_phy.c: implementation of service functions
// ======================================================================= */
#include <stdlib.h>			/* free */
#include "proc_phy.h"
#include "c_vector.h"		/* expand & collapse data (04.09.2003) */
/* ======================================================================= */
void InstanceErrExit(char* sFuncName, char* sErrFormat, ...);	/* error handler */
/* ======================================================================= */
/* "class" functions */
/* ======================================================================= */
void PHY			(tagLPHY_ST*pLPHY)
	{ pLPHY->m_bValid=0; }
/* ----------------------------------------------------------------------- */
void PHY_			(tagLPHY_ST*pLPHY)
	{ if(!PHYIsInited(pLPHY)) return;
	pLPHY->m_bValid=0; }
/* ----------------------------------------------------------------------- */
int  PHYInit		(tagLPHY_ST*pLPHY)
	{
	pLPHY->m_bValid=1;			/* "valid" flag */
	pLPHY->m_bTxActive=0;		/* flag "Tx mode is active" */
	pLPHY->m_bRxMdmActive=0;	/* flag "Rx/MDM is active" (09.09.2003) */
	pLPHY->m_nSymb=0;		/* number of OFDM symbols to Tx */
	pLPHY->m_nRate=0;		/* Data Rate */
	return(0);
	}
/* ----------------------------------------------------------------------- */
int  PHYIsInited	(tagLPHY_ST*pLPHY)
	{ return(pLPHY->m_bValid); }
/* ======================================================================= */
void PHYSanity		(tagLPHY_ST*pLPHY, char* sFuncName)
{
if(NULL==pLPHY)
	InstanceErrExit(sFuncName, "sanity failed, %s", "pLPHY==NULL");
if(0==pLPHY->m_bValid)
	InstanceErrExit(sFuncName, "sanity failed, %s", "pLPHY is NOT valid");
if(pLPHY->m_bTxActive && 0==pLPHY->m_nSymb)
	InstanceErrExit(sFuncName, "sanity failed, %s", "Tx is active, but nSymb = 0");
}
/* ======================================================================= */
/* TX PART */
/* ======================================================================= */
int PHYTxFillBuf(tagLPHY_ST* pLPHY, tagSYMBOL* pSmpls)
	{
	/* ------------------------------------------------------------------- */
	/* fills Phy Buf w. given bit sequence (05.09.2003, 10.09.2003) */
	/* ------------------------------------------------------------------- */
	PHYSanity(pLPHY, "PHYTxFillBuf()");
	pLPHY->m_tSymbSmpls=*pSmpls;
	return(0);
	}
/* ======================================================================= */
int PHYTxRequired	(tagLPHY_ST*pLPHY, int nSymb, int nDataRate)
	{
	PHYSanity(pLPHY, "PHYTxRequired()");
	pLPHY->m_bTxActive=1;
	pLPHY->m_nSymb=nSymb;
	pLPHY->m_nRate=nDataRate;
	return(0);
	}
/* ======================================================================= */
int PHYTxSendSymb (tagLPHY_ST*pLPHY, tagCMDM_CLI* pMDM_CLI)
	{
	/* ------------------------------------------------------------------- */
	/* sends one OFDM symbol, it takes 4 MDM_ACCESS iterations */
	/* (10.09.2003, 11.09.2003) */
	/* ------------------------------------------------------------------- */
	int i, j;
	int nRes;
	tagMDM_SAMPLES tSmpls;
	/* ------------------------------------------------------------------- */
	PHYSanity(pLPHY, "PHYTxSendSymb()");
	/* ------------------------------------------------------------------- */
	if(0==pLPHY->m_bTxActive)
		InstanceErrExit("PHYTxSendSymb()",
		"trying to send samples, but PHY Tx BUF is empty");
	/* ------------------------------------------------------------------- */
	for (i=0; i<4; i++)
		{
		/* --------------------------------------------------------------- */
		/* takes 20 samples from OFDM symbol */
		/* --------------------------------------------------------------- */
		for (j=0; j<20; j++)
			{
			tSmpls.m_pData[j].R=pLPHY->m_tSymbSmpls.m_pData[j+i*20].R;
			tSmpls.m_pData[j].I=pLPHY->m_tSymbSmpls.m_pData[j+i*20].I;
			}
		/* --------------------------------------------------------------- */
		nRes=CMDM_CLIIterate (pMDM_CLI, MDM_CLI_ITER_UPD_STATE, NULL, &tSmpls);
		nRes=CMDM_CLIIterate (pMDM_CLI, MDM_CLI_ITER_GET_STATE, NULL, NULL);
		/* --------------------------------------------------------------- */
		}
	/* ------------------------------------------------------------------- */
	return(0);
	}
/* ======================================================================= */
int PHYTxStop (tagLPHY_ST*pLPHY)
	{ PHYSanity(pLPHY, "PHYTxStop()"); pLPHY->m_bTxActive=0; return(0); }
/* ======================================================================= */
/* RX PART */
/* ======================================================================= */
int PHYRxRecvOfdmSymb	(tagLPHY_ST* pLPHY, tagSYMBOL* pSmpls)
	{
	/* (08.09.2003) */
	PHYSanity(pLPHY, "PHYRecvOfdmSymb()");
	*pSmpls=pLPHY->m_tSymbSmpls;
	return(0);
	}
/* ======================================================================= */
int PHYRxRecvSmpls	(tagLPHY_ST*pLPHY, tagMDM_SAMPLES* pSmpls)
	{
	/* ------------------------------------------------------------------- */
	/* receives samples from MDM (10.09.2003) */
	/* !!! STA_EMUL::MDM_CLI(..) has to be called first !!! */
	/* ------------------------------------------------------------------- */
	PHYSanity(pLPHY, "PHYRxRecvSmpls()");
	/* ------------------------------------------------------------------- */
	if(0==pLPHY->m_bRxMdmActive)
		InstanceErrExit("PHYRxRecvSmpls()",
		"trying to receive samples, but MDM is idle");
	/* ------------------------------------------------------------------- */
	*pSmpls=pLPHY->m_tMdmSmpls;
	/* ------------------------------------------------------------------- */
	return(0);
	}
/* ======================================================================= */
/* EVENT_GEN */
/* ======================================================================= */
int PHYAddSmplsToSymb (tagLPHY_ST*pLPHY, tagMDM_SAMPLES* pSmpls, int nNum)
	{ /* (05.09.2003) */
	int i, nOffset;
	PHYSanity(pLPHY, "PHYAddSmplsToSymb()");
	nOffset=(nNum%4)*20;
	for (i=0; i<20; i++)
		pLPHY->m_tSymbSmpls.m_pData[i+nOffset]=pSmpls->m_pData[i];
	return(0);
	}
/* ======================================================================= */
int PHYIsPhyMdmAct	(tagLPHY_ST*pLPHY)
	{ /* (09.09.2003) */
	PHYSanity(pLPHY, "PHYIsPhyMdmAct()");
	return(pLPHY->m_bRxMdmActive);
	}
/* ======================================================================= */
int PHYIsTxReq (tagLPHY_ST*pLPHY, double *pSymb, int* pDataRate)
	{ /* (05.09.2003) */
	double dSymb;
	PHYSanity(pLPHY, "PHYIsTxReq()");
	dSymb=pLPHY->m_nSymb;
	*pSymb=dSymb;	*pDataRate=pLPHY->m_nRate;
	return(pLPHY->m_bTxActive);
	}
/* ======================================================================= */
