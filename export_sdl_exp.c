/* =======================================================================
// PROCSYS ver. 1.0
// procedures exported into SDL system
// -----------------------------------------------------------------------
// MD: 23.07.2003
// LE: 05.09.2003
// -----------------------------------------------------------------------
// Compiler: MS VC 6.0
// =======================================================================
// intrf_sdl_exp.c: implementation of service functions
// ======================================================================= */
#include <stdlib.h>			/* exit */
#include <string.h>			/* strncmp  */
#include <stdio.h>
/* ----------------------------------------------------------------------- */
#ifdef __STDC__
#include <stdarg.h>			/* variable argument list */
#elif _WIN32
#include <stdarg.h>			/* variable argument list */
#else
#include <varargs.h>		/* Required for UNIX V compatibility */
#endif
/* ----------------------------------------------------------------------- */
/* SDL cooperation (12.09.2003) */
/* ----------------------------------------------------------------------- */
#include "..\\SDLLIB\\scttypes.h"
#include ".\\GENER\\ctypes.h"
/* ----------------------------------------------------------------------- */
#include "export_sdl_exp.h"
#include "procpkt.h"		/* packets operations (30.07.2003) */
#include "c_vector.h"		/* VectorDumpUC (15.09.2003) */
#include "sta_emul.h"		/* 07.08.2003 */
/* ======================================================================= */
/* global data */
/* ======================================================================= */
extern tagSTACFG MC;		/* MAC cfg */
extern tagMAC_ST MS;		/* MAC current state */
/* ======================================================================= */
void InstanceErrExit(char* sFuncName, const char* sErrFormat, ...);
/* ======================================================================= */
char glPSTB[256];			/* ProcSysTempBuf - PSTB */
char glPSTB8K[8192];		/* ProcSysTempBuf - PSTB 8 KB*/
/* ======================================================================= */
double I2D(int nI)
	{ return((double)nI); }
/* ======================================================================= */
int SDL_EMUL_TIME(int* pnS, int* pnMS)
	{
	/* ------------------------------------------------------------------- */
	/* returns SDL emulation time (15.09.2003) */
	/* ------------------------------------------------------------------- */
	#ifdef SDL_NOW
	*pnS=SDL_NOW.s;		*pnMS=SDL_NOW.ns/1000000; return(0);
	#else
	*pnS=0;	*pnMS=0;	return(-1);
	#endif
	}
/* ======================================================================= */
char* SDL_EMUL_TIME_STR(char* sBuf, int nS, int nMS, int nOldS, int nOldMS)
	{
	/* (16.09.2003) */
	if (nOldS==nS)
		{
		if(nOldMS==nMS) sprintf(sBuf, "\t");
		else sprintf(sBuf, "   -.-\t");
		}
	else sprintf(sBuf, "%4d.%01d\t", nS, nMS/100);
	return(sBuf);
	}
/* ======================================================================= */
/* SDL _LOGGER */
/* ======================================================================= */
int LOG_SDL(char* sFuncName, char* sParamList, ...)
	{
	/* ------------------------------------------------------------------- */
	/* (12.09.2003, 16.09.2003) */
	/* ------------------------------------------------------------------- */
	static int nOldS=-1, nOldMS=-1;
	int nS, nMs;
	int nRes;
	/* ------------------------------------------------------------------- */
	char sBuf[8192];		/* fixed buf. size, may be changed to var. */
	char sDateTime[128];	
	va_list arglist;
	va_start(arglist, sParamList);
	vsprintf (sBuf, sParamList, arglist);
	/* ------------------------------------------------------------------- */
	nRes=SDL_EMUL_TIME(&nS, &nMs);
	if (-1==nRes)
		{ CLOGRAdd(&MC.m_tECFG.m_tLSDL, sFuncName, sBuf); return(0); }
	/* ------------------------------------------------------------------- */
	CLOGRAddDirect(&MC.m_tECFG.m_tLSDL, "%s =%d : %s %s",
		SDL_EMUL_TIME_STR(sDateTime, nS, nMs, nOldS, nOldMS),
		MC.m_tMDM_CLI.m_tST.nServTime, sFuncName, sBuf);
	nOldS=nS; nOldMS=nMs;
	/* ------------------------------------------------------------------- */
	return(0);
	}
