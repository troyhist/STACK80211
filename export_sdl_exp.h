/* =======================================================================
// PROCSYS ver. 1.0
// procedures exported into SDL system
// all layers: MAC, PHY_CTRL, PHY
// -----------------------------------------------------------------------
// MD: 16.07.2003
// LE: 02.09.2003
// -----------------------------------------------------------------------
// Compiler: MS VC 6.0
// =======================================================================
// intrf_sdl_exp.h: implementation of service functions (header)
// ======================================================================= */
#ifndef __PROCSYS_EXP__
#define __PROCSYS_EXP__
/* ======================================================================= */
#include "proc_mac_exp.h"
#include "proc_phyctrl_exp.h"		/* (14.08.2003) */
#include "proc_phy_exp.h"			/* tagSYMBOL (05.09.2003) */
/* ======================================================================= */
//double I2D(int nI);				/* (15.09.2003) */
/* ======================================================================= */
/* system functions */
/* ======================================================================= */
void SPY	(int nSpyID, SDL_Charstring tDir, SDL_Charstring tEventName);	/* events-spy function (14.08.2003) */
void SPYP1	(int nSpyID, SDL_Charstring tDir, SDL_Charstring tEventName, int nPar1);			/* events-spy function (14.08.2003) */
void SPYP2	(int nSpyID, SDL_Charstring tDir, SDL_Charstring tEventName, int nPar1, int nPar2);	/* events-spy function (14.08.2003) */
void SPY_EV		(int nSpyID1, SDL_Charstring tPrName, SDL_Charstring tStName, SDL_Charstring tEvName); /* (17.09.2003) */
void SPYX_EV	(int nSpyID1, int nSpyID2, SDL_Charstring tPrName, SDL_Charstring tStName, SDL_Charstring tEvName); /* (25.09.2003) */
void SPYX_EVP1	(int nSpyID1, int nSpyID2, SDL_Charstring tPrName, SDL_Charstring tStName, SDL_Charstring tEvName, SDL_Duration tDur); /* (25.09.2003) */
/* ======================================================================= */
/* L3 Event Generator (08.09.2003) */
/* ======================================================================= */
void GetEventIDByTime		(int* pEventID, int nTimeSec);	/* (08.09.2003) */
double GetTimeOfNextEvent	(int nCurTimeSec);				/* (11.09.2003) */
/* ======================================================================= */
/* MAC LAYER operations (22.07.2003) */
/* ======================================================================= */
int  CalcNumOfFr(tagMAC_ST* pMAC);			/* 25.07.2003 */
void ClearBuf(tagMAC_ST* pMAC, int nBufID);	/* 25.07.2003 */
void FormRTS(tagMAC_ST* pMAC, tagMACADDR tRA, double dDurPktTxFr);	/* forms RTS frame for given station (23.07.2003, 11.08.2003) */
void FormCTS(tagMAC_ST* pMAC, tagMACADDR tRA, double dDurPktRTS);	/* forms CTS frame for given station (16.07.2003, 11.08.2003) */
void FormACK(tagMAC_ST* pMAC, tagMACADDR tRA, double dDurPktRx, int bMoreFrags);	/* forms ACK frame for given station (16.07.2003) */
int  GetAddrType(tagMAC_ST* pMAC, tagMACADDR tAddr);	/* returns dest. address type of a given packet (28.07.2003) */
void GetDataToTx(tagMACADDR* pDA, tagFLATARRAY* pArray);/* transfers user data into pktTx (16.07.2003) */
void GetDataInRx(tagMAC_ST* pMAC, tagFLDPSDU* pPSDU);	/* transfers inbound data into pktRx (30.07.2003) */
void GetNewFr	(tagMAC_ST* pMAC, int nFrag);	/* fills pktTxFr with new data (25.07.2003) */
int  IsDupl		(tagMAC_ST* pMAC, tagMACADDR tSA, int nSeqNo, int nFragNo);		/* checks if received packet is duplicated (12.08.2003) */
int  IsPsduCrcOk(tagFLDPSDU* pPSDU);		/* 25.07.2003 */
void UpdDuplCache(tagMAC_ST* pMAC, tagMACADDR tSA, int nSeqNo, int nFragNo);	/* updates cache of duplicates (12.08.2003) */
/* ======================================================================= */
void ErrExit	(SDL_Charstring tPrName, SDL_Charstring tStName, SDL_Charstring tEvName, SDL_Charstring tErr); /* extremal situations (16.07.2003) */
void inc_cw		(tagMAC_ST* pMAC);		/* increases CW (16.07.2003) */
void rst_cw		(tagMAC_ST* pMAC);		/* resets CW to CWMIN (16.07.2003) */
int  rand_cw	(tagMAC_ST* pMAC);		/* returns random number of slots (16.07.2003) */
void rst_cntrs	(tagMAC_ST* pMAC, int nSize);	/* resets SSRC or SLRC counters (16.07.2003, 12.08.2003) */
void RecvPSDU	(tagFLDPSDU* pPSDU);			/* fills MAC Rx buffer w. incoming DATA (07.08.2003) */
void FillL3Buf	(tagMAC_ST* pMAC);				/* fills DATA buffer of the upper layer w. inbound DATA (16.07.2003) */
void FillMacBuf	(tagMAC_ST* pMAC, tagMACADDR* pDA, tagFLATARRAY* pArray);	/* fills MAC Tx buffer w. given DATA (10.09.2003) */
void FillPhyCtrlBuf(tagMAC_ST* pMAC, int nBufID);		/* fills DATA buffer of PHY_CTRL layer w. given DATA (16.07.2003) */
/* ======================================================================= */
int  GetTxRate  (void);					/* returns Tx Rate (18.09.2003) */
/* ======================================================================= */
/* Global Data operations (Retrieve & Update) (21.07.2003) */
void GDR	(tagMAC_ST* pMAC);	/* reads current MAC state (24.07.2003) */
void GDU	(tagMAC_ST* pMAC);	/* updates current MAC state (24.07.2003) */
/* ======================================================================= */
/* PHY_CTRL LAYER functions (02.09.2003) */
/* ======================================================================= */
/* TX PART */
/* ----------------------------------------------------------------------- */
void ConvHdrToBitSeq	(tagBITSEQ* pBits, tagSIGNAL* pHdr);
void ConvPSDUToData		(int nSymb, int nNdbps);
void CreateHdr			(tagSIGNAL* pHdr, int nTxRate, int nLen);
int  DecodeFieldRate	(int nCodedRate);
void EncodeOfdmSymb		(tagSYMBOL* pSymb, tagBITSEQ* pBits, int nRate);
void GeneratePreSymb	(tagSYMBOL* pSymb, int nNum);
int  GetNdbpsByDataRate	(int nDataRate);
int  GetPsduLen			(void);		/* returns length of PSDU frame (18.09.2003) */
void GetSymbolBits		(tagBITSEQ* pBits, int nNum, int nNdbps);
/* ----------------------------------------------------------------------- */
/* RX PART */
/* ----------------------------------------------------------------------- */
void AddBitsToRxBuf		(tagBITSEQ* pBits, int nNum, int nNdbps);
void DecodeOfdmSymb		(tagBITSEQ* pBits, tagSYMBOL* pSymb, int nRate);
void DetectHdr			(tagSIGNAL* pHdr, tagBITSEQ* pBits);
void ExtractPSDU		(int nSize);
int  IsHdrParityOk		(tagSIGNAL* pHdr);
void TreatShortSeq		(tagSYMBOL* pP1, tagSYMBOL* pP2);
void TreatLongSeq		(tagSYMBOL* pP3, tagSYMBOL* pP4);
/* ======================================================================= */
/* PHY LAYER functions (02.09.2003) */
/* ======================================================================= */
/* TX PART */
/* ----------------------------------------------------------------------- */
void TxFillBuf			(tagSYMBOL* pSymb);
void TxRequired			(int nSymb, int nDataRate);
void TxSendSymb			(void);
void TxStop				(void);
/* ----------------------------------------------------------------------- */
/* RX PART */
/* ----------------------------------------------------------------------- */
void RxRecvSymb			(tagSYMBOL* pSymb);
void RxRecvSmpls		(tagMDM_SAMPLES* pSmpls);
/* ----------------------------------------------------------------------- */
/* PHY_EVENT_GEN */
/* ----------------------------------------------------------------------- */
void AddSmplsToSymb		(tagMDM_SAMPLES* pSmpls, int nNum);
int  IsPhyMdmAct		(void);
int  IsTxReq			(double *pSymb, int* pDataRate);
void MDM_SYNC			(void);
void MDM_GET_STATE		(void);
void MDM_SKIP_POLL		(void);
/* ======================================================================= */
#endif /* __PROCSYS_EXP__ */
/* ======================================================================= */
