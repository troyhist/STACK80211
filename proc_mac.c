/* =======================================================================
// MAC procedures ver. 1.0
// structs and constants definition
// -----------------------------------------------------------------------
// MD: 30.07.2003
// LE: 31.07.2003
// -----------------------------------------------------------------------
// Compiler: MS VC 6.0
// =======================================================================
// procmac.c: implementation of service functions
// ======================================================================= */
#include <stdlib.h>			/* rand */
#include <string.h>			/* strcpy */
#include <stdio.h>
#include <time.h>			/* srand */
/* ----------------------------------------------------------------------- */
#ifdef __STDC__
#include <stdarg.h>			/* variable argument list */
#elif _WIN32
#include <stdarg.h>			/* variable argument list */
#else
#include <varargs.h>		/* Required for UNIX V compatibility */
#endif
/* ----------------------------------------------------------------------- */
#include "procpkt.h"		/* sizeof(tagACK) */
#include "proc_mac.h"
#include "lib_path.h"		/* (09.10.2003) */
#include STRGETVAL_H		/* StrGetVal (10.09.2003) */
/* ======================================================================= */
void InstanceErrExit(char* sFuncName, const char* sErrFormat, ...);
int  InstanceOutputFunc(char* sFilename, char* sMsg);
/* ======================================================================= */
char glPMTB[256];			/* ProcMacTempBuf - PMTB */
char glPMTB8K[8192];		/* ProcMacTempBuf - PMTB 8KB, 26.09.2003 */
char glPMTB2[256];			/* ProcMacTempBuf - PMTB, 11.08.2003 */
/* ======================================================================= */
/* "class" functions */
/* ======================================================================= */
void MAC(tagLMAC* pMAC, tagMAC_ST* pMS)
	{ /* constructor (05.09.2003, 24.09.2003) */ 
	pMAC->m_bClassCreated=1;	/* (24.09.2003) */
	CLOGR(&pMAC->m_tLOGR);
	CLOGR(&pMAC->m_tLOGR_IO);
	CLOGR(&pMAC->m_tLOGR_ST);
	}
/* ======================================================================= */
void MAC_		(tagLMAC* pMAC, tagMAC_ST* pMS)
	{ /* destructor (24.09.2003) */
	if(!MACIsInited(pMAC)) return;
	CLOGR_(&pMAC->m_tLOGR);
	CLOGR_(&pMAC->m_tLOGR_IO);
	CLOGR_(&pMAC->m_tLOGR_ST);
	pMAC->m_bClassCreated=0;
	}
/* ======================================================================= */
int  MACIsInited(tagLMAC* pMAC)
	{ return(pMAC->m_bClassCreated); }
/* ----------------------------------------------------------------------- */
void MACSanity(tagLMAC* pMAC, char* sFuncName)
	{ if(NULL==pMAC)
		InstanceErrExit(sFuncName, "sanity check failed"); }