/* ======================================================================= */
int LOG_STA(char* sFuncName, char* sParamList, ...)
	{
	/* ------------------------------------------------------------------- */
	/* (12.09.2003, 16.09.2003) */
	/* ------------------------------------------------------------------- */
	static int nOldS=-1, nOldMS=-1;
	int nS, nMs;
	int nRes;
	/* ------------------------------------------------------------------- */
	char sBuf[8192];	/* fixed buf. size, may be changed to var. */
	char sDateTime[128];	
	va_list arglist;
	va_start(arglist, sParamList);
	vsprintf (sBuf, sParamList, arglist);
	/* ------------------------------------------------------------------- */
	//if(strlen(sBuf)>8000) sBuf[8000]=0;
	/* ------------------------------------------------------------------- */
	nRes=SDL_EMUL_TIME(&nS, &nMs);
	if (-1==nRes)
		{ CLOGRAdd(&MC.m_tLMAC.m_tLOGR_IO, sFuncName, sBuf); return(0); }
	/* ------------------------------------------------------------------- */
	CLOGRAddDirect(&MC.m_tLMAC.m_tLOGR_IO, "%s : %s %s",
		SDL_EMUL_TIME_STR(sDateTime, nS, nMs, nOldS, nOldMS), sFuncName, sBuf);
	nOldS=nS; nOldMS=nMs;
	/* ------------------------------------------------------------------- */
	return(0);
	}
/* ======================================================================= */
void SPY_EV_UNI(int nSpyID1, int nSpyID2, SDL_Charstring tPrName, SDL_Charstring tStName,
			SDL_Charstring tEvName, int nPars, int* pPars)
	{ int nSec, nNSec; char *sPrName, *sStName, *sEvName;
	sPrName=z_ctypes_32_cstring2cstar(tPrName);
	sStName=z_ctypes_32_cstring2cstar(tStName);
	sEvName=z_ctypes_32_cstring2cstar(tEvName);
	LOG_SDL("SPY_EV_UNI(..)", "spy-ev (nSpyID={%d, %d} %s, %s, %s, %d)", nSpyID1, nSpyID2,
		sPrName, sStName, sEvName, nPars);
	SDL_EMUL_TIME(&nSec, &nNSec);
	if (nSpyID1!=-1)STASpyEv(&MC, nSpyID1, nSec, nNSec, sPrName, sStName, sEvName, nPars, pPars);
	if (nSpyID2!=-1)STASpyEv(&MC, nSpyID2, nSec, nNSec, sPrName, sStName, sEvName, nPars, pPars);
	free(sPrName); free(sStName); free(sEvName); }
/* ======================================================================= */
void SPY_EV(int nSpyID1, SDL_Charstring tPrName, SDL_Charstring tStName,
			SDL_Charstring tEvName)
	{ SPY_EV_UNI(nSpyID1, -1, tPrName, tStName, tEvName, 0, NULL); }
/* ----------------------------------------------------------------------- */
void SPYX_EV(int nSpyID1, int nSpyID2, SDL_Charstring tPrName, SDL_Charstring tStName,
			SDL_Charstring tEvName)
	{ SPY_EV_UNI(nSpyID1, nSpyID2, tPrName, tStName, tEvName, 0, NULL); }
/* ----------------------------------------------------------------------- */
void SPYX_EVP1(int nSpyID1, int nSpyID2, SDL_Charstring tPrName, SDL_Charstring tStName,
			SDL_Charstring tEvName, SDL_Duration tDur)
	{ int pPars[2]; pPars[0]=tDur.s; pPars[1]=tDur.ns;
	SPY_EV_UNI(nSpyID1, nSpyID2, tPrName, tStName, tEvName, 1, pPars); }
/* ======================================================================= */
void SPY_UNI(int nSpyID, SDL_Charstring tDir, SDL_Charstring tEvName, int nPars, int* pPars)
	{ int nSec, nNSec, nRes; char *sDir, *sEvName;
	LOG_SDL("SPY_UNI(..)", "spy (nSpyID=%d, nPars=%d)", nSpyID, nPars);
	sDir=z_ctypes_32_cstring2cstar(tDir);
	sEvName=z_ctypes_32_cstring2cstar(tEvName);
	nRes=SDL_EMUL_TIME(&nSec, &nNSec);
	nRes=STASpy(&MC, nSpyID, nSec, nNSec, sDir, sEvName, nPars, pPars);
	free(sDir); free(sEvName); }
