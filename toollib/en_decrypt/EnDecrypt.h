#ifndef _EN_DECRYPT_H_
#define _EN_DECRYPT_H_

namespace En_Decrypt
{
	int Base64Encode(const char *p_szSrcStr, int p_iSrcLen, char *p_szDestStr, int p_iDestLen);
	int Base64Decode(const char *p_szSrcStr, int p_iSrcLen, char *p_szDestStr, int p_iDestLen);
	int Md5Encrypt(const char *p_szSrcStr, int p_iSrcLen, char *p_szMd5Str, int p_iMd5Len);
	/*
	int SimpleEncrypt(int p_iSrcText, int p_iKey);
	int SimpleDecrypt(int p_iSrcText, int p_iKey);
	*/
	//TODO:RSAº”Ω‚√‹
}

#endif

