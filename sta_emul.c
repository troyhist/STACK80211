/* =======================================================================
// STA EMULATION procedures ver. 1.0
// structs and constants definition
// -----------------------------------------------------------------------
// MD: 02.09.2003
// LE: 02.09.2003
// -----------------------------------------------------------------------
// Compiler: MS VC 6.0
// =======================================================================
// sta_emul.c: implementation of service functions
// ======================================================================= */
#include <stdlib.h>			/* rand */
#include <string.h>			/* strcat (18.09.2003) */
#include <stdio.h>
#include <time.h>			/* time */
/* ----------------------------------------------------------------------- */
#ifdef __STDC__
#include <stdarg.h>			/* variable argument list */
#elif _WIN32
#include <stdarg.h>			/* variable argument list */
#else
#include <varargs.h>		/* Required for UNIX V compatibility */
#endif
/* ----------------------------------------------------------------------- */
#include "emul_cfg.h"		/* tagSTACFG (10.09.2003) */
#include "procpkt.h"		/* PktConvStrToAddr, sizeof(tagACK) */
#include "sta_emul.h"		/* (10.09.2003) */
#include "lib_path.h"		/* (09.10.2003) */
#include STRGETVAL_H		/* StrGetVal (02.09.2003) */
/* ======================================================================= */
void InstanceErrExit(char* sFuncName, const char* sErrFormat, ...);
/* ======================================================================= */
extern tagCLOGR CL_STA;		/* STA global logger (12.09.2003) */
/* ======================================================================= */
char glSETB[256];			/* StaEmulTempBuf - SETB */
char glSETB8K[8192];		/* StaEmulTempBuf - SETB 8KB*/
/* ======================================================================= */
/* CSTA: implemented in a same way as C++ classes */
/* ======================================================================= */
int STAIsInited(tagSTACFG* pSTA)
	{return(pSTA->m_bInited);}
/* ======================================================================= */
void STASanity(tagSTACFG* pSTA, char* sFuncName)
{
if(!STAIsInited(pSTA))
	InstanceErrExit(sFuncName, "sanity check failed");
}
/* ======================================================================= */
/* PUBLIC SERVICE FUNCTIONS */
/* ======================================================================= */
void STA(tagSTACFG* pSTA, tagMAC_ST* pST)
	{
/* -----------------------------------------------------------------------
// Description: constructor
//   Parametrs: ...
//      Return: <none>
// -----------------------------------------------------------------------
//   MD: 29.07.2003
//   LE: 20.10.2003
// TODO: none
// ----------------------------------------------------------------------- */
	CLOGRAdd (&CL_STA, "STA()", "constructor");
	pSTA->m_bInited=0;
	/* ------------------------------------------------------------------- */
	/* constructing (05.09.2003) */
	CSOP	(&pSTA->m_tSOP);				/* socket (08.10.2003) */
	CLOGR	(&pSTA->m_tECFG.m_tLSDL);		/* SDL logger (20.10.2003) */
	CLOGR	(&pSTA->m_tECFG.m_tLSPYS[0]);	/* SPY MAC-PHYCTRL log */
	CLOGR	(&pSTA->m_tECFG.m_tLSPYS[1]);	/* SPY ... log */
	CLOGR	(&pSTA->m_tECFG.m_tLSPYS[2]);	/* SPY ... log */
	CFCT	(&pSTA->m_tFLCTRL);				/* file control - L3*/
	CMDM_CLI(&pSTA->m_tMDM_CLI);			/* 09.09.2003 */
	/* ------------------------------------------------------------------- */
	/* stack */
	MAC		(&pSTA->m_tLMAC, pST);
	PCTRL	(&pSTA->m_tLPC);
	PHY		(&pSTA->m_tLPHY);
	/* ------------------------------------------------------------------- */
	}