/* ======================================================================= */
/* private functions */
/* ======================================================================= */
int STAGetPhy(tagLMAC* pMAC, tagPLMECH_PHY* pPHY)
{
/* -----------------------------------------------------------------------
// Description: gets characteristics of PHY layer
//   Parametrs: ...
//      Return: 0 - ok, (-1) - error
// -----------------------------------------------------------------------
//   MD: 24.07.2003
//   LE: 11.09.2003
// TODO: test it, used recommended data
// ----------------------------------------------------------------------- */
CLOGRAdd(&pMAC->m_tLOGR, "STAGetPhy(..)", "get PHY characteristics");
/* ----------------------------------------------------------------------- */
/* 15.3.3. default DS PHY charactereistics */
/* ----------------------------------------------------------------------- */
pPHY->m_aSlotTime=20;			/* 20us (0.000020 sec) */
pPHY->m_aSIFSTime=10;			/* 10us (0.000010 sec) */
pPHY->m_aCCATime=15;			/* 15us (0.000015 sec) */
pPHY->m_aRxTxTurnaroundTime=5;	/*  5us (0.000005 sec) */
pPHY->m_aTxPLCPDelay=0;			/* NOT DEFINED */
pPHY->m_aRxPLCPDelay=0;			/* NOT DEFINED */
pPHY->m_aRxTxSwitchTime=5;		/*  5us (0.000005 sec) */
pPHY->m_aTxRampOnTime=0;		/* NOT DEFINED */
pPHY->m_aTxRampOffTime=0;		/* NOT DEFINED */
pPHY->m_aTxRFDelay=0;			/* NOT DEFINED */
pPHY->m_aRxRFDelay=0;			/* NOT DEFINED */
pPHY->m_aAirPropagationTime=1;	/*  1us (0.000001 sec) */
pPHY->m_aMACProcessingDelay=0;	/* NOT APPLICABLE */
pPHY->m_aPreambleLength=144;	/* 144 bits (us) */
pPHY->m_aPLCPHeaderLength=48;	/* 48 bits	(us) */
//pPHY->m_aPLCPDataRate=1.0e6;	/* 1Mbps !!! NOT in 10.4.3. (28.09.2003) !!! */
pPHY->m_aMPDUDurationFactor=0;	/* factor */
pPHY->m_aMPDUMaxLength=(1<<12-1);	/* 4 <= x <= 2^13-1, but I used 2^12-1 */
pPHY->m_aCWmin=31;				/* # of slots (aSlotTime) */
pPHY->m_aCWmax=1023;			/* # of slots (aSlotTime) */
/* ----------------------------------------------------------------------- */
return(0);
}
/* ======================================================================= */
int STAGetMib(tagLMAC* pMAC, tagMAC_MIB* pMIB)
{
/* -----------------------------------------------------------------------
// Description: gets characteristics of MIB
//   Parametrs: ...
//      Return: 0 - ok, (-1) - error
// -----------------------------------------------------------------------
//   MD: 25.07.2003
//   LE: 25.07.2003
// TODO: test it, used recommended data
// ----------------------------------------------------------------------- */
CLOGRAdd (&pMAC->m_tLOGR, "STAGetMib(..)", "get MIB (Management Information Base)");
/* ----------------------------------------------------------------------- */
/* 11.4.4.2 actually in Annex C, Process MIB, Mib_import_export_2b(2) */
/* ----------------------------------------------------------------------- */
/* opeartion table */
pMIB->MACAddress.ab[0]=1;		/* MAC address octets */
pMIB->MACAddress.ab[1]=0;		/* MAC address octets */
pMIB->MACAddress.ab[2]=0;		/* MAC address octets */
pMIB->MACAddress.ab[3]=0;		/* MAC address octets */
pMIB->MACAddress.ab[4]=0;		/* MAC address octets */
pMIB->MACAddress.ab[5]=0;		/* MAC address octets */
//pMIB->pGroupAddresses=0;
pMIB->RTSThreshold=3000;			/* bytes */
pMIB->ShortRetryLimit=7;			/* retransmissions */
pMIB->LongRetryLimit=4;				/* retransmissions */
pMIB->FragmentationThreshold=2346;	/* bytes */
pMIB->MaxTransmitMSDULifetime=512;	/* time units */
pMIB->MaxReceiveLifetime=512;		/* time units */
//	pMIB->ManufacturerID;
//	pMIB->ProductID;
/* ----------------------------------------------------------------------- */
return(0);
}
/* ======================================================================= */
int STAGetMibCntrs(tagLMAC* pMAC, tagMAC_MIBCNTRS* pMIBC)
{
/* -----------------------------------------------------------------------
// Description: resets all MIB counters
//   Parametrs: ...
//      Return: 0 - ok, (-1) - error
// -----------------------------------------------------------------------
//   MD: 30.07.2003
//   LE: 30.07.2003
// TODO: test it
// ----------------------------------------------------------------------- */
CLOGRAdd (&pMAC->m_tLOGR, "STAGetMibCntrs(..)", "reset counters of MIB");
/* ----------------------------------------------------------------------- */
/* 11.4.4.2 actually in Annex C, Process MIB, Mib_import_export_2b(2) */
/* ----------------------------------------------------------------------- */
/* counters table */
pMIBC->TransmittedFragmentCount=0;		/* MIF; t1 */
pMIBC->MulticastTransmittedFrameCount=0;/* !!!!!!!!!!!!!!!!!!!  not used !!! */
pMIBC->FailedCount=0;					/* WR; W_ACK */
pMIBC->RetryCount=0;					/* WR; W_ACK */
pMIBC->MultipleRetryCount=0;			/* WR; W_ACK */
pMIBC->RTSSuccessCount=0;				/* WR; W_CTS */
pMIBC->RTSFailureCount=0;				/* WR; W_CTS */
pMIBC->ACKFailureCount=0;				/* WR; W_ACK */
pMIBC->ReceivedFragmentCount=0;			/* PKT_ANLZ; IDLE-t4 */
pMIBC->MulticastReceivedFrameCount=0;	/* !!!!!!!!!!!!!!!!!!!  not used !!! */
pMIBC->FCSErrorCount=0;					/* PKT_ANLZ; IDLE */
pMIBC->FrameDuplicateCount=0;
/* ----------------------------------------------------------------------- */
return(0);
}
/* ======================================================================= */
int STAGetMibCnsts(tagLMAC* pMAC, tagMAC_MIBCNSTS* pMIBCNSTS)
{
/* -----------------------------------------------------------------------
// Description: gets predifined constants of MIB
//   Parametrs: ...
//      Return: 0 - ok, (-1) - error
// -----------------------------------------------------------------------
//   MD: 30.07.2003
//   LE: 30.07.2003
// TODO: test it, used recommended data
// ----------------------------------------------------------------------- */
CLOGRAdd (&pMAC->m_tLOGR, "STAGetMibCnsts(..)", "get constants of MIB");
/* ----------------------------------------------------------------------- */
/* 11.4.4.2 actually in Annex C, Process MIB, Mib_import_export_2b(2) */
/* ----------------------------------------------------------------------- */
pMIBCNSTS->m_bExcludeUnencrypted=0;		/* false (BOOL) */
/*pMIBCNSTS->m_pSTAAddrSet=NULL; */
pMIBCNSTS->m_nMediumOccupancyLimit=100;	/* time units */
pMIBCNSTS->m_bPrivacyInvoked=0;			/* false (BOOL) */
pMIBCNSTS->m_bmReceiveDTIMs=1;			/* true (BOOL) */
pMIBCNSTS->m_nCfpPeriod=1;				/* int */
pMIBCNSTS->m_nCfpMaxDuration=200;		/* time units */
pMIBCNSTS->m_nAuthenticationResponseTimeout=512;	/* time units */
pMIBCNSTS->m_nWepDefaultKeyId=0;		/* key index */
pMIBCNSTS->m_nCurrentChannelNumber=0;	/* int */
pMIBCNSTS->m_nCurrentSet=0;				/* int */
pMIBCNSTS->m_nCurrentPattern=0;			/* int */
pMIBCNSTS->m_nCurrentIndex=0;			/* int */
/* ----------------------------------------------------------------------- */
return(0);
}
/* ======================================================================= */
int MACInit	(tagLMAC* pMAC, tagMAC_ST* pST, double dDataRate, char sLogger[3][256], 
			 char* sCFGFile, int (*pLogOutputFunc) (char*, char*))
	{
	/* ----------------------------------------------------------------------- */
	/* inits MAC structure (10.09.2003, 22.09.2003, 20.10.2003) */
	/* ----------------------------------------------------------------------- */
	/* inits LOGGERS (12.09.2003) */
	/* ----------------------------------------------------------------------- */
	CLOGRInit(&pMAC->m_tLOGR,	 pLogOutputFunc, sLogger[0], "MAC logger");
	CLOGRInit(&pMAC->m_tLOGR_IO, pLogOutputFunc, sLogger[1], "MAC I/O logger");
	CLOGRInit(&pMAC->m_tLOGR_ST, pLogOutputFunc, sLogger[2], "MAC STATE logger");
	/* ----------------------------------------------------------------------- */
	/* loads MIB cfg */
	/* ----------------------------------------------------------------------- */
	MACLoadCFG(pMAC, &pMAC->m_tMIB, sCFGFile);
	/* constants */
	/* ----------------------------------------------------------------------- */
	/* characteristic of PHY */
	STAGetPhy(pMAC, &pMAC->m_tPHY);
	STAGetMibCnsts(pMAC, &pMAC->m_tMCNST);		/* (31.07.2003) */
	pMAC->m_tPHY.m_aPLCPDataRate=dDataRate;		/* (12.09.2003) */
	/* ----------------------------------------------------------------------- */
	/* 9.2.10. DCF timing relations */
	/* ----------------------------------------------------------------------- */
	pMAC->m_tTC.m_nSIFS=pMAC->m_tPHY.m_aSIFSTime;
	pMAC->m_tTC.m_nPIFS=pMAC->m_tTC.m_nSIFS+pMAC->m_tPHY.m_aSlotTime;
	pMAC->m_tTC.m_nDIFS=pMAC->m_tTC.m_nSIFS+2*pMAC->m_tPHY.m_aSlotTime;
	pMAC->m_tTC.m_nEIFS=pMAC->m_tPHY.m_aSIFSTime+
				MACFrameTxTimeL(pMAC,sizeof(tagPKTACK)*8)+
				pMAC->m_tPHY.m_aPreambleLength+
				pMAC->m_tPHY.m_aPLCPHeaderLength+
				pMAC->m_tTC.m_nDIFS;
	pMAC->m_tTC.m_nTxTimeACK=MACGetPpduHdrTxTime(pMAC) +
		PktGetDataTxTime(sizeof(tagPKTACK)*8, pMAC->m_tPHY.m_aPLCPDataRate);
	pMAC->m_tTC.m_nTxTimeCTS=MACGetPpduHdrTxTime(pMAC) +
		PktGetDataTxTime(sizeof(tagPKTCTS)*8, pMAC->m_tPHY.m_aPLCPDataRate);
	/* ----------------------------------------------------------------------- */
	/* STA variables */
	/* ----------------------------------------------------------------------- */
	/* calculates timeouts (05.08.2003, 17.09.2003) */
	/* added length of the Preamble+Header (16 us + 4 us) and one SIFT interval */
	pMAC->m_tTC.m_nACKTimeout=MACGetPpduHdrTxTime(pMAC) + 
		pMAC->m_tTC.m_nSIFS + pMAC->m_tPHY.m_aAirPropagationTime*2;
	pMAC->m_tTC.m_nCTSTimeout=MACGetPpduHdrTxTime(pMAC) + 
		pMAC->m_tTC.m_nSIFS + pMAC->m_tPHY.m_aAirPropagationTime*2;
	/* ----------------------------------------------------------------------- */
	/* inits state struct */
	/* ----------------------------------------------------------------------- */
	MACInitST(pMAC, pST);
	/* ----------------------------------------------------------------------- */
	return(0);
	}
