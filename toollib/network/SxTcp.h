#ifndef __SXTCP_H__
#define __SXTCP_H__

#include <stdio.h>

#define TIMEOUT_SEC 1
#define MAX_READ_SIZE BUFSIZ
#define DEFAULT_EPOLL_FD_NUM 1024

//Tcp��
class CTcp
{

//���캯��
public:
    CTcp ();
    CTcp (int nSock);
    virtual ~CTcp ();

//���ز�����
public:
    int operator = (int);//��ֵ
    int operator != (int) const;//�����ڲ�����
    int operator == (int) const;//���ڲ�����

//���г�Ա����
public:
	int GetHandle () const;//ȡ��m_nSock
    int Open ();//����socket
    int Close ();//�رռ���socket
    int Connect (const char *, int) const;//���ӣ�δ���ó�ʱ��
    int Bind (const char *, int) const;//��
    int Listen (int nNum) const;//����
    int Accept () const;//��������
    int Recv(int nFd, char* buf, int nBufLen);//����˽���
    int Send(int nFd, char* buf, int nBufLen);//����˷���
    void Close (int nFd);//����˹ر�����socket
    int Send (const void *, int, int = TCP_TIMEOUT) const;//�ͻ��˷�������
    int Recv (void *, int, int = TCP_TIMEOUT) const;//�ͻ��˽�������

    int Sendn (const void *, int, int = TCP_TIMEOUT) const;//����n���ֽ�����
    int Recvn (void *, int, int = TCP_TIMEOUT) const;//����n���ֽ�����
    int RecvLine_rn (void *, int, int = TCP_TIMEOUT) const;//����һ������(��"\r\n"��β)
    int RecvLine_n (void *, int, int = TCP_TIMEOUT) const;//����һ������(��"\n"��β)

    int GetHostName (char *) const;//�õ�����������
    int GetPeerName (char *) const;//�õ��Է�������
    char **GetHostAddr () const;//�õ�������ַ
    int GetPeerAddr (int sock,char *) const;//�õ��Է���ַ

	int ConnectNoblock (const char *, int, int) const;//���ӣ����ó�ʱ��
    int SetNoblock (int nSock = -1);//����socketΪ������
    int SetBlock ();//����socketΪ����
    int SetReuseAddr ();//���õ�ַ������
    int SetSendTimeout (int);//���÷��ͳ�ʱֵ
    int SetRecvTimeout (int);//���ý��ճ�ʱֵ
    int SetSendBuf (int);//���÷��ͻ������Ĵ�С
    int SetRecvBuf (int);//���ý��ջ������Ĵ�С
    int SetKeepalive ();//���ô����

    //���Բ����ź�
    void SignalHandler( );

    static const int SOCK_ERROR;
    static const int SOCK_TIMEOUT;
    static const long TCP_TIMEOUT;

//˽�г�Ա����
private:
    int m_nSock;
};

#endif
