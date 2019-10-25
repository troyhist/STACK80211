/* ======================================================================= */
/*
MD: 04.07.2003
LE: 14.07.2003
*/
/* ======================================================================= */
#include <stdio.h>			/* FILE */
#include <stdlib.h>			/* free */
#include <memory.h>			/* memcpy */
#include "_debug.h"			/* TRACEVECT4 (15.09.2003) */
#include "proc_phyctrl.h"
#include "c_vector.h"		/* expand & collapse data (04.09.2003) */
/* ======================================================================= */
void InstanceErrExit(char* sFuncName, char* sErrFormat, ...);	/* error handler */
/* ======================================================================= */
/* "class" functions (04.09.2003) */
/* ======================================================================= */
void PCTRL(tagLPC_ST* pLPC)
	{ /* constructor */
	pLPC->m_bValid=0;
	CIF(&pLPC->m_tCIF); }
/* ----------------------------------------------------------------------- */
void PCTRL_		(tagLPC_ST* pLPC)
	{ /* destructor (24.09.2003) */
	if(!PCIsInited(pLPC)) return;
	pLPC->m_bValid=0;
	CIF_(&pLPC->m_tCIF); }
/* ----------------------------------------------------------------------- */
int  PCInit(tagLPC_ST* pLPC)
	{ /* init */
	pLPC->m_bValid=1;
	CIFInit(&pLPC->m_tCIF);	// 22.09.2003
	return(0);
	}
/* ----------------------------------------------------------------------- */
int  PCIsInited	(tagLPC_ST* pLPC)
	{ return(pLPC->m_bValid); }
/* ----------------------------------------------------------------------- */
void PCSanity	(tagLPC_ST* pLPC, char* sFuncName)
	{
	if(NULL==pLPC)
		InstanceErrExit(sFuncName, "sanity failed, %s", "pLPC==NULL");
	if(0==pLPC->m_bValid)
		InstanceErrExit(sFuncName, "sanity failed, %s", "pLPC is NOT valid");
	}
/* ======================================================================= */
/* PHY_CTRL LAYER functions (02.09.2003) */
/* ======================================================================= */
/* TX SERVICE PART */
/* ======================================================================= */
int PCFillTxBuf	(tagLPC_ST* pLPC, tagEMULCFG* pCFG, void* pDataTx, int nSize)
	{
	/* ------------------------------------------------------------------- */
	/* fill Tx Buffer (10.09.2003)*/
	/* ------------------------------------------------------------------- */
	FILE* fp;
	/* ------------------------------------------------------------------- */
	PCSanity(pLPC, "PCFillTxBuf()");
	/* ------------------------------------------------------------------- */
	if (nSize>MAX_FLDPSDULEN)
		InstanceErrExit("PCFillTxBuf()", "packet size is too big. "
		"nSize=%d > MAX_FLDPSDULEN=%d", nSize, MAX_FLDPSDULEN);
	/* ----------------------------------------------------------------------- */
	/* writes data into LOG file */
	if ((fp=fopen(pCFG->m_sLPC[1], "a+b"))==NULL)
		InstanceErrExit("PCFillTxBuf()", "cann't open LPC output file (%s)", 
		pCFG->m_sLPC[1]);
	fwrite(pDataTx, 1, nSize, fp); fclose(fp);
	/* ----------------------------------------------------------------------- */
	pLPC->m_tPSDU.m_nSize=nSize;
	memcpy(&pLPC->m_tPSDU.m_pData, pDataTx, nSize);
	return(0);
	/* ------------------------------------------------------------------- */
	}
