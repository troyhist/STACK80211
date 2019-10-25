/* =======================================================================
// Packet Processing ver. 1.0
// functions library to process MAC 802.11 packets
// -----------------------------------------------------------------------
// MD: 25.07.2003
// LE: 25.07.2003
// -----------------------------------------------------------------------
// Compiler: MS VC 6.0
// =======================================================================
// procpkt.h: implementation of service functions (header)
// ======================================================================= */
#include <stdlib.h>			/* rand, malloc */
#include <string.h>			/* free */
#include <stdio.h>			/* sprintf */
/* ----------------------------------------------------------------------- */
#include "procpkt.h"
/* ----------------------------------------------------------------------- */
void InstanceErrExit(char* sFuncName, char* sErrFormat, ...);	/* error handler */
/* ======================================================================= */
/* CRC32 table */
/* SRC: http://www.cs.washington.edu/homes/suciu/XMLTK/xmill/www/XMILL/html/crc32_8c-source.html */
/* ======================================================================= */
const unsigned long crc_table[256] = {
0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
0x2d02ef8dL
};
/* ========================================================================= */
#define DO1(buf) crc = crc_table[((int)crc ^ (*buf++)) & 0xff] ^ (crc >> 8);
#define DO2(buf)  DO1(buf); DO1(buf);
#define DO4(buf)  DO2(buf); DO2(buf);
#define DO8(buf)  DO4(buf); DO4(buf);
/* ========================================================================= */
unsigned long crc32(unsigned long crc, const char* buf, unsigned int len)
{
if (buf == NULL) return 0L;
crc = crc ^ 0xffffffffL;
while (len >= 8)
	{
	DO8(buf);
	len -= 8;
	}
if (len) 
	do	{
		DO1(buf);
		} while (--len);
return crc ^ 0xffffffffL;
}
/* ======================================================================= */
/* packets types (05.08.2003) */
/* ======================================================================= */
char* glPKTP_T[4]= { "MGM", "CTRL", "DATA", "RES" };
/* ======================================================================= */
/* packets subtypes (05.08.2003) */
/* ======================================================================= */
/* Management */
/* ----------------------------------------------------------------------- */
char* glPKTP_ST[3][16]=
	{{
	"Association request",		"Association response",
	"Reassociation request",	"Reassociation response",
	"Probe request",			"Probe response",
	"-", "-",
	"Beacon",	"ATIM",			"Disassociation",
	"Authentication",			"Deauthentication",
	"-", "-", "-" },
/* ----------------------------------------------------------------------- */
/* Control */
/* ----------------------------------------------------------------------- */
	{
	"-",		"-",		"-",	"-",	"-",
	"-",		"-",		"-",	"-",	"-",
	"PS-Poll",	"RTS",		"CTS",
	"ACK",		"CF-End",	"CF-End + CF-Ack" },
/* ----------------------------------------------------------------------- */
/* Data */
/* ----------------------------------------------------------------------- */
	{
	"DATA",		"DATA + CF-Ack",	"DATA + CF-Poll",
	"DATA + CF-Ack + CF-Poll",		"no data",
	"CF-Ack (no data)",				"CF-Poll (no data)",
	"CF-Ack + CF-Poll (no data)",	
	"-",	"-",	"-",	"-",	"-",
	"-",	"-",	"-"}};
/* ======================================================================= */
/* intenal types (12.08.2003) */
/* ======================================================================= */
#define bFLD_SIZE		100
#define bFLD_SEQCTRL	101
/* ======================================================================= */
char glPPTB[256];			/* ProcPktTempBuf - PPTB */
/* ======================================================================= */
char* prnYN(int bValue) { return(bValue?"YES":"NO"); }
/* ======================================================================= */
/* SERVICE - Packet functions (need only for the implementation) */
/* ======================================================================= */
unsigned int PktCalcCRC32(void* pBuf, int nSize)
{
/* calculates CRC of a buffer (04.08.2003) */
return(crc32(0, pBuf, nSize));
}
/* ======================================================================= */
int PktCalcFragRealSize(int nFragNum, int nFragSize, int nDataSize)
{
/* calculates fragment real size (07.08.2003) */
int nOffset;
int nRealFragSize;
nOffset=nFragNum*nFragSize;
nRealFragSize=nFragSize;
if (nOffset+nFragSize>nDataSize) nRealFragSize=nDataSize-nOffset;
return(nRealFragSize);
}
/* ======================================================================= */
char* PktConvAddrToStr(tagMACADDR tAddr, char* sBuf)
	{
/* -----------------------------------------------------------------------
// Description: converts address to a text string
//   Parametrs: ...
//      Return: ptr - ok, NULL - error
// -----------------------------------------------------------------------
//   MD: 31.07.2003
//   LE: 31.07.2003
// TODO: none
// ----------------------------------------------------------------------- */
if(sBuf==NULL) InstanceErrExit("PktConvAddrToStr(..)", "sBuf is NULL");
/* ----------------------------------------------------------------------- */
sprintf(sBuf, "%02x %02x %02x %02x %02x %02x",
		(int)(tAddr.ab[5]), (int)(tAddr.ab[4]), (int)(tAddr.ab[3]),
		(int)(tAddr.ab[2]), (int)(tAddr.ab[1]), (int)(tAddr.ab[0]));
/* ----------------------------------------------------------------------- */
return(sBuf);
	}