/* ======================================================================= */
void SPY(int nSpyID, SDL_Charstring tDir, SDL_Charstring tEvName)
	{ SPY_UNI(nSpyID, tDir, tEvName, 0, NULL); }
/* ----------------------------------------------------------------------- */
void SPYP1(int nSpyID, SDL_Charstring tDir, SDL_Charstring tEvName, int nPar1)
	{ int pPars[2]; pPars[0]=nPar1;
	SPY_UNI(nSpyID, tDir, tEvName, 1, pPars); }
/* ----------------------------------------------------------------------- */
void SPYP2(int nSpyID, SDL_Charstring tDir, SDL_Charstring tEvName, int nPar1, int nPar2)
	{ int pPars[2]; pPars[0]=nPar1; pPars[1]=nPar2;
	SPY_UNI(nSpyID, tDir, tEvName, 2, pPars); }
/* ======================================================================= */
/* L3 Event Generator (08.09.2003) */
/* ======================================================================= */
void GetEventIDByTime	(int* pEventID, int nTimeSec)
	{
	/* (08.09.2003) */
	LOG_SDL("GetEventIDByTime()", "nTimeSec=%d", nTimeSec);
	*pEventID=CFCTGetEventIDByTime(&MC.m_tFLCTRL, nTimeSec);
	LOG_SDL("GetEventIDByTime()", "=%d", *pEventID);
	}
/* ----------------------------------------------------------------------- */
double GetTimeOfNextEvent	(int nCurTimeSec)
	{ /* (11.09.2003) */ int nRes;
	LOG_SDL("GetTimeOfNextEvent()", "nCurTimeSec=%d", nCurTimeSec);
	nRes=CFCTGetTimeOfNextEvent	(&MC.m_tFLCTRL, nCurTimeSec);
	LOG_SDL("GetTimeOfNextEvent()", "=%d", nRes); 
	return((double)nRes); }
/* ======================================================================= */
/* MAC LAYER operations - simple public service functions (23.07.2003) */
/* ======================================================================= */
int CalcNumOfFr(tagMAC_ST* pMS)
	{
	int nRes;
	LOG_SDL("CalcNumOfFr()", "get no. of fragments");
	nRes=MACCalcNumOfFrags(&MC.m_tLMAC, pMS, MC.m_tECFG.m_nFragSize);
	LOG_SDL("CalcNumOfFr()", "=%d", nRes);
	return(nRes); }
/* ======================================================================= */
void ClearBuf(tagMAC_ST* pMS, int nBufID)
	{ LOG_SDL("ClearBuf(..)", "clears packet (nBufID=%d)", nBufID);
	MACClearBuf(&MC.m_tLMAC, pMS, nBufID); }
/* ======================================================================= */
void FormRTS(tagMAC_ST* pMS, tagMACADDR tRA, double dDurPktTxFr)
	{ LOG_SDL("FormRTS(..)", "forms RTS packet in RTS buffer (tRA=%s, dDurPktTxFr=%5.2lf)", PktConvAddrToStr(tRA, glPSTB), dDurPktTxFr);
	MACFormRTS(&MC.m_tLMAC, pMS, tRA, (int)dDurPktTxFr); }
/* ----------------------------------------------------------------------- */
void FormCTS(tagMAC_ST* pMS, tagMACADDR tRA, double dDurPktRTS)
	{ LOG_SDL("FormCTS(..)", "forms CTS packet in CTRL buffer (tRA=%s, dDurPktRTS=%5.2lf)", PktConvAddrToStr(tRA, glPSTB), dDurPktRTS);
	MACFormCTS(&MC.m_tLMAC, pMS, tRA, (int)dDurPktRTS); }
/* ----------------------------------------------------------------------- */
void FormACK(tagMAC_ST* pMS, tagMACADDR tRA, double dDurPktRx, int bMoreFrags)
	{ LOG_SDL("FormACK(..)", "forms ACK packet in CTRL buffer (tRA=%s, dDurPktRx=%5.2lf, bMoreFrags=%d)", PktConvAddrToStr(tRA, glPSTB), dDurPktRx, bMoreFrags);
	MACFormACK(&MC.m_tLMAC, pMS, tRA, (int)dDurPktRx, bMoreFrags); }