/* ======================================================================= */
/* TX PART */
/* ======================================================================= */
int PCConvHdrToBitSeq (tagLPC_ST* pLPC, tagBITSEQ* pBits, tagSIGNAL* pHdr)
	{
	/* ------------------------------------------------------------------- */
	/* expands SIGNAL struct into a bit sequence (04.09.2003, 08.09.2003)*/
	/* ------------------------------------------------------------------- */
	PCSanity(pLPC, "PCConvHdrToBitSeq()");
	/* ------------------------------------------------------------------- */
	pBits->m_nSize=8*sizeof(tagSIGNAL);
	if(pBits->m_nSize>MAX_BITSEQSIZE)
		InstanceErrExit("PCConvHdrToBitSeq(..)",
		"bit sequence exceeds maximal length (%d > %d)",
		pBits->m_nSize, MAX_BITSEQSIZE);
	/* ------------------------------------------------------------------- */
	VectorExpandBitsUC(pBits->m_pData, pHdr, sizeof(tagSIGNAL));
	/* ------------------------------------------------------------------- */
	return(0);
	}
/* ======================================================================= */
int PCConvPSDUToData (tagLPC_ST* pLPC, int nSymb, int nNdbps)
	{
	/* ------------------------------------------------------------------- */
	/* (04.09.2003, 08.09.2003) (ref.: 17.3.2. PLPC frame format) */
	/* ------------------------------------------------------------------- */
	int i;
	unsigned char* pExpandedPSDU;
	/* ------------------------------------------------------------------- */
	PCSanity(pLPC, "PCConvPSDUToData()");
	/* ------------------------------------------------------------------- */
	for (i=0; i<MAX_FLDDATALEN; i++) pLPC->m_tDATA.m_pData[i]=0;
	/* ------------------------------------------------------------------- */
	pLPC->m_tDATA.m_nSize=nSymb*nNdbps;
	pExpandedPSDU=(pLPC->m_tDATA.m_pData+16);
	VectorExpandBitsUC(pExpandedPSDU, pLPC->m_tPSDU.m_pData, pLPC->m_tPSDU.m_nSize);
	/* ------------------------------------------------------------------- */
	return(0);
	}
/* ======================================================================= */
int PCCreateHdr	(tagLPC_ST* pLPC, tagSIGNAL* pHdr, int nDataRate, int nLen)
	{
	/* ------------------------------------------------------------------- */
	/* (04.09.2003, 18.09.2003) ref.: 17.3.4. Signal field */
	/* ------------------------------------------------------------------- */
	int i;
	int nByteNr, nBitNr, nOnes;
	unsigned char* pHdrBits;
	/* ------------------------------------------------------------------- */
	PCSanity(pLPC, "PCCreateHdr()");
	/* ------------------------------------------------------------------- */
	pHdr->nRate=PCEncodeFieldRate(pLPC, nDataRate);	/* encoded Data Rate */
	pHdr->nR=0;									/* reserved */
	//pHdr->nLen=pLPC->m_tPSDU.m_nSize;			/* length of PAYLOAD in bytes */
	pHdr->nLen=nLen;							/* length of PAYLOAD in bytes */
	/* ------------------------------------------------------------------- */
	/* calculate positive parity (even parity) */
	pHdrBits=(unsigned char*)pHdr;
	nOnes=0;
	for(i=0; i<17; i++)
		{
		nByteNr=i/8;
		nBitNr=i%8;
		if (pHdrBits[nByteNr] & (1<<nBitNr)) nOnes++;
		}
	/* ------------------------------------------------------------------- */
	pHdr->nParity=nOnes&1;						/* parity check */
	pHdr->nTail=0;								/* tail bits (=0) */
	/* ------------------------------------------------------------------- */
	return(0);
	}
/* ======================================================================= */
int PCDecodeFieldRate	(tagLPC_ST* pLPC, int nEncodedRate)
	{
	int nRes;
	switch(nEncodedRate)
		{
		case 11:	nRes=6;	break;
		case 15:	nRes=9;	break;
		case 10:	nRes=12;	break;
		case 14:	nRes=18;	break;
		case 9:		nRes=24;	break;
		case 13:	nRes=36;	break;
		case 8:		nRes=48;	break;
		case 12:	nRes=54;	break;
		default:	InstanceErrExit("PCDecodeFieldRate(..)",
						"cann't decode rate (=%d)", nEncodedRate);
			nRes=-1; break;
		}
	return(nRes);
	}
