/* =======================================================================
// STAtion instance
// MD: 07.08.2003
// LE: 20.10.2003
// ======================================================================= */
#ifndef __STA_INST__
#define __STA_INST__
/* ======================================================================= */
/* environmet varibales, where configuration is placed (20.10.2003) */
/* ======================================================================= */
#define STAINST_VARCFG		"STA_INST_CFG"
#define STAINST_VARLOG		"STA_INST_LOG"
/* ======================================================================= */
/* instance-header idetification sequence (20.10.2003) */
/* ======================================================================= */
#define STAINST_IDSEQ0		(0x20031020)	// ID 0
#define STAINST_IDSEQ1		(0x19791120)	// ID 1
/* ======================================================================= */
/* configuration structure (20.10.2003) */
/* ----------------------------------------------------------------------- */
typedef struct
	{
	unsigned int sInitSeq[2];		// ID sequence
	int bInited;			// "instance is inited" flag
	char sStaCfgFile[256];	// instance config file
	char sStaLogFile[256];	// station log file
	} tagSTAINST;
/* ======================================================================= */
#endif /* __STA_INST__ */
/* ======================================================================= */
