/* ======================================================================= */
/* MD: 10.09.2003 */
/* ======================================================================= */
#ifndef __EMUL_CFG__
#define __EMUL_CFG__
/* ======================================================================= */
#include "lib_path.h"		/* (09.10.2003) */
//#include "c_logger.h"		/* tagCLOGR (14.08.2003) */
#include C_LOGGER_H			/* tagCLOGR (14.08.2003) */
/* ======================================================================= */
/* emulation configuration, loaded from cfg file  (06.08.2003) */
/* ======================================================================= */
typedef struct {
	int m_nFragSize;		/* fragment size in bytes (01.08.2003) */
	int m_nTxRate;			/* TxRate [Mbits/sec] (08.09.2003) */
	int m_nMaxCWVal;		/* maximal CW value (11.09.2003) */
	char m_sStaName[256];	/* STA's name (06.08.2003) */
	char m_sL3IA[256];		/* file w. address of dest data (31.07.2003) */
	char m_sL3[2][256];		/* transm/recv data (L3) (30.07.2003) */
	char m_sLPC[2][256];	/* transm/recv data (PHY_CTRL) (31.07.2003) */
	char m_sL3O_MPDU[256];	/* recv. data MPDUs (L3) (12.08.2003) */
	char m_sSPYS[3][256];	/* spys' file names (14.08.2003) */
	char m_sLGRSDL[256];	// output filename of SDL logger (20.10.2003)
	char m_sLGRsMAC[3][256];/* MAC LOGGERS (12.09.2003) */
	char m_sFLEvnts[256];	/* events - scenarion (08.09.2003) */
	char m_sFLEvIDs[256];	/* events IDs (08.09.2003) */
	int  m_nLogOutput;		/* LOG output (30.09.2003) */
	int  m_nLogSocketPort;	/* socket port (14.10.2003) */
	int  m_nLogSocketRecon;	/* max. number of reconnections (15.10.2003) */
	char m_sLogSocketIp[256];	/* socket ID (IpAddr or DNS-name) (30.09.2003) */
	tagCLOGR m_tLSPYS[3];	/* SPY loggers */
	tagCLOGR m_tLSDL;		// SDL logger (29.07.2003, 20.10.2003)
	} tagEMULCFG;
/* ======================================================================= */
#endif /* __EMUL_CFG__ */
/* ======================================================================= */
