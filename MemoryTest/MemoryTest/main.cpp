#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "ParseDTI.h"
#include "CLog.h"
#include "CObj.h"
#include "debug_new.h"
#include <time.h> 
using namespace std;

void printfxy(int* x)
{
	printf("%x\n",++*x);
}

/**
  * @brief: quick sort simple simulate
  *
  */

class T
{
	
public:
	T():nVal(0)
	{
	}

	T(int n):nVal(n)
	{
	}
	virtual int GetKey()
	{
		return nVal;
	}
	int nVal;	
};

template<class Tbase>
int Patition( Tbase* parVal, int nLeft, int nRight )
{
	Tbase PivotVal = parVal[nLeft];
	int nPivotVal = parVal[nLeft].GetKey();
	while(nLeft < nRight)
	{
		while( (nLeft < nRight) && (parVal[nRight].GetKey()>= nPivotVal) )
			--nRight;
		
		if(nLeft < nRight)
		{
			parVal[nLeft++] = parVal[nRight];
		}
		
		while( (nLeft < nRight) && (parVal[nLeft].GetKey()<= nPivotVal) )
			++nLeft;
			
		if(nLeft < nRight)
		{
			parVal[nRight--] = parVal[nLeft];
		}	
	}
	
	parVal[nLeft] = PivotVal;
	return nLeft;	
}

template<class Tbase>
void QuickSort( Tbase* parVal, int nLeft, int nRight )
{
	if( parVal && (nLeft < nRight) )
	{
		int nPivotIndex = Patition( parVal, nLeft, nRight );
		QuickSort( parVal, nLeft, nPivotIndex - 1 );
		QuickSort( parVal, nPivotIndex + 1, nRight );
	}
}
void printfVal(T* par, int nLen)
{
	for(int n = 0; n < nLen; ++n)
		cout<<par[n].GetKey()<<" ";
	cout<<endl;
}

void testQuickSort()
{
	const int nLen = 20;
	T arVal[nLen];
	for(int n = 0; n < nLen; ++n)
		arVal[n] = T(nLen - n);

	printfVal(arVal, nLen);
	cout<<"sort\n";
	QuickSort(arVal, 0, nLen-1);
	printfVal(arVal, nLen);
}

template<class Tbase>
Tbase* MergeSort( Tbase* parVal1, int nLen1,  T* parVal2, int nLen2 )
{
	if( !(parVal1  && parVal2 && (nLen1 > 0) && (nLen2 >0) ) )
	{
		return NULL;
	}

	T* pEndVal = new (std::nothrow) T [nLen1 + nLen2];
	if(!pEndVal)
		return NULL;

	int nLoopVal1 = 0;
	int nLoopVal2 = 0;
	int nLoopEnd = 0;
	while( (nLoopVal1 < nLen1) && (nLoopVal2 < nLen2) )
	{
		if( parVal1[nLoopVal1].GetKey() < parVal2[nLoopVal2].GetKey() )
		{
			pEndVal[nLoopEnd++] = parVal1[nLoopVal1++];
		}else{
			pEndVal[nLoopEnd++] = parVal2[nLoopVal2++];
		}
	}
	while(nLoopVal1 < nLen1)
	{
		pEndVal[nLoopEnd++] = parVal1[nLoopVal1++];
	}
	while(nLoopVal2 < nLen2)
	{
		pEndVal[nLoopEnd++] = parVal2[nLoopVal2++];
	}
	
	return pEndVal;
}

void testMergeSort()
{
	const int nLen = 20;
	T arVal1[nLen];
	for(int n = 0; n < nLen; ++n)
		arVal1[n] = T(1 + n);

	T arVal2[nLen];
	for(int n = 0; n < nLen; ++n)
		arVal2[n] = T(3 + n );

	printfVal(arVal1, nLen);
	printfVal(arVal2, nLen);
	cout<<"sort\n";
	T* pEndVal = MergeSort(arVal1, nLen, arVal2, nLen);
	if(pEndVal)
		printfVal(pEndVal, 2*nLen);
}

void testexit()
{
	cout<<"exit call\n";
}

void logtest()
{
	atexit(testexit);
	try {
		auto_ptr<char>pauto(debug_new char[64]);
		char* p = (char*)pauto.get();
		memcpy((void*)p, "this is test!", 16);
		MN_LOG("main", "%s\n",p);
		
		//delete[] p;
	} catch (const std::exception& e) {
		MN_LOG("main", "Exception: %s\\n", e.what());
	}catch(...){
		cout<<"default exception\n";
	}

}
//glog
#include "glog/logging.h"
#include "glog/raw_logging.h"
void main(int argc, char* argv[])
{
	google::InitGoogleLogging(argv[0]);
	google::SetLogDestination(google::INFO, "./fcdlog.log_");    //高于INFO级别的，记录在当前目录(eclipse工程在.project同级目录)并以myInfo_为前缀的日志
	
	//google::SetStderrLogging(google::ERROR);  
	//google::LogToStderr();
	RAW_LOG(INFO, "service  init........");

	DLOG(WARNING)<<"one log warning 111";
	LOG(INFO) << "INFO  init........";
	LOG(ERROR) << "ERROR  init........";

	int* pnum;
	*pnum++;
	google::FlushLogFiles(google::GLOG_INFO); 
	google::ShutdownGoogleLogging();
	//testMergeSort();
	//logtest();
	return ;

	int a = 25;
	printfxy(&a);

	const int size = 1024*100;
	fstream File("d:\\DTITest.data", ios::in|ios::_Nocreate);
	char buf[size] = {0};
	::streampos ps = 0;
	if(File.is_open())
	{
		File.seekg(0,ios::end);
		ps = File.tellg();
		std::cout<<"read size "<<ps<<std::endl;

		{			
			File.seekg(0,ios::beg);
			File.read(buf, ps);
		}

		CParseDTI parser;
		parser.ParseDTI(buf, ps, 110000);
		File.close();
	}
}