/* ======================================================================= */
int  MACInitST(tagLMAC* pMAC, tagMAC_ST* pST)
	{
	/* ----------------------------------------------------------------------- */
	/* inits MAC state structure (03.09.2003, 10.09.2003) */
	/* ----------------------------------------------------------------------- */
	pST->bVCFree=1;
	pST->nCW=pMAC->m_tPHY.m_aCWmin;
	pST->nCurSeqNo=0;
	/* ----------------------------------------------------------------------- */
	/* process specific variables */
	pST->nFrags=0;
	pST->nCurFrag=0;
	/* ----------------------------------------------------------------------- */
	STAGetMibCntrs(pMAC, &pST->MIBC);	/* (30.07.2003) */
	/* ----------------------------------------------------------------------- */
	/* buffers */
	pST->pktCTRL.bValid=0;
	pST->pktRTS.bValid=0;
	pST->pktTx.bValid=0;
	pST->pktRx.bValid=0;
	pST->pktTxFr.bValid=0;
	/* ----------------------------------------------------------------------- */
	pST->m_nCacheSize=0;	/* # of used records in the cache (12.08.2003) */
	pST->m_nCacheAccessNo=0;
	/* ----------------------------------------------------------------------- */
	return(0);
	}
/* ======================================================================= */
int	 MACLoadCFG	(tagLMAC* pMAC, tagMAC_MIB* pMIB, char* sCFGFile)
	{
	/* load MAC CFG from given file (10.09.2003) */
/* -----------------------------------------------------------------------
// Description: load MAC CFG from given file (10.09.2003)
//   Parametrs: ...
//      Return: 0 - ok, (-1) - error
// -----------------------------------------------------------------------
//   MD: 10.09.2003
//   LE: 10.09.2003
// TODO: test it
// ----------------------------------------------------------------------- */
char sBuf[4096];
char sValue[256];
FILE* fp;
/* ----------------------------------------------------------------------- */
CLOGRAdd (&pMAC->m_tLOGR, "MACLoadCFG(..)", "load CFG from file %s", sCFGFile);
/* ----------------------------------------------------------------------- */
fp=fopen(sCFGFile, "rt");
if (NULL==fp)
	InstanceErrExit("MACLoadCFG(..)", "cann't open cfg-file %s", sCFGFile);
/* ----------------------------------------------------------------------- */
fread(sBuf, 1, sizeof(sBuf), fp); fclose(fp);
/* ----------------------------------------------------------------------- */
#define STA_PAREE(_n_)	InstanceErrExit("MACLoadCFG(..)", "parameter %s not found", _n_)
if(!StrGetValue	(sBuf, "MAC_ADDR",					sValue))					STA_PAREE("STA_ADDR");	else { PktConvStrToAddr(sValue, &pMIB->MACAddress); }
if(!StrGetValAsInt(sBuf, "RTSThreshold",			&pMIB->RTSThreshold))		STA_PAREE("RTSThreshold");
if(!StrGetValAsInt(sBuf, "ShortRetryLimit",			&pMIB->ShortRetryLimit))	STA_PAREE("ShortRetryLimit");
if(!StrGetValAsInt(sBuf, "LongRetryLimit",			&pMIB->LongRetryLimit))		STA_PAREE("LongRetryLimit");
if(!StrGetValAsInt(sBuf, "FragmentationThreshold",	&pMIB->FragmentationThreshold))		STA_PAREE("FragmentationThreshold");
if(!StrGetValAsInt(sBuf, "MaxTransmitMSDULifetime",	&pMIB->MaxTransmitMSDULifetime))	STA_PAREE("MaxTransmitMSDULifetime");
if(!StrGetValAsInt(sBuf, "MaxReceiveLifetime",		&pMIB->MaxReceiveLifetime))	STA_PAREE("MaxReceiveLifetime");
/* ----------------------------------------------------------------------- */
CLOGRAdd(&pMAC->m_tLOGR, "MACLoadCFG(..)",
		"cfg dump:\n\n"
		"STAAddress    = %s\n\n"
		"MIB:\n"
		"RTSThreshold            = %d\tbytes\n"
		"ShortRetryLimit         = %d\tretransmissions\n"
		"LongRetryLimit          = %d\tretransmissions\n"
		"FragmentationThreshold  = %d\tbytes\n"
		"MaxTransmitMSDULifetime = %d\ttime units\n"
		"MaxReceiveLifetime      = %d\ttime units\n",
		PktConvAddrToStr(pMIB->MACAddress, sValue),
		pMIB->RTSThreshold,
		pMIB->ShortRetryLimit,
		pMIB->LongRetryLimit,
		pMIB->FragmentationThreshold,
		pMIB->MaxTransmitMSDULifetime,
		pMIB->MaxReceiveLifetime);
/* ----------------------------------------------------------------------- */
return(0);
#undef STA_PAREE
	}
