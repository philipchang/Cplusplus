#ifndef __MAPTRIXC_SOCKET_LIB__
#define __MAPTRIXC_SOCKET_LIB__

#if defined WIN32 || defined _WIN32_WCE
	#include <winsock.h>
	#define InitialSocket(nVers)		\
	{									\
		WSADATA wsaData;				\
		WSAStartup(nVers, &wsaData);	\
	}
	#define ReleaseSocket() WSACleanup()
#else
	#include <unistd.h>
	#include <netdb.h>
	#include <sys/socket.h>
	#include <arpa/inet.h>

	#define InitialSocket(nVers)
	#define ReleaseSocket()

	#define closesocket   close
	#define SOCKET        long
	// under linux, SIGPIPE must be processed.
	// signal(SIGPIPE, (void (*)(long))SockInterrupt);
#endif

#include "../mcMTL.hpp"

namespace MTL
{

template <class NONE>
class TSocket : public NONE
{
public:
	static long ConnSock(const char *pHost, long nPort);
	static long Shutdown(long nSock);
	static long WaitRead(long nSock, long s);
	static long SendData(long nSock, const char *pData, long nSize);
	static long ReadLine(long nSock, char *pData, long nSize);

	static long&LoopFlag();
	static void KeepLoop();
	static void StopLoop();
};

typedef TSocket<mcMTL> CSocket;


/////////////////////////////////////////////////////////////////////////////
//
// TSocket
//
/////////////////////////////////////////////////////////////////////////////

template <class NONE>
long TSocket<NONE>::ConnSock(const char *pHost, long nPort)
{
	sockaddr_in Addr;
	Addr.sin_addr.s_addr = inet_addr(pHost);

	if (Addr.sin_addr.s_addr == INADDR_NONE)
	{
		hostent *pHostEnt = gethostbyname(pHost);
		if (pHostEnt == NULL)
			return -1;
		Addr.sin_addr.s_addr = ((in_addr *)pHostEnt->h_addr)->s_addr;
	}

	Addr.sin_family = AF_INET;
	Addr.sin_port   = htons((WORD)nPort);

	long nSock = (long)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nSock == -1)
		return -1;

	if (connect(nSock, (sockaddr *)&Addr, sizeof(Addr)))
	{
		closesocket(nSock);
		return -1;
	}
	return nSock;
}

template <class NONE>
long TSocket<NONE>::Shutdown(long nSock)
{
	shutdown(nSock, 0x2);
	closesocket(nSock);
	return 0;
}

template <class NONE>
long TSocket<NONE>::WaitRead(long nSock, long s)
{
	while(s-- > 0 && LoopFlag())
	{
		timeval tv;
		tv.tv_sec  = 1;
		tv.tv_usec = 0;

		fd_set rfds;
		FD_ZERO(&rfds);
		FD_SET((SOCKET)nSock, &rfds);

		long r = select(nSock + 1, &rfds, 0, 0, &tv);
		if (r)
			return r;
	}
	return 0;
}

template <class NONE>
long TSocket<NONE>::SendData(long nSock, const char *pData, long nSize)
{
	do
	{
		long n;
		if ((n = send(nSock, pData, nSize, 0)) <= 0)
			break;

		pData += n;
		nSize -= n;
	}
	while(nSize > 0);
	return nSize;
}

template <class NONE>
long TSocket<NONE>::ReadLine(long nSock, char *pData, long nSize)
{
	if (--nSize <= 0)
		return 0;

	long nIdxs = 0;
	do
	{
		char c;
		if (WaitRead(nSock, 30) <= 0 || recv(nSock, &c, 1, 0) <= 0)
			break;
		if (c == '\n')
			break;
		if (c != '\r' && nIdxs < nSize)
			pData[nIdxs++] = c;
	}
	while (1);

	pData[nIdxs] = 0;
	return nIdxs;
}

template <class NONE>
long& TSocket<NONE>::LoopFlag()
{
	static long i = 1;
	return i;
}

template <class NONE>
inline void TSocket<NONE>::KeepLoop()
{
	LoopFlag() = 1;
}

template <class NONE>
inline void TSocket<NONE>::StopLoop()
{
	LoopFlag() = 0;
}

/////////////////////////////////////////////////////////////////////////////
//
// TSocket end
//
/////////////////////////////////////////////////////////////////////////////

}

#endif
