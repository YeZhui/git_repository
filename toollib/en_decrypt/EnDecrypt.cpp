#include <string.h>
#include <map>
#include <iostream>
#include <string>
#include "EnDecrypt.h"
#include "Md5.h"
#include "BigInt.h"
#include "RSA.h"

namespace En_Decrypt
{
	using namespace std;
	RSA g_clRsa;
	static char g_szBase64Code[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	/*
	**返回值:0  正确      非0  则返回目标字符串所需要的长度
	**p_iSrcLen  要是strlen(p_szSrcStr)
	*/
	int Base64Encode(const char *p_szSrcStr, int p_iSrcLen, char *p_szDestStr, int p_iDestLen)
	{
		char c1,c2,c3;
		char *pDest = p_szDestStr;
		int iMod = p_iSrcLen % 3;
		int iNum = p_iSrcLen / 3;
		unsigned int iIndex = 0,j;
		int n = (iMod==0)?iNum:(iNum+1);
		if (p_iDestLen < n*4)
		{
			return (n*4);
		}

		for (;iIndex < iNum;iIndex++)
		{
			j = iIndex*3;
			c1 = p_szSrcStr[j];
			c2 = p_szSrcStr[j+1];
			c3 = p_szSrcStr[j+2];
			*pDest++ = g_szBase64Code[c1 >> 2];
			*pDest++ = g_szBase64Code[((c1 & 3) << 4) | (c2 >> 4)];
			*pDest++ = g_szBase64Code[((c2 & 0xF) << 2) | (c3 >> 6)];
			*pDest++ = g_szBase64Code[(c3 & 63)];
		}
		if (iMod != 0)
		{
			*pDest++ = g_szBase64Code[p_szSrcStr[iIndex*3] >> 2];
			if (iMod == 1)
			{
				*pDest++ = g_szBase64Code[(( p_szSrcStr[iIndex*3] & 3) << 4)];
				*pDest++ = '=';
			}
			else if(iMod == 2)
			{
				*pDest++ = g_szBase64Code[((p_szSrcStr[iIndex*3] & 3) << 4) | (p_szSrcStr[iIndex*3+1] >> 4)];
				*pDest++ = g_szBase64Code[(p_szSrcStr[iIndex*3+1] & 0xF)];
			}
			*pDest++ = '=';
		}
		return 0;
	}

	int Base64Decode(const char *p_szSrcStr, int p_iSrcLen, char *p_szDestStr, int p_iDestLen)
	{
		char *pDest = p_szDestStr;
		char c1,c2,c3,c4;
		int iNum = p_iSrcLen / 4;
		if (p_iDestLen < iNum*3)
		{
			return iNum*3;
		}
		map<char,int> base64Map;
		for(int i=0;i<strlen(g_szBase64Code);i++)
		{
			base64Map[g_szBase64Code[i]] = i;
		}
		for(int i=0;i<iNum;i++)
		{
			c1 = base64Map[p_szSrcStr[i*4]];
			c2 = base64Map[p_szSrcStr[i*4+1]];
			c3 = base64Map[p_szSrcStr[i*4+2]];
			c4 = base64Map[p_szSrcStr[i*4+3]];
			if (c3 == '=' && c4 == '=')
			{
				*pDest++ = (c1 << 2) | (c2 >> 4);
				break;
			}
			else if(c4 == '=')
			{
				*pDest++ = (c1 << 2) | (c2 >> 4);
				*pDest++ = (c2 << 4) | (c3 >> 2);
				break;
			}
			else
			{
				*pDest++ = (c1 << 2) | (c2 >> 4);
				*pDest++ = (c2 << 4) | (c3 >> 2);
				*pDest++ = (c3 << 6) | c4;	
			}
		}
		return 0;
	}

	int Md5Encrypt(const char *p_szSrcStr, int p_iSrcLen, char *p_szMd5Str, int p_iMd5Len)
	{
		string md5Str("");
		md5Str = getMD5(string(p_szSrcStr));
		strncpy(p_szMd5Str, md5Str.c_str(),p_iMd5Len-1);
		return 0;
	}

	//RSA初始化
	void RSAInit(const unsigned p_uiLen)
	{
		g_clRsa.init(p_uiLen);
	}

	//入参p_szSrcStr需为十六进制字符串
	int RSAPublicEncrypt(const string p_szSrcStr, string &p_szDestStr)
	{
		string str = p_szSrcStr;
		if ( !g_clRsa.islegal(str))
		{
			std::cout << "the string for encrypt:[" << p_szSrcStr << "] not regulation" << endl;
			return -1;
		}
		BigInteger m(str);  //m为明文
		BigInteger c = g_clRsa.encryptByPublic(m);  //使用公钥加密，c就是密文

		p_szDestStr = c.toString();
		
		return 0;
	}

	//入参p_szSrcStr需为十六进制字符串
	int RSAPrivateDecrypt(const string p_szSrcStr, string &p_szDestStr)
	{
		string str = p_szSrcStr;
		if ( !g_clRsa.islegal(str))
		{
			std::cout << "the string for decrypt:[" << p_szSrcStr << "] not regulation" << endl;
			return -1;
		}
		BigInteger m(str);  //m为密文
		BigInteger c = g_clRsa.decryptByPrivate(m);  //使用私钥解密，c就是解密后的明文

		p_szDestStr = c.toString();
		return 0;
	}

	int RSAPrivateEncrypt(const string p_szSrcStr, string &p_szDestStr)
	{
		string str = p_szSrcStr;
		if ( !g_clRsa.islegal(str))
		{
			std::cout << "the string for encrypt:[" << p_szSrcStr << "] not regulation" << endl;
			return -1;
		}
		BigInteger m(str);  //m为明文
		BigInteger c = g_clRsa.encryptByPrivate(m);  //使用私钥加密，c就是加密后的密文

		p_szDestStr = c.toString();
		return 0;
	}

	int RSAPublicDecrypt(const string p_szSrcStr, string &p_szDestStr)
	{
		string str = p_szSrcStr;
		if ( !g_clRsa.islegal(str))
		{
			std::cout << "the string for decrypt:[" << p_szSrcStr << "] not regulation" << endl;
			return -1;
		}
		BigInteger m(str);  //m为密文
		BigInteger c = g_clRsa.decryptByPublic(m);  //使用公钥解密，c就是解密后的明文

		p_szDestStr = c.toString();
		return 0;
	}

}