/* ======================================================================= */
/* debug functions */
/* ======================================================================= */
int MACCopySt(tagLMAC* pMAC, tagMAC_ST* pDest, tagMAC_ST* pSrc)
{
/* -----------------------------------------------------------------------
// Description: copies all content of MAC structure into a give dest
//   Parametrs: ...
//      Return: (0) - success, (-1) - error
// -----------------------------------------------------------------------
//   MD: 30.07.2003
//   LE: 12.08.2003
// TODO: test it
// ----------------------------------------------------------------------- */
int i;
/* ------------------------------------------------------------------- */
CLOGRAdd (&pMAC->m_tLOGR, "MACCopySt(..)", "copies MAC state");
//STASanity(pSrc, "MACCopySt(..)");
/* ------------------------------------------------------------------- */
/* variables */
pDest->bVCFree		=pSrc->bVCFree;
pDest->nCW			=pSrc->nCW;
pDest->nCurSeqNo	=pSrc->nCurSeqNo;
pDest->nSSRC		=pSrc->nSSRC;
pDest->nSLRC		=pSrc->nSLRC;
/* ------------------------------------------------------------------- */
/* process specific variables */
pDest->nFrags		=pSrc->nFrags;
pDest->nCurFrag		=pSrc->nCurFrag;
/* ------------------------------------------------------------------- */
/* physical medium settings */
pDest->MIBC			=pSrc->MIBC;
/* ------------------------------------------------------------------- */
/* clears buffers if they are not empty (04.08.2003) */
if(pDest->pktCTRL.bValid)	PktClear(&pDest->pktCTRL);
if(pDest->pktRTS.bValid)	PktClear(&pDest->pktRTS);
if(pDest->pktTx.bValid)		PktClear(&pDest->pktTx);
if(pDest->pktRx.bValid)		PktClear(&pDest->pktRx);
if(pDest->pktTxFr.bValid)	PktClear(&pDest->pktTxFr);
/* ------------------------------------------------------------------- */
/* data buffers */
PktCopy(&pDest->pktCTRL,	&pSrc->pktCTRL);
PktCopy(&pDest->pktRTS,		&pSrc->pktRTS);
PktCopy(&pDest->pktTx,		&pSrc->pktTx);
PktCopy(&pDest->pktRx,		&pSrc->pktRx);
PktCopy(&pDest->pktTxFr,	&pSrc->pktTxFr);
/* ------------------------------------------------------------------- */
pDest->m_nCacheSize			=pSrc->m_nCacheSize;
pDest->m_nCacheAccessNo		=pSrc->m_nCacheAccessNo;
for (i=0; i<pDest->m_nCacheSize; i++) pDest->m_pDC[i]=pSrc->m_pDC[i];
/* ------------------------------------------------------------------- */
return(0);
}
/* ======================================================================= */
char* MACDumpST (tagLMAC* pMAC, tagMAC_ST* pMS, char* sBuf)
	{
/* -----------------------------------------------------------------------
// Description: makes dump of current state
//   Parametrs: ...
//      Return: ptr to sBuf - ok, NULL - error
// -----------------------------------------------------------------------
//   MD: 13.08.2003
//   LE: 13.08.2003
// TODO: test it
// ----------------------------------------------------------------------- */
if (pMS==NULL) return(NULL);
if (sBuf==NULL) return(NULL);
/* ----------------------------------------------------------------------- */
#define prnIDB(_x_) ((_x_)?"IDLE":"BUSY")
#define prnVN(_x_) ((_x_)?"FILLED":"------")
sprintf(sBuf,
		"\n"
		"MAC STATE (0x%p):\n"
		"bVCFree   = %s\n"
		"nCW       = %d\t\t(contention window size in SLOTS)\n"
		"nCurSeqNo = %d\t\t(seq. no. field)\n"
		"nSSRC     = %d\n"
		"nSLRC     = %d\n"
		"nCurFrag  = %d\t\t(%d of %d)\n"
		"pktCTRL   = %s\t(ACK & CTS buffer)\n"
		"pktRTS    = %s\n"
		"pktTx     = %s\n"
		"pktRx     = %s\n"
		"pktTxFr   = %s\t(fragment of the data packet)\n\n"
		"m_nCacheAccessNo = %d\t(# of current cache seq.)\n"
		"m_nCacheSize     = %d\t(# of records in the cache)\n\n"
		"MIB COUNTERS:\n"
		"TransmittedFragmentCount       = %d\n"
		"MulticastTransmittedFrameCount = %d\n"
		"FailedCount                    = %d\n"
		"RetryCount                     = %d\n"
		"MultipleRetryCount             = %d\n"
		"RTSSuccessCount                = %d\n"
		"RTSFailureCount                = %d\n"
		"ACKFailureCount                = %d\n"
		"ReceivedFragmentCount          = %d\n"
		"MulticastReceivedFrameCount    = %d\n"
		"FCSErrorCount                  = %d\n"
		"FrameDuplicateCount            = %d\n\n",
		pMS,				prnIDB(pMS->bVCFree),
		pMS->nCW,			pMS->nCurSeqNo,
		pMS->nSSRC,			pMS->nSLRC,		
		pMS->nCurFrag,		pMS->nCurFrag+1,	pMS->nFrags,
		prnVN(pMS->pktCTRL.bValid),	prnVN(pMS->pktRTS.bValid),
		prnVN(pMS->pktTx.bValid),	prnVN(pMS->pktRx.bValid),			
		prnVN(pMS->pktTxFr.bValid),
		pMS->m_nCacheAccessNo,		pMS->m_nCacheSize,
		pMS->MIBC.TransmittedFragmentCount,	pMS->MIBC.MulticastTransmittedFrameCount,
		pMS->MIBC.FailedCount,				pMS->MIBC.RetryCount,
		pMS->MIBC.MultipleRetryCount,		pMS->MIBC.RTSSuccessCount,
		pMS->MIBC.RTSFailureCount,			pMS->MIBC.ACKFailureCount,
		pMS->MIBC.ReceivedFragmentCount,	pMS->MIBC.MulticastReceivedFrameCount,
		pMS->MIBC.FCSErrorCount,			pMS->MIBC.FrameDuplicateCount);
/* ----------------------------------------------------------------------- */
return(sBuf);
}
/* ======================================================================= */
/* private functions */
/* ======================================================================= */
tagPKT* MACGetPktByID(tagLMAC* pMAC, tagMAC_ST* pST, int nBufID)
{
/* -----------------------------------------------------------------------
// Description: returns pointer to Pkt w. given ID
//   Parametrs: ...
//      Return: pointer - ok, (0) - error
// -----------------------------------------------------------------------
//   MD: 25.07.2003
//   LE: 25.07.2003
// TODO: test it
// ----------------------------------------------------------------------- */
tagPKT* pPkt;
CLOGRAdd (&pMAC->m_tLOGR, "MACGetPktByID(..)", "called (nBufID=%d)", nBufID);
MACSanity(pMAC, "MACGetPktByID(..)");
switch(nBufID)
	{
	case bCTRL:	pPkt=&pST->pktCTRL;	break;
	case bRTS:	pPkt=&pST->pktRTS;	break;
	case bTx:	pPkt=&pST->pktTx;	break;
	case bRx:	pPkt=&pST->pktRx;	break;
	case bTxFr:	pPkt=&pST->pktTxFr;	break;
	default:	pPkt=NULL;					break;
	};
CLOGRAdd (&pMAC->m_tLOGR, "MACGetPktByID(..)", "=0x%p", pPkt);
return(pPkt);
}
/* ======================================================================= */
int	MACDataTxTimeL(tagLMAC* pMAC, int nBits)
{
/* time in us to transmit nBits at the lowest rate (28.07.2003) */
MACSanity(pMAC, "MACDataTxTimeL(..)");
return(MACDataTxTime(pMAC, nBits, pMAC->m_tPHY.m_aPLCPDataRate));
}
/* ----------------------------------------------------------------------- */
int	MACDataTxTime(tagLMAC* pMAC, int nBits, double dRate)
{
/* time in us to transmit nBits at the given rate (28.07.2003) */
double dBits=(double)nBits;
double dTxTime;
int nTxTime;
CLOGRAdd (&pMAC->m_tLOGR, "MACDataTxTime(..)", "called (nBits=%d, dRate=%3.2f)", nBits, dRate);
MACSanity(pMAC, "MACDataTxTime(..)");
dTxTime=dBits/dRate;
nTxTime=(int)(dTxTime*1e6);
CLOGRAdd(&pMAC->m_tLOGR, "MACDataTxTime(..)", "=%d", nTxTime);
return(nTxTime);
}
/* ======================================================================= */
int	MACFrameTxTimeL(tagLMAC* pMAC, int nBits)
{
/* time in us to transmit nBits at the lowest rate (29.09.2003) */
MACSanity(pMAC, "MACFrameTxTimeL(..)");
return(MACFrameTxTime(pMAC, nBits, pMAC->m_tPHY.m_aPLCPDataRate));
}
/* ----------------------------------------------------------------------- */
int	MACFrameTxTime(tagLMAC* pMAC, int nBits, double dRate)
{
/* time in us to transmit frame of size nBits at the given rate (29.09.2003) */
int nTxTime;
CLOGRAdd (&pMAC->m_tLOGR, "MACFrameTxTime(..)", "called (nBits=%d, dRate=%3.2f)", nBits, dRate);
MACSanity(pMAC, "MACFrameTxTime(..)");
nTxTime=MACDataTxTime(pMAC, nBits, dRate)+MACGetPpduHdrTxTime(pMAC);
CLOGRAdd(&pMAC->m_tLOGR, "MACFrameTxTime(..)", "=%d", nTxTime);
return(nTxTime);
}
/* ======================================================================= */
/* MAC public functions */
/* ======================================================================= */
int MACCalcDurFrag(tagLMAC* pMAC, tagMAC_ST* pST, int nCurFrag, int nFragSize, int nFrags, int nDataSize)
{
/* calculates value of DURATION field of given fragment (07.08.2003) */
/* ref: 7.2.2. Data Frames */
int nDuration;
int nNextFragSize;
CLOGRAdd (&pMAC->m_tLOGR, "MACCalcDurFrag(..)", "get value of DURATION field for fragment no. %d", nCurFrag);
MACSanity(pMAC, "MACCalcDurFrag(..)");
nDuration=pMAC->m_tTC.m_nTxTimeACK+pMAC->m_tTC.m_nSIFS;
if (nCurFrag+1<nFrags)
	{
	/* size is equal: PREAMBLE + HDR + DATA + CRC (29.09.2003) */
	nNextFragSize=PktCalcFragRealSize(nCurFrag+1, nFragSize, nDataSize);
	nDuration+=
		MACGetPpduHdrTxTime(pMAC)+
		PktGetDataTxTime(sizeof(tagHDRPKTDATA)*8, pMAC->m_tPHY.m_aPLCPDataRate)+
		PktGetDataTxTime(nNextFragSize*8, pMAC->m_tPHY.m_aPLCPDataRate)+
		PktGetDataTxTime(4*8, pMAC->m_tPHY.m_aPLCPDataRate)+
		pMAC->m_tTC.m_nTxTimeACK+pMAC->m_tTC.m_nSIFS*2;
	}
CLOGRAdd (&pMAC->m_tLOGR, "MACCalcDurFrag(..)", "=%d", nDuration);
return(nDuration);
}
/* ======================================================================= */
int MACCalcNumOfFrags(tagLMAC* pMAC, tagMAC_ST* pST, int nFragSize)
{
/* ----------------------------------------------------------------------- */
/* 25.07.2003, 12.08.2003, 22.09.2003 */
/* ----------------------------------------------------------------------- */
int nFrags;
int nPktFragSize;
CLOGRAdd (&pMAC->m_tLOGR, "MACCalcNumOfFrags()", "get number of fragments");
MACSanity(pMAC, "MACCalcNumOfFrags()");
nPktFragSize=	(pST->pktTx.nSize > pMAC->m_tMIB.FragmentationThreshold)?
				nFragSize:pST->pktTx.nSize;
nFrags=PktCalcNumOfFrags(&pST->pktTx, nPktFragSize);
CLOGRAdd (&pMAC->m_tLOGR, "MACCalcNumOfFrags()", "=%d", nFrags);
/* ----------------------------------------------------------------------- */
if (nFrags>=16) InstanceErrExit("MACCalcNumOfFrags()", "too many fragments (=%d)", nFrags);
/* ----------------------------------------------------------------------- */
return(nFrags);
}
/* ======================================================================= */
int MACClearBuf(tagLMAC* pMAC, tagMAC_ST* pST, int nBufID)
{
/* 25.07.2003 */
tagPKT* pPkt;
CLOGRAdd (&pMAC->m_tLOGR, "MACClearBuf()", "clear pkt (nBufID=%d)", nBufID);
MACSanity(pMAC, "MACClearBuf(..)");
pPkt=MACGetPktByID(pMAC, pST, nBufID);
if (NULL==pPkt) InstanceErrExit("MACClearBuf()", "invalid buffer ID (nBufID=%d)", nBufID);
PktClear(pPkt);
return(0);
}
/* ======================================================================= */
int MACFillL3Buf(tagLMAC* pMAC, tagMAC_ST* pST, tagEMULCFG* pCFG)
{
/* -----------------------------------------------------------------------
// Description: fills DATA buffer of the upper layer w. inbound DATA (16.07.2003)
//   Parametrs: ...
//      Return: 0 - ok
// -----------------------------------------------------------------------
//   MD: 07.08.2003
//   LE: 12.08.2003
// TODO: test it
// ----------------------------------------------------------------------- */
FILE* fp;
/* ----------------------------------------------------------------------- */
CLOGRAdd (&pMAC->m_tLOGR, "MACFillL3Buf()", "fill buf of L3 w. buf");
MACSanity(pMAC, "MACFillL3Buf()");
PktSanity(&pST->pktRx, "MACFillL3Buf()");
/* ----------------------------------------------------------------------- */
sprintf(glPMTB, pCFG->m_sL3O_MPDU,
		PktConvAddrToStr2(pST->pktRx.sa, glPMTB2), pST->pktRx.nSeqNo);
/* ----------------------------------------------------------------------- */
/* updates received data file (L3 outbound) */
/* ----------------------------------------------------------------------- */
if ((fp=fopen(pCFG->m_sL3[1], "a+b"))==NULL)
	InstanceErrExit("MACFillL3Buf()", "cann't open L3 outbound file (%s)", 
	pCFG->m_sL3[1]);
fwrite(pST->pktRx.m_pData, 1, pST->pktRx.nSize, fp); fclose(fp);
/* ----------------------------------------------------------------------- */
/* saves fragment in a MPDU file */
/* ----------------------------------------------------------------------- */
if ((fp=fopen(glPMTB, "a+b"))==NULL)
	InstanceErrExit("MACFillL3Buf()", "cann't open L3 MPDU file (%s)", glPMTB);
fwrite(pST->pktRx.m_pData, 1, pST->pktRx.nSize, fp); fclose(fp);
/* ----------------------------------------------------------------------- */
CLOGRAdd (&pMAC->m_tLOGR, "MACFillL3Buf()", "write = %d bytes (%d bits), file %s", pST->pktRx.nSize, pST->pktRx.nSize*8, glPMTB);
/* ----------------------------------------------------------------------- */
return(0);
}
/* ======================================================================= */
int MACFillPhyCtrlBuf(tagLMAC* pMAC, tagMAC_ST* pST, tagLPC_ST* pLPC, tagEMULCFG* pCFG, int nBufID)
{
/* -----------------------------------------------------------------------
// Description: fills DATA buffer of PHY_CTRL layer w. given DATA (16.07.2003)
//   Parametrs: ...
//      Return: 0 - ok, (-1) - error
// -----------------------------------------------------------------------
//   MD: 04.07.2003
//   LE: 10.09.2003
// TODO: complete it
// ----------------------------------------------------------------------- */
void* pPacket;
int nSize;
tagPKT* pPkt;
/* ----------------------------------------------------------------------- */
CLOGRAdd (&pMAC->m_tLOGR, "MACFillPhyCtrlBuf(..)", "fill buf of PHY_CTRL w. buf (nBufID=%d)", nBufID);
MACSanity(pMAC, "MACFillPhyCtrlBuf(..)"); 
pPkt=MACGetPktByID(pMAC, pST, nBufID);
if (pPkt==NULL) InstanceErrExit("MACFillPhyCtrlBuf(..)", "invalid buffer ID (nBufID=%d)", nBufID);
/* ----------------------------------------------------------------------- */
PktDump(pPkt, glPMTB8K);
CLOGRAdd (&pMAC->m_tLOGR_IO, "MACFillPhyCtrlBuf(..)", "pkt dump (nBufID=%d) %s", nBufID, glPMTB8K);
/* ----------------------------------------------------------------------- */
nSize=PktCalcRealSize(pPkt);
pPacket=PktFormRealPkt(pPkt, nSize);
/* ----------------------------------------------------------------------- */
/* transfers data into PHY_CTRL layer */
PCFillTxBuf(pLPC, pCFG, pPacket, nSize);
free(pPacket);
/* ----------------------------------------------------------------------- */
CLOGRAdd (&pMAC->m_tLOGR, "MACFillPhyCtrlBuf(..)", "write = %d bytes (%d bits)",
		  nSize, nSize*8);
/* ----------------------------------------------------------------------- */
return(0);
}
/* ======================================================================= */
int MACFillTxBuf(tagLMAC* pMAC, tagMAC_ST* pST, tagEMULCFG* pCFG, 
				 tagMACADDR* pDA, tagFLATARRAY* pArray)
	{
/* -----------------------------------------------------------------------
// Description: transfers user data into pktTx
//   Parametrs: ...
//      Return: 0 - ok
// -----------------------------------------------------------------------
//   MD: 16.07.2003
//   LE: 10.09.2003
// TODO: complete it
// ----------------------------------------------------------------------- */
CLOGRAdd (&pMAC->m_tLOGR, "MACFillTxBuf()", "receives data from L3 to Tx");
MACSanity(pMAC, "MACFillTxBuf()");
/* ----------------------------------------------------------------------- */
/* checks if pktTx is clear */
/* ----------------------------------------------------------------------- */
if (pST->pktTx.bValid)
	{
	CLOGRAdd (&pMAC->m_tLOGR, "MACFillTxBuf()", "pktTx is not empty (is valid)");
	PktClear(&pST->pktTx);
	}
/* ----------------------------------------------------------------------- */
/* adjust packet fields*/
/* ----------------------------------------------------------------------- */
PktCreate(&pST->pktTx,
		  pDA, &pMAC->m_tMIB.MACAddress, NULL, NULL, NULL,/* 7.1.3.3.5. SRC addr */
		  TYP_DATA, STYP_DATA, pArray->nSize, 0,	0,		/* will be changed later */
		  -1, -1, PKT_NORETRY, PKT_VALID, PKT_NOFRAGS, PKT_HAVEDATA, 
		  pArray->pData);
/* ----------------------------------------------------------------------- */
PktDump(&pST->pktTx, glPMTB8K);
CLOGRAdd(&pMAC->m_tLOGR,	"MACGetDataInTx()", "packet size = %d bytes (%d bits)", pArray->nSize, pArray->nSize*8);
CLOGRAdd(&pMAC->m_tLOGR_IO, "MACGetDataInTx()", "Tx pkt dump %s", glPMTB8K);
/* ----------------------------------------------------------------------- */
return(0);
	}
