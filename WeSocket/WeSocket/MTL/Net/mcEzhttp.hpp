#ifndef __MAPTRIXC_EZHTTP_LIB__
#define __MAPTRIXC_EZHTTP_LIB__

#include "../mcMTL.hpp"
#include "mcSocket.hpp"
#include <string.h>
#include <stdio.h>

namespace MTL
{

class HTTPHead
{
public:
	char *Name;
	char *Data;
};

class HTTPData
{
public:
	char *Data;
	long  Size;
};

typedef void (*DATAHANDLER)(HTTPData *, void *);
typedef void (*HEADHANDLER)(HTTPHead *, void *);

template <class NONE>
class TEzhttp : public NONE
{
public:
	TEzhttp();
	~TEzhttp();

	void AddHead(const char *head, const char *vals);
	long Request(const char *http, char *recv, long lens, const char *post = NULL, long send = 0);

	void SetHandle(DATAHANDLER pDataFun, void *pContext) { m_pDataFun = pDataFun; m_pDataCxt = pContext; }
	void SetHeadle(HEADHANDLER pHeadFun, void *pContext) { m_pHeadFun = pHeadFun; m_pHeadCxt = pContext; }
	void CloseHttp();

public:
	static void urlEncode(char *dst, const char *src);
	static void urlDecode(char *dst, const char *src);

protected:
	long GetHead(long sock);
	long GetData(long sock, char *recv, long lens, long need);
	long GetLens(long sock);

public:
	long        m_bChunked;
	DATAHANDLER m_pDataFun;
	HEADHANDLER m_pHeadFun;
	void       *m_pDataCxt;
	void       *m_pHeadCxt;