/* ======================================================================= */
char* PktConvAddrToStr2(tagMACADDR tAddr, char* sBuf)
	{
/* -----------------------------------------------------------------------
// Description: converts address to a text string w/o spaces
//   Parametrs: ...
//      Return: ptr - ok, NULL - error
// -----------------------------------------------------------------------
//   MD: 13.08.2003
//   LE: 13.08.2003
// TODO: none
// ----------------------------------------------------------------------- */
if(sBuf==NULL) InstanceErrExit("PktConvAddrToStr2(..)", "sBuf is NULL");
/* ----------------------------------------------------------------------- */
sprintf(sBuf, "%02x%02x%02x%02x%02x%02x",
		(int)(tAddr.ab[5]), (int)(tAddr.ab[4]), (int)(tAddr.ab[3]),
		(int)(tAddr.ab[2]), (int)(tAddr.ab[1]), (int)(tAddr.ab[0]));
/* ----------------------------------------------------------------------- */
return(sBuf);
	}
/* ======================================================================= */
tagMACADDR PktConvStrToAddr(char* sBuf, tagMACADDR* pAddr)
	{
/* -----------------------------------------------------------------------
// Description: converts text string to a MAC address
//   Parametrs: ...
//      Return: ptr - ok, NULL - error
// -----------------------------------------------------------------------
//   MD: 06.08.2003
//   LE: 06.08.2003
// TODO: test it
// ----------------------------------------------------------------------- */
int nAddr[6];
/* ----------------------------------------------------------------------- */
if(sBuf==NULL) InstanceErrExit("PktConvStrToAddr(..)", "sBuf is NULL");
if(pAddr==NULL) InstanceErrExit("PktConvStrToAddr(..)", "pAddr is NULL");
/* ----------------------------------------------------------------------- */
sscanf(sBuf, "%d %d %d %d %d %d",
	   &nAddr[5], &nAddr[4], &nAddr[3],
	   &nAddr[2], &nAddr[1], &nAddr[0]);
pAddr->ab[5]=(unsigned char)nAddr[5];	pAddr->ab[4]=(unsigned char)nAddr[4];
pAddr->ab[3]=(unsigned char)nAddr[3];	pAddr->ab[2]=(unsigned char)nAddr[2];
pAddr->ab[1]=(unsigned char)nAddr[1];	pAddr->ab[0]=(unsigned char)nAddr[0];
return(*pAddr);
	}
