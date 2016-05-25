#ifndef __FILE_LOG_H_
#define __FILE_LOG_H_

#include <iostream>
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
private:
	ofstream m_file;
	char     m_szFilePath[128];
};

#endif

