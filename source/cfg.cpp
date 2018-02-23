#include "../include/cfg.h"
#include "../include/aes.h"

static int readOneLine(char *readLineBuf, FILE *fileStream)
{
	int flag = 1, i, j = 0;
	char tmpBuf[BUF_LEN];

	if (fgets(tmpBuf, BUF_LEN, fileStream) != NULL)
	{
//		printf("tmpBuf=[%s]\n", tmpBuf);
		/* Delete the last '\r' or '\n' or ' ' or '\t' character */
		for (i = strlen(tmpBuf) - 1; i >= 0; i--)
		{
			if (tmpBuf[i] == '\r' || tmpBuf[i] == '\n' || tmpBuf[i] == ' ' || tmpBuf[i] == '\t')
				tmpBuf[i] = '\0';
			else
				break; /* dap loop */
		}

		/* Delete the front '\r' or '\n' or ' ' or '\t' character */
		for (i = 0; i <= strlen(tmpBuf); i++)
		{
			if (flag && (tmpBuf[i] == '\r' || tmpBuf[i] == '\n' || tmpBuf[i] == ' ' || tmpBuf[i] == '\t'))
				continue;
			else
			{
				flag = 0;
				readLineBuf[j++] = tmpBuf[i];
			}
		}
		return 0;
	}
	return -1;
}

static int isremark(const char *inLine)
{
	unsigned int i;

	for (i = 0; i < strlen(inLine); i++)
	{

		if (isgraph(inLine[i]))
		{
			if (inLine[i] == '#') return 1;
			else return 0;
		}
	}
	return 1;
}

static int getsection(const char *line, char *section)
{
	unsigned int start, end;

	for (start = 0; start < strlen(line); start++)
	{

		if (line[start] != '[') return -1; /* fail */
		else break;
	}

	for (end = strlen(line) - 1; end > 1; end--)
	{

		if (line[end] != ']') return -1; /* fail */
		else break;
	}

	if (end - start < 2)
		return -1;

	memcpy(section, line + start + 1, end - start - 1);
	section[end - start - 1] = '\0';
	return 0;
}

static int getkey(const char *line, char *keyname, char *keyvalue)
{
	unsigned int i, start;

	/* Find key name */
	for (start = 0; start < strlen(line); start++)
	{
		/* Find '=' character */
		if (line[start] == '=') break;
	}
	if (start >= strlen(line))
		return -1; /* not find '=', return */

	memcpy(keyname, line, start);
	keyname[start] = '\0';

	/* Delete the last '\t' or ' ' character */
	for (i = strlen(keyname) - 1; i >= 0; i--)
	{
		if (keyname[i] == ' ' || keyname[i] == '\t') keyname[i] = '\0';
		else break;
	}

	/* Find key value */
	for (start = start + 1; start < strlen(line); start++)
	{
		if (line[start] != ' ' && line[start] != '\t')
			break;
	}

	strcpy(keyvalue, line + start);

	/* Delete the last '\t' or ' ' character */
	for (i = strlen(keyvalue) - 1; i >= 0; i--)
	{
		if (keyvalue[i] == ' ' || keyvalue[i] == '\t') keyname[i] = '\0';
		else break;
	}
	return 0;
}

void getPwdPath(char *inPath, int inSize)
{
	char buf[256];
	memset(inPath, 0, inSize);
	sprintf(buf, "/proc/%d/exe", getpid());
	if(access(buf, F_OK)==0) {
		readlink(buf, inPath, inSize-1);
		inPath[inSize-1]='\0';
		char *ptr = strrchr(inPath, '/');
		if(ptr) {
			*ptr = '\0';
		}
	}
	else {
		char tmpStr[512];
		memset(tmpStr, 0, sizeof(tmpStr));
		sprintf(tmpStr, "%s is not exist, get_pwd_path failed!\n");
		printf(tmpStr);
	}

	return;
}

