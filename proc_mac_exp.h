/* =======================================================================
// MAC procedures (exported to SDL) ver. 1.0
// structs and constants definition
// -----------------------------------------------------------------------
// MD: 30.07.2003
// LE: 08.09.2003
// -----------------------------------------------------------------------
// Compiler: MS VC 6.0
// =======================================================================
// procmac_exp.h: implementation of service functions (header)
// ======================================================================= */
#ifndef __PROCMAC_EXP__
#define __PROCMAC_EXP__
/* ======================================================================= */
#include "procpkt_exp.h"	/* tagPKT */
/* ======================================================================= */
//#include "c_vector_exp.h"	/* tagDATABUF, instead of tagMACRXBUF (02.09.2003) */
/* ======================================================================= */
/* CONSTANTS */
/* ======================================================================= */
#define MAC_DUPLCSIZE	(16)	/* size of the dupclicate cache */
/* ======================================================================= */
/* buffer selection (25.07.2003) */
/* ======================================================================= */
#define bCTRL	(0)		/* buffer CTRL */
#define bRTS	(1)		/* buffer RTS */
#define bTx		(2)		/* buffer Tx */
#define bRx		(3)		/* buffer Rx */
#define bTxFr	(4)		/* buffer TxFr */
/* ======================================================================= */
/* data structure (10.09.2003) */
/* ======================================================================= */
typedef struct
	{
	int nSize;
	unsigned char* pData;
	} tagFLATARRAY;
/* ======================================================================= */
/* duplicate cache (12.08.2003) */
/* ======================================================================= */
typedef struct {
	int nAccessNo;			/* treated as a Time Stamp */
	tagMACADDR tAddr2;		/* SA, or Address 2 field */
	int nSeqNo;				/* sequence # */
	int nFragNo;			/* fragment # */
	} tagMACDUPLC;
/* ======================================================================= */
/* MIB COUNTERS (30.07.2003) */
/* ======================================================================= */
typedef struct {
	/* counters table */
	int	TransmittedFragmentCount;
	int	MulticastTransmittedFrameCount;
	int	FailedCount;
	int	RetryCount;
	int	MultipleRetryCount;
	int	RTSSuccessCount;
	int	RTSFailureCount;
	int	ACKFailureCount;
	int	ReceivedFragmentCount;
	int	MulticastReceivedFrameCount;
	int	FCSErrorCount;
	int	FrameDuplicateCount;
	} tagMAC_MIBCNTRS;
/* ======================================================================= */
/* state of the MAC library (23.07.2003) */
/* ======================================================================= */
typedef struct
	{
	/* ------------------------------------------------------------------- */
	/* variables */
	int bVCFree;			/* flag "Virtual Carrier is free" */
	int nCW;				/* contention window */
	int nCurSeqNo;			/* seq. no. field */
	int nSSRC;				/* Short Retry counter (25.07.2003) */
	int nSLRC;				/* Long Retry counter (25.07.2003) */
	/* ------------------------------------------------------------------- */
	/* process specific variables */
	int nFrags;				/* no. of packets in the Tx packet (>=1) */
	int nCurFrag;			/* no. of the current packet (<nFrags) */
	/* ------------------------------------------------------------------- */
	/* physical medium settings */
	tagMAC_MIBCNTRS	MIBC;	/* MIB counters (30.07.2003) */
	/* ------------------------------------------------------------------- */
	/* data buffers */
	tagPKT pktCTRL;			/* ACK/CTS */
	tagPKT pktRTS;			/* RTS */
	tagPKT pktTx;			/* Tx */
	tagPKT pktRx;			/* Rx */
	tagPKT pktTxFr;			/* Tx Fragment */
	/* ------------------------------------------------------------------- */
	/* cache (12.08.2003) */
	int m_nCacheAccessNo;				/* counter of cache accesses */
	int m_nCacheSize;					/* # of used records in the cache */
	tagMACDUPLC m_pDC[MAC_DUPLCSIZE];	/* duplicate cache */
	} tagMAC_ST;
/* ======================================================================= */
#endif /* __PROCMAC_EXP__ */
/* ======================================================================= */
