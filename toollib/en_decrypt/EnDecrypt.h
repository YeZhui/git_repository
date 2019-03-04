#ifndef _EN_DECRYPT_H_
#define _EN_DECRYPT_H_

#include <string>

namespace En_Decrypt
{
	int Base64Encode(const char *p_szSrcStr, int p_iSrcLen, char *p_szDestStr, int p_iDestLen);
	int Base64Decode(const char *p_szSrcStr, int p_iSrcLen, char *p_szDestStr, int p_iDestLen);
	int Md5Encrypt(const char *p_szSrcStr, int p_iSrcLen, char *p_szMd5Str, int p_iMd5Len);

	//RSA���ܺͽ����㷨
	//p_uiLen��ʾ�������Ķ�����λ����һ���ָ��16
	void RSAInit(const unsigned p_uiLen);
	//��Կ����
	int RSAPublicEncrypt(const std::string p_szSrcStr, std::string &p_szDestStr);
	//˽Կ����
	int RSAPrivateDecrypt(const std::string p_szSrcStr, std::string &p_szDestStr);
	//˽Կ����
	int RSAPrivateEncrypt(const std::string p_szSrcStr, std::string &p_szDestStr);
	//��Կ����
	int RSAPublicDecrypt(const std::string p_szSrcStr, std::string &p_szDestStr);
}

#endif

