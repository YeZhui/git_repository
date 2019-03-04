#include <stdio.h>
#include <iostream>
#include <string.h>
#include "../../network/SxTcp.h"
using namespace std;

int main()
{
	CTcp tcp;
	int iRet = 0;
	int iFd = 0;
	char buf[128] = {0};
	
	iRet = tcp.Open();
	if (iRet == 0)
	{
		perror("socket create failed");
		return -1;
	}

	iRet = tcp.ConnectNoblock("192.168.233.250", 9995, 10000);
	if (iRet == 0)
	{
		perror("socket connect failed");
		return -1;
	}

	while(1)
	{
		memset(buf, 0, sizeof(buf));
		cout << "please input some string:";
		cin >> buf;
		iRet = tcp.Send(buf, strlen(buf));
		if (iRet < -1 && errno != EAGAIN)
		{
			perror("send failed");
			return -1;
		}
		else if(iRet == 0)
		{
			perror("connect is closed");
			return -1;
		}

		memset(buf, 0, sizeof(buf));
	
		iRet = tcp.Recv(buf, sizeof(buf)); 
		if (iRet < 0 && errno != EAGAIN)
		{
			perror("recv failed");
			return -1;
		}
		else if(iRet == 0)
		{
			perror("socket not connect");
			return -1;
		}

		fprintf(stdout, "recv data is:%s\n", buf);
		
	}

	return 0;
}