/* ======================================================================= */
int PCEncodeFieldRate	(tagLPC_ST* pLPC, int nDataRate){
	int nRes;
	switch(nDataRate)
		{
		case 6:		nRes=11;	break;
		case 9:		nRes=15;	break;
		case 12:	nRes=10;	break;
		case 18:	nRes=14;	break;
		case 24:	nRes=9;		break;
		case 36:	nRes=13;	break;
		case 48:	nRes=8;		break;
		case 54:	nRes=12;	break;
		default:	InstanceErrExit("PCEncodeFieldRate(..)",
						"invalid data rate (=%d)", nDataRate);
			nRes=-1; break;
		}
	return(nRes);
	}
/* ======================================================================= */
int PCEncodeOfdmSymb	(tagLPC_ST* pLPC, tagSYMBOL* pSmpls, tagBITSEQ* pBits, int nRate)
	{
	/* ------------------------------------------------------------------- */
	/* !!! PSEUDO ENCODING !!! */
	/* ------------------------------------------------------------------- */
	/* pseudo-encodes OFDM symbol (10.09.2003) */
	/* at this moment this function just copies data instead of calling */
	/* low-level procedures which can encode incoming bit sequence. */
	/* it uses only first real 27 values: */
	/* it corresponds to (=27 bytes = 216 bits ^= 54 Mbits/sec)*/
	/* 1. converts them into 'unsigned char' */
	/* 2. then, depending on data rate, expands first Ndbps into a bit seq.*/
	/* ------------------------------------------------------------------- */
#ifdef PC_NOCODING
	int i;
	int nNdbps;
	unsigned int c;
	unsigned char pBuf[27];
	/* ------------------------------------------------------------------- */
	PCSanity(pLPC, "PCEncodeOfdmSymb()");
	/* ------------------------------------------------------------------- */
	nNdbps=PCGetNdbpsByDataRate(pLPC, nRate);
	/* ------------------------------------------------------------------- */
	for(i=0; i<27; i++) pBuf[i]=0;
	/* ------------------------------------------------------------------- */
	VectorCollapseBitsUC(pBuf, pBits->m_pData, nNdbps);
	/* ------------------------------------------------------------------- */
	for(i=0; i<27; i++)
		{
		c=pBuf[i];
		pSmpls->m_pData[i].R=(double)1.0*c;
		pSmpls->m_pData[i].I=0;
		}
	/* ------------------------------------------------------------------- */
	for (i=27; i<80; i++) {pSmpls->m_pData[i].R=0; pSmpls->m_pData[i].I=0; }
	/* ------------------------------------------------------------------- */
#else
	CIFEncodeOFDMSymb(&pLPC->m_tCIF, pSmpls, pBits, nRate);
#endif
	return(0);
	}
/* ======================================================================= */
int PCGeneratePreSymb	(tagLPC_ST* pLPC, tagSYMBOL* pSmpls, int nNum)
	{
	/* ------------------------------------------------------------------- */
	/* dummy function, has to return a preamble symbol w. given # (10.09.2003) */
	/* ------------------------------------------------------------------- */
	int i;
	/* ------------------------------------------------------------------- */
	PCSanity(pLPC, "PCGeneratePreSymb()");
	/* ------------------------------------------------------------------- */
	for (i=0; i<80; i++)
		{
		pSmpls->m_pData[i].R=(1+nNum%4)*1;
		pSmpls->m_pData[i].I=(1+nNum%4)*(-1);
		}
	/* ------------------------------------------------------------------- */
	return(0);
	}
/* ======================================================================= */
int PCGetNdbpsByDataRate(tagLPC_ST* pLPC, int nDataRate)
	{
	/* ------------------------------------------------------------------- */
	/* returns Ndbps coressponding to given Data Rate (05.09.2003) */
	/* ------------------------------------------------------------------- */
	int nRes;
	switch(nDataRate)
		{
		case 6:		nRes=24;	break;
		case 9:		nRes=36;	break;
		case 12:	nRes=48;	break;
		case 18:	nRes=72;	break;
		case 24:	nRes=96;	break;
		case 36:	nRes=144;	break;
		case 48:	nRes=192;	break;
		case 54:	nRes=216;	break;
		default:	InstanceErrExit("PCGetNdbpsByDataRate(..)",
						"invalid data rate (=%d)", nDataRate);
			nRes=-1; break;
		}
	return(nRes);
	}
