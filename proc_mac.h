/* =======================================================================
// MAC procedures ver. 1.0
// structs and constants definition
// -----------------------------------------------------------------------
// MD: 30.07.2003
// LE: 02.09.2003
// -----------------------------------------------------------------------
// Compiler: MS VC 6.0
// =======================================================================
// procmac.h: implementation of service functions (header)
// ======================================================================= */
#ifndef __PROCMAC__
#define __PROCMAC__
/* ======================================================================= */
#include "proc_mac_exp.h"
#include "proc_phyctrl.h"	/* (10.09.2003) */
#include "emul_cfg.h"		/* (10.09.2003) */
#include "lib_path.h"		/* (09.10.2003) */
#include C_LOGGER_H			/* tagCLOGR (14.08.2003) */
/* ======================================================================= */
#define	MAC_ProtocolVersion		(0x00)
/* ----------------------------------------------------------------------- 
 * Must account for propagation delays added by the channel model when
 * calculating tx timeouts (as set in tcl/lan/ns-mac.tcl).
 *   -- Gavin Holland, March 2002
 *
 * ----------------------------------------------------------------------- */
#define DSSS_MaxPropagationDelay        0.000002      /* 2us   XXXX */
/* ======================================================================= */
/* 10.4.3.2. PLME-CHARACTERISTICS.confirm */
/* ======================================================================= */
typedef struct {
	int m_aSlotTime;
	int m_aSIFSTime;
	int m_aCCATime;
	int m_aRxTxTurnaroundTime;
	int m_aTxPLCPDelay;
	int m_aRxPLCPDelay;
	int m_aRxTxSwitchTime;
	int m_aTxRampOnTime;
	int m_aTxRampOffTime;
	int m_aTxRFDelay;
	int m_aRxRFDelay;
	int m_aAirPropagationTime;
	int m_aMACProcessingDelay;
	int m_aPreambleLength;
	int m_aPLCPHeaderLength;
	double m_aPLCPDataRate;			/* NOT in 10.4.3. (28.09.2003) */
	int m_aMPDUDurationFactor;
	int m_aMPDUMaxLength;
	int m_aCWmin;
	int m_aCWmax;
	} tagPLMECH_PHY;
/* ======================================================================= */
/* Annex C.2 Package  macmib */
/* ======================================================================= */
typedef struct {
	tagMACADDR MACAddress;
	/* tagMACADDR*	pGroupAddresses; */ /* not supported */
	int	RTSThreshold;
	int	ShortRetryLimit;
	int	LongRetryLimit;
	int	FragmentationThreshold;
	int	MaxTransmitMSDULifetime;
	int	MaxReceiveLifetime;
	//		ManufacturerID;
	//		ProductID;
	} tagMAC_MIB;
/* ======================================================================= */
/* additional constants (30.07.2003) - Annex C.2 Package  macmib */
/* ======================================================================= */
typedef struct {
	int m_bExcludeUnencrypted;
	/*tagMACADDR* pSTAAddrSet; */	/* not supported */
	int m_nMediumOccupancyLimit;
	int m_bPrivacyInvoked;
	int m_bmReceiveDTIMs;
	int m_nCfpPeriod;
	int m_nCfpMaxDuration;
	int m_nAuthenticationResponseTimeout;
	int m_nWepDefaultKeyId;
	int m_nCurrentChannelNumber;
	int m_nCurrentSet;
	int m_nCurrentPattern;
	int m_nCurrentIndex;
	} tagMAC_MIBCNSTS;
/* ======================================================================= */
/* timing constants (13.08.2003) */
/* ======================================================================= */
typedef struct
	{
	int m_nSIFS;			/* SIFS */
	int m_nPIFS;			/* PIFS */
	int m_nDIFS;			/* DIFS */
	int m_nEIFS;			/* EIFS */
	int m_nTxTimeACK;		/* TxTime of ACK frame (08.08.2003) */
	int m_nTxTimeCTS;		/* TxTime of CTS frame (11.08.2003) */
	/* ------------------------------------------------------------------- */
	int m_nACKTimeout;		/* 9.2.8. ACKTimeout interval */
	int m_nCTSTimeout;		/* 9.2.5.7. ACKTimeout interval */
	} tagMAC_TIME;
/* ======================================================================= */
/* MAC layer (02.09.2003) */
/* ======================================================================= */
typedef struct
	{
	int m_bClassCreated;		// class created (24.09.2003)
	tagMAC_TIME		m_tTC;		/* timing constants (13.08.2003) */
	tagMAC_MIB		m_tMIB;		/* MIB - management information base */
	tagMAC_MIBCNSTS m_tMCNST;	/* MIB constants (30.07.2003) */
	tagPLMECH_PHY	m_tPHY;		/* PHY - physical parameters */
	/* ------------------------------------------------------------------- */
	tagCLOGR		m_tLOGR;	/* MAC logger (12.09.2003) */
	tagCLOGR		m_tLOGR_IO;	/* MAC I/O logger (12.09.2003)*/
	tagCLOGR		m_tLOGR_ST;	/* MAC STATE logger (12.09.2003)*/
	/* ------------------------------------------------------------------- */
	} tagLMAC;
