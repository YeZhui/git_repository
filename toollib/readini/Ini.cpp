#include "Ini.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;
	
typedef multimap<string,NODE_VALUE>::iterator typeMapIt;

CIni::CIni()
{
	m_strFileName.clear();
}
	
CIni::CIni(const string p_strFileName)
{
	m_strFileName = p_strFileName;
	Init(p_strFileName);
}

CIni::~CIni()
{
	m_strFileName.clear();
	m_mmapKeyValue.clear();
}

string CIni::ReadStr(const string p_strNode, const string p_strKey)
{
	string strRet("");
	pair<typeMapIt, typeMapIt> p = m_mmapKeyValue.equal_range(p_strKey);
	for(typeMapIt k = p.first;k != p.second; k++)
	{
		if (k->second.strNode == p_strNode)
		{
			strRet =  k->second.strValue;
		}
	}
	return strRet;
}

int    CIni::ReadInt(const string p_strNode, const string p_strKey)
{
	int iRet = -1;
	string strRet = ReadStr(p_strNode,p_strKey);
	if (strRet.length() != 0)
	{
		iRet = atoi(strRet.c_str());
	}
	return iRet;
}

long   CIni::ReadLong(const string p_strNode, const string p_strKey)
{
	long lRet = -1;
	string strRet = ReadStr(p_strNode,p_strKey);
	if (strRet.length() != 0)
	{
		lRet = atol(strRet.c_str());
	}
	return lRet;
}

char   CIni::ReadChar(const string p_strNode, const string p_strKey)
{
	char cRet = 0;
	string strRet = ReadStr(p_strNode,p_strKey);
	if (strRet.length() != 0)
	{
		cRet = strRet[0];
	}
	return cRet;
}

void   CIni::WriteStrToIni(const string p_strNode, const string p_strKey, const string p_strValue)
{
	//要考虑不能修改文件布局和注释
}

void   CIni::WriteIntToIni(const string p_strNode, const string p_strKey, const int p_iValue)
{
}

void   CIni::WriteCharToIni(const string p_strNode, const string p_strKey, const char p_cValue)
{
}

void   CIni::WriteLongToIni(const string p_strNode, const string p_strKey, const long p_lValue)
{
}

void CIni::Init(const string p_strFileName)
{
	string strLine;
	ifstream inFile;
	int iPos = -1;
	string strKey;
	NODE_VALUE stNodeValue;
	inFile.open(p_strFileName.c_str());
	if (!inFile)
	{
		cout << "打开配置文件[" << p_strFileName << "]失败" << endl;
		exit(-1);
	}

	while (getline(inFile, strLine))
	{
		iPos = -1;
		strKey.clear();
		stNodeValue.strValue.clear();
		strLine = TrimSpaceWrap(strLine);
		if ( strLine[0] == '#' || strLine.length() == 0 || strLine[0] == '~' )
		{
			continue;
		}
		if (strLine[0] == '[')
		{
			stNodeValue.strNode.clear();
			stNodeValue.strNode.assign(strLine, 1, strLine.length()-2);
			continue;
		}
		iPos = strLine.find('=');
		if (iPos != string::npos)
		{
			strKey.assign(strLine, 0, iPos);
			stNodeValue.strValue.assign(strLine, iPos+1, strLine.length()-iPos-1);
			m_mmapKeyValue.insert(pair<string,NODE_VALUE>(strKey, stNodeValue));
		}
	}

	inFile.close();

	TraversalMap();
}

//去掉头尾空格、换行
string CIni::TrimSpaceWrap(string p_strLine)
{
	string strLine = p_strLine;
	int iPos = -1;
	iPos = strLine.find('\n');
	if (iPos != string::npos)
	{
		strLine.erase(iPos);
	}

	return strLine;	
}

void CIni::TraversalMap()
{
	typeMapIt itMap;
	pair<typeMapIt, typeMapIt> ret;
	for(itMap = m_mmapKeyValue.begin(); itMap != m_mmapKeyValue.end();)
	{
		cout << itMap->first << "=>";
		ret = m_mmapKeyValue.equal_range(itMap->first);
		for(itMap = ret.first;itMap != ret.second;++itMap)
		{
			cout << " " << (itMap->second.strNode) << itMap->second.strValue;
		}
		cout << endl;
	}
}

