#include "../../readini/ConfigIni.h"
#include <stdio.h>
#include <unistd.h>

void CheckReadIni()
{
	ConfigIni::Init("./log.ini");
	string strValue = ConfigIni::ReadStr(string("SYSLOG"), string("ADDRESS"));
	if (strValue == "")
	{
		fprintf(stderr, "read address failed\n");
	}
	else
	{
		fprintf(stdout, "read address is:%s\n", strValue.c_str());
	}
	
	ConfigIni::Init("./config.ini");
	auto filename = ConfigIni::ReadStr(string("FILE"), string("FILENAME"));
	if (filename == "")
	{
		fprintf(stderr, "read filename failed\n");
	}
	else
	{
		fprintf(stdout, "read filename is:%s\n", filename.c_str());
	}

	auto port = ConfigIni::ReadInt(string("SYSLOG"), string("PORT"));
	if (port == -1)
	{
		fprintf(stderr, "read port failed\n");
	}
	else
	{
		fprintf(stdout, "read port is:%d\n", port);
	}

	auto flag = ConfigIni::ReadCh(string("FLAG"), string("PRINT_FLAG"));
	if (flag == 0)
	{
		fprintf(stderr, "read flag failed\n");
	}
	else
	{
		fprintf(stdout, "read flag is:%c\n", flag);
	}

	auto num = ConfigIni::ReadStr(string("FILE"), string("NUM"));
	if (num == "")
	{
		fprintf(stderr, "read num failed\n");
	}
	else
	{
		fprintf(stdout, "read num is:%s\n", num);
	}

	ConfigIni::Init("./socket.ini");
	auto port1 = ConfigIni::ReadInt(string("SERVER"), string("Port"));
	if (port1 == -1)
	{
		fprintf(stderr, "read port failed\n");
	}
	else
	{
		fprintf(stdout, "read port is:%d\n", port1);
	}
}

int main()
{
	CheckReadIni();
	return 0;
}
