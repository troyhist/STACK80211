/* =======================================================================
// coder interface ver. 1.0 (pure C)
// provides comfortable interface to coder DLL library
// -----------------------------------------------------------------------
// MD: 22.09.2003
// LE: 22.09.2003
// -----------------------------------------------------------------------
//   Compiler: MS VC 6.0
// Aplication: WIN32
//  Edited by: Serguei Kolevatov
// =======================================================================
// coder_intface.h: implementation of service functions (header)
// ======================================================================= */
#include "c_vector.h"		/* VectorCollapseBits */
#include "coder_intface.h"
#include "proc_phyctrl.h"	/* PCGetNdbpsByDataRate */
/* ======================================================================= */
void InstanceErrExit(char* sFuncName, const char* sErrFormat, ...);
/* ======================================================================= */
/* private functions */
/* ======================================================================= */
void CIFinit_Tx(tagCODERIF* pCIF)
	{
    pCIF->txMod = malloc ( sizeof(tx_11a_data) );
    pCIF->prmTx = malloc ( sizeof(Prmtx_11a_data) );

	tx_11a_data_Construct ( pCIF->txMod );
	tx_11a_data_InitNet ( pCIF->txMod );

	pCIF->prmTx->GlobalSeed	=   123;
	pCIF->prmTx->RunLength	= 	1;
	pCIF->prmTx->Dbg_Sink	=	0;

    tx_11a_data_SetParams ( pCIF->txMod, pCIF->prmTx );
  	tx_11a_data_Init ( pCIF->txMod );
	}
/* ----------------------------------------------------------------------- */
void CIFinit_Rx(tagCODERIF* pCIF)
	{
    pCIF->rxMod = malloc ( sizeof(rx_11a_data) );
    pCIF->prmRx = malloc ( sizeof(Prmrx_11a_data) );

	rx_11a_data_Construct ( pCIF->rxMod );

	rx_11a_data_InitNet ( pCIF->rxMod );

	pCIF->prmRx->GlobalSeed =   123;
	pCIF->prmRx->RunLength	= 	1;
	pCIF->prmRx->Dbg_Sink	=	0;
	pCIF->prmRx->VitPathLen	=	4;

    rx_11a_data_SetParams ( pCIF->rxMod, pCIF->prmRx );

  	rx_11a_data_Init ( pCIF->rxMod );
	}
/* ======================================================================= */
/* "class" functions */
/* ======================================================================= */
void CIF(tagCODERIF* pCIF)
	{ pCIF->m_bInited=0; }
/* ----------------------------------------------------------------------- */
void CIF_(tagCODERIF* pCIF)
	{ if(!CIFIsInited(pCIF)) return;
	pCIF->m_bInited=0; }
/* ----------------------------------------------------------------------- */
int  CIFIsInited (tagCODERIF* pCIF)
	{ return(pCIF->m_bInited); }
/* ----------------------------------------------------------------------- */
void CIFSanity(tagCODERIF* pCIF, char* sFuncName)
	{
	if(NULL==pCIF) InstanceErrExit(sFuncName, "sanity check failed");
	if(0==pCIF->m_bInited) InstanceErrExit(sFuncName, "sanity check failed, not inited");
	}
/* ======================================================================= */
/* public functions */
/* ======================================================================= */
int CIFInit(tagCODERIF* pCIF)
	{
	CIFinit_Tx(pCIF);
	CIFinit_Rx(pCIF);
	pCIF->m_bInited=1;
	return(0);
	}
/* ======================================================================= */
int CIFConvDataRateToRateSelector	(tagCODERIF* pCIF, int nDataRate)
	{
	/* (22.09.2003, 20.10.2003) */
	int nRes=-1; /* added this '-1' value to prevent warning message */
	switch(nDataRate)
		{
		case 6: nRes=1; break;
		default:
			InstanceErrExit("CIFConvDataRateToRateSelector()",
				"unsupported rate %d Mbits/sec", nDataRate);
			break;
		}
	return(nRes);
	}
