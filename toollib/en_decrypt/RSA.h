#ifndef __RSA_H__
#define __RSA_H__

#include "BigInt.h"

class RSA {
public:
    RSA() {}
    RSA(const unsigned len) { init(len); }    // ����len��ʼ������
    ~RSA() {}

    void init(const unsigned);// ��ʼ��,������˽Կ��
    void reset(const unsigned);

    BigInteger encryptByPublic(const BigInteger &);    // ��Կ����
    BigInteger decryptByPrivate(const BigInteger &);// ˽Կ����

    // ������Ҫ��������ǩ��
    BigInteger encryptByPrivate(const BigInteger &);// ˽Կ����
    BigInteger decryptByPublic(const BigInteger &);    // ��Կ����

    //�жϴ������ַ����Ƿ�Ϸ�
    bool islegal(const std::string str);
protected:
    friend std::ostream & operator << (std::ostream &, const RSA &);// ����������
private:
    BigInteger createOddNum(unsigned);// ����һ��������,����Ϊ�䳤��
    bool isPrime(const BigInteger &, const unsigned);// �ж��Ƿ�Ϊ����
    BigInteger createRandomSmaller(const BigInteger &);// �������һ����С����
    BigInteger createPrime(unsigned, const unsigned);// ����һ��������,����Ϊ�䳤��
    void createExponent(const BigInteger &);// �����ṩ��ŷ�������ɹ�Կ��˽Կָ��
public:
    BigInteger n, e;// ��Կ
private:
    BigInteger d;// ˽Կ
    BigInteger p, q;// ������p��q
    BigInteger eul;// n��ŷ������
};

#endif // __RSA_H__