/* ======================================================================= */
int MACFormRTS(tagLMAC* pMAC, tagMAC_ST* pST, tagMACADDR tRA, int nDurPktTxFr)
{
/* forms RTS frame for given station (23.07.2003) */
/* (11.08.2003) */
/* ref: 7.2.1.1. */
int nDuration;
CLOGRAdd (&pMAC->m_tLOGR, "MACFormRTS(..)", "fills RTS buf (tRA=%s, tTA=%s, nDurPktTxFr=%d)", 
		  PktConvAddrToStr(tRA, glPMTB), PktConvAddrToStr(pMAC->m_tMIB.MACAddress, glPMTB2), nDurPktTxFr);
MACSanity(pMAC, "MACFormRTS(..)");
nDuration=nDurPktTxFr+
		pMAC->m_tTC.m_nTxTimeACK+
		pMAC->m_tTC.m_nTxTimeCTS+
		pMAC->m_tTC.m_nSIFS*3;
PktFormRTS(&pST->pktRTS, tRA, pMAC->m_tMIB.MACAddress, nDuration);
CLOGRAdd (&pMAC->m_tLOGR, "MACFormRTS(..)", "RTS duration=%d", nDuration);
return(0);
}
/* ======================================================================= */
int MACFormCTS(tagLMAC* pMAC, tagMAC_ST* pST, tagMACADDR tRA, int nDurPktRTS)
{
/* forms CTS frame for given station (16.07.2003) */
/* (11.08.2003, 26.09.2003 - bug deleted !!!) */
/* ref: 7.2.1.2. */
int nDuration;
CLOGRAdd (&pMAC->m_tLOGR, "MACFormCTS(..)", "fills CTRL buf (tRA=%s, nDurPktRTS=%d)", PktConvAddrToStr(tRA, glPMTB), nDurPktRTS);
MACSanity(pMAC, "MACFormCTS(..)");
nDuration=nDurPktRTS-
		pMAC->m_tTC.m_nTxTimeCTS-
		pMAC->m_tTC.m_nSIFS;
PktFormCTS(&pST->pktCTRL, tRA, nDuration);
CLOGRAdd (&pMAC->m_tLOGR, "MACFormCTS(..)", "CTS duration=%d", nDuration);
return(0);
}
/* ======================================================================= */
int MACFormACK(tagLMAC* pMAC, tagMAC_ST* pST, tagMACADDR tRA, int nDurPktRx, int bMoreFrags)
{
/* forms ACK frame for given station (16.07.2003) */
/* 08.08.2003 */
/* ref: 7.2.1.3. */
int nDuration;
CLOGRAdd (&pMAC->m_tLOGR, "MACFormACK(..)", "fills CTRL buf (tRA=%s, nDurPktRx=%d, bMoreFrags=%d)", PktConvAddrToStr(tRA, glPMTB), nDurPktRx, bMoreFrags);
MACSanity(pMAC, "MACFormACK(..)");
nDuration=bMoreFrags?(nDurPktRx-pMAC->m_tTC.m_nTxTimeACK-pMAC->m_tTC.m_nSIFS):0;
PktFormACK(&pST->pktCTRL, tRA, nDuration);
CLOGRAdd (&pMAC->m_tLOGR, "MACFormACK(..)", "ACK duration=%d", nDuration);
return(0);
}
/* ======================================================================= */
int MACGetAddrType(tagLMAC* pMAC, tagMAC_ST* pST, tagMACADDR tAddr)
{
/* -----------------------------------------------------------------------
// Description: returns dest. address type of a given packet
//   Parametrs: ...
//      Return: MAC_FRGN, MAC_ADDR, MAC_BRDCST, MAC_MLTCST
// -----------------------------------------------------------------------
//   MD: 28.07.2003
//   LE: 31.07.2003
// TODO: test it
// ----------------------------------------------------------------------- */
int i;
int bEqual;
CLOGRAdd (&pMAC->m_tLOGR, "MACGetAddrType(..)", "addr type (tAddr=%s)", PktConvAddrToStr(tAddr, glPMTB));
MACSanity(pMAC, "MACGetAddrType(..)");
/* ----------------------------------------------------------------------- */
bEqual=1;
for (i=0; i<ETHER_ADDR_LEN; i++)
	if (tAddr.ab[i]!=pMAC->m_tMIB.MACAddress.ab[i])
		bEqual=0; 
if (bEqual) return(MAC_ADDR);
/* ----------------------------------------------------------------------- */
/* 7.1.3.3.2. MAC broadcast address */
bEqual=1;
for (i=0; i<ETHER_ADDR_LEN; i++)
	if (tAddr.ab[i]!=0xff)
		bEqual=0;
if (bEqual) return(MAC_BRDCST);
/* ----------------------------------------------------------------------- */
return(MAC_FRGN);
}
/* ======================================================================= */
int MACGetDataInRx(tagLMAC* pMAC, tagMAC_ST* pST, tagFLDPSDU* pPSDU)
{
/* -----------------------------------------------------------------------
// Description: transfers inbound data into tBufRx (30.07.2003)
//   Parametrs: ...
//      Return: 0 - ok
// -----------------------------------------------------------------------
//   MD: 07.08.2003
//   LE: 05.09.2003
// TODO: complete it
// ----------------------------------------------------------------------- */
void* pData;
int nSize;
/* ----------------------------------------------------------------------- */
CLOGRAdd (&pMAC->m_tLOGR, "MACGetDataInRx()", "transforms pPSDU buffer into pktRx");
MACSanity(pMAC, "MACGetDataInRx()");
/* ----------------------------------------------------------------------- */
/* checks if pktRx is clear */
/* ----------------------------------------------------------------------- */
if (pST->pktRx.bValid)
	{
	CLOGRAdd (&pMAC->m_tLOGR, "MACGetDataInRx()", "pktRx is not empty (is valid), CLEARED");
	PktClear(&pST->pktRx);
	}
/* ----------------------------------------------------------------------- */
/* check size of received data and duration */
/* ----------------------------------------------------------------------- */
pData=pPSDU->m_pData;
nSize=pPSDU->m_nSize;
/* ----------------------------------------------------------------------- */
CLOGRAdd (&pMAC->m_tLOGR, "MACGetDataInRx()", "recv: MDM %d bytes", nSize);
/* ----------------------------------------------------------------------- */
/* adjust packet fields*/
/* ----------------------------------------------------------------------- */
PktPacketize(&pST->pktRx, pData, nSize, pMAC->m_tPHY.m_aPLCPDataRate);
/* ----------------------------------------------------------------------- */
PktDump(&pST->pktRx, glPMTB8K);
CLOGRAdd (&pMAC->m_tLOGR_IO, "MACGetDataInRx()", "Rx pkt dump %s", glPMTB8K);
/* ----------------------------------------------------------------------- */
return(0);
}
/* ======================================================================= */
int MACGetNewFr(tagLMAC* pMAC, tagMAC_ST* pST, int nFrag, int nFragSize)
{
/* fills pktTxFr with new data (25.07.2003) */
int nDuration;
CLOGRAdd (&pMAC->m_tLOGR, "MACGetNewFr(..)", "get data fragment (nFrag=%d)", nFrag);
MACSanity(pMAC, "MACGetNewFr(..)");
if (nFrag!=pST->nCurFrag) InstanceErrExit("MACGetNewFr(..)", "resynchronized fragments (nFrag=%d, nCurFrag=%d)", nFrag, pST->nCurFrag);
nDuration=MACCalcDurFrag(pMAC, pST, nFrag, nFragSize, pST->nFrags, pST->pktTx.nSize);
PktGetFrNum(&pST->pktTx, &pST->pktTxFr, nFrag, nFragSize, pST->nFrags, 
				 nDuration);
PktDump(&pST->pktTxFr, glPMTB8K);
CLOGRAdd (&pMAC->m_tLOGR_IO, "MACGetNewFr(..)", "TxFr pkt dump %s", glPMTB8K);
return(0);
}
/* ======================================================================= */
int MACGetPpduHdrTxTime(tagLMAC* pMAC)
	{
	/* (29.09.2003) */
	MACSanity(pMAC, "MACGetPpduHdrTxTime(..)");
	//return(pMAC->m_tPHY.m_aPreambleLength + pMAC->m_tPHY.m_aPLCPHeaderLength);
	return(20);
	}