/* ======================================================================= */
int GetAddrType(tagMAC_ST* pMS, tagMACADDR tAddr)
	{
	int nRes;
	LOG_SDL("GetAddrType(..)", "get address type (tAddr=%s)", PktConvAddrToStr(tAddr, glPSTB));
	nRes=MACGetAddrType(&MC.m_tLMAC, pMS, tAddr);
	LOG_SDL("GetAddrType(..)", "=%d", nRes);
	return(nRes); }
/* ======================================================================= */
void GetDataToTx(tagMACADDR* pDA, tagFLATARRAY* pArray)
	{
	LOG_SDL("GetDataToTx()", "receives data from L3 (file)");
	STAGetDataToTx(&MC, pDA, pArray); }
/* ----------------------------------------------------------------------- */
void GetDataInRx(tagMAC_ST* pMS, tagFLDPSDU* pPSDU)
	{
	LOG_SDL("GetDataInRx()", "transfer inbound data into pktRx");
	MACGetDataInRx(&MC.m_tLMAC, pMS, pPSDU); }
/* ======================================================================= */
void GetNewFr(tagMAC_ST* pMS, int nFrag)
	{ LOG_SDL("GetNewFr(..)", "fill TxFr buffer (nFrag=%d)", nFrag);
	MACGetNewFr(&MC.m_tLMAC, pMS, nFrag, MC.m_tECFG.m_nFragSize); }
/* ======================================================================= */
int IsPsduCrcOk(tagFLDPSDU* pPSDU)
	{
	int bRes;
	LOG_SDL("CheckCRC(..)", "checks CRC of the incoming packet");
	bRes=MACIsPsduCrcOk(&MC.m_tLMAC, pPSDU);
	LOG_SDL("CheckCRC(..)", "=%s", bRes?"OK":"ERROR");
	return(bRes); }
/* ======================================================================= */
int IsDupl(tagMAC_ST* pMS, tagMACADDR tSA, int nSeqNo, int nFragNo)
	{
	int bRes;
	/* checks if received packet is duplicated (12.08.2003) */
	LOG_SDL("IsDupl(..)", "check if a packet is a duplicate (tSA=%s, nSeqNo=%d:%d)", PktConvAddrToStr(tSA, glPSTB), nSeqNo, nFragNo);
	bRes=MACIsDupl(&MC.m_tLMAC, pMS, tSA, nSeqNo, nFragNo);
	LOG_SDL("IsDupl(..)", "=%s", bRes?"YES":"NO");
	return(bRes);
	}
/* ----------------------------------------------------------------------- */
void UpdDuplCache(tagMAC_ST* pMS, tagMACADDR tSA, int nSeqNo, int nFragNo)
	{
	/* updates cache of duplicates (12.08.2003) */
	LOG_SDL("UpdDuplCache(..)", "update duplicate cache (tSA=%s, nSeqNo=%d:%d)", PktConvAddrToStr(tSA, glPSTB), nSeqNo, nFragNo);
	MACUpdDuplCache(&MC.m_tLMAC, pMS, tSA, nSeqNo, nFragNo);
	}
/* ======================================================================= */
/* system functions */
/* ======================================================================= */
void ErrExit	(SDL_Charstring tPrName, SDL_Charstring tStName,
				 SDL_Charstring tEvName, SDL_Charstring tErr)
	{
	char *sPrName, *sStName, *sEvName, *sErr;
	sPrName=z_ctypes_32_cstring2cstar(tPrName);
	sStName=z_ctypes_32_cstring2cstar(tStName);
	sEvName=z_ctypes_32_cstring2cstar(tEvName);
	sErr=z_ctypes_32_cstring2cstar(tErr);
	InstanceErrExit("ErrExit(..)", "pr: %s, st: %s, ev: %s, Error: %s", sPrName, sStName, sEvName, sErr);
	free(sPrName); free(sStName); free(sEvName); free(sErr); }
/* ======================================================================= */
void inc_cw		(tagMAC_ST* pMS) /* 9.2.4. Random BO proc. */
	{ LOG_SDL("inc_cw()", "increase Contention Window");
	MACCWInc(&MC.m_tLMAC, pMS); }