/* ======================================================================= */
int CIFDecodeOFDMSymb	(tagCODERIF* pCIF, tagBITSEQ* pBits,
						 tagSYMBOL* pSymb, int nRate)
	{
	/* ------------------------------------------------------------------- */
	/* decodes OFDM symbol (22.09.2003) */
	/* ------------------------------------------------------------------- */
	unsigned char pBytes[216];
	int i;
	int nNdbps;
	int nRateSelector;
	/* ------------------------------------------------------------------- */
	CIFSanity(pCIF, "CIFDecodeOFDMSymb()");
	/* ------------------------------------------------------------------- */
	nRateSelector=CIFConvDataRateToRateSelector(pCIF, nRate);
	nNdbps=PCGetNdbpsByDataRate(NULL, nRate);
	/* ------------------------------------------------------------------- */
	pCIF->rxMod->M_DataRate_Rx->mData[0]=nRateSelector;
	/* ------------------------------------------------------------------- */
	for (i=0; i<80; i++)
		{
		pCIF->rxMod->Mem_RF_RxRe->mData[i]=(int)pSymb->m_pData[i].R;
		pCIF->rxMod->Mem_RF_RxIm->mData[i]=(int)pSymb->m_pData[i].I;
		}
	/* ------------------------------------------------------------------- */
	rx_11a_data_Run(pCIF->rxMod);
	/* ------------------------------------------------------------------- */
	for (i=0; i<nNdbps; i++)
		pBytes[i]=pCIF->rxMod->Mem_MAC_RxData->mData[i];
	/* ------------------------------------------------------------------- */
	VectorExpandBitsUC(pBits->m_pData, pBytes, nNdbps*8);
	pBits->m_nSize=nNdbps*8;
	/* ------------------------------------------------------------------- */
	return(0);
	}
/* ======================================================================= */
int CIFEncodeOFDMSymb	(tagCODERIF* pCIF, tagSYMBOL* pSymb,
						 tagBITSEQ* pBits, int nRate)
	{
	/* ------------------------------------------------------------------- */
	/* encodes OFDM symbol (22.09.2003) */
	/* ------------------------------------------------------------------- */
	unsigned char pBytes[27];
	int i;
	int nNdbps;
	int nRateSelector;
	/* ------------------------------------------------------------------- */
	CIFSanity(pCIF, "CIFEncodeOFDMSymb()");
	/* ------------------------------------------------------------------- */
	nRateSelector=CIFConvDataRateToRateSelector(pCIF, nRate);
	nNdbps=PCGetNdbpsByDataRate(NULL, nRate);
	/* ------------------------------------------------------------------- */
	VectorCollapseBitsUC(pBytes, pBits->m_pData, pBits->m_nSize);
	/* ------------------------------------------------------------------- */
	/* data to encode */
	/* ------------------------------------------------------------------- */
	for (i=0; i<nNdbps; i++)
		pCIF->txMod->Mem_MAC_TxData->mData[i]=pBytes[i];
	pCIF->txMod->M_DataRateSel->mData[0]=nRateSelector;
	/* ------------------------------------------------------------------- */
	tx_11a_data_Run(pCIF->txMod);
	/* ------------------------------------------------------------------- */
	/* result */
	for (i=0; i<80; i++)
		{
		pSymb->m_pData[i].R=pCIF->txMod->Mem_RF_TxRe->mData[i];
		pSymb->m_pData[i].I=pCIF->txMod->Mem_RF_TxIm->mData[i];
		}
	/* ------------------------------------------------------------------- */
	return(0);
	}
/* ======================================================================= */
int CIFGetVitPathLen	(tagCODERIF* pCIF)
	{
	/* ------------------------------------------------------------------- */
	/* returns Viterbi Path Length (24.09.2003) */
	/* ------------------------------------------------------------------- */
	CIFSanity(pCIF, "CIFGetVitPathLen()");
	/* ------------------------------------------------------------------- */
	return(pCIF->prmRx->VitPathLen);
	}
/* ======================================================================= */