/* ======================================================================= */
int PktClear(tagPKT* pPkt)
{
/* -----------------------------------------------------------------------
// Description: clears packet & releases memory
//   Parametrs: ...
//      Return: 0 - ok, (-1) - error
// -----------------------------------------------------------------------
//   MD: 23.07.2003
//   LE: 23.07.2003
// TODO: test it
// ----------------------------------------------------------------------- */
// if(pPkt==NULL) InstanceErrExit("PktClear(..)", "pPkt is NULL");
PktSanity(pPkt, "PktClear(..)");
/* ----------------------------------------------------------------------- */
if (pPkt->m_bHaveData)
	{
	if(pPkt->m_pData==NULL)
		InstanceErrExit("PktClear(..)", "pData == NULL, but bHaveData == true");
	free(pPkt->m_pData);
	pPkt->m_pData=0;
	}
/* ----------------------------------------------------------------------- */
pPkt->bValid=0;
/* ----------------------------------------------------------------------- */
return(0);
}
/* ======================================================================= */
int PktCopy(tagPKT* pDest, tagPKT* pSrc)
{
/* -----------------------------------------------------------------------
// Description: make a copy of a packet
//   Parametrs: ...
//      Return: 0 - ok, (-1) - error
// -----------------------------------------------------------------------
//   MD: 23.07.2003
//   LE: 08.08.2003
// TODO: test it
// ----------------------------------------------------------------------- */
if(pSrc==NULL) InstanceErrExit("PktCopy(...)", "pSrc is NULL");
if(pDest==NULL) InstanceErrExit("PktCopy(...)", "pDest is NULL");
if(pSrc->bValid && pSrc->m_bHaveData && pSrc->nSize==0) InstanceErrExit("PktCopy(...)", "pSrc is invalid");
if(pDest->bValid && pDest->m_bHaveData) InstanceErrExit("PktCopy(...)", "pDest is not empty");
/* ----------------------------------------------------------------------- */
pDest->bValid	=pSrc->bValid;	if (0==pSrc->bValid) return(0);
pDest->da		=pSrc->da;
pDest->sa		=pSrc->sa;
pDest->bssid	=pSrc->bssid;
pDest->ra		=pSrc->ra;
pDest->ta		=pSrc->ta;
pDest->nType	=pSrc->nType;
pDest->nSubtype	=pSrc->nSubtype;
pDest->nSize	=pSrc->nSize;
pDest->nSeqNo	=pSrc->nSeqNo;
pDest->nFragNo	=pSrc->nFragNo;
pDest->dDurat	=pSrc->dDurat;
pDest->nCRC32	=pSrc->nCRC32;
pDest->bRetry	=pSrc->bRetry;
pDest->bMF		=pSrc->bMF;	/* (08.08.2003) */
pDest->m_bHaveData	=pSrc->m_bHaveData;
pDest->m_pData	=NULL;
/* ----------------------------------------------------------------------- */
if (pDest->m_bHaveData)		/* (08.08.2003) */
	{
	pDest->m_pData=malloc(pDest->nSize);
	memcpy(pDest->m_pData, pSrc->m_pData, pDest->nSize);
	}
/* ----------------------------------------------------------------------- */
return(0);
}
/* ======================================================================= */
int PktDump(tagPKT* pPkt, char* sBuf)
{
/* -----------------------------------------------------------------------
// Description: prints packet dump into a given text buffer
//   Parametrs: ...
//      Return: 0 - ok, (-1) - error
// -----------------------------------------------------------------------
//   MD: 23.07.2003
//   LE: 12.08.2003
// TODO: test it
// ----------------------------------------------------------------------- */
int nT, nST;
char pAddrs[5][32];		/* text string w. addresses */
char pPLSize[128];
char pSeqCtrl[128];
char sNotApp[]="-- -- -- -- -- --";
char sNA[]="--";
/* ----------------------------------------------------------------------- */
/*PktSanity(pPkt, "PktDump(..)");*/
if(sBuf==NULL) InstanceErrExit("PktDump(..)", "sBuf is NULL");
/* ----------------------------------------------------------------------- */
nT=pPkt->nType;	nST=pPkt->nSubtype;
sprintf(pAddrs[0], "%s", PktIsAddrInUse(nT,nST,bDA)?PktConvAddrToStr(pPkt->da,glPPTB):sNotApp);
sprintf(pAddrs[1], "%s", PktIsAddrInUse(nT,nST,bSA)?PktConvAddrToStr(pPkt->sa,glPPTB):sNotApp);
sprintf(pAddrs[2], "%s", PktIsAddrInUse(nT,nST,bBSSID)?PktConvAddrToStr(pPkt->bssid,glPPTB):sNotApp);
sprintf(pAddrs[3], "%s", PktIsAddrInUse(nT,nST,bRA)?PktConvAddrToStr(pPkt->ra,glPPTB):sNotApp);
sprintf(pAddrs[4], "%s", PktIsAddrInUse(nT,nST,bTA)?PktConvAddrToStr(pPkt->ta,glPPTB):sNotApp);
/* ----------------------------------------------------------------------- */
sprintf(pPLSize,	"%d bytes, %d bits", pPkt->nSize, pPkt->nSize*8);
sprintf(pSeqCtrl,	"%d (nFragNo = %d)\t(7.1.3.4. seq. control)", pPkt->nSeqNo,	pPkt->nFragNo);
/* ----------------------------------------------------------------------- */
sprintf(sBuf,
		"\n"
		"packet:\n"
		"bValid   = %s (data = %s, bRetry = %d, bMF = %s)\n"
		"da       = %s\t(7.1.3.3.4. dest. addr. of the MAC entity)\n"
		"sa       = %s\t(7.1.3.3.5. address of the MAC entity)\n"
		"bssid    = %s\t(7.1.3.3.3. address of the group)\n"
		"ra       = %s\t(7.1.3.3.6. dest. addr. of the STA(s))\n"
		"ta       = %s\t(7.1.3.3.7. MAC addr. of the transmitting STA)\n"
		"nType    = %d\t\t\t(%s)\n"
		"nSubtype = %d\t\t\t(%s)\n"
		"nSize    = %s\n"
		"nSeqNo   = %s\n"
		"dDurat   = %5.2f\t\t(duration in us)\n"
		"nCRC32   = 0x%08x\t\t(7.1.3.6. fcs)\n\n",
		prnYN(pPkt->bValid), 
		pPkt->bValid?prnYN(pPkt->m_bHaveData):"NO",	
		pPkt->bValid?pPkt->bRetry:-1,
		pPkt->bValid?prnYN(pPkt->bMF):"NO",
		pPkt->bValid?pAddrs[0]:sNotApp,
		pPkt->bValid?pAddrs[1]:sNotApp,
		pPkt->bValid?pAddrs[2]:sNotApp,
		pPkt->bValid?pAddrs[3]:sNotApp,
		pPkt->bValid?pAddrs[4]:sNotApp,
		pPkt->bValid?pPkt->nType:-1,	pPkt->bValid?PktGetTypeStr(pPkt->nType):"<error>",
		pPkt->bValid?pPkt->nSubtype:-1,	pPkt->bValid?PktGetSubTypeStr(pPkt->nType, pPkt->nSubtype):"<error>",
		pPkt->bValid?PktIsAddrInUse(nT,nST,bFLD_SIZE)?pPLSize:sNA:sNA,
		pPkt->bValid?PktIsAddrInUse(nT,nST,bFLD_SEQCTRL)?pSeqCtrl:sNA:sNA,
		pPkt->bValid?pPkt->dDurat:-1,
		pPkt->bValid?pPkt->nCRC32:-1);
/* ----------------------------------------------------------------------- */
return(0);
}
/* ======================================================================= */
int PktDumpErr(tagPKT* pPkt)
{
/* prints packet dump to stderr */
char sBuf[1024];
PktDump(pPkt, sBuf);
fprintf(stderr, sBuf);
return(0);
}
/* ======================================================================= */
char* PktGetTypeStr(int nType)
	{
	/* 01.08.2003, 05.08.2003 */
	if (nType<0 || nType>4) return("INVALID");
	return(glPKTP_T[nType]);
	};
