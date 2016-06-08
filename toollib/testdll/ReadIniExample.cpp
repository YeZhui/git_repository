#include "ReadIniExample.h"
#include "../readini/ConfigIni.h"
#include "../log/SingleLog.h"

void CheckReadIni()
{
	string strValue = ConfigIni::ReadStr("./config.ini", string("Option"), string("TcpKeepLive"));
	if (strValue != "")
	{
		SingleLog::WriteLog(1,__FILE__,__LINE__,"%s", strValue.c_str());
	}
	else
	{
		SingleLog::WriteLog(2,__FILE__,__LINE__,"∂¡»°≈‰÷√Œƒº˛ ß∞‹");
	}
}
