/* =======================================================================
// procedures of PHY CTRL (exported to SDL functions) ver. 1.0
// structs and constants definition
// -----------------------------------------------------------------------
// MD: 04.07.2003
// LE: 05.09.2003
// -----------------------------------------------------------------------
// Compiler: MS VC 6.0
// =======================================================================
// procphyctrl_exp.h: implementation of service functions (header)
// ======================================================================= */
#ifndef __PROC_PHYCTRL_EXP__
#define __PROC_PHYCTRL_EXP__
/* ======================================================================= */
#define MAX_BITSEQSIZE	(216)	/* ref:  17.3.2.2 Rate-depended parametrs */
#define MAX_FLDPSDULEN	(4096)	/* ref:  17.3.2 PLPC frame format */
/* ======================================================================= */
/* PHY_RXEND error codes (12.3.5.12) */
/* ======================================================================= */
#define PHY_ERR_NONE	(0)		/* NoError */
#define PHY_ERR_FRMT	(1)		/* FormatViolation */
#define PHY_ERR_CARR	(2)		/* CarrierLost */
#define PHY_ERR_RATE	(3)		/* UnsupportedRate */
/* ======================================================================= */
/* PHY_CCA medium states (12.3.5.10) */
/* ======================================================================= */
#define PHY_MDM_IDLE	(0)		/* medium is idle */
#define PHY_MDM_BUSY	(1)		/* medium is busy */
/* ======================================================================= */
/* field PSDU (05.09.2003) */
/* ======================================================================= */
typedef struct
	{
	int m_nSize;							/* # of elements */
	unsigned char m_pData[MAX_FLDPSDULEN];	/* ptr to data */
	} tagFLDPSDU;
/* ======================================================================= */
/* SIGNAL field 17.3.4. */
/* ======================================================================= */
typedef struct
	{
	unsigned int nRate	:4;		/* encoded Data Rate */
	unsigned int nR		:1;		/* reserved */
	unsigned int nLen	:12;	/* length of PAYLOAD in bytes */
	unsigned int nParity:1;		/* parity check */
	unsigned int nTail	:6;		/* tail bits (=0) */
	} tagSIGNAL;
/* ======================================================================= */
/* BIT SEQUENCE of max 216 bits (03.09.2003) */
/* ======================================================================= */
typedef struct
	{
	int m_nSize;
	unsigned char m_pData[MAX_BITSEQSIZE];
	} tagBITSEQ;
/* ======================================================================= */
typedef tagFLDPSDU	PSDU;	/* PSDU (05.09.2003) */
typedef tagSIGNAL	HDR;	/* PPDU header (02.09.2003) */
typedef tagBITSEQ	BITSEQ;	/* decoded OFDM symbol (03.09.2003) */
/* ======================================================================= */
#endif /* __PROC_PHYCTRL_EXP__ */
/* ======================================================================= */