/* ======================================================================= */
int PCGetPSDUDataLen	(tagLPC_ST* pLPC)
	{ /* returns PSDU data len in bytes (18.09.2003) */
	PCSanity(pLPC, "PCGetPSDUDataLen()");
	return(pLPC->m_tPSDU.m_nSize);
	}
/* ======================================================================= */
int PCGetSymbolBits (tagLPC_ST* pLPC, tagBITSEQ* pBits, int nNum, int nNdbps)
	{
	/* ------------------------------------------------------------------- */
	/* extracts one bit sequence from the DATA field (04.09.2003, 05.09.2003) */
	/* ------------------------------------------------------------------- */
	int i, nOffset;
	unsigned char* pDATA;
	/* ------------------------------------------------------------------- */
	PCSanity(pLPC, "PCGetSymbolBits()");
	/* ------------------------------------------------------------------- */
	if(nNdbps>MAX_BITSEQSIZE)
		InstanceErrExit("PCGetSymbolBits(..)",
		"nNdbps is greated than MAX_BITSEQSIZE (%d > %d)",
		nNdbps, MAX_BITSEQSIZE);
	/* ------------------------------------------------------------------- */
	nOffset=nNum*nNdbps;
	pDATA=pLPC->m_tDATA.m_pData;
	/* ------------------------------------------------------------------- */
	pBits->m_nSize=nNdbps;
	for(i=0; i<nNdbps; i++) pBits->m_pData[i]=pDATA[nOffset+i];
	/* ------------------------------------------------------------------- */
	return(0);
	}
/* ======================================================================= */
/* RX PART */
/* ======================================================================= */
int PCAddBitsToRxBuf (tagLPC_ST* pLPC, tagBITSEQ* pBits, int nNum, int nNdbps)
	{
	/* ------------------------------------------------------------------- */
	/* adds given bit sequence to DATA buf */
	/* (05.09.2003) */
	/* ------------------------------------------------------------------- */
	int i;
	int nBits, nOffset;
	/* ------------------------------------------------------------------- */
	PCSanity(pLPC, "PCAddBitsToRxBuf()");
	/* ------------------------------------------------------------------- */
	nOffset=nNum*nNdbps;
	nBits=pBits->m_nSize;
	/* ------------------------------------------------------------------- */
	for (i=0; i<nBits; i++)
		pLPC->m_tDATA.m_pData[i+nOffset]=pBits->m_pData[i];
	/* ------------------------------------------------------------------- */
	return(0);
	}
/* ======================================================================= */
int PCDetectHdr	(tagLPC_ST* pLPC, tagSIGNAL* pHdr, tagBITSEQ* pBits)
	{
	/* -------------------------------------------------------------------
	// this function just collapses & copies bit sequence 'pBits'
	// into 'pHdr' struct 
	// (05.09.2003, 08.09.2003)
	// TODO: make flexible shift (22.09.2003)
	// ------------------------------------------------------------------- */
	int nShift;		// Viterbi decoder shift
	/* ------------------------------------------------------------------- */
	PCSanity(pLPC, "PCDetectHdr()");
	/* ------------------------------------------------------------------- */
	nShift=CIFGetVitPathLen(&pLPC->m_tCIF);
	VectorCollapseBitsUC(pHdr, pBits->m_pData+nShift, 8*(sizeof(tagSIGNAL)-1));
	/* ------------------------------------------------------------------- */
	return(0);
	}
