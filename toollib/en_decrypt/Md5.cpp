#include "Md5.h"

static unsigned char PADDING[64] = {
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define F(x,y,z) (((x) & (y)) | ((~x) & (z)))
#define G(x,y,z) (((x) & (z)) | ((y) & (~z)))
#define H(x,y,z) ((x) ^ (y) ^ (z))
#define I(x,y,z) ((y) ^ ((x) | (~z)))

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

#define FF(a, b, c, d, x, s, ac)\
	{(a) += F((b),(c),(d)) + (x) + (UINT4)(ac); \
	 (a) = ROTATE_LEFT ((a), (s)); \
	 (a) += (b); \
	}


#define GG(a, b, c, d, x, s, ac)\
	{(a) += G((b),(c),(d)) + (x) + (UINT4)(ac); \
	 (a) = ROTATE_LEFT ((a), (s)); \
	 (a) += (b); \
	}

#define HH(a, b, c, d, x, s, ac)\
	{(a) += H((b),(c),(d)) + (x) + (UINT4)(ac); \
	 (a) = ROTATE_LEFT ((a), (s)); \
	 (a) += (b); \
	}

#define II(a, b, c, d, x, s, ac)\
	{(a) += I((b),(c),(d)) + (x) + (UINT4)(ac); \
	 (a) = ROTATE_LEFT ((a), (s)); \
	 (a) += (b); \
	}

void MD5Init(MD5_CTX *p_mdContext)
{
	p_mdContext->i[0] = p_mdContext->i[1] = (UINT4) 0;

	p_mdContext->buf[0] = (UINT4)0x67452301;
	p_mdContext->buf[1] = (UINT4)0xefcdab89;
	p_mdContext->buf[2] = (UINT4)0x98badcfe;
	p_mdContext->buf[3] = (UINT4)0x10325476;
}


void MD5Update(MD5_CTX *p_mdContext, char *p_pInBuf, unsigned int p_iLen)
{
	UINT4 in[16];
	int mdi;
	unsigned int i,ii;

	mdi = (int) ((p_mdContext->i[0] >> 3) & 0x3F);

	if ((p_mdContext->i[0] + ((UINT4) p_iLen << 3)) < p_mdContext->i[0])
		p_mdContext->i[1]++;
	p_mdContext->i[0] += ((UINT4) p_iLen << 3);
	p_mdContext->i[1] += ((UINT4) p_iLen >> 29);


}

void MD5Final(MD5_CTX *p_mdContext)
{
}

void Transform(UINT4 *p_Buf, UINT4 *p_In)
{
}