/* ======================================================================= */
/* system functions */
/* ======================================================================= */
int MACCWInc(tagLMAC* pMAC, tagMAC_ST* pST) /* 9.2.4. Random BO proc. */
	{
	CLOGRAdd (&pMAC->m_tLOGR, "MACCWInc()", "increase CW");
	MACSanity(pMAC, "MACCWInc()"); 
	pST->nCW=pST->nCW*2+1;
	if (pST->nCW>pMAC->m_tPHY.m_aCWmax)
		pST->nCW=pMAC->m_tPHY.m_aCWmax;
	CLOGRAdd (&pMAC->m_tLOGR, "MACCWInc()", "ok, nCW=%d time slots", pST->nCW);
	return(0);}
/* ======================================================================= */
int MACCWReset(tagLMAC* pMAC, tagMAC_ST* pST) /* */
	{
	CLOGRAdd (&pMAC->m_tLOGR, "MACCWReset()", "reset CW");
	MACSanity(pMAC, "MACCWReset()");
	pST->nCW=pMAC->m_tPHY.m_aCWmin; 
	CLOGRAdd (&pMAC->m_tLOGR, "MACCWReset()", "ok, nCW=%d time slots", pST->nCW);
	return(0);}
/* ----------------------------------------------------------------------- */
int MACGetRandNumOfSlots(tagLMAC* pMAC, tagMAC_ST* pST)
	{
	int nRand;
	CLOGRAdd (&pMAC->m_tLOGR, "MACGetRandNumOfSlots()", "get BO slots");
	MACSanity(pMAC, "MACGetRandNumOfSlots()");
	nRand=rand()%pST->nCW;
	CLOGRAdd (&pMAC->m_tLOGR, "MACGetRandNumOfSlots()", "=%d", nRand);
	return(nRand); }