/* ======================================================================= */
int PCDecodeOfdmSymb(tagLPC_ST* pLPC, tagBITSEQ* pBits, tagSYMBOL* pSmpls, int nRate)
	{
	/* ------------------------------------------------------------------- */
	/* !!! PSEUDO DECODING !!! */
	/* ------------------------------------------------------------------- */
	/* pseudo-decodes OFDM symbol (05.09.2003) */
	/* at this moment this function just copies data instead of calling */
	/* low-level procedures which can decode incoming samples. */
	/* it uses only first real 27 values: */
	/* it corresponds to (=27 bytes = 216 bits ^= 54 Mbits/sec)*/
	/* 1. converts them into 'unsigned char' */
	/* 2. then, depending on data rate, expands first Ndbps into a bit seq.*/
	/* ------------------------------------------------------------------- */
#ifdef PC_NOCODING
	int i;
	int nNdbps;
	unsigned char pBuf[27];
	/* ------------------------------------------------------------------- */
	PCSanity(pLPC, "PCDecodeOfdmSymb()");
	/* ------------------------------------------------------------------- */
	nNdbps=PCGetNdbpsByDataRate(pLPC, nRate);
	pBits->m_nSize=nNdbps;
	/* ------------------------------------------------------------------- */
	for(i=0; i<27; i++)
		pBuf[i]=(unsigned char)(pSmpls->m_pData[i].R);
	/* ------------------------------------------------------------------- */
	VectorExpandBitsUC(pBits->m_pData, pBuf, (nNdbps+7)/8);
	/* ------------------------------------------------------------------- */
#else
	CIFDecodeOFDMSymb(&pLPC->m_tCIF, pBits, pSmpls, nRate);
#endif
	return(0);
	}
/* ======================================================================= */
int PCExtractPSDU (tagLPC_ST* pLPC, int nSize)
	{
	/* ------------------------------------------------------------------- */
	/* (08.09.2003, 22.09.2003) */
	/* TODO: make flexible shift (22.09.2003) */
	/* ------------------------------------------------------------------- */
	int nShift;
	/* ------------------------------------------------------------------- */
	PCSanity(pLPC, "PCExtractPSDU()");
	/* ------------------------------------------------------------------- */
	nShift=CIFGetVitPathLen(&pLPC->m_tCIF);
	pLPC->m_tPSDU.m_nSize=nSize;
	VectorCollapseBitsUC(pLPC->m_tPSDU.m_pData,
		pLPC->m_tDATA.m_pData+16+nShift, 8*nSize);
	/* ------------------------------------------------------------------- */
	return(0);
	}
/* ======================================================================= */
int PCIsHdrParityOk	(tagLPC_ST* pLPC, tagSIGNAL* pHdr)
	{
	/* ------------------------------------------------------------------- */
	/* returns '1' is header parity field is correct (05.09.2003) */
	/* ------------------------------------------------------------------- */
	unsigned int bParity;
	int i;
	int nByteNr, nBitNr, nOnes;
	unsigned char* pHdrBits;
	/* ------------------------------------------------------------------- */
	PCSanity(pLPC, "PCIsHdrParityOk()");
	/* ------------------------------------------------------------------- */
	pHdrBits=(unsigned char*)pHdr;
	nOnes=0;
	/* ------------------------------------------------------------------- */
	/* calculate positive parity (even parity) */
	for(i=0; i<17; i++)
		{
		nByteNr=i/8;
		nBitNr=i%8;
		if (pHdrBits[nByteNr] & (1<<nBitNr)) nOnes++;
		}
	/* ------------------------------------------------------------------- */
	/* calculated parity */
	bParity=nOnes&1;
	/* ------------------------------------------------------------------- */
	return(pHdr->nParity==bParity);
	}
/* ======================================================================= */
int PCTreatShortSeq		(tagLPC_ST* pLPC, tagSYMBOL* pP1, tagSYMBOL* pP2)
	{ /* dummy function (05.09.2003) */
	PCSanity(pLPC, "PCTreatShortSeq()"); return (0); }
/* ======================================================================= */
int PCTreatLongSeq		(tagLPC_ST* pLPC, tagSYMBOL* pP3, tagSYMBOL* pP4)
	{ /* dummy function (05.09.2003) */
	PCSanity(pLPC, "PCTreatLongSeq()"); return (0); }
/* ======================================================================= */