/* ======================================================================= */
char* PktGetSubTypeStr(int nType, int nSType)
	{
	/* 01.08.2003, 05.08.2003 */
	if (nType==3) return("RES");
	if (nType<0 || nType>4) return("INVALID");
	if (nSType<0 || nSType>15) return("INVALID");
	return(glPKTP_ST[nType][nSType]);
	};
/* ======================================================================= */
int	PktIsAddrInUse(int nType, int nSType, int nAddrID)
{
/* returns true, if address field is used in given type of the packet (05.08.2003) */
switch(nAddrID)
	{
	case bDA:	if (nType==TYP_DATA && nSType==STYP_DATA) return(1); break;
	case bSA:	if (nType==TYP_DATA && nSType==STYP_DATA) return(1); break;
	case bBSSID:if (nType==TYP_DATA && nSType==STYP_DATA) return(1); break;
	case bRA:	if (nType==TYP_CTRL && (nSType==STYP_RTS || 
					nSType==STYP_CTS || nSType==STYP_ACK)) return(1); break;
	case bTA:	if (nType==TYP_CTRL && nSType==STYP_RTS) return(1); break;
	case bFLD_SIZE:		if (nType==TYP_DATA && nSType==STYP_DATA) return(1); break;
	case bFLD_SEQCTRL:	if (nType==TYP_DATA && nSType==STYP_DATA) return(1); break;
	default:	break;
	}
return(0);
}
/* ======================================================================= */
void PktSanity(tagPKT* pPkt, char* sFuncName)
{
/* packet sanity (01.08.2003) */
if(pPkt==NULL) InstanceErrExit(sFuncName, "pPkt is NULL");
if(pPkt->bValid==0) InstanceErrExit(sFuncName, "pPkt is not valid");
}
/* ======================================================================= */
/* creates a packet (01.08.2003) */
/* ======================================================================= */
int PktCreate(tagPKT*pPkt, 
			  tagMACADDR*pDA, tagMACADDR*pSA, tagMACADDR*pBSSID, 
			  tagMACADDR*pRA, tagMACADDR*pTA,
	int ptype, 	int psubtype, int psize, int nSeqNo, int nFragNo, int durat,
	int nCRC32, int retry, int valid, int m_bMoreFrags, int m_bHaveData,
	char* m_pData)
{
/* 01.08.2003, 05.08.2003 */
tagMACADDR tNULL={0,0,0,0,0,0};
if(pPkt==NULL) InstanceErrExit("PktCreate(..)", "pPkt is NULL");
if(pPkt->bValid) InstanceErrExit("PktCreate(..)", "pPkt is ALREADY valid");
pPkt->da=pPkt->sa=pPkt->bssid=pPkt->ra=pPkt->ta=tNULL;
if (pDA) pPkt->da=*pDA;
if (pSA) pPkt->sa=*pSA;
if (pBSSID) pPkt->bssid=*pBSSID;
if (pRA) pPkt->ra=*pRA;
if (pTA) pPkt->ta=*pTA;
pPkt->nType=ptype;
pPkt->nSubtype=psubtype;
pPkt->nSize=psize;
pPkt->nSeqNo=nSeqNo;
pPkt->nFragNo=nFragNo;
pPkt->dDurat=(double)durat;
pPkt->nCRC32=nCRC32;
pPkt->bRetry=retry;
/* ------------------------------------------------------------------- */
/* service fields */
pPkt->bValid=valid;
pPkt->bMF=m_bMoreFrags;
pPkt->m_bHaveData=m_bHaveData;
pPkt->m_pData=m_pData;
return(0);
}
/* ======================================================================= */
/* Packet functions */
/* ======================================================================= */
int PktCalcNumOfFrags(tagPKT* pPkt, int nFragSize)
{
/* -----------------------------------------------------------------------
// Description: returns number of fragments in given data packet
//   Parametrs: ...
//      Return: 0 - ok, (-1) - error
// -----------------------------------------------------------------------
//   MD: 21.07.2003
//   LE: 01.08.2003
// TODO: test it
// ----------------------------------------------------------------------- */
int nSize, nFrags;
PktSanity(pPkt, "PktCalcNumOfFrags(..)");
nSize=pPkt->nSize;
nFrags=nSize/nFragSize;
if((nSize%nFragSize)!=0) nFrags++;
if(nSize==0) InstanceErrExit("PktCalcNumOfFrags(..)", "empty DATA packet, not supported");
return(nFrags);	
}
/* ======================================================================= */
int PktCalcRealSize(tagPKT*pPkt)
{
/* -----------------------------------------------------------------------
// Description: calculates size of the real packet in bytes
//   Parametrs: ...
//      Return: >=0 - ok, (-1) - error
// -----------------------------------------------------------------------
//   MD: 04.07.2003
//   LE: 04.08.2003
// TODO: test it
// ----------------------------------------------------------------------- */
int nSize;
PktSanity(pPkt, "PktCalcRealSize(..)");
switch(pPkt->nType)
	{
	case TYP_MGM: nSize=-1; break;
	case TYP_CTRL: 
		{
		switch(pPkt->nSubtype)
			{
			case STYP_RTS: nSize=sizeof(tagPKTRTS); break;
			case STYP_CTS: nSize=sizeof(tagPKTCTS); break;
			case STYP_ACK: nSize=sizeof(tagPKTACK); break;
			default: nSize=-1; break;
			}
		}
		break;
	case TYP_DATA:
		{
		switch(pPkt->nSubtype)	/* plus 4 bytes for FCS (nCRC32) */
			{
			case STYP_DATA: nSize=sizeof(tagHDRPKTDATA)+pPkt->nSize+4; break;
			default: nSize=-1; break;
			}
		}
		break;
	default: nSize=-1; break;
	}
return(nSize);
}
/* ======================================================================= */
tagMACADDR PktGetAddrByID(tagPKT* pPkt, int nAddrID)
{
/* get desired address field of a packed by given IDs (28.07.2003) */
tagMACADDR tAddr={0,0,0,0,0,0};
/* ----------------------------------------------------------------------- */
if(pPkt==NULL) InstanceErrExit("PktGetAddrByID(..)", "pPkt is NULL");
/* ----------------------------------------------------------------------- */
switch(nAddrID)
	{
	case bDA:	tAddr=pPkt->da;	break;	/* DATA: DEST address */
	case bSA:	tAddr=pPkt->sa;	break;	/* DATA: SRC address */
	case bBSSID:	tAddr=pPkt->bssid;	break;	/* DATA: SRC address */
	case bRA:	tAddr=pPkt->ra;	break;	/* CTRL: RECV address */
	case bTA:	tAddr=pPkt->ta;	break;	/* CTRL: TX address */
	default:	break;
	}
return(tAddr);
}
/* ======================================================================= */
int PktGetFrNum(tagPKT* pPkt, tagPKT* pPktFr, int nFragNum, int nFragSize, int nFrags, int nDuration)
{
/* -----------------------------------------------------------------------
// Description: returns fragment w. given number (18.07.2003)
//				!!! no checking whether nFragNum >= nFrags !!!
//   Parametrs: ...
//      Return: 0 - ok, (-1) - error
// -----------------------------------------------------------------------
//   MD: 18.07.2003
//   LE: 07.08.2003
// TODO: check condition (nOffset+nFragSize)
// ----------------------------------------------------------------------- */
char* pData;
int nRealFragSize;
/* ----------------------------------------------------------------------- */
PktSanity(pPkt, "PktGetFrNum(..)");
if(pPkt->bRetry) InstanceErrExit("PktGetFrNum(..)", "pPkt has a RETRY flag (incorrect)");
if(pPktFr==NULL) InstanceErrExit("PktGetFrNum(..)", "pPktFr is NULL");
if(pPktFr->bValid!=0) InstanceErrExit("PktGetFrNum(..)", "pPktFr is still VALID");
if(nFragNum>15) InstanceErrExit("PktGetFrNum(..)", "fragment no. has to be lower than 16 (nFragNum=%d)", nFragNum);
/* ----------------------------------------------------------------------- */
nRealFragSize=PktCalcFragRealSize(nFragNum, nFragSize, pPkt->nSize);
/* ----------------------------------------------------------------------- */
pData=malloc(nRealFragSize);
if(pData==NULL) InstanceErrExit("nRealFragSize(..)", "cann't allocate %d bytes", nRealFragSize);
memcpy(pData, pPkt->m_pData + nFragNum*nFragSize, nRealFragSize);
/* ----------------------------------------------------------------------- */
/* forms a packet */
/* ----------------------------------------------------------------------- */
PktCreate(pPktFr,	&pPkt->da,		&pPkt->sa,		&pPkt->bssid, NULL, NULL,
	pPkt->nType,	pPkt->nSubtype,	nRealFragSize,	pPkt->nSeqNo,
	nFragNum,
	nDuration,		-1,
	PKT_NORETRY, PKT_VALID,	(nFragNum+1==nFrags)?PKT_NOFRAGS:PKT_MOREFRAGS, PKT_HAVEDATA, pData);
/* ----------------------------------------------------------------------- */
return(0);
}
/* ======================================================================= */
int  PktGetTxBits(int nTxTime, double dDataRate)
{
/* converts TxTime -> Bits (07.08.2003) */
double dTxTime=(double)nTxTime;
double dBits;
int nBits;
dTxTime/=1e6;				/* converts 'us' in 'sec' */
dBits=dTxTime*dDataRate;
nBits=(int)dBits;
return(nBits);
}
/* ======================================================================= */
int	PktGetDataTxTime(int nBits, double dDataRate)
{
/* time in us to transmit nBits at the given rate (28.07.2003) */
double dBits=(double)nBits;
double dTxTime;
int nTxTime;
dTxTime=dBits/dDataRate;
nTxTime=(int)(dTxTime*1e6);
return(nTxTime);
}
/* ======================================================================= */
void*PktFormRealPkt(tagPKT*pPkt, int nSize)
{
/* -----------------------------------------------------------------------
// Description: froms a real packet from a given buffer
//   Parametrs: ...
//      Return: !NULL - ok, NULL - error
// -----------------------------------------------------------------------
//   MD: 04.07.2003
//   LE: 04.08.2003
// TODO: test it
// ----------------------------------------------------------------------- */
/*int nSize;*/				/* passed as a parametr */
unsigned int nCRC32;
char* pBuf;					/* buffer w. real packet */
tagFC fc;
tagPKTRTS tRTS;
tagPKTCTS tCTS;
tagPKTACK tACK;
tagHDRPKTDATA tDATA;
/* ----------------------------------------------------------------------- */
PktSanity(pPkt, "PktFormRealPkt(..)");
/* ----------------------------------------------------------------------- */
//fc.fc_protocol_version=MAC_ProtocolVersion;	/* = 0x00 2bits */
fc.fc_protocol_version=0;					/* = 0x00 2bits */
fc.fc_type		=pPkt->nType;
fc.fc_subtype	=pPkt->nSubtype;
fc.fc_to_ds		=0;							/* not supported */
fc.fc_from_ds	=0;							/* not supported */
fc.fc_more_frag	=pPkt->bMF;
fc.fc_retry		=pPkt->bRetry;
fc.fc_pwr_mgt	=0;							/* not supported */
fc.fc_more_data	=0;							/* not supported */
fc.fc_wep		=0;							/* not supported */
fc.fc_order		=0;							/* not supported */
/* ----------------------------------------------------------------------- */
if(nSize<=0) InstanceErrExit("PktFormRealPkt(..)", "incorrect packet size (=%d)", nSize);
pBuf=malloc(nSize);
if(pBuf==NULL) InstanceErrExit("PktFormRealPkt(..)", "cann't allocate %d bytes", nSize);
/* ----------------------------------------------------------------------- */
switch(pPkt->nType)
	{
	case TYP_MGM: break;
	case TYP_CTRL: 
		{
		switch(pPkt->nSubtype)
			{
			case STYP_RTS:
				tRTS.fc			=fc;
				tRTS.duration	=(u_int16_t)pPkt->dDurat;
				tRTS.ra			=pPkt->ra;
				tRTS.ta			=pPkt->ta;
				tRTS.fcs		=PktCalcCRC32(&tRTS, nSize-4);
				memcpy(pBuf, &tRTS, nSize);
				break;
			case STYP_CTS:
				tCTS.fc			=fc;
				tCTS.duration	=(u_int16_t)pPkt->dDurat;
				tCTS.ra			=pPkt->ra;
				tCTS.fcs		=PktCalcCRC32(&tCTS, nSize-4);
				memcpy(pBuf, &tCTS, nSize);
				break;
			case STYP_ACK:
				tACK.fc			=fc;
				tACK.duration	=(u_int16_t)pPkt->dDurat;
				tACK.ra			=pPkt->ra;
				tACK.fcs		=PktCalcCRC32(&tACK, nSize-4);
				memcpy(pBuf, &tACK, nSize);
				break;
			default: break;
			}
		break;
		}
	case TYP_DATA:
		if(pPkt->nSubtype==STYP_DATA)
			{
			tDATA.fc			=fc;
			tDATA.duration		=(u_int16_t)pPkt->dDurat;
			tDATA.da			=pPkt->da;
			tDATA.sa			=pPkt->sa;
			tDATA.bssid			=pPkt->bssid;
			tDATA.scontrol		=(pPkt->nFragNo&0x0f) | (pPkt->nSeqNo << 4); /* 7.1.3.4. */
			memcpy(pBuf, &tDATA, sizeof(tagHDRPKTDATA));
			memcpy(pBuf+sizeof(tagHDRPKTDATA), pPkt->m_pData, pPkt->nSize);
			nCRC32				=PktCalcCRC32(pBuf, nSize-4);
			memcpy(pBuf+nSize-4, &nCRC32, 4);
			}
		break;
	default: break;
	}
return(pBuf);
}
/* ======================================================================= */
int PktFormRTS(tagPKT* pPkt, tagMACADDR tRA, tagMACADDR tTA, int nDuration)
	{
	/* forms RTS frame for given station (23.07.2003, 11.08.2003) */
	return(PktCreate(pPkt, NULL, NULL, NULL, &tRA, &tTA, TYP_CTRL, STYP_RTS,
		-1, 0, 0, nDuration, -1, PKT_NORETRY, PKT_VALID, PKT_NOFRAGS, 
		PKT_HAVENODATA, NULL));
	}
