/* =======================================================================
// Packet Processing ver. 1.0
// functions library to process MAC 802.11 packets
// -----------------------------------------------------------------------
// MD: 25.07.2003
// LE: 28.07.2003
// -----------------------------------------------------------------------
// Compiler: MS VC 6.0
// =======================================================================
// procpkt.h: implementation of service functions (header)
// ======================================================================= */
#ifndef __PKTPROC__
#define __PKTPROC__
/* ======================================================================= */
/* definitions */
/* ======================================================================= */
#include "procpkt_exp.h"
/* ======================================================================= */
typedef unsigned char	u_char;
typedef unsigned int	u_int32_t;
typedef unsigned short	u_int16_t;
/* ======================================================================= */
/* constants */
/* ======================================================================= */
/* address field, for INTERNAL use only (05.08.2003) */
/* ----------------------------------------------------------------------- */
#define PKT_ADR_DA		(0)		/* used in DATA packet*/
#define PKT_ADR_SA		(1)		/* used in DATA packet*/
#define PKT_ADR_BSSID	(2)		/* used in DATA packet*/
#define PKT_ADR_RA		(3)		/* used in RTS, CTS, ACK packet*/
#define PKT_ADR_TA		(4)		/* used in RTS packet*/
/* ======================================================================= */
#define PKT_MOREFRAGS	(1)
#define PKT_NOFRAGS		(0)
#define PKT_RETRY		(1)
#define PKT_NORETRY		(0)
#define PKT_HAVEDATA	(1)
#define PKT_HAVENODATA	(0)
#define PKT_VALID		(1)
#define PKT_NOTVALID	(0)
/* ======================================================================= */
/* packet structs */
/* ======================================================================= */
/* 7.1.3.1. FCS field */
/* ----------------------------------------------------------------------- */
/*
typedef struct {
	u_char	fc_subtype		: 4;
	u_char	fc_type			: 2;
	u_char	fc_protocol_version	: 2;

	u_char	fc_order		: 1;
	u_char	fc_wep			: 1;
	u_char	fc_more_data	: 1;
	u_char	fc_pwr_mgt		: 1;
	u_char	fc_retry		: 1;
	u_char	fc_more_frag	: 1;
	u_char	fc_from_ds		: 1;
	u_char	fc_to_ds		: 1;
	} tagFC;
	//*/
typedef struct {
	u_int16_t	fc_subtype		: 4;
	u_int16_t	fc_type			: 2;
	u_int16_t	fc_protocol_version	: 2;

	u_int16_t	fc_order		: 1;
	u_int16_t	fc_wep			: 1;
	u_int16_t	fc_more_data	: 1;
	u_int16_t	fc_pwr_mgt		: 1;
	u_int16_t	fc_retry		: 1;
	u_int16_t	fc_more_frag	: 1;
	u_int16_t	fc_from_ds		: 1;
	u_int16_t	fc_to_ds		: 1;
	} tagFC;
/* ----------------------------------------------------------------------- */
/* 7.2.1.1. RTS frame */
/* ----------------------------------------------------------------------- */
typedef struct {
	tagFC		fc;					/* 2 octets */
	u_int16_t	duration;			/* 2 octets */
	tagMACADDR	ra;					/* 6 octets */
	tagMACADDR	ta;					/* 6 octets */
	u_int32_t	fcs;				/* 4 octets */
	} tagPKTRTS;
/* ----------------------------------------------------------------------- */
/* 7.2.1.2. CTS frame */
/* ----------------------------------------------------------------------- */
typedef struct {
	tagFC		fc;					/* 2 octets */
	u_int16_t	duration;			/* 2 octets */
	tagMACADDR	ra;					/* 6 octets */
	u_int32_t	fcs;				/* 4 octets */
	} tagPKTCTS;
/* ----------------------------------------------------------------------- */
/* 7.2.1.3. ACK frame */
/* ----------------------------------------------------------------------- */
typedef struct {
	tagFC		fc;					/* 2 octets */
	u_int16_t	duration;			/* 2 octets */
	tagMACADDR	ra;					/* 6 octets */
	u_int32_t	fcs;				/* 4 octets */
	} tagPKTACK;
/* ----------------------------------------------------------------------- */
/* 7.2.2. DATA frame (flags ToDS and FromDS are not set) */
/* ----------------------------------------------------------------------- */
typedef struct {
	tagFC		fc;						/* 2 octets */
	u_int16_t	duration;				/* duration/id 2 octets */
	tagMACADDR	da;						/* adr1 */
	tagMACADDR	sa;						/* adr2 */
	tagMACADDR	bssid;					/* adr3 */
	u_int16_t	scontrol;				/* 2 octets */
	} tagHDRPKTDATA;
/* ======================================================================= */
/* Definitions */
/* ======================================================================= */
#define PLCP_HDR_LEN(_M_)	\
	((_M_->PHY.m_aPreambleLength >> 3)+(_M_->PHY.m_aPLCPHeaderLength >> 3))
