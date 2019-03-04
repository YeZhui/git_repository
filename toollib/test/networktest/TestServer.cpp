#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <thread>
#include <sys/socket.h>
#include "../../network/SxTcp.h"

CTcp tcp;

void ReadThread(void* arg)
{
	int *pFd = (int*)arg;
	int iFd = *pFd;

	char buf[128] = {0};
	int iRet = 0;
	while(1){
				memset(buf, 0, sizeof(buf));
				iRet = tcp.Recv(iFd, buf, sizeof(buf)); 
				if (iRet < 0 )
				{
					perror("recv failed");
					tcp.Close(iFd);
					break;
				}
				else if(iRet == 0)
				{
					perror("socket not connect");
					tcp.Close(iFd);
					break;
				}

				shutdown(iFd, SHUT_WR);

				fprintf(stdout, "recv data is:%s\n", buf);
				memset(buf, 0, sizeof(buf));
				strncpy(buf, "I have redv your data,over!", sizeof(buf)-1);
				
				iRet = tcp.Send(iFd, buf, strlen(buf));
				if (iRet < 0)
				{
					perror("send failed");
					tcp.Close(iFd);
					break;
				}
				else if(iRet == 0)
				{
					perror("socket not connect");
					tcp.Close(iFd);
					break;
				}
	}
			
}

int main()
{
	int iRet = 0;
	int iFd = 0;
	char buf[128] = {0};
	
	iRet = tcp.Open();
	if (iRet == 0)
	{
		perror("socket create failed");
		return -1;
	}

	iRet = tcp.Bind("192.168.233.250", 6666);
	if (iRet == 0)
	{
		perror("socket bind failed");
		return -1;
	}

	iRet = tcp.Listen(10);
	if (iRet == 0)
	{
		perror("socket listen failed");
		return -1;
	}

	while(1)
	{
		memset(buf, 0, sizeof(buf));

		iFd = tcp.Accept();
		if (iFd == -1 )
		{
			perror("socket accept failed");
			return -1;
		}

		std::thread t_read(ReadThread, (void*)&iFd);
		t_read.detach();

	}

	return 0;
}
