#ifndef _INI_H__
#define _INI_H__
#include <map>
#include <iostream>
#include <string>
using namespace std;

/*
若手动修改配置文件，需重启程序
*/

typedef struct
{
	string strNode;
	string strValue;
}NODE_VALUE;

class CIni
{
public:
	CIni();
	CIni(const string p_strFileName);
	~CIni();

	string ReadStr(const string p_strNode, const string p_strKey);
	int    ReadInt(const string p_strNode, const string p_strKey);
	long   ReadLong(const string p_strNode, const string p_strKey);
	char   ReadChar(const string p_strNode, const string p_strKey);

	void   WriteStrToIni(const string p_strNode, const string p_strKey, const string p_strValue);
	void   WriteIntToIni(const string p_strNode, const string p_strKey, const int p_iValue);
	void   WriteCharToIni(const string p_strNode, const string p_strKey, const char p_cValue);
	void   WriteLongToIni(const string p_strNode, const string p_strKey, const long p_lValue);
private:
	void Init(const string p_strFileName);
	string TrimSpaceWrap(string p_strLine);
	void TraversalMap();

private:
	string m_strFileName;
	multimap<string,NODE_VALUE> m_mmapKeyValue;
};

#endif

