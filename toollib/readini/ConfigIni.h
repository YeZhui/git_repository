#ifndef _CONFIG_INI_H_
#define _CONFIG_INI_H_

#include "Ini.h"

namespace ConfigIni
{
	string ReadStr(const char *p_szFileName, const string p_strNode, const string p_strKey);
	int WriteStr(const string p_strNode, const string p_strKey, const string p_strValue);
};

#endif

