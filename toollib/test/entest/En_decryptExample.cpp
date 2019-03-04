#include "../../en_decrypt/EnDecrypt.h"
#include <iostream>
#include <string.h>
using namespace std;

int main()
{
	long lRet = 0;
	char szStr[64] = "guanyezhui";
	char szDest[64] = {0};	
	char szDeBase64[128] = {0}; 
	string str1, str2, str3;
	lRet = En_Decrypt::Base64Encode(szStr, strlen(szStr), szDest, sizeof(szDest));
	if (lRet != 0)
	{
		cout << "base64 encode failed" << endl;
		return -1;
	}
	else
	{
		cout << szDest << endl;
	}

	lRet = En_Decrypt::Base64Decode(szDest, strlen(szDest), szDeBase64, sizeof(szDeBase64));
	if (lRet != 0)
	{
		cout << "base64 decode failed" << endl;
		return -1;
	}
	else
	{
		cout << szDeBase64 << endl;
	}

	memset(szDest, 0, sizeof(szDest));
	lRet = En_Decrypt::Md5Encrypt(szStr, strlen(szStr),szDest, sizeof(szDest));
	if (lRet != 0)
	{
		cout << "md5 encrypt failed" << endl;
		return -1;
	}
	else
	{
		cout << szDest << endl;
	}

	En_Decrypt::RSAInit(16);
	lRet = En_Decrypt::RSAPublicEncrypt(string("0234ff"), str1);
	if (lRet == -1 )
	{
		cout << "RSA public encrypt failed" << endl;
		return -1;
	}
	else
	{
		cout << str1 << endl;
	}

	lRet = En_Decrypt::RSAPrivateDecrypt(str1, str2);
	if ( lRet == -1 )
	{
		cout << "RSA private decrypt failed" << endl;
		return -1;
	}
	else
	{
		cout << str2 << endl;
	}
	
	str1.clear();
	lRet = En_Decrypt::RSAPrivateEncrypt("23444fdd", str1);
	if ( lRet == -1 )
	{
		cout << "RSA private decrypt failed" << endl;
		return -1;
	}
	else
	{
		cout << str1 << endl;
	}

	lRet = En_Decrypt::RSAPublicDecrypt(str1, str2);
	if ( lRet == -1 )
	{
		cout << "RSA public decrypt failed" << endl;
		return -1;
	}
	else
	{
		cout << str2 << endl;
	}

	return 0;
}

