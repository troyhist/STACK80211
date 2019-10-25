/* =======================================================================
// File_control mechanism ver. 2.0
// reads text events from text file, and returns type of event
// -----------------------------------------------------------------------
// MD: 08.07.2003
// LE: 08.09.2003
// -----------------------------------------------------------------------
// Compiler: MS VC 6.0
// =======================================================================
// file_ctrl.c: implementation of service functions
// ======================================================================= */
#include <string.h>			/* strcmp */
#include <ctype.h>			/* isspace */
#include "FileCtrl.h"
/* ======================================================================= */
void InstanceErrExit(char* sFuncName, char* sErrFormat, ...);	/* error handler */
/* ======================================================================= */
/* "class" functions */
/* ======================================================================= */
void CFCT		(tCFCT* pFCT)
	{ /* constructor (08.09.2003) */
	pFCT->m_bInited=0;
	}
/* ----------------------------------------------------------------------- */
void CFCT_		(tCFCT* pFCT)
	{ if(!CFCTIsInited(pFCT)) return;
	pFCT->m_bInited=0; }
/* ----------------------------------------------------------------------- */
int CFCTIsInited(tCFCT* pFCT)
	{ return(pFCT->m_bInited); }
/* ======================================================================= */
void CFCTSanity	(tCFCT* pFCT, char* sFuncName)
	{
	if(NULL==pFCT)
		InstanceErrExit(sFuncName, "sanity failed, %s", "pFCT==NULL");
	if(0==pFCT->m_bInited)
		InstanceErrExit(sFuncName, "sanity failed, %s", "pFCT is NOT inited");
	}
/* ======================================================================= */
int  CFCTInit	(tCFCT* pFCT, char* sFileEvIDs, char* sFileEvents)
	{
/* -----------------------------------------------------------------------
// Description: loads events IDs and Events List (scenarion) & inits class
//   Parametrs: ...
//      Return: (0) - success, (-1) - error
// -----------------------------------------------------------------------
//   MD: 08.09.2003
//   LE: 11.09.2003
// TODO: test it
// ----------------------------------------------------------------------- */
	pFCT->m_bInited=1;				/* inited */
	pFCT->m_nSize=0;				/* number of elements used in the events array */
	pFCT->m_nNextEventIdx=0;		/* idx of the next event (value read from the file) */
	pFCT->m_nLoadedEventIDs=0;
	/* ------------------------------------------------------------------- */
	CFCTLoadEventsOrIDs(pFCT, sFileEvIDs, 1);	/* load IDs first !!!*/
	CFCTLoadEventsOrIDs(pFCT, sFileEvents, 0);
	/* ------------------------------------------------------------------- */
	return(0);
	}
/* ======================================================================= */
/* private functions */
/* ======================================================================= */
int  CFCTAddEventRec		(tCFCT* pFCT, tagCFCT_EVREC* pEvent)
	{
	/* ------------------------------------------------------------------- */
	/* 08.09.2003 */
	/* ------------------------------------------------------------------- */
	CFCTSanity(pFCT, "CFCTAddEventRec()");
	/* ------------------------------------------------------------------- */
	if (pFCT->m_nSize+1>CFCT_MAX_EVENTS)
		InstanceErrExit("CFCTAddEventRec()", "too many events (=%d, MAX = %d)",
			pFCT->m_nSize, CFCT_MAX_EVENTS);
	/* ------------------------------------------------------------------- */
	pFCT->m_pEvents[pFCT->m_nSize]=*pEvent;
	pFCT->m_nSize++;
	/* ------------------------------------------------------------------- */
	return(0);
	}
/* ======================================================================= */
int  CFCTAddEvIDRec			(tCFCT* pFCT, tagCFCT_EVIDS* pEvID)
	{
	/* ------------------------------------------------------------------- */
	/* 08.09.2003 */
	/* ------------------------------------------------------------------- */
	CFCTSanity(pFCT, "CFCTAddEvIDRec()");
	/* ------------------------------------------------------------------- */
	if (pFCT->m_nLoadedEventIDs+1>CFCT_MAX_EVENT_IDS)
		InstanceErrExit("CFCTAddEvIDRec()", "too many event IDs (=%d, MAX = %d)",
			pFCT->m_nLoadedEventIDs, CFCT_MAX_EVENT_IDS);
	/* ------------------------------------------------------------------- */
	pFCT->m_pEvIDs[pFCT->m_nLoadedEventIDs]=*pEvID;
	pFCT->m_nLoadedEventIDs++;
	/* ------------------------------------------------------------------- */
	return(0);
	}