/* ----------------------------------------------------------------------- */
void rst_cw		(tagMAC_ST* pMS) /* */
	{ LOG_SDL("rst_cw()", "reset Contention Window");
	MACCWReset(&MC.m_tLMAC, pMS); }
/* ----------------------------------------------------------------------- */
int rand_cw		(tagMAC_ST* pMS)
	{ int nRes, nVal;
	LOG_SDL("rand_cw()", "get random no. of slots");
	nRes=MACGetRandNumOfSlots(&MC.m_tLMAC, pMS);
	nVal=nRes;
	if(-1!=MC.m_tECFG.m_nMaxCWVal && nRes>MC.m_tECFG.m_nMaxCWVal) nVal=MC.m_tECFG.m_nMaxCWVal;
	LOG_SDL("rand_cw()", "=%d slots, (%d sim. time units), used =%d slots (%d STU)", 
		nRes, nRes*MC.m_tLMAC.m_tPHY.m_aSlotTime,
		nVal, nVal*MC.m_tLMAC.m_tPHY.m_aSlotTime);
	return(nVal); }
/* ----------------------------------------------------------------------- */
void rst_cntrs	(tagMAC_ST* pMS, int nSize)	/* resets SSRC or SLRC counters (16.07.2003, 12.08.2003) */
	{ LOG_SDL("rst_cntrs(..)", "reset SSRC of SLRC counter (nSize=%d)", nSize);
	MACResetSLC(&MC.m_tLMAC, pMS, nSize); }
/* ======================================================================= */
void RecvPSDU	(tagFLDPSDU* pPSDU)
	{ LOG_SDL("RecvPSDU(..)", "obtain PSDU data from PHY_CTRL layer");
	MACRecvPSDU(&MC.m_tLMAC, &MC.m_tLPC, &MC.m_tECFG, pPSDU); }
/* ----------------------------------------------------------------------- */
void FillL3Buf		(tagMAC_ST* pMS)
	{ LOG_SDL("FillL3Buf(..)", "fill user (L3) buf w. inbound data");
	MACFillL3Buf(&MC.m_tLMAC, pMS, &MC.m_tECFG); }
/* ----------------------------------------------------------------------- */
void FillMacBuf	(tagMAC_ST* pMS, tagMACADDR* pDA, tagFLATARRAY* pArray)
	{ LOG_SDL("FillMacBuf(..)", "fills MAC Tx buffer w. given DATA");
	MACFillTxBuf(&MC.m_tLMAC, pMS, &MC.m_tECFG, pDA, pArray);
	LOG_SDL("FillMacBuf(..)", "ok - DEL IT");}
/* ----------------------------------------------------------------------- */
void FillPhyCtrlBuf	(tagMAC_ST* pMS, int nBufID)
	{ LOG_SDL("FillPhyCtrlBuf(..)", "fill PHY_CTRL buf w. data (nBufID=%d)", nBufID);
	MACFillPhyCtrlBuf(&MC.m_tLMAC, pMS, &MC.m_tLPC, &MC.m_tECFG, nBufID); }
/* ======================================================================= */
int  GetTxRate  (void)
	{ /* returns Tx Rate (18.09.2003) */
	int nRes=STAGetTxRate(&MC);
	LOG_SDL("GetTxRate()", "=%d [Mbits/sec]", nRes);
	return(nRes);
	}
/* ======================================================================= */
int  GetPsduLen (void);					/* returns length of PSDU frame (18.09.2003) */
/* ======================================================================= */
/* Global Data operations (Retrieve & Update) (21.07.2003) */
/* ======================================================================= */
void GDR(tagMAC_ST* pMS)
	{
	/* reads current MAC state (24.07.2003) */
	LOG_SDL("GDR()", "read global data");
	MACCopySt(&MC.m_tLMAC, pMS, &MS);
	}
/* ======================================================================= */
void GDU(tagMAC_ST* pMS)
	{
	/* updates current MAC state (24.07.2003) */
	LOG_SDL("GDU()", "update global data");
	CLOGRAdd(&(MC.m_tLMAC.m_tLOGR_ST), "GDU()", "%s", MACDumpST(&MC.m_tLMAC, pMS, glPSTB8K));
	MACCopySt(&MC.m_tLMAC, &MS, pMS);
	}
