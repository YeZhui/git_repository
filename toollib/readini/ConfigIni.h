#ifndef _CONFIG_INI_H_
#define _CONFIG_INI_H_

#include "Ini.h"

namespace ConfigIni
{
	void Init(const string p_strFileName);
	string ReadStr(const string p_strNode, const string p_strKey);
	int ReadInt(const string p_strNode, const string p_strKey);
	long ReadLong(const string p_strNode, const string p_strKey);
	char ReadCh(const string p_strNode, const string p_strKey);
};

#endif

