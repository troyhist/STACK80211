/* =======================================================================
// File_control mechanism ver. 2.0
// reads text events from text file, and returns type of event
// -----------------------------------------------------------------------
// MD: 08.07.2003
// LE: 08.09.2003
// -----------------------------------------------------------------------
// Compiler: MS VC 6.0
// =======================================================================
// FileCtrl.h: implementation of service functions (header)
// ======================================================================= */
#ifndef __FILE_CTRL__
#define __FILE_CTRL__
/* ======================================================================= */
#include <stdio.h>
/* ======================================================================= */
/* constants */
/* ======================================================================= */
#define CFCT_ERROR			(-1)
#define CFCT_MAX_EVENT_IDS	(15)	/* MAX # of different type of events */
#define CFCT_MAX_EVENTS		(256)	/* MAX # of incoming events */
/* ======================================================================= */
/* internal structs */
/* ======================================================================= */
typedef struct
	{
	int m_nTime;			/* time of event */
	int m_nEventID;			/* event ID */
	} tagCFCT_EVREC;
/* ======================================================================= */
/* Event types */
/* ======================================================================= */
typedef struct 
	{
	int m_nID;				/* event ID */
	char m_sName[64];		/* event name */
	} tagCFCT_EVIDS;
/* ======================================================================= */
/* internal data (like class private members) */
/* ======================================================================= */
typedef struct
	{
	int	m_bInited;				/* inited */
	int	m_nSizeReal;			/* real size of events array */
	int	m_nSize;				/* number of elements used in the events array */
	int	m_nNextEventIdx;		/* idx of the next event (value read from the file) */
	int m_nLoadedEventIDs;		/* # of different ID loaded from Event ID file */
	/* ------------------------------------------------------------------- */
	tagCFCT_EVREC m_pEvents[CFCT_MAX_EVENTS];	/* array w. events */
	tagCFCT_EVIDS m_pEvIDs[CFCT_MAX_EVENT_IDS];	/* array w. event IDs */
	/* ------------------------------------------------------------------- */
	} tagCFCT_CLASS;
/* ----------------------------------------------------------------------- */
typedef tagCFCT_CLASS tCFCT;
/* ======================================================================= */
/* "class" functions */
/* ======================================================================= */
void CFCT		(tCFCT* pFCT);		/* constructor (08.09.2003) */
void CFCT_		(tCFCT* pFCT);		/* destructor (24.09.2003) */
int  CFCTIsInited(tCFCT* pFCT);		/* Is Inited (24.09.2003) */
void CFCSanity	(tCFCT* pLPC, char* sFuncName);
/* ======================================================================= */
int  CFCTInit	(tCFCT* pFCT, char* sFileEvIDs, char* sFileEvents);		/* init (08.09.2003) */
/* ======================================================================= */
/* private functions */
/* ======================================================================= */
int  CFCTAddEventRec		(tCFCT* pFCT, tagCFCT_EVREC* pEvent);
int  CFCTAddEvIDRec			(tCFCT* pFCT, tagCFCT_EVIDS* pEvID);
int  CFCTGetEventIDByName	(tCFCT* pFCT, char* sStr);
int  CFCTGetEventIDFromStr	(tCFCT* pFCT, char* sStr, tagCFCT_EVIDS* pEvID);
int  CFCTGetEventRecFromStr	(tCFCT* pFCT, char* sStr, tagCFCT_EVREC* pEvent);
int  CFCTLoadEventsOrIDs	(tCFCT* pFCT, char* sFileEvents, int bLoadIDs);
char* CFCTStrToken			(tCFCT* pFCT, char* sStr);
/* ======================================================================= */
/* public functions */
/* ======================================================================= */
int CFCTGetEventIDByTime	(tCFCT* pFCT, int nTime);	/* returns type of event at given timepoint (08.07.2003) */
int CFCTGetTimeOfNextEvent	(tCFCT* pFCT, int nCurTime);/* returns time of next event or '-1' (11.09.2003) */
/* ======================================================================= */
#endif /* __FILE_CTRL__ */
/* ======================================================================= */