/* ======================================================================= */
/* PHY_CTRL LAYER functions (02.09.2003) */
/* ======================================================================= */
/* TX PART */
/* ======================================================================= */
void ConvHdrToBitSeq	(tagBITSEQ* pBits, tagSIGNAL* pHdr)
	{ LOG_SDL("ConvHdrToBitSeq(..)", "converts HDR into a bit sequence");
	PCConvHdrToBitSeq(&MC.m_tLPC, pBits, pHdr); 
	LOG_STA("ConvHdrToBitSeq(..)", "HDR {%s}", VectorDumpUC_BITS(glPSTB, pBits->m_pData, pBits->m_nSize));
	}
/* ----------------------------------------------------------------------- */
void ConvPSDUToData		(int nSymb, int nNdbps)
	{ LOG_SDL("ConvPSDUToData(..)", "converts PSDU into a bit sequence (nSymb=%d, nNdbps=%d)", nSymb, nNdbps);
	PCConvPSDUToData(&MC.m_tLPC, nSymb, nNdbps);
	LOG_SDL("ConvPSDUToData(..)", "ok"); // DEL IT
	LOG_STA("ConvPSDUToData(..)", "DATA {%s}", VectorDumpUC_BITS(glPSTB8K, MC.m_tLPC.m_tDATA.m_pData, MC.m_tLPC.m_tDATA.m_nSize<8000?MC.m_tLPC.m_tDATA.m_nSize:8000));
	LOG_SDL("ConvPSDUToData(..)", "ok2"); // DEL IT
	}
/* ----------------------------------------------------------------------- */
void CreateHdr			(tagSIGNAL* pHdr, int nTxRate, int nLen)
	{ PCCreateHdr(&MC.m_tLPC, pHdr, nTxRate, nLen); 
	LOG_SDL("CreateHdr(..)", "(nTxRate=%d) HDR {%d, %d, %d bytes, %d}", 
		nTxRate, pHdr->nRate, pHdr->nR, pHdr->nLen, pHdr->nParity);}
/* ----------------------------------------------------------------------- */
int  DecodeFieldRate	(int nCodedRate)
	{ int nRes; 
	LOG_SDL("DecodeFieldRate(..)", "decode filed rate (nCodedRate=%d)", nCodedRate);
	nRes=PCDecodeFieldRate (&MC.m_tLPC, nCodedRate);
	LOG_SDL("DecodeFieldRate(..)", "=%d [Mbits/sec]", nRes);
	return(nRes); }
/* ----------------------------------------------------------------------- */
void EncodeOfdmSymb		(tagSYMBOL* pSymb, tagBITSEQ* pBits, int nRate)
	{ LOG_SDL("EncodeOfdmSymb(..)", "PSEUDO-encode OFDM symbol (nRate=%d)", nRate);
	PCEncodeOfdmSymb(&MC.m_tLPC, pSymb, pBits, nRate);
	LOG_STA("EncodeOfdmSymb(..)", "BITS[%d] {%s}", pBits->m_nSize, VectorDumpUC_BITS(glPSTB8K, pBits->m_pData, pBits->m_nSize<8000?pBits->m_nSize:8000));
	}
/* ----------------------------------------------------------------------- */
void GeneratePreSymb	(tagSYMBOL* pSymb, int nNum)
	{ LOG_SDL("GeneratePreSymb(..)", "generates one symbol of the PRE (nNum=%d)", nNum);
	PCGeneratePreSymb(&MC.m_tLPC, pSymb, nNum); }
/* ----------------------------------------------------------------------- */
int  GetNdbpsByDataRate	(int nDataRate)
	{ int nRes;
	LOG_SDL("GetNdbpsByDataRate(..)", "get Ndbps (nDataRate=%d)", nDataRate);
	nRes=PCGetNdbpsByDataRate (&MC.m_tLPC, nDataRate); 
	LOG_SDL("GetNdbpsByDataRate(..)", "=%d [DataBits/symbol]", nRes);
	return(nRes); }
/* ----------------------------------------------------------------------- */
int  GetPsduLen			(void)
	{ /* returns length of PSDU frame (18.09.2003) */
	int nRes=PCGetPSDUDataLen(&MC.m_tLPC);
	LOG_SDL("GetPsduLen(..)", "size of PAYLOAD (=%d bytes)", nRes);
	return(nRes);
	}
