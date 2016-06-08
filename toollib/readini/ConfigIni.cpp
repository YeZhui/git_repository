#include "ConfigIni.h"

namespace ConfigIni
{

	string ReadStr(const char* p_szFileName, const string p_strNode, const string p_strKey)
	{
		CIni ini(p_szFileName);
		return ini.ReadStr(p_strNode, p_strKey);
	}

	int WriteStr(const string p_strNode, const string p_strKey, const string p_strValue)
	{
	}
};