int getConfigInt(const char *insection, const char *inkeyname, int *outkeyvalue, const char *filename)
{
	int ret = 0;
	char buf[20] = {'\0'};
	memset(buf, 0, sizeof(buf));

	ret = getConfigStr(insection, inkeyname, buf, sizeof(buf), filename);
	if (ret == CFG_OK) {
		*outkeyvalue = atoi(buf);
	}

	return ret;
}

int getConfigStr(const char *insection, const char *inkeyname, char *outkeyvalue, unsigned int len,
				   const char *filename)
{
	int step = 0;
	FILE *stream;
	char sec[BUF_LEN];
	char ken[BUF_LEN];
	char kev[BUF_LEN];
	char line[BUF_LEN];

	if ((stream = fopen(filename, "r")) == NULL) {
		return CFG_FILE_ERR;
	}

	while (!feof(stream)) {
		if (readOneLine(line, stream) == -1) {
			fclose(stream);
			return CFG_PAR_ERR;
		}

		if (!isremark(line)) {
			if (step == 0) {
				if (getsection(line, sec) == 0) {
					if (strcmp(sec, insection) == 0) {
						step = 1;
					}
				}
			}
			else {
				if (getkey(line, ken, kev) == 0) {
					if (strcmp(ken, inkeyname) == 0) {
						strncpy(outkeyvalue, kev, len);
						fclose(stream);
						return CFG_OK;
					}
				}
			}
		}
	}

	fclose(stream);
	return CFG_PAR_ERR;
}

void printConfig(STRUCT_FILE_CFG fCfg)
{
	printf("m_InputPath=%s\n",fCfg.m_InputPath);
	printf("m_OutPath=%s\n",fCfg.m_OutPath);

	printf("m_UpHost=%s\n",fCfg.m_UpHost);

	printf("m_UpAddr=%s\n",fCfg.m_UpAddr);
	printf("m_UpAddrCNC=%s\n",fCfg.m_UpAddrCNC);
	printf("m_UpAddrCT=%s\n",fCfg.m_UpAddrCT);
	printf("m_UpAddrProxy=%s\n",fCfg.m_UpAddrProxy);
	printf("m_UpAddrProxy2=%s\n",fCfg.m_UpAddrProxy2);

	printf("m_UpUrlEd=%s\n",fCfg.m_UpUrlEd);
	printf("m_UpUrlFa=%s\n",fCfg.m_UpUrlFa);
	printf("m_UpUrlBs=%s\n",fCfg.m_UpUrlBs);

	printf("m_UpUrlEdSample=%s\n",fCfg.m_UpUrlEdSample);
	printf("m_UpUrlFaSample=%s\n",fCfg.m_UpUrlFaSample);
	printf("m_UpUrlBsSample=%s\n",fCfg.m_UpUrlBsSample);

//	printf("m_CdnKey=%s\n",fCfg.m_CdnKey);
	printf("m_CdnTag=%s\n",fCfg.m_CdnTag);

	printf("m_logSave=%d\n",fCfg.m_logSave);
	printf("m_packSize=%d\n",fCfg.m_packSize);
	printf("m_sampleRate=%d\n",fCfg.m_sampleRate);
	printf("m_loopTime=%d\n",fCfg.m_loopTime);
	printf("m_maxSendTime=%d\n",fCfg.m_maxSendTime);
	printf("m_retryTime=%d\n",fCfg.m_retryTime);
	printf("m_timeout=%d\n",fCfg.m_timeout);
	printf("m_maxValidLogTime=%d\n",fCfg.m_maxValidLogTime);
	printf("m_sendThreadNum=%d\n",fCfg.m_sendThreadNum);
}

