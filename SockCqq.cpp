#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "SockCqq.h"
#pragma comment(lib,"ws2_32.lib")
CSockCqq::CSockCqq():m_hSocket(INVALID_SOCKET)
{
#ifdef _WIN32
    WSADATA wd;
    WSAStartup(0x303, &wd);
#endif // _WIN32
}

CSockCqq::~CSockCqq()
{
    closesocket(m_hSocket);
}

BOOL CSockCqq::Create(UINT nPort, int nType, LPCSTR sIP)
{
    m_hSocket = socket(AF_INET, nType, 0);
    if (m_hSocket == INVALID_SOCKET)
        return FALSE;
    sockaddr_in sa{ AF_INET,htons(nPort) };
    if (sIP)
        sa.sin_addr.s_addr=inet_addr(sIP);
    int n = bind(m_hSocket, (sockaddr*)&sa, sizeof(sa));
    if (n)
    { 
        closesocket(m_hSocket);
        m_hSocket = INVALID_SOCKET;
    }
    return !n;
}

int CSockCqq::SendTo(void* buff, int nLen, LPCSTR sIP, UINT nPort)
{
    sockaddr_in sa = { AF_INET,htons(nPort) };
    if (sIP)
        sa.sin_addr.s_addr = inet_addr(sIP);
    return sendto(m_hSocket, (char*)buff, nLen, 0, (sockaddr*)&sa, sizeof(sa));
}

BOOL CSockCqq::Connect(LPCSTR sIP, UINT nPort)
{
    sockaddr_in sa = { AF_INET,htons(nPort) };
    sa.sin_addr.s_addr = inet_addr(sIP);
    return !connect(m_hSocket, (sockaddr*)&sa, sizeof(sa));
}

BOOL CSockCqq::Accept(CSockCqq& socka, LPSTR sIP, UINT* pPort)
{
    sockaddr_in sa;
    socklen_t n = sizeof(sa);
    SOCKET s = accept(m_hSocket, (sockaddr*)&sa, &n);
    if (s == INVALID_SOCKET)
        return FALSE;
    socka.m_hSocket = s;
    if (sIP)
        strcpy(sIP,inet_ntoa(sa.sin_addr));
    if (pPort)
        *pPort = htons(sa.sin_port);
    return TRUE;
}

int CSockCqq::ReceiveFrom(void* buff, int nLen, LPSTR sIP, UINT* pPort)
{
    sockaddr_in sa;
    socklen_t size = sizeof(sa);
    int res = recv(m_hSocket, (char*)buff, nLen, 0);
    if (res > 0)
    {
        if (sIP || pPort)
            getpeername(m_hSocket, (sockaddr*)&sa, &size);
        if (sIP)
            inet_addr(sIP);
        if (pPort)
            *pPort = htons(sa.sin_port);
    }
    return res;
}

int  CSockCqq::GetSockName(char* sIP, UINT& nPort)
{
    sockaddr_in sa;
    socklen_t Size = sizeof(sa);
    int Res = getsockname(m_hSocket, (sockaddr*)&sa, &Size);
    if (!Res)
    {
        if (sIP)
            inet_addr(sIP);
        if (nPort)
            nPort = htons(sa.sin_port);
    }
    return Res;
}

int CSockCqq::GetPeerName(char* sIP, UINT& nPort)
{
    sockaddr_in sa;
    socklen_t Size = sizeof(sa);
    int Res = getpeername(m_hSocket, (sockaddr*)&sa, &Size);
    if (!Res)
    {
        if (sIP)
            inet_addr(sIP);
        if (nPort)
            nPort = htons(sa.sin_port);
    }
    return Res;
}