/* ----------------------------------------------------------------------- */
void GetSymbolBits		(tagBITSEQ* pBits, int nNum, int nNdbps)
	{ LOG_SDL("GetSymbolBits(..)", "get bits for given symb. (nNum=%d, nNdbps=%d)", nNum, nNdbps);
	PCGetSymbolBits(&MC.m_tLPC, pBits, nNum, nNdbps);
	}
/* ======================================================================= */
/* RX PART */
/* ======================================================================= */
void AddBitsToRxBuf		(tagBITSEQ* pBits, int nNum, int nNdbps)
	{ LOG_SDL("AddBitsToRxBuf(..)", "add recv. bits to DATA buf (nNum=%d, nNdbps=%d)", nNum, nNdbps);
	PCAddBitsToRxBuf(&MC.m_tLPC, pBits, nNum, nNdbps); }
/* ----------------------------------------------------------------------- */
void DecodeOfdmSymb		(tagBITSEQ* pBits, tagSYMBOL* pSymb, int nRate)
	{ LOG_SDL("DecodeOfdmSymb(..)", "PSEUDO-decode OFDM symbol (nRate=%d)", nRate);
	PCDecodeOfdmSymb(&MC.m_tLPC, pBits, pSymb, nRate);
	LOG_STA("DecodeOfdmSymb(..)", "BITS[%d] {%s}\n", pBits->m_nSize, VectorDumpUC_BITS(glPSTB8K, pBits->m_pData, pBits->m_nSize<8000?pBits->m_nSize:8000));
	}
/* ----------------------------------------------------------------------- */
void DetectHdr			(tagSIGNAL* pHdr, tagBITSEQ* pBits)
	{ PCDetectHdr(&MC.m_tLPC, pHdr, pBits); 
	LOG_SDL("DetectHdr(..)", "={%d, %d, %d bytes, %d}", pHdr->nRate, pHdr->nR, pHdr->nLen, pHdr->nParity);
	}
/* ----------------------------------------------------------------------- */
void ExtractPSDU		(int nSize)
	{ LOG_SDL("ExtractPSDU(..)", "extract PSDU (nSize=%d)", nSize);
	PCExtractPSDU(&MC.m_tLPC, nSize); }
/* ----------------------------------------------------------------------- */
int  IsHdrParityOk		(tagSIGNAL* pHdr)
	{ int bRes;
	LOG_SDL("IsHdrParityOk(..)", "check HDR parity");
	bRes=PCIsHdrParityOk (&MC.m_tLPC, pHdr);
	LOG_SDL("IsHdrParityOk(..)", "=%s", bRes?"OK":"ERROR");
	return(bRes); }
/* ----------------------------------------------------------------------- */
void TreatShortSeq		(tagSYMBOL* pP1, tagSYMBOL* pP2)
	{ LOG_SDL("TreatShortSeq(..)", "dummy");
	PCTreatShortSeq(&MC.m_tLPC, pP1, pP2); }
/* ----------------------------------------------------------------------- */
void TreatLongSeq		(tagSYMBOL* pP3, tagSYMBOL* pP4)
	{ LOG_SDL("TreatLongSeq(..)", "dummy");
	PCTreatLongSeq(&MC.m_tLPC, pP3, pP4); }
/* ======================================================================= */
/* PHY LAYER functions (02.09.2003) */
/* ======================================================================= */
/* TX PART */
/* ======================================================================= */
void TxFillBuf			(tagSYMBOL* pSmpls)
	{ LOG_SDL("TxFillBuf(..)", "fill PHY Tx");
	PHYTxFillBuf(&MC.m_tLPHY, pSmpls); 
	LOG_STA("TxFillBuf(..)", "SYMB[80] {%s}\n", VectorDumpCMPLX(glPSTB8K, pSmpls->m_pData, 80));
	}
/* ----------------------------------------------------------------------- */
void TxRequired			(int nSymb, int nDataRate)
	{ LOG_SDL("TxRequired(..)", "set up PHY Tx part (nSymb=%d, nDataRate=%d)", nSymb, nDataRate);
	PHYTxRequired(&MC.m_tLPHY, nSymb, nDataRate); }
