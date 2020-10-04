#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<thread>
#include<stdio.h>
#ifdef _WIN32
#include<WinSock2.h>
#include<process.h>
typedef int socklen_t;
#else
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
typedef int SOCKET;
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef FLOAT* PFLOAT;
typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int* PUINT;
typedef const char* LPCSTR;
typedef char * LPSTR;

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif
#define closesocket(x) close(x)
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)
inline int WSAGetLastError()
{
	return errno;
}
#endif // _WIN32

enum{SERVER_PORT = 0x8765};

class CSockCqq
{
	SOCKET m_hSocket;
public:
	CSockCqq();
	~CSockCqq();
	BOOL Create(UINT nPort=0, int nType = SOCK_STREAM, LPCSTR sIP = NULL);
	int SendTo(void* buff, int nLen, LPCSTR sIP, UINT nPort);
	int Receive(void* buff, int nLen,int nFlags=0)
	{
		return recv(m_hSocket, (char*)buff, nLen, nFlags);
	}
	int Send(const void* buff, int nLen, int nFlags = 0)
	{
		return send(m_hSocket, (const char*)buff, nLen, nFlags);
	}
	BOOL Listen(int nConnectionBacklog = 5)
	{
		return !listen(m_hSocket, nConnectionBacklog);
	}
	BOOL Connect(LPCSTR sIP, UINT nPort);
	BOOL Accept(CSockCqq& socka, LPSTR sIP = NULL, UINT* pPort = NULL);
	int ReceiveFrom(void* buff, int nLen, LPSTR sIP = NULL, UINT* pPort = NULL);
	inline static int GetLastError()
	{
		return WSAGetLastError();
	}
	int GetSockName(char* sIP, UINT& nPort);
	int GetPeerName(char* sIP, UINT& nPort);
	void Close()
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}
};

class CSockCqEx :public CSockCqq
{
public:
	CSockCqEx()
	{
		if (!Create())
		{
			puts("创建失败！\n");
			return;
		}
		if (!Connect("192.168.2.134", SERVER_PORT))
		{
			puts("连接失败！\n");
			return;
		}
	}
};