/* =======================================================================
// Packet Processing (exported to SDL) ver. 1.0
// functions library to process MAC packets
// -----------------------------------------------------------------------
// MD: 16.07.2003
// LE: 30.07.2003
// -----------------------------------------------------------------------
// Compiler: MS VC 6.0
// =======================================================================
// procpkt_exp.h: implementation of service functions (header)
// ======================================================================= */
#ifndef __PKTPROC_EXP__
#define __PKTPROC_EXP__
/* ======================================================================= */
/* address types (28.07.2003) */
/* ======================================================================= */
#define MAC_FRGN 	(0)		/* foreign address */
#define MAC_ADDR	(1)		/* my address (address of the station) */
#define MAC_BRDCST	(2)		/* broadcast address */
#define MAC_MLTCST	(3)		/* multicast group address (30.07.2003) */
/* ======================================================================= */
/* 7.1.3.3. address selection (28.07.2003, 05.08.2003) */
/* ======================================================================= */
#define bDA		(0)		/* DATA: DEST address */
#define bSA		(1)		/* DATA: SRC address */
#define bBSSID	(2)		/* DATA: SRC address */
#define bRA		(3)		/* CTRL: RECV address */
#define bTA		(4)		/* CTRL: TX address */
/* ======================================================================= */
/* 7.1.3.1.2. Type and Subtype subfield */
/* ======================================================================= */
/* bits b3 and b2*/
/* ----------------------------------------------------------------------- */
#define TYP_MGM			(0x00)
#define TYP_CTRL		(0x01)
#define TYP_DATA		(0x02)
#define TYP_RES			(0x03)
/* ----------------------------------------------------------------------- */
/* bits b7...b4 */
/* ----------------------------------------------------------------------- */
#define STYP_RTS		(0x0B)
#define STYP_CTS		(0x0C)
#define STYP_ACK		(0x0D)
#define STYP_DATA		(0x00)
/* ----------------------------------------------------------------------- */
/* 7.2.1. sizes of diff. frame format (in bytes) */
/* ----------------------------------------------------------------------- */
#define RTSSize				(20)	/* 7.2.1.1. */
#define CTSSize				(14)	/* 7.2.1.2. */
#define ACKSize				(14)	/* 7.2.1.3. */
/* ======================================================================= */
/* additional (adapted from NS\ns-2.26\mac\mac.h) */
/* ======================================================================= */
#define MAC_HDR_LEN		64
#define ETHER_TYPE_LEN	(2)
#define ETHER_FCS_LEN	(4)
#define ETHER_ADDR_LEN	(6)		/* 7.1.3.3.1. addr. len 48 bits */
/* ======================================================================= */
/* 7.1.3.3.1. MAC address (28.07.2003) */
/* ======================================================================= */
typedef struct
	{
	unsigned char ab[ETHER_ADDR_LEN];
	} tagMACADDR;
/* ======================================================================= */
/* BUFFER (16.07.2003) */
/* ======================================================================= */
typedef struct{
	/* ------------------------------------------------------------------- */
	/* "real" fields */
	/* ------------------------------------------------------------------- */
	tagMACADDR da;		/* DATA: 7.1.3.3.4. DST addr */
	tagMACADDR sa;		/* DATA: 7.1.3.3.5. SRC addr */
	tagMACADDR bssid;	/* DATA: 7.1.3.3.3. BSSID addr (addr. of AP or group) */
	tagMACADDR ra;		/* CTRL: 7.1.3.3.6. Rx addr */
	tagMACADDR ta;		/* CTRL: 7.1.3.3.7. Tx addr */
	int nType;		/* packet type */
	int nSubtype;	/* packet subtype */
	int nSize;		/* size of the packet's payload */
	int nSeqNo;		/* seq. control */
	int nFragNo;	/* seq. control */
	double dDurat;	/* duration in us */
	int nCRC32;		/* CRC 32 */
	int bRetry;		/* flag "retry" */
	int bMF;		/* more fragments flag (01.08.2003) */
	/* ------------------------------------------------------------------- */
	/* service fields */
	/* ------------------------------------------------------------------- */
	int bValid;			/* flag "packet is valid" */
	int m_bHaveData;	/* flag "have data" */
	char* m_pData;		/* data in the packet */
	} tagPKT;
/* ======================================================================= */
#endif /* __PKTPROC_EXP__ */
/* ======================================================================= */