/* ======================================================================= */
void STA_(tagSTACFG* pSTA, tagMAC_ST* pST)
	{ /* (24.09.2003, 20.10.2003) */
	if(!STAIsInited(pSTA)) return;
	pSTA->m_bInited=0;
	/* ------------------------------------------------------------------- */
	CSOP_	(&pSTA->m_tSOP);
	CLOGR_	(&pSTA->m_tECFG.m_tLSDL);		/* SDL logger (20.10.2003) */
	CLOGR_	(&pSTA->m_tECFG.m_tLSPYS[0]);	/* SPY MAC-PHYCTRL log */
	CLOGR_	(&pSTA->m_tECFG.m_tLSPYS[1]);	/* SPY ... log */
	CLOGR_	(&pSTA->m_tECFG.m_tLSPYS[2]);	/* SPY ... log */
	CFCT_	(&pSTA->m_tFLCTRL);				/* file control - L3*/
	CMDM_CLI_(&pSTA->m_tMDM_CLI);
	/* ------------------------------------------------------------------- */
	MAC_	(&pSTA->m_tLMAC, pST);
	PCTRL_	(&pSTA->m_tLPC);
	PHY_	(&pSTA->m_tLPHY);
	/* ------------------------------------------------------------------- */
	}
/* ======================================================================= */
int STAInit(tagSTACFG* pSTA, tagMAC_ST* pST, char* sCFGFile,
			int (*pLogOutputFunc) (char*, char*))
{
/* -----------------------------------------------------------------------
// Description: inits internal data, has to be called first
//   Parametrs: ...
//      Return: 0 - ok, (-1) - error
// -----------------------------------------------------------------------
//   MD: 23.07.2003
//   LE: 14.10.2003 - added pLogOutputFunc as param.
//		 20.10.2003 - CL_SDL logger put in class as m_tLSDL
// TODO: test timeouts
// ----------------------------------------------------------------------- */
CLOGRAdd (&CL_STA, "STAInit()", "initialization");
/* ----------------------------------------------------------------------- */
if (STAIsInited(pSTA))
	InstanceErrExit("STAInit()", "Already inited !!!");
/* ----------------------------------------------------------------------- */
srand((unsigned)time(NULL));	/* random seed (31.07.2003) */
/* ----------------------------------------------------------------------- */
/* service data */
/* ----------------------------------------------------------------------- */
STALoadCFG(&pSTA->m_tECFG, sCFGFile);
/* ----------------------------------------------------------------------- */
CLOGRInit(&pSTA->m_tECFG.m_tLSDL,	  pLogOutputFunc, pSTA->m_tECFG.m_sLGRSDL, "EXPORT_SDL detailed logger");
CLOGRInit(&pSTA->m_tECFG.m_tLSPYS[0], pLogOutputFunc, pSTA->m_tECFG.m_sSPYS[0], "SPY MAC-PHY_CTRL logger");
CLOGRInit(&pSTA->m_tECFG.m_tLSPYS[1], pLogOutputFunc, pSTA->m_tECFG.m_sSPYS[1], "SDL-MAC EVENTS");
CLOGRInit(&pSTA->m_tECFG.m_tLSPYS[2], pLogOutputFunc, pSTA->m_tECFG.m_sSPYS[2], "MAC LAYER EVENTS");
/* ----------------------------------------------------------------------- */
/* inits */
//CSOPInitDef	(&pSTA->m_tSOP);		// DEL IT (14.10.2003)
CFCTInit	(&pSTA->m_tFLCTRL, pSTA->m_tECFG.m_sFLEvIDs, pSTA->m_tECFG.m_sFLEvnts);
MACInit		(&pSTA->m_tLMAC, pST, pSTA->m_tECFG.m_nTxRate*(1e6),
			 pSTA->m_tECFG.m_sLGRsMAC, sCFGFile, pLogOutputFunc);
PHYInit		(&pSTA->m_tLPHY);
PCInit		(&pSTA->m_tLPC);
CMDM_CLIInit(&pSTA->m_tMDM_CLI, sCFGFile, 0);	/* 09.09.2003 */
/* ----------------------------------------------------------------------- */
pSTA->m_bInited=1;
/* ----------------------------------------------------------------------- */
CLOGRAdd (&CL_STA, "STAInit()", "\n"
		  "m_nACKTimeout         = %d\n"
		  "m_nCTSTimeout         = %d\n"
		  "m_nTxTimeACK          = %d\n"
		  "m_nTxTimeCTS          = %d\n"
		  "m_aAirPropagationTime = %d\n"
		  "m_aSlotTime           = %d\n"
		  "m_nSIFS               = %d\n"
		  "m_nPIFS               = %d\n"
		  "m_nDIFS               = %d\n"
		  "m_nEIFS               = %d\n",
		  pSTA->m_tLMAC.m_tTC.m_nACKTimeout,	pSTA->m_tLMAC.m_tTC.m_nCTSTimeout,
		  pSTA->m_tLMAC.m_tTC.m_nTxTimeACK,		pSTA->m_tLMAC.m_tTC.m_nTxTimeCTS,
		  pSTA->m_tLMAC.m_tPHY.m_aAirPropagationTime,
		  pSTA->m_tLMAC.m_tPHY.m_aSlotTime,
		  pSTA->m_tLMAC.m_tTC.m_nSIFS,			pSTA->m_tLMAC.m_tTC.m_nPIFS,
		  pSTA->m_tLMAC.m_tTC.m_nDIFS,			pSTA->m_tLMAC.m_tTC.m_nEIFS);
/* ----------------------------------------------------------------------- */
return(0);
}
/* ======================================================================= */
int STALoadCFG(tagEMULCFG* pCFG, char* sCFGFile)
{
/* -----------------------------------------------------------------------
// Description: load STA CFG from given file (06.08.2003)
//   Parametrs: ...
//      Return: 0 - ok, (-1) - error
// -----------------------------------------------------------------------
//   MD: 06.08.2003
//   LE: 10.09.2003
// TODO: test it
// ----------------------------------------------------------------------- */
char sBuf[4096];
FILE* fp;
/* ----------------------------------------------------------------------- */
CLOGRAdd (&CL_STA, "STALoadCFG(..)", "load CFG from file %s", sCFGFile);
/* ----------------------------------------------------------------------- */
fp=fopen(sCFGFile, "rt");
if (fp==NULL)
	InstanceErrExit("STALoadCFG(..)", "cann't open cfg-file %s", sCFGFile);
/* ----------------------------------------------------------------------- */
fread(sBuf, 1, sizeof(sBuf), fp); fclose(fp);
/* ----------------------------------------------------------------------- */
#define STA_PAREE(_n_)	InstanceErrExit("STALoadCFG(..)", "parameter %s not found", _n_)
#define STA_GV(_n_)		StrGetValue(sBuf, _n_, glSETB)
#define STA_PARSE(_n_)	if(!STA_GV(_n_)) STA_PAREE(_n_)
/* ----------------------------------------------------------------------- */
/* process STA cfg */
STA_PARSE("FragSize");	else { sscanf(glSETB, "%d", &pCFG->m_nFragSize); }
STA_PARSE("TX_RATE");	else { sscanf(glSETB, "%d", &pCFG->m_nTxRate); }
STA_PARSE("MAC_MAX_CW_VALUE");	else { sscanf(glSETB, "%d", &pCFG->m_nMaxCWVal); }
/* ----------------------------------------------------------------------- */
STA_PARSE("SCENARIO");			else { sscanf(glSETB, "%s", &pCFG->m_sFLEvnts); }
STA_PARSE("EVENT_ID_LIST");		else { sscanf(glSETB, "%s", &pCFG->m_sFLEvIDs); }
/* ----------------------------------------------------------------------- */
STA_PARSE("STA_NAME");	else { sscanf(glSETB, "%s", &pCFG->m_sStaName); }
STA_PARSE("L3I_ADDR");	else { sscanf(glSETB, "%s", &pCFG->m_sL3IA); }
STA_PARSE("L3O_MPDU");	else { sscanf(glSETB, "%s", &pCFG->m_sL3O_MPDU); }
STA_PARSE("L3I");		else { sscanf(glSETB, "%s", &pCFG->m_sL3[0]); }
STA_PARSE("L3O");		else { sscanf(glSETB, "%s", &pCFG->m_sL3[1]); }
STA_PARSE("LPCI");		else { sscanf(glSETB, "%s", &pCFG->m_sLPC[0]); }
STA_PARSE("LPCO");		else { sscanf(glSETB, "%s", &pCFG->m_sLPC[1]); }
STA_PARSE("SPY0");		else { sscanf(glSETB, "%s", &pCFG->m_sSPYS[0]); }
STA_PARSE("SPY1");		else { sscanf(glSETB, "%s", &pCFG->m_sSPYS[1]); }
STA_PARSE("SPY2");		else { sscanf(glSETB, "%s", &pCFG->m_sSPYS[2]); }
/* ----------------------------------------------------------------------- */
/* LOGGING (30.09.2003) */
STA_PARSE("LOG_OUTPUT");	else { sscanf(glSETB, "%d", &pCFG->m_nLogOutput); }
STA_PARSE("LOG_SOCKET_IP");	else { sscanf(glSETB, "%s", &pCFG->m_sLogSocketIp); }
STA_PARSE("LOG_SOCKET_PORT");	else { sscanf(glSETB, "%d", &pCFG->m_nLogSocketPort); }
STA_PARSE("LOG_SOCKET_RECON");	else { sscanf(glSETB, "%d", &pCFG->m_nLogSocketRecon); }
/* ----------------------------------------------------------------------- */
/* LOGGRES */
STA_PARSE("SDL_LOGGER");	else { sscanf(glSETB, "%s", &pCFG->m_sLGRSDL); }
STA_PARSE("MAC_LOGGER");	else { sscanf(glSETB, "%s", &pCFG->m_sLGRsMAC[0]); }
STA_PARSE("MAC_LOGGER_IO");	else { sscanf(glSETB, "%s", &pCFG->m_sLGRsMAC[1]); }
STA_PARSE("MAC_LOGGER_ST");	else { sscanf(glSETB, "%s", &pCFG->m_sLGRsMAC[2]); }
/* ----------------------------------------------------------------------- */
CLOGRAdd(&CL_STA, "STALoadCFG(..)",
		"cfg dump:\n\n"
		"Station       : %s\n"
		"CFG:\n"
		"m_nFragSize   = %d [bytes]\n"
		"m_nTxRate     = %d [Mbits/sec]\n"
		"m_nMacCWVal   = %d [time slots, '-1' = no limitation]\n"
		"Scenario      = %s\n"
		"Events IDs    = %s\n"
		"L3I address   = %s\n"
		"L3O MPDUs     = %s (template)\n"
		"L3[0] input   = %s\n"
		"L3[1] output  = %s\n"
		"LPC[0] input  = %s\n"
		"LPC[1] output = %s\n"
		"SPY [0]       = %s\n"
		"SPY [1]       = %s\n"
		"SPY [2]       = %s\n\n"
		"SDLLGR        = %s\n"
		"MACLOGRS[0]   = %s\n"
		"MACLOGRS[1]   = %s\n"
		"MACLOGRS[2]   = %s\n\n",
		pCFG->m_sStaName,
		pCFG->m_nFragSize,	pCFG->m_nTxRate,	pCFG->m_nMaxCWVal,
		pCFG->m_sFLEvnts,	pCFG->m_sFLEvIDs,
		pCFG->m_sL3IA,		pCFG->m_sL3O_MPDU,	pCFG->m_sL3[0],
		pCFG->m_sL3[1],		pCFG->m_sLPC[0],	pCFG->m_sLPC[1],
		pCFG->m_sSPYS[0],	pCFG->m_sSPYS[1],	pCFG->m_sSPYS[2],
		pCFG->m_sLGRSDL,
		pCFG->m_sLGRsMAC[0],pCFG->m_sLGRsMAC[1],pCFG->m_sLGRsMAC[2]);
/* ----------------------------------------------------------------------- */
return(0);
#undef STA_PAREE
}
/* ======================================================================= */
int STASpy(tagSTACFG* pSTA, int nSpyID, int nSec, int nMSec, char* sDir, 
		   char* sEventName, int nPars, int* pPars)
{
/* -----------------------------------------------------------------------
// Description: spy function: events between blocks
//   Parametrs: nPars	- no. of params
//              pPars   - params array
//      Return: 0 - ok
// -----------------------------------------------------------------------
//   MD: 14.08.2003
//   LE: 18.09.2003
// TODO: test it
// ----------------------------------------------------------------------- */
int i;
CLOGRAdd (&CL_STA, "STASpy(..)", "spy is called (nSpyID=%d)", nSpyID);
STASanity(pSTA, "STASpy(..)");
/* ----------------------------------------------------------------------- */
*glSETB8K=0;
if(nPars>0)
	{
	sprintf(glSETB8K, "(");
	for(i=0; i<nPars; i++)
		{ sprintf(glSETB, "%d%s", pPars[i], (i+1<nPars)?", ":"");
		strcat(glSETB8K, glSETB); }
	strcat(glSETB8K, ")");
	}
/* ----------------------------------------------------------------------- */
if (nSpyID<0 || nSpyID>2) nSpyID=0;
CLOGRAddDirect(&pSTA->m_tECFG.m_tLSPYS[nSpyID], "%4d.%01d\t%s:\t\t%s%s", 
			   nSec, nMSec/100, sDir, sEventName, glSETB8K);
return(0);
}
/* ======================================================================= */
int STASpyEv(tagSTACFG* pSTA, int nSpyID, int nSec, int nMSec, char* sPrName, 
		   char* sStName, char* sEvName, int nPars, int* pPars)
{
/* -----------------------------------------------------------------------
// Description: spy function: events control
//   Parametrs: ...
//      Return: 0 - ok
// -----------------------------------------------------------------------
//   MD: 17.09.2003
//   LE: 25.09.2003
// TODO: test it
// ----------------------------------------------------------------------- */
int i;
CLOGRAdd (&CL_STA, "STASpyEv(..)", "spy is called (nSpyID=%d, nPars=%d)", nSpyID, nPars);
STASanity(pSTA, "STASpyEv(..)");
/* ----------------------------------------------------------------------- */
*glSETB8K=0;
if(nPars>0)
	{
	sprintf(glSETB8K, "(");
	for(i=0; i<nPars; i++)
		{ sprintf(glSETB, "%d%s", pPars[i], (i+1<nPars)?", ":"");
		strcat(glSETB8K, glSETB); }
	strcat(glSETB8K, ")");
	}
/* ----------------------------------------------------------------------- */
if (nSpyID<0 || nSpyID>2) nSpyID=0;
CLOGRAddDirect(&pSTA->m_tECFG.m_tLSPYS[nSpyID], "%4d.%01d\t%10s: %20s, %10s%s",
			   nSec, nMSec/100, sPrName, sStName, sEvName, glSETB8K);
return(0);
}
/* ======================================================================= */
int STAGetDataToTx(tagSTACFG* pSTA, tagMACADDR* pDA, tagFLATARRAY* pArray)
{
/* -----------------------------------------------------------------------
// Description: transfers user data into pktTx
//   Parametrs: ...
//      Return: 0 - ok
// -----------------------------------------------------------------------
//   MD: 16.07.2003
//   LE: 10.09.2003
// TODO: complete it
// ----------------------------------------------------------------------- */
FILE* fp;
/* ----------------------------------------------------------------------- */
CLOGRAdd (&CL_STA, "STAGetDataToTx()", "get data from L3");
STASanity(pSTA, "STAGetDataToTx()");
/* ----------------------------------------------------------------------- */
/* reads destination address */
/* ----------------------------------------------------------------------- */
if ((fp=fopen(pSTA->m_tECFG.m_sL3IA, "rt"))==NULL)
	InstanceErrExit("STAGetDataToTx()", "cann't open L3 input file w. address (%s)",
	pSTA->m_tECFG.m_sL3IA);
/* ----------------------------------------------------------------------- */
fread(glSETB, 1, sizeof(glSETB), fp); fclose(fp);
PktConvStrToAddr(glSETB, pDA);
/* ----------------------------------------------------------------------- */
CLOGRAdd (&CL_STA, "STAGetDataToTx()", "DA = %s", PktConvAddrToStr(*pDA, glSETB));
/* ----------------------------------------------------------------------- */
/* reads data in pktTx buffer */
/* ----------------------------------------------------------------------- */
if ((fp=fopen(pSTA->m_tECFG.m_sL3[0], "rb"))==NULL)
	InstanceErrExit("STAGetDataToTx()", "cann't open L3 input file (%s)", 
	pSTA->m_tECFG.m_sL3[0]);
/* ----------------------------------------------------------------------- */
fseek(fp,0L,SEEK_END); pArray->nSize=(int)ftell(fp); rewind(fp);
pArray->pData=malloc(pArray->nSize);
if(pArray->pData==NULL) InstanceErrExit("MACGetDataInTx()", "cann't allocate %d bytes", pArray->nSize);
fread(pArray->pData, 1, pArray->nSize, fp); fclose(fp);
/* ----------------------------------------------------------------------- */
return(0);
}
/* ======================================================================= */
int STAGetTxRate	(tagSTACFG* pSTA)
	{
	/* ----------------------------------------------------------------------- */
	/* returns Tx Rate (18.09.2003) */
	/* ----------------------------------------------------------------------- */
	CLOGRAdd (&CL_STA, "STAGetTxRate()", "return Tx Rate");
	STASanity(pSTA, "STAGetTxRate()");
	/* ----------------------------------------------------------------------- */
	return(pSTA->m_tECFG.m_nTxRate);
	}
/* ======================================================================= */
