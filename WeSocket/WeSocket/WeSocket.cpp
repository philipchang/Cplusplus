// WeSocket.cpp : Defines the entry point for the console application.
//
#include "MTL/GBCode/mcGBCode.h"
#include "MTL/Net/mcEzhttp.hpp"
#include <locale.h>
#include <wchar.h>
#include <iostream>
using namespace std;

using TBT_BaseLib::mcGBCode;
#define _USE_XML 1

BOOL parserTmc(char* pData, int nSize, char* pInfo)
{

	if (!pData || nSize == 0 || !pInfo)
	{
		return 0;
	}


	char* p = pData;
	char* headTmc = NULL;
	char* headError = NULL;

	int nCode = 0;

	for(; *p != 0; ++p)
	{		
#if _USE_XML
		int offset = 13;
		if(memcmp(p, "<description>", offset) == 0)
#else
		int offset = 15;
		if(memcmp(p, "\"description\":\"", offset) == 0)
#endif
		{
			p += offset;
			headTmc = p;
		}

#if _USE_XML		
		if(headTmc 
			&& (memcmp(p, "</description>", 14) == 0))
#else
		if(headTmc 
			&& (memcmp(p, "\",\"", 3) == 0))
#endif
		{			
			*p = 0;
			nCode = 1;
			strcpy(pInfo, headTmc); 
			break;
		}

#if _USE_XML	
		if(memcmp(p, "<error>", 7) == 0)
		{
			p += 7;
			headError = p;
		}

		if(headError && 
			(memcmp(p, "</error>", 8) == 0))
		{
			*p = 0;
			nCode = 2;
			strcpy(pInfo, headError); 

			break;
		}
#endif
	}
	return nCode;
}

BOOL parseRouteTrafficResult(char* pBuf, int iSize)
{
	char cTmc[512] = {0};
	if(parserTmc(pBuf, iSize, cTmc) != 1)
	{
		return FALSE;
	}

#ifdef WIN32
	int len = 512 ;
	WORD wStrBufer[512] = {0};
    int size =strlen(cTmc);
	
	mcGBCode::UTF8ToUnicode((WORD*)wStrBufer, len,(BYTE*)cTmc);
	setlocale(LC_CTYPE,"chs");
	printf("%S\n", wStrBufer);

#endif
#ifdef linux
	// linux, string is utf-8
	printf("%s\n",cTmc);
#endif
		
	printf("test end\n");
	return TRUE;
}

#include "CXmlProcer.h"
void test()
{
	CXmlProcer xmler;
	xmler.CreateXml("src.xml");
	xmler.PaintXml("src.xml");
}

int main(void)
{
	// test();
	typedef MTL::CEzhttp HTTP;
	HTTP m_xHttp;

	long lens = 0;
	long nums = 0;
	char m_cBuff[1024*96] = {};

#if _USE_XML	
	char strUrl[] = {"http://traffic.chetuobang.com/tmc/front.xml?locus=116.474469,39.953931&lh=78&speed=60&_en=0&range=5"};
#else
	char strUrl[] = {"http://192.168.146.134/helloworld?arg=2222222222&cmdtype=logon&usercode=AN_Amap_ADR_FC&userid=13120&userbatch=0&deviceid=13120"};
	/*{"http://traffic.chetuobang.com/tmc/front.json?locus=116.474469,39.953931&lh=78&speed=60&_en=0&range=5"};
	*/
#endif

	//{"http://traffic.chetuobang.com/tmc/city.byte?_en=1&citycode=110000"};
	//{"http://m.amap.com/amapsrv/MPS?t=TMCV&mesh=132100133"};

	InitialSocket(0x0101); // init

	do
	{
		lens = m_xHttp.Request(strUrl, m_cBuff, sizeof(m_cBuff));
		if (lens >= 0 || ++nums >= 3)
			break;
	}while(1);

	parseRouteTrafficResult(m_cBuff, lens);

	//fprintf(stdout,"lens %d			nums %d\n %s\n",lens,nums,m_cBuff);
	return 0;
}

