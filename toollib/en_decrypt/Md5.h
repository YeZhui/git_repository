#ifndef __MD5_H__
#define __MD5_H__

typedef unsigned long int UINT4;

typedef struct
{
	UINT4 i[2];
	UINT4 buf[4];
	unsigned char in[64];
	unsigned char digest[16];
}MD5_CTX;

void MD5Init(MD5_CTX *p_mdContext);
void MD5Update(MD5_CTX *p_mdContext, char *p_pInBuf, unsigned int p_iLen);
void MD5Final(MD5_CTX *p_mdContext);
void Transform(UINT4 *p_Buf, UINT4 *p_In);

#endif