/* ======================================================================= */
/* "class" functions */
/* ======================================================================= */
void MAC		(tagLMAC* pMAC, tagMAC_ST* pMS);			/* constructor (05.09.2003) */
void MAC_		(tagLMAC* pMAC, tagMAC_ST* pMS);			/* destructor (24.09.2003) */
int  MACIsInited(tagLMAC* pMAC);							/* (24.09.2003) */
void MACSanity	(tagLMAC* pMAC, char* sFuncName);			/* MAC Sanity (10.09.2003) */
/* ======================================================================= */
int  MACInit	(tagLMAC* pMAC, tagMAC_ST* pMS, double dDataRate,
				 char sLogger[3][256], char* sCFGFile,
				 int (*pLogOutputFunc) (char*, char*));		/* inits MAC structure (10.09.2003) */
int  MACInitST	(tagLMAC* pMAC, tagMAC_ST* pMS);						/* inits MAC state structure (03.09.2003) */
int	 MACLoadCFG	(tagLMAC* pMAC, tagMAC_MIB* pMIB, char* sCFGFile);		/* load MAC CFG from given file (10.09.2003) */
/* ======================================================================= */
/* debug functions */
/* ======================================================================= */
int	 MACCopySt	(tagLMAC* pMAC, tagMAC_ST* pDest, tagMAC_ST* pSrc);	/* makes a copy of a MAC state (30.07.2003) */
char* MACDumpST (tagLMAC* pMAC, tagMAC_ST* pMS, char* sBuf);		/* makes dump of current state (13.08.2003) */
/* ======================================================================= */
/* private functions */
/* ======================================================================= */
tagPKT* MACGetPktByID	(tagLMAC* pMAC, tagMAC_ST* pST, int nID);
int		MACFrameTxTimeL	(tagLMAC* pMAC, int nBits);					/* time in us to transmit nBits at the lowest rate (29.09.2003) */
int		MACFrameTxTime	(tagLMAC* pMAC, int nBits, double dRate);	/* time in us to transmit nBits at the given rate (29.09.2003) */
int		MACDataTxTimeL	(tagLMAC* pMAC, int nBits);					/* time in us to transmit nBits at the lowest rate (28.07.2003) */
int		MACDataTxTime	(tagLMAC* pMAC, int nBits, double dRate);	/* time in us to transmit nBits at the given rate (28.07.2003) */
/* ======================================================================= */
/* MAC public functions */
/* ======================================================================= */
int MACCalcDurFrag	(tagLMAC* pMAC, tagMAC_ST* pST, int nCurFrag, int nFragSize, int nFrags, int nDataSize);	/* calculates value of DURATION filed of given fragment (07.08.2003) */
int MACCalcNumOfFrags(tagLMAC* pMAC, tagMAC_ST* pST, int nFragSize);
int MACClearBuf		(tagLMAC* pMAC, tagMAC_ST* pST, int nBufID);
int MACFillL3Buf	(tagLMAC* pMAC, tagMAC_ST* pST, tagEMULCFG* pCFG);
int MACFillPhyCtrlBuf(tagLMAC*pMAC, tagMAC_ST* pST, tagLPC_ST* pLPC, tagEMULCFG* pCFG, int nBufID);
int MACFillTxBuf	(tagLMAC* pMAC, tagMAC_ST* pST, tagEMULCFG* pCFG, tagMACADDR *pDA, tagFLATARRAY* pArray);
int MACFormRTS		(tagLMAC* pMAC, tagMAC_ST* pST, tagMACADDR tRA, int nDurPktTxFr);	/* (11.08.2003) */
int MACFormCTS		(tagLMAC* pMAC, tagMAC_ST* pST, tagMACADDR tRA, int nDurPktRTS);	/* (11.08.2003) */
int MACFormACK		(tagLMAC* pMAC, tagMAC_ST* pST, tagMACADDR tRA, int nDurPktRx, int bMoreFrags);	/* (08.08.2003) */
int MACGetAddrType	(tagLMAC* pMAC, tagMAC_ST* pST, tagMACADDR tAddr);
int MACGetDataInRx	(tagLMAC* pMAC, tagMAC_ST* pST, tagFLDPSDU* pPSDU);
int MACGetNewFr		(tagLMAC* pMAC, tagMAC_ST* pST, int nFrag, int nFragSize);
int MACGetPpduHdrTxTime(tagLMAC* pMAC);				/* (29.09.2003) */
int MACCWInc		(tagLMAC* pMAC, tagMAC_ST* pST); /* 9.2.4. Random BO proc. */
int MACCWReset		(tagLMAC* pMAC, tagMAC_ST* pST); /* */
int MACGetRandNumOfSlots	(tagLMAC* pMAC, tagMAC_ST* pST);
int MACIsDupl		(tagLMAC* pMAC, tagMAC_ST* pST, tagMACADDR tSA, int nSeqNo, int nFragNo);		/* (12.08.2003) */
int MACIsPsduCrcOk	(tagLMAC* pMAC, tagFLDPSDU* pPSDU);
int MACRecvPSDU		(tagLMAC* pMAC, tagLPC_ST* pLPC, tagEMULCFG* pCFG, tagFLDPSDU* pPSDU);
int MACResetSLC		(tagLMAC* pMAC, tagMAC_ST* pST, int nSize);
int MACUpdDuplCache	(tagLMAC* pMAC, tagMAC_ST* pST, tagMACADDR tSA, int nSeqNo, int nFragNo);	/* (12.08.2003) */
/* ======================================================================= */
#endif /* __PROCMAC__ */
/* ======================================================================= */
