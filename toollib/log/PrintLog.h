#ifndef __PRINT_LOG_H_
#define __PRINT_LOG_H_

class CPrintLog : public CBaseLog
{
public:
	CPrintLog();
	virtual ~CPrintLog();
	virtual int ReadAndPrint();
private:
	void Init();
	void Close();

private:
};

#endif

