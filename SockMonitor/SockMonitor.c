/* =======================================================================
// SockMonitor ver. 1.0 (pure C)
// socket monitor for C_LOGGER output
// -----------------------------------------------------------------------
// MD: 07.10.2003
// LE: 14.10.2003
// -----------------------------------------------------------------------
//   Compiler: MS VC 6.0
// Aplication: ANSI, DOS16, WIN32
//  Edited by: Serguei Kolevatov
// =======================================================================
// SockMonitor.c: Defines the entry point for the console application
// ======================================================================= */
/* ======================================================================= */
#include <stdio.h>
#include <conio.h>			/* kbhit, getch (08.10.2003) */
/* ----------------------------------------------------------------------- */
//#include "..\\strgetval.h"			/* StrGetVal (07.10.2003) */
#include "..\\..\\TCILIB\\CLASS_C\\SOCKETOP\\socketop.h"	/* CSOP (07.10.2003) */
#include "AutoBuild.h"			/* AUTO_BUILD_NUMBER (14.10.2003) */
#include "..\\LastBackup.h"		/* LAST_BACKUP_NUMBER (07.10.2003) */
/* ======================================================================= */
/* constants */
/* ======================================================================= */
#define GL_MAXRXSIZE		(16384)
#define MAX_ACCSOCK			(3)
/* ======================================================================= */
/* global data */
/* ======================================================================= */
char gl_sDataRxBuf[MAX_ACCSOCK][GL_MAXRXSIZE];
int gl_nDataRxBufSize[MAX_ACCSOCK];
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
/* Functions */
/* ======================================================================= */
int ShowSysInfo(tagSOCKETOP* pSOP)
{
int* pIpAddr=CSOPGetLocalHostIpAddr(pSOP);
printf("Local system info:\n"
	   "FDQN name = %s\n"
	   "host      = %s\n"
	   "host IP   = %d.%d.%d.%d\n",
	   CSOPGetLocalOfficialHostName(pSOP), CSOPGetLocalHostName(pSOP),
	   pIpAddr[3], pIpAddr[2], pIpAddr[1], pIpAddr[0]);
return(0);
}
/* ======================================================================= */
int InitGlobalData(void)
{ /* inits all global data (14.10.2003) */
int i;
for(i=0; i<MAX_ACCSOCK; i++) { gl_sDataRxBuf[i][0]=0; gl_nDataRxBufSize[i]=0; }
return(0);
}
/* ======================================================================= */
int SaveData(char* sFilename, char* sSaveData)
{
/* ----------------------------------------------------------------------- */
/* saves given data into a file with given filename (14.10.2003) */
/* ----------------------------------------------------------------------- */
FILE* fp;
if(NULL==(fp=fopen(sFilename, "a+t")))
	{ return(-1); }
fprintf(fp, "%s\n", sSaveData); fclose(fp); printf("."); return(0);
}
/* ======================================================================= */
int GetToken(char* sTextData, char* sTokenName, char* sTokenValue, int* pnOffset)
{
/* ----------------------------------------------------------------------- */
/* retrieves value of given token (14.10.2003) */
/* ----------------------------------------------------------------------- */
char* sPtr[2];
char sTags[2][32];
char sTagSizes[2];
int nLen;
/* ----------------------------------------------------------------------- */
sprintf(sTags[0], "<%s>", sTokenName);
sprintf(sTags[1], "</%s>", sTokenName);
sTagSizes[0]=strlen(sTags[0]);
sTagSizes[1]=strlen(sTags[1]);
/* ----------------------------------------------------------------------- */
sPtr[0]=sTextData;
if(NULL==(sPtr[0]=strstr(sPtr[0], sTags[0]))) return(0);
if(NULL==(sPtr[1]=strstr(sPtr[0]+sTagSizes[0], sTags[1]))) return(0);
/* ----------------------------------------------------------------------- */
nLen=sPtr[1]-(sPtr[0]+sTagSizes[0]);
strncpy(sTokenValue, sPtr[0]+sTagSizes[0], nLen); sTokenValue[nLen]=0;
if(pnOffset) *pnOffset=nLen+sTagSizes[0]+sTagSizes[1];
/* ----------------------------------------------------------------------- */
return(1);
}
/* ======================================================================= */
int IsTokenProcessed(char* sData, char* sFilename, char* sSaveData, int* pnOffset)
{
/* ----------------------------------------------------------------------- */
/* parses incoming data (14.10.2003) */
/* ----------------------------------------------------------------------- */
int nRes;
int nOffsets[2];
char sTags[2][32]={"FILE", "DATA"};
/* ----------------------------------------------------------------------- */
nRes=GetToken(sData, sTags[0], sFilename, &nOffsets[0]);
if(0==nRes) return(0);
nRes=GetToken(sData+nOffsets[0], sTags[1], sSaveData, &nOffsets[1]);
if(0==nRes) return(0);
/* ----------------------------------------------------------------------- */
if(pnOffset) *pnOffset=nOffsets[0]+nOffsets[1];
/* ----------------------------------------------------------------------- */
return(1);
}
/* ======================================================================= */
int ProcessData(char* sData, int nDataSize, int *pnOffset)
{
/* ----------------------------------------------------------------------- */
/* process data in given text buffer (14.10.2003) */
/* ----------------------------------------------------------------------- */
int bTokenFound=0;
char sFilename[256], sSaveData[GL_MAXRXSIZE];
int i, nOffset;
/* ----------------------------------------------------------------------- */
if(pnOffset) *pnOffset=0;
/* ----------------------------------------------------------------------- */
for(i=0; i<nDataSize; i++)
	{
	if(IsTokenProcessed(sData+i, sFilename, sSaveData, &nOffset))
		{
		if(0==bTokenFound) printf("\nSMON: saving data ");
		SaveData(sFilename, sSaveData);
		if(pnOffset) *pnOffset=i+nOffset;
		i+=nOffset;
		bTokenFound=1;
		}
	}
/* ----------------------------------------------------------------------- */
if(bTokenFound) printf(" ok\n\n");
/* ----------------------------------------------------------------------- */
return(0);
}
/* ======================================================================= */
int FlushBuffer(char* sDataBuf, int* pnDataBufSize)
{
/* ----------------------------------------------------------------------- */
/* flushes given buffer (14.10.2003) */
/* ----------------------------------------------------------------------- */
int nOffset;
if(NULL==pnDataBufSize) return(-1);
if(*pnDataBufSize>0)
	{
	ProcessData(sDataBuf, *pnDataBufSize, &nOffset);
	strncpy(sDataBuf, sDataBuf+nOffset, GL_MAXRXSIZE-nOffset);
	*pnDataBufSize=GL_MAXRXSIZE-nOffset;
	sDataBuf[*pnDataBufSize]=0;
	}
return(0);
}
/* ======================================================================= */
int PassDataToTextProcessor(char* sNewData, char* sDataBuf, int* pnDataBufSize)
{
/* ----------------------------------------------------------------------- */
/* copies all incoming data into a global buffer (14.10.2003) */
/* ----------------------------------------------------------------------- */
int nNewDataSize=strlen(sNewData);
/* ----------------------------------------------------------------------- */
if (*pnDataBufSize+nNewDataSize>GL_MAXRXSIZE)
	FlushBuffer(sDataBuf, pnDataBufSize);
/* ----------------------------------------------------------------------- */
strcat(sDataBuf, sNewData);
*pnDataBufSize=strlen(sDataBuf);
/* ----------------------------------------------------------------------- */
return(0);
}
/* ======================================================================= */
/* global data processing */
/* ======================================================================= */
int GL_FlushBuffers(void)
{
/* ----------------------------------------------------------------------- */
/* flushes non-empty buffers (14.10.2003) */
/* ----------------------------------------------------------------------- */
int i;
for(i=0; i<MAX_ACCSOCK; i++)
	FlushBuffer(gl_sDataRxBuf[i], &gl_nDataRxBufSize[i]);
return(0);
}
/* ======================================================================= */
int GL_PerformDataExchange(tagSOCKETOP* pSOP, int nBufId)
{
/* ----------------------------------------------------------------------- */
/* receives all data (14.10.2003) */
/* ----------------------------------------------------------------------- */
char sBufDataRx[GL_MAXRXSIZE];
int nErr;
int nRecvBytes;
int *pIpAddr=CSOPGetLocalHostIpAddr(pSOP);
/* ----------------------------------------------------------------------- */
nErr=CSOPSockRecv(pSOP, sBufDataRx, sizeof(sBufDataRx), &nRecvBytes);
if(nErr) return(-1);
if(0>=nRecvBytes) return(0);
/* ----------------------------------------------------------------------- */
printf("SMON: recv. %d bytes from %s (%d.%d.%d.%d)\n",
	   nRecvBytes, CSOPGetLocalHostName(pSOP),
	   pIpAddr[3], pIpAddr[2], pIpAddr[1], pIpAddr[0]);
/* ----------------------------------------------------------------------- */
sBufDataRx[nRecvBytes]=0;
PassDataToTextProcessor(sBufDataRx, gl_sDataRxBuf[nBufId],
						&gl_nDataRxBufSize[nBufId]);
/* ----------------------------------------------------------------------- */
return(0);
}
/* ======================================================================= */
#define EXAMPL_SRV_PORT		(3181)
/* ======================================================================= */
/* main program */
/* ======================================================================= */
int main(int argc, char* argv[])
{
/* ----------------------------------------------------------------------- */
int nErr;
int i, bNotDone;
int nPort;
int pIpAddr[16];		// ip address
int *pSockIpAddr;
int nAccSocks=0;		// number of already accepted sockets
tagSOCKETOP pSocks[MAX_ACCSOCK];
tagSOCKETOP tSock, *pSOP=&tSock;
/* ----------------------------------------------------------------------- */
CSOP(&tSock);
CSOPInitDef(&tSock);
/* ----------------------------------------------------------------------- */
InitGlobalData();
/* ----------------------------------------------------------------------- */
printf("\n"
	"SockMonitor ver. 0.1.%d (build # %d)\n"
	"socket monitor for C_LOGGER output\n\n",
	LAST_BACKUP_NUMBER, AUTO_BUILD_NUMBER);
/* ----------------------------------------------------------------------- */
ShowSysInfo(pSOP);
/* ----------------------------------------------------------------------- */
if(2!=argc)
	{
	printf("\n"
	"  USAGE: SockMonitor <port_number>\n"
	"EXAMPLE: SockMonitor %d\n\n"
	, EXAMPL_SRV_PORT);
	CSOP_(&tSock);
	return(0);
	}
/* ----------------------------------------------------------------------- */
sscanf(argv[1], "%d", &nPort);
printf("\n"
	   "SMON: Maximal # of supported simultaneous connections = %d\n"
	   "      Rx buffer size for each connection = %d bytes\n"
	   "SMON: setting up listening mode at localport %d ... ", 
	   MAX_ACCSOCK, GL_MAXRXSIZE, nPort);
/* ----------------------------------------------------------------------- */
nErr=CSOPListenPort(&tSock, nPort);
if(nErr)
	{ printf("err: %d.%s\n", CSOPGetExtErrCode(pSOP),
	CSOPGetExtErrText(pSOP)); CSOP_(&tSock); return(0); }
/* ----------------------------------------------------------------------- */
printf("ok\nSMON: press ESC to stop listening\n");
bNotDone=1;
/* ----------------------------------------------------------------------- */
for (i=0; i<MAX_ACCSOCK; i++) CSOP(&pSocks[i]);
/* ----------------------------------------------------------------------- */
while(bNotDone)
	{
	/* ------------------------------------------------------------------- */
	/* checks incoming connections */
	/* ------------------------------------------------------------------- */
	if(CSOPIsRxConnAvail(pSOP))
		{
		/* --------------------------------------------------------------- */
		printf("SMON: incoming connect request\n");
		if(MAX_ACCSOCK==nAccSocks)
			{
			char *sForce="forced disconnection\n";
			tagSOCKETOP tTemp;
			printf("SMON: cann't accept, "
			"maximal number of sockets is reached (=%d)\n", MAX_ACCSOCK);
			CSOP(&tTemp);
			CSOPAcceptConnection(pSOP, &tTemp, pIpAddr);
			CSOPSockSend(&tTemp, sForce, strlen(sForce), NULL);
			CSOPDisconnect(&tTemp);
			CSOP_(&tTemp);
			}
		else
			{
		/* --------------------------------------------------------------- */
		printf("SMON: accepting ... ");
		nErr=CSOPAcceptConnection(pSOP, &pSocks[nAccSocks], pIpAddr);
		if(nErr)
			{ printf("err: %d.%s\n",
			CSOPGetExtErrCode(&pSocks[nAccSocks]),
			CSOPGetExtErrText(&pSocks[nAccSocks])); }
		/* --------------------------------------------------------------- */
		else
			{ printf("ok (IpAddr = %d.%d.%d.%d)\n", 
			pIpAddr[3], pIpAddr[2], pIpAddr[1], pIpAddr[0]); nAccSocks++; }
		/* --------------------------------------------------------------- */
			}
		}
	/* ------------------------------------------------------------------- */
	/* checks available data from open sockets */
	/* ------------------------------------------------------------------- */
	for(i=0; i<nAccSocks; i++)
		{
		/* --------------------------------------------------------------- */
		if(0==CSOPIsConnected(&pSocks[i])) continue;
		/* --------------------------------------------------------------- */
		if(0==CSOPIsTxPossible(&pSocks[i])) printf("tx is not possible\n");
		if(CSOPIsErrorOccur(&pSocks[i])) printf("socket error occured\n");
		/* --------------------------------------------------------------- */
		pSockIpAddr=CSOPGetLocalHostIpAddr(&pSocks[i]);
		if(CSOPIsRxDataAvail(&pSocks[i]))
			{
			nErr=GL_PerformDataExchange(&pSocks[i], i);
			/* ----------------------------------------------------------- */
			if (nErr)
				{
				printf("(%d.%d.%d.%d) rx.err: %d.%s\n", pSockIpAddr[3],
				pSockIpAddr[2], pSockIpAddr[1], pSockIpAddr[0],
				CSOPGetExtErrCode(&pSocks[i]), CSOPGetExtErrText(&pSocks[i]));
				printf("SMON: closing conn. with %d.%d.%d.%d ... ",
					pSockIpAddr[3], pSockIpAddr[2], pSockIpAddr[1], pSockIpAddr[0]);
				nErr=CSOPDisconnect(&pSocks[i]);
				if(nErr)
					{ printf("err: %d.%s\n", CSOPGetExtErrCode(&pSocks[i]),
					CSOPGetExtErrText(&pSocks[i])); }
				else
					{ printf("ok\n"); }
				nAccSocks--;
				}
			/* ----------------------------------------------------------- */
			}
		/* --------------------------------------------------------------- */
		}
	/* ------------------------------------------------------------------- */
	if (kbhit()) if(0x1b==getch()) bNotDone=0;
	}
/* ----------------------------------------------------------------------- */
/* closes open sockets/connections */
/* ----------------------------------------------------------------------- */
for(i=0; i<nAccSocks; i++)
	{
	if(0==CSOPIsConnected(&pSocks[i])) continue;
	pSockIpAddr=CSOPGetLocalHostIpAddr(&pSocks[i]);
	printf("SMON: closing conn. with %d.%d.%d.%d ... ",
		pSockIpAddr[3], pSockIpAddr[2], pSockIpAddr[1], pSockIpAddr[0]);
	nErr=CSOPDisconnect(&pSocks[i]);
	if(nErr) { printf("err: %d.%s\n", CSOPGetExtErrCode(&pSocks[i]),
		CSOPGetExtErrText(&pSocks[i])); }
	else { printf("ok\n"); }
	}
/* ----------------------------------------------------------------------- */
printf("SMON: disconnecting ... ");
/* ----------------------------------------------------------------------- */
nErr=CSOPDisconnect(pSOP);
if(nErr) { printf("err: %d.%s\n",
	CSOPGetExtErrCode(pSOP), CSOPGetExtErrText(pSOP)); }
else printf("ok\n");
/* ----------------------------------------------------------------------- */
GL_FlushBuffers();
/* ----------------------------------------------------------------------- */
for (i=0; i<MAX_ACCSOCK; i++) CSOP_(&pSocks[i]);
/* ----------------------------------------------------------------------- */
return(0);
}
/* ======================================================================= */