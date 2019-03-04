#ifndef __FILE_LOG_H_
#define __FILE_LOG_H_

#include "BaseLog.h"
#include <fstream>

class CFileLog : public CBaseLog
{
public:
	CFileLog();
	virtual ~CFileLog();
	virtual int ReadAndPrint();

private:
	void Init();
	void Close();
	int ReOpen();
	int CheckFile();
	void CheckDir(const char* p_szAllPath);
private:
	fstream  m_file;
	char     m_szFilePath[128];
	int      m_iFileNum;
	long     m_lCurTime; 
};

#endif