/* ----------------------------------------------------------------------- */
void TxSendSymb			(void)
	{ LOG_SDL("TxSendSymb(..)", "send SYMBOL samples");
	PHYTxSendSymb(&MC.m_tLPHY, &MC.m_tMDM_CLI); 
	LOG_SDL("TxSendSymb(..)", "server time =%d [STU]",
		MC.m_tMDM_CLI.m_tST.nServTime); }
/* ----------------------------------------------------------------------- */
void TxStop				(void)
	{ LOG_SDL("TxStop(..)", "stop Tx");
	PHYTxStop(&MC.m_tLPHY); }
/* ======================================================================= */
/* RX PART */
/* ======================================================================= */
void RxRecvSymb	(tagSYMBOL* pSymb)
	{ LOG_SDL("RxRecvSymb(..)", "receive one OFDM symbol");
	PHYRxRecvOfdmSymb(&MC.m_tLPHY, pSymb);
	LOG_STA("RxRecvSymb(..)", "SYMB[80] {%s}", VectorDumpCMPLX(glPSTB8K, pSymb->m_pData, 80));
	}
/* ----------------------------------------------------------------------- */
void RxRecvSmpls (tagMDM_SAMPLES* pSmpls)
	{ LOG_SDL("RxRecvSmpls(..)", "receive 20 samples");
	PHYRxRecvSmpls(&MC.m_tLPHY, pSmpls); }
/* ======================================================================= */
/* EVENT_GEN */
/* ======================================================================= */
void AddSmplsToSymb		(tagMDM_SAMPLES* pSmpls, int nNum)
	{ LOG_SDL("AddSmplsToSymb(..)", "add recv. samples to OFDM symbol (nNum=%d)", nNum);
	PHYAddSmplsToSymb(&MC.m_tLPHY, pSmpls, nNum); }
/* ----------------------------------------------------------------------- */
int  IsPhyMdmAct		(void)
	{ int bRes;
	LOG_SDL("IsPhyMdmAct(..)", "check whether MDM is active");
	bRes=PHYIsPhyMdmAct(&MC.m_tLPHY);
	LOG_SDL("IsPhyMdmAct(..)", "=%s", bRes?"ACTIVE":"IDLE");
	return(bRes);
	}
/* ----------------------------------------------------------------------- */
int  IsTxReq			(double *pSymb, int* pDataRate)
	{ int bRes;
	LOG_SDL("IsTxReq(..)", "check whether Tx is required");
	bRes=PHYIsTxReq(&MC.m_tLPHY, pSymb, pDataRate);
	LOG_SDL("IsTxReq(..)", "=%s", bRes?"YES":"NO");
	return(bRes);
	}
/* ======================================================================= */
/* MDM CLI commands (17.09.2003) */
/* ======================================================================= */
void MDM_SYNC	(void)
	{ CMDM_CLIIterate (&MC.m_tMDM_CLI, MDM_CLI_ITER_SYNC,
	&MC.m_tLPHY.m_bRxMdmActive, &MC.m_tLPHY.m_tMdmSmpls);
	LOG_SDL("MDM_SYNC()", "server time =%d [STU]",
		MC.m_tMDM_CLI.m_tST.nServTime); }
/* ======================================================================= */
void MDM_GET_STATE	(void)
	{
	CMDM_CLIIterate (&MC.m_tMDM_CLI, MDM_CLI_ITER_GET_STATE,
	&MC.m_tLPHY.m_bRxMdmActive, &MC.m_tLPHY.m_tMdmSmpls);
	LOG_SDL("MDM_GET_STATE()", "server time =%d [STU], active=%d",
		MC.m_tMDM_CLI.m_tST.nServTime, MC.m_tLPHY.m_bRxMdmActive); }
/* ======================================================================= */
void MDM_SKIP_POLL	(void)
	{
	CMDM_CLIIterate (&MC.m_tMDM_CLI, MDM_CLI_ITER_SKIP_POLL,
	NULL, &MC.m_tLPHY.m_tMdmSmpls);
	LOG_SDL("MDM_SKIP_POLL()", "server time =%d [STU]", MC.m_tMDM_CLI.m_tST.nServTime); }
/* ======================================================================= */
