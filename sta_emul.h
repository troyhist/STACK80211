/* =======================================================================
// STA procedures ver. 1.0
// structs and constants definition
// -----------------------------------------------------------------------
// MD: 30.07.2003
// LE: 08.09.2003
// -----------------------------------------------------------------------
// Compiler: MS VC 6.0
// =======================================================================
// procSTA.h: implementation of service functions (header)
// ======================================================================= */
#ifndef __STA_EMUL__
#define __STA_EMUL__
/* ======================================================================= */
#include "FileCtrl.h"					/* (08.09.2003) */
#include "proc_mac.h"
#include "proc_phyctrl.h"
#include "proc_phy.h"
#include "emul_cfg.h"					/* (10.09.2003) */
#include "..\\MDM_ACCESS\\mdm_cli.h"	/* (09.09.2003) */
#include "lib_path.h"					/* (09.10.2003) */
#include SOCKETOP_H						/* (08.10.2003) */
/* ======================================================================= */
/* configuration of STA (13.08.2003) */
/* ======================================================================= */
typedef struct
	{
	/* ------------------------------------------------------------------- */
	/* service data */
	int m_bInited;				/* flag "structure is initialized" */
	tagSOCKETOP		m_tSOP;		/* socket for output (08.10.2003) */
	tagEMULCFG		m_tECFG;	/* configuration (06.08.2003) */
	tagCMDM_CLI		m_tMDM_CLI;	/* MDM client */
	tagCFCT_CLASS m_tFLCTRL;	/* file control (08.09.2003) */
	/* ------------------------------------------------------------------- */
	/* protocol stack */
	tagLMAC			m_tLMAC;	/* Layer MAC (02.09.2003) */
	tagLPC_ST		m_tLPC;		/* Layer PHY_CTRL state (02.09.2003) */
	tagLPHY_ST		m_tLPHY;	/* Layer PHY state (04.09.2003) */
	} tagSTACFG;
/* ======================================================================= */
/* "class" functions */
/* ======================================================================= */
void STA			(tagSTACFG* pSTA, tagMAC_ST* pST);	/* constructor */
void STA_			(tagSTACFG* pSTA, tagMAC_ST* pST);	/* destructor (24.09.2003) */
int	 STAIsInited	(tagSTACFG* pSTA);
void STASanity		(tagSTACFG* pSTA, char* sFuncName);
/* ======================================================================= */
/* STA public SERVICE functions */
/* ======================================================================= */
int STAInit		(tagSTACFG* pSTA, tagMAC_ST* pST, char* sCFGFile, int (*pLogOutputFunc) (char*, char*));		/* initialization of the library (23.07.2003) */
int	STALoadCFG	(tagEMULCFG* pCFG, char* sCFGFile);						/* load STA CFG from given file (06.08.2003) */
int STASpy		(tagSTACFG* pSTA, int nSpyID, int nSec, int nMSec, char* sDir, char* sEventName, int nPars, int* pPars);	/* events-spy function (14.08.2003) */
int STASpyEv	(tagSTACFG* pSTA, int nSpyID, int nSec, int nMSec, char* sPrName, char* sStName, char* sEvName, int nPars, int* pPars); /* spy function: events control (17.09.2003) */
/* ======================================================================= */
int STAGetDataToTx	(tagSTACFG* pSTA, tagMACADDR* pDA, tagFLATARRAY* pArray);	/* 10.09.2003 */
int STAGetTxRate	(tagSTACFG* pSTA);	/* returns Tx Rate (18.09.2003) */
/* ======================================================================= */
#endif /* __STA_EMUL__ */
/* ======================================================================= */
