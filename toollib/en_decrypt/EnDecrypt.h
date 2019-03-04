#ifndef _EN_DECRYPT_H_
#define _EN_DECRYPT_H_

#include <string>

namespace En_Decrypt
{
	int Base64Encode(const char *p_szSrcStr, int p_iSrcLen, char *p_szDestStr, int p_iDestLen);
	int Base64Decode(const char *p_szSrcStr, int p_iSrcLen, char *p_szDestStr, int p_iDestLen);
	int Md5Encrypt(const char *p_szSrcStr, int p_iSrcLen, char *p_szMd5Str, int p_iMd5Len);

	//RSA加密和解密算法
	//p_uiLen表示大素数的二进制位数，一般可指定16
	void RSAInit(const unsigned p_uiLen);
	//公钥加密
	int RSAPublicEncrypt(const std::string p_szSrcStr, std::string &p_szDestStr);
	//私钥解密
	int RSAPrivateDecrypt(const std::string p_szSrcStr, std::string &p_szDestStr);
	//私钥加密
	int RSAPrivateEncrypt(const std::string p_szSrcStr, std::string &p_szDestStr);
	//公钥解密
	int RSAPublicDecrypt(const std::string p_szSrcStr, std::string &p_szDestStr);
}

#endif