/* ======================================================================= */
int CFCTGetEventIDByName(tCFCT* pFCT, char* sStr)
{
/* -----------------------------------------------------------------------
// Description: returns numerical ID of given text event
//   Parametrs: ...
//      Return: CFCT_ERROR (-1) - error
// -----------------------------------------------------------------------
//   MD: 09.07.2003
//   LE: 08.09.2003
// TODO: test it
// ----------------------------------------------------------------------- */
int i=0;
/* ----------------------------------------------------------------------- */
CFCTSanity(pFCT, "CFCTGetEventIDByName()");
/* ----------------------------------------------------------------------- */
do	{
	if (0==strcmp(sStr, pFCT->m_pEvIDs[i].m_sName))
		return(pFCT->m_pEvIDs[i].m_nID);
	i++;
	} while (i<pFCT->m_nSize);
/* ----------------------------------------------------------------------- */
return(CFCT_ERROR);
}
/* ======================================================================= */
int  CFCTGetEventIDFromStr	(tCFCT* pFCT, char* sStr, tagCFCT_EVIDS* pEvID)
{
/* -----------------------------------------------------------------------
// Description: convert text string into event ID record
//   Parametrs: ...
//      Return: 0 - ok, CFCT_ERROR (-1) - error
// -----------------------------------------------------------------------
//   MD: 09.07.2003
//   LE: 08.09.2003
// TODO: test it
// ----------------------------------------------------------------------- */
char sEventName[64];
/* ----------------------------------------------------------------------- */
CFCTSanity(pFCT, "CFCTGetEventIDFromStr()");
/* ----------------------------------------------------------------------- */
sscanf(sStr, "%d %s", &pEvID->m_nID, sEventName);
sprintf(pEvID->m_sName, "%s", CFCTStrToken(pFCT, sEventName));
/* ----------------------------------------------------------------------- */
return(0);
}
/* ======================================================================= */
int CFCTGetEventRecFromStr	(tCFCT* pFCT, char* sStr, tagCFCT_EVREC* pEvent)
{
/* -----------------------------------------------------------------------
// Description: convert text string into event
//   Parametrs: ...
//      Return: 0 - ok, CFCT_ERROR (-1) - error
// -----------------------------------------------------------------------
//   MD: 09.07.2003
//   LE: 08.09.2003
// TODO: test it
// ----------------------------------------------------------------------- */
char* pSemicolon;
int nEventTime;
int nEventID;
/* ----------------------------------------------------------------------- */
CFCTSanity(pFCT, "CFCTGetEventFromString()");
/* ----------------------------------------------------------------------- */
pSemicolon=strstr(sStr, ":"); if (NULL==pSemicolon) return(CFCT_ERROR);
*pSemicolon=0x20;	/* inserts space instead of semicolon */
sscanf(sStr, "%d", &nEventTime);	if (nEventTime<0) return(CFCT_ERROR);
nEventID=CFCTGetEventIDByName(pFCT, CFCTStrToken(pFCT, pSemicolon+1));
if (CFCT_ERROR==nEventID) return(CFCT_ERROR);
/* ----------------------------------------------------------------------- */
pEvent->m_nTime=nEventTime;
pEvent->m_nEventID=nEventID;
/* ----------------------------------------------------------------------- */
return(0);
}
/* ======================================================================= */
int CFCTLoadEventsOrIDs(tCFCT* pFCT, char* sFileName, int bLoadIDs)
{
/* -----------------------------------------------------------------------
// Description: loads events from opened file
//   Parametrs: sFileName	- file name w. events or w. events' IDs
//				bLoadIDs	- flag, if =1 then event's ID will be loaded
//      Return: (0) - ok, CFCT_ERROR (-1) - error
// -----------------------------------------------------------------------
//   MD: 09.07.2003
//   LE: 08.09.2003
// TODO: test it
// ----------------------------------------------------------------------- */
char sTemp[256];	/* temporal variable */
int nErr;
tagCFCT_EVREC tEvRec;
tagCFCT_EVIDS tEvID;
FILE* fp;
/* ----------------------------------------------------------------------- */
CFCTSanity(pFCT, "CFCTLoadEventsOrIDs()");
/* ----------------------------------------------------------------------- */
if (NULL==(fp=fopen(sFileName, "rt")))
	InstanceErrExit("CFCTLoadEvents()", "can't open file w. %s %s",
	bLoadIDs?"event IDs":"events", sFileName);
/* -----------------------------------------------------------------------
// all events have to be saved in the following format
// <integer_time>: <event_name>
// Example:
// 10: RxSighThres
// ----------------------------------------------------------------------- */
while (!feof(fp))
	{
	fgets(sTemp, sizeof(sTemp)-1, fp);
	nErr=(1==bLoadIDs)?
		CFCTGetEventIDFromStr	(pFCT, sTemp, &tEvID):
		CFCTGetEventRecFromStr	(pFCT, sTemp, &tEvRec);
	/* ------------------------------------------------------------------- */
	if (CFCT_ERROR==nErr) return(CFCT_ERROR);
	/* ------------------------------------------------------------------- */
	nErr=(1==bLoadIDs)?
		CFCTAddEvIDRec	(pFCT, &tEvID):
		CFCTAddEventRec	(pFCT, &tEvRec);
	/* ------------------------------------------------------------------- */
	};
/* ----------------------------------------------------------------------- */
fclose(fp);
/* ----------------------------------------------------------------------- */
return(0);
}
/* ======================================================================= */
char* CFCTStrToken(tCFCT* pFCT, char* sStr)
{
int nStart=0;
int i, n;
/* ----------------------------------------------------------------------- */
CFCTSanity(pFCT, "CFCTStrToken()");
/* ----------------------------------------------------------------------- */
while(isspace(sStr[nStart]))
	  nStart++;
n=strlen(sStr+nStart);
for (i=0; i<n; i++)
	if (isspace(sStr[nStart+i]))
		{
		sStr[nStart+i]=0x00;
		return(sStr+nStart);
		}
return(sStr+nStart);
}
/* ======================================================================= */
/* public functions */
/* ======================================================================= */
int CFCTGetEventIDByTime(tCFCT* pFCT, int nTime)
{
/* -----------------------------------------------------------------------
// Description: returns type of event at given timepoint
//   Parametrs: ...
//      Return: (0) - none, CFCT_ERROR (-1) - error
// -----------------------------------------------------------------------
//   MD: 08.07.2003
//   LE: 11.09.2003
// TODO: test it
// ----------------------------------------------------------------------- */
CFCTSanity(pFCT, "CFCTGetEventIDByTime()");
/* ----------------------------------------------------------------------- */
if (pFCT->m_nNextEventIdx>=pFCT->m_nSize) return(0); /* no more events */
if (nTime==pFCT->m_pEvents[pFCT->m_nNextEventIdx].m_nTime)
	{
	pFCT->m_nNextEventIdx++;
	return(pFCT->m_pEvents[pFCT->m_nNextEventIdx-1].m_nEventID);
	}
/* ----------------------------------------------------------------------- */
return(0);
}
/* ======================================================================= */
int CFCTGetTimeOfNextEvent	(tCFCT* pFCT, int nCurTime)
{
/* -----------------------------------------------------------------------
// Description: returns time of next event or '-1'
//   Parametrs: ...
//      Return: (>=0) - ok, CFCT_ERROR (-1) - no more events
// -----------------------------------------------------------------------
//   MD: 11.09.2003
//   LE: 11.09.2003
// TODO: test it
// ----------------------------------------------------------------------- */
int i;
/* ----------------------------------------------------------------------- */
CFCTSanity(pFCT, "CFCTGetTimeOfNextEvent()");
/* ----------------------------------------------------------------------- */
for (i=0; i<pFCT->m_nSize; i++)
	if (pFCT->m_pEvents[i].m_nTime>nCurTime)
		return(pFCT->m_pEvents[i].m_nTime);
/* ----------------------------------------------------------------------- */
return(-1);
}
/* ======================================================================= */