/* ======================================================================= */
int MACIsDupl(tagLMAC* pMAC, tagMAC_ST* pST, tagMACADDR tSA, int nSeqNo, int nFragNo)
	{
	int i;
	int bRes=0;
	/* (12.08.2003) */
	CLOGRAdd (&pMAC->m_tLOGR, "MACIsDupl(..)", "check if a packet is a duplicate (tSA=%s, nSeqNo=%d:%d)", PktConvAddrToStr(tSA, glPMTB), nSeqNo, nFragNo);
	MACSanity(pMAC, "MACIsDupl(..)");
	for(i=0; i<pST->m_nCacheSize; i++)
		if (pST->m_pDC[i].tAddr2.ab[0]==tSA.ab[0] &&
			pST->m_pDC[i].tAddr2.ab[1]==tSA.ab[1] &&
			pST->m_pDC[i].tAddr2.ab[2]==tSA.ab[2] &&
			pST->m_pDC[i].tAddr2.ab[3]==tSA.ab[3] &&
			pST->m_pDC[i].tAddr2.ab[4]==tSA.ab[4] &&
			pST->m_pDC[i].tAddr2.ab[5]==tSA.ab[5] &&
			
			pST->m_pDC[i].nSeqNo==nSeqNo)
			if(pST->m_pDC[i].nFragNo==nFragNo)
				{ bRes=1; break; }
	CLOGRAdd (&pMAC->m_tLOGR, "MACIsDupl(..)", "=%s", bRes?"YES":"NO");
	return(bRes);
	}