bool readConfig(STRUCT_FILE_CFG& fCfg)
{
	char tmpDec[1024]="";
	char proxyUrl1[1024]="";
	char proxyUrl2[1024]="";

	char pwd[1024] = "";
	char iniFile[1024] = "";
	getPwdPath(pwd, sizeof(pwd));
	sprintf(iniFile, "%s/config/"INI_NAME, pwd);
	printf("Get iniFile = %s\n", iniFile);

	char dBuf[1024] = "";
	char *pSTA = NULL, *pSTO = NULL;
	char tmpBuf[512] = "\0";
	int isptype=0;

	if (getConfigInt(MAIN_SECTION, "LOG_SAVE", &fCfg.m_logSave, iniFile) != 0) {
		fCfg.m_logSave = 3;
	}

	if (getConfigInt(MAIN_SECTION, "PACK_SIZE", &fCfg.m_packSize, iniFile) != 0) {
		fCfg.m_packSize = 10;
	}

	if (getConfigInt(MAIN_SECTION, "SAMPLE_RATE", &fCfg.m_sampleRate, iniFile) != 0) {
		fCfg.m_sampleRate = 0;
	}

	if (getConfigInt(MAIN_SECTION, "SEND_INTERVAL_TIME", &fCfg.m_loopTime, iniFile) != 0) {
		fCfg.m_loopTime = 1;
	}

	if (getConfigInt(MAIN_SECTION, "MAX_SEND_INTERVAL_TIME", &fCfg.m_maxSendTime, iniFile) != 0) {
		fCfg.m_maxSendTime = 5;
	}

	if (getConfigInt(MAIN_SECTION, "RETRY_TIME", &fCfg.m_retryTime, iniFile) != 0) {
		fCfg.m_retryTime = 3;
	}

	if (getConfigInt(MAIN_SECTION, "TIMEOUT_TIME", &fCfg.m_timeout, iniFile) != 0) {
		fCfg.m_timeout = 5;
	}

	if(fCfg.m_timeout < 0) {
		printf("wrong  TIMEOUT_TIME, please check the config file!\n");
		return false;
	}

	if (getConfigInt(MAIN_SECTION, "MAX_VALID_LOG_TIME", &fCfg.m_maxValidLogTime, iniFile) != 0) {
		fCfg.m_maxValidLogTime = 30;
	}

	if(fCfg.m_maxValidLogTime < 0) {
		printf("wrong  MAX_VALID_LOG_TIME, please check the config file!\n");
		return false;
	}

	if (getConfigInt(MAIN_SECTION, "SEND_THREAD_NUM", &fCfg.m_sendThreadNum, iniFile) != 0) {
		fCfg.m_sendThreadNum = 10;
	}

	if(fCfg.m_sendThreadNum < 0) {
		printf("wrong  SEND_THREAD_NUM, please check the config file!\n");
		return false;
	}

	memset(tmpDec,0, sizeof(tmpDec));
	if (getConfigStr(MAIN_SECTION, "CDNKEY", tmpDec, sizeof(tmpDec), iniFile) != 0) {
		strcpy(fCfg.m_CdnKey, "C22A92962D90010AF82347829D37A86AECEE28");
	} else {
		vDecrypt(tmpDec, fCfg.m_CdnKey);
	}

	if (getConfigStr(MAIN_SECTION, "IN_FILE", fCfg.m_InputPath,  sizeof(fCfg.m_InputPath), iniFile) != 0) {
		strcpy(fCfg.m_InputPath, "/vdncloud/vcs_os/nginx/logs/baidu.log");
	}

	if (getConfigStr(MAIN_SECTION, "UP_ADDR", fCfg.m_UpAddr,  sizeof(fCfg.m_UpAddr), iniFile) != 0) {
		strcpy(fCfg.m_UpAddr, "rtlog.baidu.com");
	}

	if (getConfigStr(MAIN_SECTION, "UP_ADDR_CNC", fCfg.m_UpAddrCNC,  sizeof(fCfg.m_UpAddrCNC), iniFile) != 0) {
		printf("Get UP_ADDR_CNC error!\n");
		strcpy(fCfg.m_UpAddrCNC, "rtlog.baidu.com");
	}

	if (getConfigStr(MAIN_SECTION, "UP_ADDR_CT", fCfg.m_UpAddrCT,  sizeof(fCfg.m_UpAddrCT), iniFile) != 0) {
		printf("Get UP_ADDR_CT error!\n");
		strcpy(fCfg.m_UpAddrCT, "rtlog.baidu.com");
	}

	if (getConfigStr(MAIN_SECTION, "UP_HOST", fCfg.m_UpHost,  sizeof(fCfg.m_UpHost), iniFile) != 0) {
		strcpy(fCfg.m_UpHost, "rtlog.baidu.com");
	}

	memset(tmpDec,0, sizeof(tmpDec));
	memset(proxyUrl1,0, sizeof(proxyUrl1));
	memset(proxyUrl2,0, sizeof(proxyUrl2));
	if (getConfigStr(MAIN_SECTION, "UP_ADDR_PROXY", tmpDec,  sizeof(tmpDec), iniFile) != 0) {
		strcpy(fCfg.m_UpAddrProxy, "testlogsend.vdncloud.com");
		strcpy(fCfg.m_UpAddrProxy2, "testlogsend.vdncloud.com");
	} else {
		char* ptr=NULL;
		ptr=strchr(tmpDec, ',');
		if(ptr) {
			strncpy(proxyUrl1, tmpDec, ptr-&tmpDec[0]);
		} else {
			strcpy(proxyUrl1, tmpDec);
		}

		ptr=strrchr(tmpDec, ',');
		if(ptr) {
			strcpy(proxyUrl2, tmpDec+(ptr-&tmpDec[0]) +1);
		} else {
			strcpy(proxyUrl2, tmpDec);
		}

		strcpy(fCfg.m_UpAddrProxy, proxyUrl1);
		strcpy(fCfg.m_UpAddrProxy2, proxyUrl2);
	}

	if (getConfigStr(MAIN_SECTION, "UP_URL_ED", fCfg.m_UpUrlEd,  sizeof(fCfg.m_UpUrlEd), iniFile) != 0) {
		strcpy(fCfg.m_UpUrlEd, "/rtlog/v1/edge/accesslog");
	}

	if (getConfigStr(MAIN_SECTION, "UP_URL_FA", fCfg.m_UpUrlFa,  sizeof(fCfg.m_UpUrlFa), iniFile) != 0) {
		strcpy(fCfg.m_UpUrlFa, "/rtlog/v1/middle/accesslog");
	}

	if (getConfigStr(MAIN_SECTION, "UP_URL_BS", fCfg.m_UpUrlBs,  sizeof(fCfg.m_UpUrlBs), iniFile) != 0) {
		strcpy(fCfg.m_UpUrlBs, "/rtlog/v1/source/accesslog");
	}

	if (getConfigStr(MAIN_SECTION, "UP_URL_ED_SAMPLE", fCfg.m_UpUrlEdSample,  sizeof(fCfg.m_UpUrlEdSample), iniFile) != 0) {
		strcpy(fCfg.m_UpUrlEdSample, "/rtlog/v1/edge/samplelog");
	}

	if (getConfigStr(MAIN_SECTION, "UP_URL_FA_SAMPLE", fCfg.m_UpUrlFaSample,  sizeof(fCfg.m_UpUrlFaSample), iniFile) != 0) {
		strcpy(fCfg.m_UpUrlFaSample, "/rtlog/v1/middle/samplelog");
	}

	if (getConfigStr(MAIN_SECTION, "UP_URL_BS_SAMPLE", fCfg.m_UpUrlBsSample,  sizeof(fCfg.m_UpUrlBsSample), iniFile) != 0) {
		strcpy(fCfg.m_UpUrlBsSample, "/rtlog/v1/source/samplelog");
	}

	if (getConfigStr(MAIN_SECTION, "CDNTAG", fCfg.m_CdnTag,  sizeof(fCfg.m_CdnTag), iniFile) != 0) {
		strcpy(fCfg.m_CdnTag, "vdn");
	}

	printConfig(fCfg);
	return true;
}