	long m_nSock;
	char m_cHost[64];
	long m_nPort;
};

typedef TEzhttp<mcMTL> CEzhttp;

/////////////////////////////////////////////////////////////////////////////
//
// TEzhttp
//
/////////////////////////////////////////////////////////////////////////////

template <class NONE>
TEzhttp<NONE>::TEzhttp()
{
	m_pDataFun = NULL;
	m_pHeadFun = NULL;
	m_pDataCxt = NULL;
	m_pHeadCxt = NULL;

	m_nSock = -1;
	m_nPort = 80;
}

template <class NONE>
TEzhttp<NONE>::~TEzhttp()
{
	CloseHttp();
}

template <class NONE>
void TEzhttp<NONE>::AddHead(const char *head, const char *vals)
{
}

template <class NONE>
long TEzhttp<NONE>::Request(const char *http, char *recv, long lens, const char *post, long send)
{
	if (http == NULL || recv == NULL)
		return -1;

	m_bChunked = 0;

	if (http[0] == 'h' && http[1] == 't' && http[2] == 't' && http[3] == 'p')
		http += 7;

	char host[64];
	{
		long i = 0;
		for(; *http != 0 && *http != '/' && *http != ':' && i < 63; i++, http++)
			host[i] = *http;
		host[i] = 0;
	}

	long port;
	if (*http == ':')
	{
		port = 0;
		for(http++ ; *http != 0 && *http != '/'; http++)
			port = port * 10 + *http - '0';
	}
	else
	{
		port = 80;
	}

	long sock = -1;
	if (m_nSock != -1)
	{
		if (strcmp(m_cHost, host) == 0 && m_nPort == port)
			sock = m_nSock;
		else
			CSocket::Shutdown(m_nSock);

		m_nSock = -1;
	}

	if (sock == -1)
	{
		sock = CSocket::ConnSock(host, port);
		if (sock == -1)
			return -1;
	}

	long need = 0;
	do
	{
		{
		#define _HTTP_GETS_			"GET "
		#define _HTTP_POST_			"POST "
		#define _HTTP_VERS_			" HTTP/1.1\r\n"
		#define _HTTP_CTRL_			"\r\n"
		#define _HTTP_GETS_LENS_	4
		#define _HTTP_POST_LENS_	5
		#define _HTTP_VERS_LENS_	11
		#define _HTTP_CTRL_LENS_	2

			if (post != NULL)
				CSocket::SendData(sock, _HTTP_POST_, _HTTP_POST_LENS_);
			else
				CSocket::SendData(sock, _HTTP_GETS_, _HTTP_GETS_LENS_);

			if (*http == 0)
				http = "/";

			CSocket::SendData(sock, http, strlen(http));
			CSocket::SendData(sock, _HTTP_VERS_, _HTTP_VERS_LENS_);

			char http[256];
			long size = 0;
			if (send != 0)
				size += sprintf(http + size, "Content-Length: %ld\r\n", send);

		#if 1
				size += sprintf(http + size, "User-Agent: Mozilla/4.0\r\n"
											 "Host: %s\r\n", host);
		#endif
			if (size != 0)
				CSocket::SendData(sock, http, size);

			CSocket::SendData(sock, _HTTP_CTRL_, _HTTP_CTRL_LENS_);
		}
		if (post != NULL)
		{
			CSocket::SendData(sock, post, send);
		}

		need = GetHead(sock);
		need = GetData(sock, recv, lens, need);
	}
	while (0);

	if (m_nSock == -1)
	{
		CSocket::Shutdown(sock);
	}
	else
	{
		strcpy(m_cHost, host);
		m_nPort = port;
	}
	return need;
}

template <class NONE>
void TEzhttp<NONE>::CloseHttp()
{
	if (m_nSock == -1)
	{
		CSocket::Shutdown(m_nSock);
		m_nSock = -1;
	}
}

template <class NONE>
long TEzhttp<NONE>::GetHead(long sock)
{
	char head[64];

	long lens = -1;
	while(CSocket::ReadLine(sock, head, sizeof(head)) > 0)
	{
		char *temp;
		if ((temp = strchr(head, ':')) != NULL)
		{
			*temp++ = 0;
			while(*temp == ' ') temp++;

			if (m_pHeadFun)
			{
				HTTPHead httpHead;
				httpHead.Name = head;
				httpHead.Data = temp;
				(*m_pHeadFun)(&httpHead, m_pHeadCxt);
			}
		}
		long *flag = (long *)head;
		// Content-Length: xxxx
		if (flag[0] == 0x746e6f43 &&
			flag[1] == 0x2d746e65 &&
			flag[2] == 0x676e654c &&
			flag[3] == 0x20006874)
		{
			for(lens = 0; *temp != 0; temp++)
				lens = lens * 10 + *temp - '0';
			continue;
		}
		// Transfer-Encoding: chunked
		if (flag[0] == 0x6e617254 &&
			flag[1] == 0x72656673 &&
			flag[2] == 0x636e452d &&
			flag[3] == 0x6e69646f)
		{
			if (temp != NULL && strcmp(temp, "chunked") == 0)
				m_bChunked = -1;
			continue;
		}
		// Connection: keep-alive
		if (flag[0] == 0x6e6e6f43 &&
			flag[1] == 0x69746365 &&
			flag[2] == 0x20006e6f)
		{
			if (temp != NULL && strcmp(temp, "keep-alive") == 0)
				m_nSock = sock;
			continue;
		}
	}
	return lens;
}

template <class NONE>
long TEzhttp<NONE>::GetData(long sock, char *buff, long lens, long need)
{
	if (lens < need || need == 0)
		return 0;
	long size = 0;
	long sums = 0;

	if (m_bChunked)
	{
		for(;;)
		{
			long n = GetLens(sock);
			if (n == 0)
				break;

			need = size + n;
			if (need > lens)
				need = lens;
			do
			{
				if (CSocket::WaitRead(sock, 30) <= 0)
					break;

				long read = recv(sock, buff + size, need - size, 0);
				if (read <= 0)
					break;
				size += read;
				sums += read;

				if (m_pDataFun)
				{
					HTTPData httpData;
					httpData.Data = buff;
					httpData.Size = size;
					(*m_pDataFun)(&httpData, m_pDataCxt);
					size = httpData.Size;
				}
			}
			while(need > sums);

			{
				char temp[32];
				if (CSocket::ReadLine(sock, temp, sizeof(temp)) > 0)
					break;
			}
		}
	}
	else
	{
		if (need < 0)
			need = lens;
		do
		{
			if (CSocket::WaitRead(sock, 30) <= 0)
				break;

			long read = recv(sock, buff + size, need - size, 0);
			if (read <= 0)
				break;
			size += read;
			sums += read;

			if (m_pDataFun)
			{
				HTTPData httpData;
				httpData.Data = buff;
				httpData.Size = size;
				(*m_pDataFun)(&httpData, m_pDataCxt);
				size = httpData.Size;
			}
		}
		while(need > sums);
	}
	return sums;
}

template <class NONE>
long TEzhttp<NONE>::GetLens(long sock)
{
	char temp[32];
	if (CSocket::ReadLine(sock, temp, sizeof(temp)) <= 0)
		return 0;

	long n = 0;
	long i = 0;
	for(; i < 32; i++)
	{
		char c = temp[i];
		if (c == 0)
			break;
		if (c >= '0' && c <= '9')
			n = (n << 4) + c - '0';
		else if (c >= 'A' && c <= 'F')
			n = (n << 4) + c - '7';	//'7' = 'A' - 10;
		else if (c >= 'a' && c <= 'f')
			n = (n << 4) + c - 'W';	//'W' = 'a' - 10;
	}
	if (n == 0)
		CSocket::ReadLine(sock, temp, sizeof(temp));

	return n;
}

template <class NONE>
void TEzhttp<NONE>::urlEncode(char *dst, const char *src)
{
	BYTE ch;
	while((ch = (BYTE)(*src++)) != 0)
	{
		if ('0' <= ch && ch <= '9' ||
			'A' <= ch && ch <= 'Z' ||
			'a' <= ch && ch <= 'z')
			*dst++ = ch;
		else if(ch == ' ')
			*dst++ = '+';
		else
		{
#define Int2Hex(n) ((n) < 10 ? (n) + '0' : (n) + '7')
			*dst++ = '%';
			*dst++ = Int2Hex(ch >> 4);
			*dst++ = Int2Hex(ch & 15);
#undef Int2Hex
		}
	}
	*dst = 0;
}

template <class NONE>
void TEzhttp<NONE>::urlDecode(char *dst, const char *src)
{
	BYTE ch;
	while((ch = *src++) != 0)
	{
		if (ch == '+')
			*dst++ = ' ';
		else if (ch == '%')
		{
#define Hex2Int(n) ((n) < 'A' ? (n) - '0' : (n) - '7')
			char h = *src++;
			char l = *src++;
			*dst++ = ((Hex2Int(h)) << 4) + (Hex2Int(l));
#undef Hex2Int
		}
		else
			*dst++ = ch;
	}
	*dst = 0;
}

/////////////////////////////////////////////////////////////////////////////
//
// TEzhttp end
//
/////////////////////////////////////////////////////////////////////////////

}

#endif
