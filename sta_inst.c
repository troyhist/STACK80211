/* =======================================================================
// Station Instance functions 2.0.0
// -----------------------------------------------------------------------
//  Description: this function performs initialization of the STA
// Uses library: ...
// -----------------------------------------------------------------------
//  Made date: 14.10.2003
//  Last edit: 20.10.2003
// -----------------------------------------------------------------------
//   Compiler: MS VC 6.0
// Aplication: ANSI, WIN32
//  Edited by: Serguei Kolevatov
// =======================================================================
// sta_inst.c: implementation of service functions
// ======================================================================= */
/* ======================================================================= */
/* the following lines have to be included into generated C-file */
/* ======================================================================= */
/*
#include "..\\sta_emul.h"	// tagSTACFG
extern tagSTACFG MC;
void InstanceInit(void);	// calls STAInit (28.07.2003, 15.09.2003)
#define MAXPropDelay	(0)
*/
/* ======================================================================= */
#include <stdlib.h>			/* getenv (20.10.2003) */
#include <assert.h>			/* assert (20.10.2003) */
#include "sta_emul.h"		/* tagSTACFG (10.09.2003) */
#include "sta_inst.h"		/* instance config (07.08.2003) */
/* ======================================================================= */
tagSTAINST tSTAINST={{STAINST_IDSEQ0, STAINST_IDSEQ1}, 0, "", ""};
/* ======================================================================= */
tagSTACFG MC;		/* MAC cfg */
tagMAC_ST MS;		/* MAC current state */
/* ----------------------------------------------------------------------- */
tagCLOGR CL_STA;		/* STA global logger (12.09.2003) */
/* ======================================================================= */
int  InstanceOutputFunc(char* sFilename, char* sMsg);
void IntErrExit(char* sFuncName, char* sErr);
/* ======================================================================= */
int InstanceGetConfig(char* sInstanceCfgFile, char* sInstanceLogFile)
{
/* -----------------------------------------------------------------------
// Description: returns configuration of an instance
//   Parametrs: ...
//      Return: 0 - ok, (-1) - error
// -----------------------------------------------------------------------
//   MD: 20.10.2003
//   LE: 20.10.2003
// TODO: test it
// ----------------------------------------------------------------------- */
char *sEnvCfg, *sEnvLog;
int bInitCfg=0, bInitLog=0;
/* ----------------------------------------------------------------------- */
assert(sInstanceCfgFile);	sEnvCfg=getenv(STAINST_VARCFG);	
assert(sInstanceLogFile);	sEnvLog=getenv(STAINST_VARLOG);
/* ----------------------------------------------------------------------- */
if(sEnvCfg)
	{ strcpy(sInstanceCfgFile, sEnvCfg); bInitCfg=1; }
else if(tSTAINST.bInited)
	{ strcpy(sInstanceCfgFile, tSTAINST.sStaCfgFile); bInitCfg=1;  }
/* ----------------------------------------------------------------------- */
if(sEnvLog)
	{ strcpy(sInstanceLogFile, sEnvLog); bInitLog=1; }
else if(tSTAINST.bInited)
	{ strcpy(sInstanceLogFile, tSTAINST.sStaLogFile); bInitLog=1;  }
/* ----------------------------------------------------------------------- */
return((1==bInitCfg && 1==bInitLog)?0:(-1));
}
/* ======================================================================= */
void InstanceInit(void)
{
/* ----------------------------------------------------------------------- */
/* Inits instance (29.07.2003, 08.10.2003) */
/* ----------------------------------------------------------------------- */
char sInstanceCfgFile[256];
char sInstanceLogFile[256];
static int bInited=0;
int nErr;
/* ----------------------------------------------------------------------- */
nErr=InstanceGetConfig(sInstanceCfgFile, sInstanceLogFile);
/* ----------------------------------------------------------------------- */
if(bInited) IntErrExit("InstanceInit()", "repeated initialization");
if(-1==nErr)
	{ IntErrExit("InstanceInit()",
	"cannot get configuration of the instance"); return; }
/* ----------------------------------------------------------------------- */
bInited=1;
MC.m_bInited=0;		/* necessary (08.10.2003) */
/* ----------------------------------------------------------------------- */
CLOGR(&CL_STA);		/* STA global logger (12.09.2003) */
CLOGRInit(&CL_STA, &InstanceOutputFunc, sInstanceLogFile, "STA logger");
/* ----------------------------------------------------------------------- */
STA(&MC, &MS);
STAInit(&MC, &MS, sInstanceCfgFile, &InstanceOutputFunc); /* adjust this parameter (30.07.2003) */
/* ----------------------------------------------------------------------- */
CMDM_CLIStart (&MC.m_tMDM_CLI);		/* starts MDM client (10.09.2003) */
}
/* ======================================================================= */
int InstanceOutputFunc(char* sFilename, char* sMsg)
{
/* ----------------------------------------------------------------------- */
/* (08.10.2003, 14.10.2003) */
/* ----------------------------------------------------------------------- */
char *sHostName;
char sXMLOutput[256+8192+256];
int nPort, nMaxRecon;
int nSentBytes, nErr;
static int bConnected=0;
static int nReconTries=0;
tagSOCKETOP* pSOP;
/* ----------------------------------------------------------------------- */
if(0==STAIsInited(&MC))
	return(0);
/* ----------------------------------------------------------------------- */
sHostName	=MC.m_tECFG.m_sLogSocketIp;
nPort		=MC.m_tECFG.m_nLogSocketPort;
nMaxRecon	=MC.m_tECFG.m_nLogSocketRecon;
pSOP		=&MC.m_tSOP;
/* ----------------------------------------------------------------------- */
switch(MC.m_tECFG.m_nLogOutput)
	{
	/* ------------------------------------------------------------------- */
	/* NONE */
	case 0: break;
	/* ------------------------------------------------------------------- */
	/* file */
	case 1:
		{
		FILE* fp;
		if (NULL==(fp=fopen(sFilename, "a+t"))) return(CLOGR_ERROR);
		fprintf(fp, "%s\n", sMsg); fclose(fp);
		return(0);
		}
		break;
	/* ------------------------------------------------------------------- */
	/* socket */
	case 2:
		{
		/* --------------------------------------------------------------- */
		/* not inited */
		/* --------------------------------------------------------------- */
		if(0==CSOPIsInstanceCreated(pSOP)) break;
		if(0==CSOPIsInited(pSOP))
			{ CSOPInitDef(pSOP); }
		/* --------------------------------------------------------------- */
		/* not connected, tries to re-/connect */
		/* --------------------------------------------------------------- */
		if(0==bConnected && nReconTries>nMaxRecon) break;
		if(0==bConnected)
			{
			printf("try %d, connecting to %s:%d ... ",
				nReconTries, sHostName, nPort);
			nReconTries++;
			nErr=CSOPConnectByHostName(pSOP, sHostName, nPort);
			if(nErr)
				{ printf("err: %d.%s\n", CSOPGetExtErrCode(pSOP),
				CSOPGetExtErrText(pSOP)); break; }
			bConnected=1; printf("ok\n");
			}
		/* --------------------------------------------------------------- */
		sprintf(sXMLOutput,
			"<LOG>"
			"<FILE>%s</FILE>"
			"<DATA>%s</DATA>"
			"</LOG>", sFilename, sMsg);
		/* --------------------------------------------------------------- */
		nErr=CSOPSockSend(pSOP, sXMLOutput, strlen(sXMLOutput), &nSentBytes);
		/* --------------------------------------------------------------- */
		/* error occured, disconnecting */
		/* --------------------------------------------------------------- */
		if(nErr)
			{ printf("tx.err: %d.%s\n", CSOPGetExtErrCode(pSOP),
				CSOPGetExtErrText(pSOP));
			CSOPDisconnect(pSOP); bConnected=0; }
		/* --------------------------------------------------------------- */
		}
		break;
	/* ------------------------------------------------------------------- */
	default: break;		// NONE
	}
return(0);
}
/* ======================================================================= */
/* Global Function */
/* ======================================================================= */
void IntErrExit(char* sFuncName, char* sErr)
	{
	fprintf(stderr, "ERROR: %s, %s (stderr)\n", sFuncName, sErr);
	fprintf(stdout, "ERROR: %s, %s (stdout)\n", sFuncName, sErr);
	exit(0);
	}
/* ======================================================================= */
/* System functions */
/* ======================================================================= */
void InstanceErrExit(char* sFuncName, const char* sErrFormat, ...)
{
/* ----------------------------------------------------------------------- */
char sErr[8192];		/* fixed buf. size, may be changed to var. */
char sBuf[8192];
va_list arglist;
va_start(arglist, sErrFormat);
/* ----------------------------------------------------------------------- */
vsprintf(sErr, sErrFormat, arglist);
sprintf	(sBuf, "func. %s: %s", sFuncName, sErr);
CLOGRAdd(&CL_STA, "InstanceErrExit(..)", sBuf);
/* ----------------------------------------------------------------------- */
STA_(&MC, &MS);
/* ----------------------------------------------------------------------- */
CLOGR_	(&CL_STA);
/* ----------------------------------------------------------------------- */
IntErrExit("InstanceErrExit(..)", sBuf);
}
/* ======================================================================= */
