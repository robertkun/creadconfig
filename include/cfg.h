#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <fstream>
#include <string.h>
#include <stdlib.h>

#ifndef EXE_NAME
#define EXE_NAME "creadconfig"
#endif

#define INI_NAME "config.ini"

#define MAIN_SECTION "MAIN"
#define	MAX_PATH 2048
#define DEFAULT_DOMAIN_SIZE 256
#define DEFAULT_COMMON_SIZE 256
#define BUF_LEN 1024
#define CFG_OK 0
#define CFG_FILE_ERR -1
#define CFG_PAR_ERR -2

typedef struct _STRUCT_FILE_CFG
{
	char m_InputPath[MAX_PATH];
	char m_OutPath[MAX_PATH];

	char m_UpHost[DEFAULT_DOMAIN_SIZE];
	char m_UpAddr[DEFAULT_DOMAIN_SIZE];
	char m_UpAddrCNC[DEFAULT_DOMAIN_SIZE];
	char m_UpAddrCT[DEFAULT_DOMAIN_SIZE];
	char m_UpAddrProxy[DEFAULT_DOMAIN_SIZE];
	char m_UpAddrProxy2[DEFAULT_DOMAIN_SIZE];

	char m_UpUrlEd[DEFAULT_DOMAIN_SIZE];
	char m_UpUrlFa[DEFAULT_DOMAIN_SIZE];
	char m_UpUrlBs[DEFAULT_DOMAIN_SIZE];

	char m_UpUrlEdSample[DEFAULT_DOMAIN_SIZE];
	char m_UpUrlFaSample[DEFAULT_DOMAIN_SIZE];
	char m_UpUrlBsSample[DEFAULT_DOMAIN_SIZE];

	char m_CdnKey[DEFAULT_COMMON_SIZE];
	char m_CdnTag[DEFAULT_COMMON_SIZE];

	int m_logSave;
	int m_packSize;
	int m_sampleRate;
	int m_loopTime;
	int m_maxSendTime;
	int m_retryTime;
	int m_timeout;
	int m_maxValidLogTime;
	int m_sendThreadNum;

	_STRUCT_FILE_CFG()
	{
		memset(m_InputPath,	0, sizeof(m_InputPath));
		memset(m_OutPath,	0, sizeof(m_OutPath));
		memset(m_UpAddr,	0, sizeof(m_UpAddr));
		memset(m_UpAddrCNC,	0, sizeof(m_UpAddrCNC));
		memset(m_UpAddrCT,	0, sizeof(m_UpAddrCT));
		memset(m_UpHost,	0, sizeof(m_UpHost));
		memset(m_UpAddrProxy, 0, sizeof(m_UpAddrProxy));
		memset(m_UpAddrProxy2, 0, sizeof(m_UpAddrProxy2));

		memset(m_UpUrlEd,	0, sizeof(m_UpUrlEd));
		memset(m_UpUrlFa,	0, sizeof(m_UpUrlFa));
		memset(m_UpUrlBs,	0, sizeof(m_UpUrlBs));
		memset(m_UpUrlEdSample, 0, sizeof(m_UpUrlEdSample));
		memset(m_UpUrlFaSample,	0, sizeof(m_UpUrlFaSample));
		memset(m_UpUrlBsSample,	0, sizeof(m_UpUrlBsSample));

		memset(m_CdnKey,	0, sizeof(m_CdnKey));
		memset(m_CdnTag, 0, sizeof(m_CdnTag));

		m_packSize = 10;
		m_sampleRate= 0;
		m_loopTime=1;
		m_maxSendTime=10;
		m_retryTime=3;
		m_timeout=5;
		m_logSave=3;
		m_maxValidLogTime=30;
		m_sendThreadNum=10;
	}
} STRUCT_FILE_CFG;

bool readConfig(STRUCT_FILE_CFG& fCfg);
int getConfigStr(const char *insection, const char *inkeyname, char *outkeyvalue, unsigned int len,
				 const char *filename);

int getConfigInt(const char *insection, const char *inkeyname, int *outkeyvalue, const char *filename);
#endif