#define ETHER_HDR_LEN11				\
	(PLCP_HDR_LEN + sizeof(tagDATA) + ETHER_FCS_LEN)
#define ETHER_RTS_LEN				\
	(PLCP_HDR_LEN +	sizeof(tagRTS))
#define ETHER_CTS_LEN				\
	(PLCP_HDR_LEN + sizeof(tagCTS))
#define ETHER_ACK_LEN(_M_)			\
	(PLCP_HDR_LEN + sizeof(tagACK))
/* ======================================================================= */
/* SERVICE - Packet functions (need only for the implementation) */
/* ======================================================================= */
unsigned int	PktCalcCRC32(void* pBuf, int nSize);	/* calculates CRC of a buffer (04.08.2003) */
int   PktCalcFragRealSize(int nFragNum, int nFragSize, int nDataSize);	/* calculates fragment real size (07.08.2003) */
char* PktConvAddrToStr(tagMACADDR tAddr, char* sBuf);	/* converts address to a text string (31.07.2003) */
char* PktConvAddrToStr2(tagMACADDR tAddr, char* sBuf);	/* converts address to a text string w/o spaces (13.08.2003) */
tagMACADDR PktConvStrToAddr(char* sBuf, tagMACADDR*pAddr);	/* converts text string to a MAC address (06.08.2003) */
int   PktClear(tagPKT* pPkt);				/* clears packet & releases memory */
int   PktCopy(tagPKT* pDest, tagPKT* pSrc);	/* make a copy of a packet */
int   PktDump(tagPKT* pPkt, char* sBuf);	/* prints packet dump into a given text buffer */
int   PktDumpErr(tagPKT* pPkt);				/* prints packet dump to stderr */
char* PktGetTypeStr(int nType);				/* returns text string w. type of the packet (01.08.2003) */
char* PktGetSubTypeStr(int nType, int nSType);				/* returns text string w. subtype of the packet (01.08.2003) */
int	  PktIsAddrInUse(int nType, int nSType, int nAddrID);	/* returns true, if address field is used in given type of the packet (05.08.2003) */
void  PktSanity(tagPKT* pPkt, char* sFuncName);	/* packet sanity (01.08.2003) */
/* ======================================================================= */
/* creates a packet (01.08.2003) */
/* ======================================================================= */
int  PktCreate(tagPKT*pPkt,
			  tagMACADDR*pDA, tagMACADDR*pSA, tagMACADDR*pBSSID, 
			  tagMACADDR*pRA, tagMACADDR*pTA,
	int ptype, 	int psubtype, 
	int psize,
	int nSeqNo,
	int nFragNo,
	int durat,
	int nCRC32,
	int retry,
	int valid,
	int m_bMoreFrags,
	int m_bHaveData,
	char* m_pData);
/* ======================================================================= */
/* Packet functions */
/* ======================================================================= */	 
int  PktCalcNumOfFrags(tagPKT*pPkt,int nFragSize);		/* returns number of fragments in given data packet(21.07.2003) */
int  PktCalcRealSize(tagPKT*pPkt);						/* returns size of real pkt in bytes (04.08.2003) */
tagMACADDR PktGetAddrByID(tagPKT* pPkt, int nAddrID);	/* get desired address field of a packed by given ID (28.07.2003) */
int  PktGetTxBits(int nTxTime, double dDataRate);		/* converts TxTime -> Bits (07.08.2003) */
int	 PktGetDataTxTime(int nBits, double dDataRate);		/* time to TX data only (28.07.2003) */
int	 PktGetFrameTxTime(int nBits, double dDataRate);	/* time to TX premble, header and data (29.09.2003) */
void*PktFormRealPkt(tagPKT*pPkt, int nSize);			/* forms a real packet from given buffer (04.08.2003) */
int  PktFormRTS(tagPKT* pPkt, tagMACADDR tRA, tagMACADDR tTA, int nDuration);	/* forms RTS frame for given station (23.07.2003) */
int  PktFormCTS(tagPKT* pPkt, tagMACADDR tRA, int nDuration);	/* forms CTS frame for given station (16.07.2003) */
int  PktFormACK(tagPKT* pPkt, tagMACADDR tRA, int nDuration);	/* forms ACK frame for given station (16.07.2003) */
int  PktGetFrNum(tagPKT* pPkt,
				 tagPKT* pPktFr,
				 int nFragNum,
				 int nFragSize,
				 int nFrags, 
				 int nDuration);					/* returns fragment w. given number (18.07.2003) */
int PktPacketize(tagPKT* pPkt, void* pData, int nSizeBytes, double dDataRate);	/* creates a packet from gived data array (07.08.2003) */
/* ======================================================================= */
#endif /* __PKTPROC__ */
/* ======================================================================= */