/* ======================================================================= */
int MACIsPsduCrcOk(tagLMAC* pMAC, tagFLDPSDU* pPSDU)
{
/* (25.07.2003, 05.09.2003) */
int bRes;
int nSize;
char* pData;
unsigned int nCRC32, nEvalCRC32;
CLOGRAdd (&pMAC->m_tLOGR, "MACIsPsduCrcOk(..)", "check CRC of the incoming packet");
MACSanity(pMAC, "MACIsPsduCrcOk(..)");
/* ----------------------------------------------------------------------- */
/* calculates CRC32 and compares to receivded CRC */
pData=pPSDU->m_pData;	// (05.09.2003)
nSize=pPSDU->m_nSize;	// (05.09.2003)
nCRC32=*(unsigned int*)((pData+nSize-4));
nEvalCRC32=PktCalcCRC32(pData, nSize-4);
bRes=(nCRC32==nEvalCRC32)?1:0;
/* ----------------------------------------------------------------------- */
CLOGRAdd (&pMAC->m_tLOGR, "MACIsPsduCrcOk(..)", "received CRC32=0x%08x, evaluated CRC32=0x%08x", nCRC32, nEvalCRC32);
CLOGRAdd (&pMAC->m_tLOGR, "MACIsPsduCrcOk(..)", "=%d", bRes);
return(bRes);
}
/* ======================================================================= */
int MACRecvPSDU(tagLMAC* pMAC, tagLPC_ST* pLPC, tagEMULCFG* pCFG, tagFLDPSDU* pPSDU)
{
/* -----------------------------------------------------------------------
// Description: transfers inbound data into pPSDU (30.07.2003)
//   Parametrs: ...
//      Return: 0 - ok
// -----------------------------------------------------------------------
//   MD: 07.08.2003
//   LE: 05.09.2003
// TODO: test it
// ----------------------------------------------------------------------- */
FILE* fp;
/* ----------------------------------------------------------------------- */
CLOGRAdd (&pMAC->m_tLOGR, "MACRecvPSDU(..)", "obtain PSDU data from PHY_CTRL layer");
MACSanity(pMAC, "MACRecvPSDU(..)");
/* ----------------------------------------------------------------------- */
/* checks if pktRx is clear */
/* ----------------------------------------------------------------------- */
if (0==pLPC->m_tPSDU.m_nSize)
	InstanceErrExit("MACRecvPSDU(..)", "pLPC->m_tPSDU is EMPTY");
/* ----------------------------------------------------------------------- */
/* copies PSDU (05.09.2003) */
/* ----------------------------------------------------------------------- */
*pPSDU=pLPC->m_tPSDU;
/* ----------------------------------------------------------------------- */
/* updates received data file (LPC inbound) */
/* ----------------------------------------------------------------------- */
if ((fp=fopen(pCFG->m_sLPC[0], "a+b"))==NULL)
	InstanceErrExit("MACRecvPSDU(..)", "cann't open LPC inbound file (%s)", 
	pCFG->m_sLPC[0]);
fwrite(pPSDU->m_pData, 1, pPSDU->m_nSize, fp); fclose(fp);
/* ----------------------------------------------------------------------- */
CLOGRAdd (&pMAC->m_tLOGR, "MACRecvPSDU(..)", "read = %d bytes (%d bits)",
		  pPSDU->m_nSize, pPSDU->m_nSize*8);
/* ----------------------------------------------------------------------- */
return(0);
}
/* ======================================================================= */
int MACResetSLC(tagLMAC* pMAC, tagMAC_ST* pST, int nSize)
	{
	CLOGRAdd (&pMAC->m_tLOGR, "MACResetSLC(..)", "resets SSRC or SLRC counters (nSize=%d)", nSize);
	MACSanity(pMAC, "MACResetSLC(..)"); /* resets SSRC or SLRC counters (16.07.2003, 12.08.2003) */
	if (nSize<=pMAC->m_tMIB.RTSThreshold) pST->nSSRC=0;
	else pST->nSLRC=0;
	CLOGRAdd (&pMAC->m_tLOGR, "MACResetSLC(..)", "ok, RTS_THRESH=%d, ssrc=%d, slrc=%d", pMAC->m_tMIB.RTSThreshold, pST->nSSRC, pST->nSLRC);
	return(0);}
/* ======================================================================= */
int MACUpdDuplCache(tagLMAC* pMAC, tagMAC_ST* pST, tagMACADDR tSA, int nSeqNo, int nFragNo)
{
/* (12.08.2003) */
int i;
int bRes=0;
CLOGRAdd (&pMAC->m_tLOGR, "MACUpdDuplCache(..)", "check if a packet is a duplicate (tSA=%s, nSeqNo=%d:%d)", PktConvAddrToStr(tSA, glPMTB), nSeqNo, nFragNo);
MACSanity(pMAC, "MACUpdDuplCache(..)");
/* ----------------------------------------------------------------------- */
/* updates existing record */
for(i=0; i<pST->m_nCacheSize; i++)
	if (pST->m_pDC[i].tAddr2.ab[0]==tSA.ab[0] &&
		pST->m_pDC[i].tAddr2.ab[1]==tSA.ab[1] &&
		pST->m_pDC[i].tAddr2.ab[2]==tSA.ab[2] &&
		pST->m_pDC[i].tAddr2.ab[3]==tSA.ab[3] &&
		pST->m_pDC[i].tAddr2.ab[4]==tSA.ab[4] &&
		pST->m_pDC[i].tAddr2.ab[5]==tSA.ab[5] &&

		pST->m_pDC[i].nSeqNo==nSeqNo)
			{
			pST->m_pDC[i].nAccessNo=pST->m_nCacheAccessNo;
			pST->m_pDC[i].nFragNo=nFragNo;
			pST->m_nCacheAccessNo++;
			CLOGRAdd (&pMAC->m_tLOGR, "MACUpdDuplCache(..)", "updated rec. %d", i);
			return(0);
			}
/* ----------------------------------------------------------------------- */
/* sequence not found */
if (pST->m_nCacheSize>=MAC_DUPLCSIZE)
	{
	/* ------------------------------------------------------------------- */
	/* cache is full */
	int nOldest=0;
	/* ------------------------------------------------------------------- */
	/* searching for the oldest record */
	for (i=0; i<pST->m_nCacheSize; i++)
		if (pST->m_pDC[i].nAccessNo<pST->m_pDC[nOldest].nAccessNo)
			nOldest=i;
	/* ------------------------------------------------------------------- */
	/* overwrite the oldest record */
	pST->m_pDC[nOldest].nAccessNo=pST->m_nCacheAccessNo;
	pST->m_pDC[nOldest].tAddr2=tSA;
	pST->m_pDC[nOldest].nFragNo=nFragNo;
	pST->m_pDC[nOldest].nSeqNo=nSeqNo;
	pST->m_nCacheAccessNo++;
	/* ------------------------------------------------------------------- */
	CLOGRAdd (&pMAC->m_tLOGR, "MACUpdDuplCache(..)", "overwritten rec. %d", nOldest);
	}
/* ----------------------------------------------------------------------- */
else
	{
	int nLastRec=pST->m_nCacheSize;
	/* ------------------------------------------------------------------- */
	/* add a new record */
	pST->m_pDC[nLastRec].nAccessNo=pST->m_nCacheAccessNo;
	pST->m_pDC[nLastRec].tAddr2=tSA;
	pST->m_pDC[nLastRec].nFragNo=nFragNo;
	pST->m_pDC[nLastRec].nSeqNo=nSeqNo;
	pST->m_nCacheAccessNo++;
	pST->m_nCacheSize++;
	CLOGRAdd (&pMAC->m_tLOGR, "MACUpdDuplCache(..)", "added rec. %d", nLastRec);
	}
/* ----------------------------------------------------------------------- */
return(0);
}
/* ======================================================================= */
