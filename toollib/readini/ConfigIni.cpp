#include "ConfigIni.h"

namespace ConfigIni
{
	static CIni g_cIni;
	void Init(const string p_strFileName)
	{
		g_cIni.Init(p_strFileName);
	}

	string ReadStr(const string p_strNode, const string p_strKey)
	{
		return g_cIni.ReadStr(p_strNode, p_strKey);
	}

	int ReadInt(const string p_strNode, const string p_strKey)
	{
		return g_cIni.ReadInt(p_strNode, p_strKey);
	}

	long ReadLong(const string p_strNode, const string p_strKey)
	{
		return g_cIni.ReadLong(p_strNode, p_strKey);
	}

	char ReadCh(const string p_strNode, const string p_strKey)
	{
		return g_cIni.ReadChar(p_strNode, p_strKey);
	}
};