/* ======================================================================= */
int  PktFormCTS(tagPKT* pPkt, tagMACADDR tRA, int nDuration)
	{
	/* forms CTS frame for given station (16.07.2003, 11.08.2003) */
	return(PktCreate(pPkt, NULL, NULL, NULL, &tRA, NULL, TYP_CTRL, STYP_CTS, 
		-1, 0, 0, nDuration, -1, PKT_NORETRY, PKT_VALID, PKT_NOFRAGS,
		PKT_HAVENODATA, NULL));
	}
/* ======================================================================= */
int PktFormACK(tagPKT* pPkt, tagMACADDR tRA, int nDuration)
	{
	/* forms ACK frame for given station (16.07.2003) */
	return(PktCreate(pPkt, NULL, NULL, NULL, &tRA, NULL, TYP_CTRL, STYP_ACK, 
		-1, 0, 0, nDuration, -1, PKT_NORETRY, PKT_VALID, PKT_NOFRAGS,
		PKT_HAVENODATA, NULL));
	}
/* ======================================================================= */
int PktPacketize(tagPKT* pPkt, void* pDataBuf, int nSizeBytes, double dDataRate)
{
/* -----------------------------------------------------------------------
// Description: creates a packet from gived data array (07.08.2003)
//   Parametrs: ...
//      Return: 0 - ok, (-1) - error
// -----------------------------------------------------------------------
//   MD: 07.08.2003
//   LE: 07.08.2003
// TODO: test it
// ----------------------------------------------------------------------- */
int nPayloadSize;
int nSeqNo, nFragNo;
unsigned int nCRC32;
char* pPayload;
tagFC* pFC;
tagPKTRTS* pRTS;
tagPKTCTS* pCTS;
tagPKTACK* pACK;
tagHDRPKTDATA* pDATA;
/* ----------------------------------------------------------------------- */
if(pDataBuf==NULL)
	return(-1);
pFC=pDataBuf;	pDATA=pDataBuf;	pRTS=pDataBuf;	pCTS=pDataBuf;	pACK=pDataBuf;
/* ----------------------------------------------------------------------- */
/* checks version */
/*if(MAC_ProtocolVersion!=pFC->fc_protocol_version) return(-1);*/
/* ----------------------------------------------------------------------- */
switch(pFC->fc_type)
	{
	case TYP_MGM:
		InstanceErrExit("PktPacketize(..)", "received a MGM packet (STYP=%d)", pFC->fc_subtype);
		break;
	case TYP_CTRL: 
		{
		switch(pFC->fc_subtype)
			{
			case STYP_RTS:
				PktCreate(pPkt, NULL, NULL, NULL, &pRTS->ra, &pRTS->ta, 
					TYP_CTRL, STYP_RTS, -1, -1, -1, 
					(int)pRTS->duration, pRTS->fcs, 
					pRTS->fc.fc_retry, PKT_VALID, pRTS->fc.fc_more_frag,
					PKT_HAVENODATA, NULL);
				break;
			case STYP_CTS:
				PktCreate(pPkt, NULL, NULL, NULL, &pCTS->ra, NULL, 
					TYP_CTRL, STYP_CTS, -1, -1, -1, 
					(int)pCTS->duration, pCTS->fcs, 
					pCTS->fc.fc_retry, PKT_VALID, pCTS->fc.fc_more_frag,
					PKT_HAVENODATA, NULL);	
				break;
			case STYP_ACK:
				PktCreate(pPkt, NULL, NULL, NULL, &pACK->ra, NULL, 
					TYP_CTRL, STYP_ACK, -1, -1, -1, 
					(int)pACK->duration, pACK->fcs, 
					pACK->fc.fc_retry, PKT_VALID, pACK->fc.fc_more_frag,
					PKT_HAVENODATA, NULL);
				break;
			default:
				InstanceErrExit("PktPacketize(..)", "received unknown CTRL packet (STYP=%d)", pFC->fc_subtype);
				break;
			}
		}
		break;
	case TYP_DATA:
		switch(pFC->fc_subtype)
			{
			case STYP_DATA:
				/* ------------------------------------------------------- */
				nPayloadSize=nSizeBytes-sizeof(tagHDRPKTDATA)-4;
				nSeqNo=pDATA->scontrol>>4;
				nFragNo=pDATA->scontrol&0x0f;
				nCRC32=*(unsigned int*)((char*)pDataBuf+nSizeBytes-4);
				/* ------------------------------------------------------- */
				pPayload=malloc(nPayloadSize);
				if(pPayload==NULL) InstanceErrExit("PktPacketize(..)", "cann't allocate %d bytes", nPayloadSize);
				memcpy(pPayload, (char*)pDataBuf+sizeof(tagHDRPKTDATA), nPayloadSize);
				/* ------------------------------------------------------- */
				PktCreate(pPkt, &pDATA->da, &pDATA->sa, &pDATA->bssid, NULL, NULL,
					TYP_DATA, STYP_DATA, nPayloadSize, nSeqNo, nFragNo,
					(int)pDATA->duration, nCRC32,
					pDATA->fc.fc_retry, PKT_VALID, pDATA->fc.fc_more_frag,
					PKT_HAVEDATA, pPayload);
				break;
			default:
				InstanceErrExit("PktPacketize(..)", "received unknown DATA packet (STYP=%d)", pFC->fc_subtype);
				break;
			}
		break;
	default:
		InstanceErrExit("PktPacketize(..)", "received totally unknown packet (TYP=%d, STYP=%d)", pFC->fc_type, pFC->fc_subtype);
		break;
	}
/* ----------------------------------------------------------------------- */
return(0);
}
/* ======================================================================= */
