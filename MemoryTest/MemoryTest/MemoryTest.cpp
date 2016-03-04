// MemoryTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GeoImage.h"
#include <stdexcept>
#include <stdio.h>
#include <cstdlib>
#include "CObj.h"
#include "CLog.h"
#include "CRect.h"
//引用头文件
#include "debug_new.h"
#include <string>


void test1()
{
	int* p = debug_new int(); 
	//delete p;
}

void test()
{
	int* p = debug_new int(); 
	delete p;

	COrg* pObj = debug_new COrg();
	delete pObj;
}

using namespace std;


typedef enum{
	INIT_STATE = 1,
	WORD_STATE,
	SPACE_STATE,
};

int count_word_number(const char* pStr)
{
	int count = 0;
	int state = INIT_STATE;
	char value ;

	if(NULL == pStr)
		return 0;

	while(value = *pStr++){

		if('\0' == *pStr)
			break;

		switch (state)
		{
		case INIT_STATE:
			if(' ' != value)
				count ++, state = WORD_STATE;
			else
				state = SPACE_STATE;
			break;
		case WORD_STATE:
			if(' ' == value)
				state = SPACE_STATE;				
			break;
		case SPACE_STATE:
			if(' ' != value)
				count ++, state = WORD_STATE;
			break;
		default:
			break;
		}
	}
	return count;
}

#include <cassert>
void testCount()
{
	assert(0 == count_word_number(NULL));
	assert(0 == count_word_number(""));
	assert(1 == count_word_number("hello"));
	assert(3 == count_word_number("china baby hello"));
}

 /////Check函数功能：检验第n行的皇后是否和之前的皇后有冲突，没有的话返回1
int Check(int a[],int n)
{
	for(int i=1;i<n;i++)
	{
		if(abs(a[i]-a[n])==abs(i-n) || a[i]==a[n])//////////////见下面注释
			return 0;
	}
	return 1;
}

void for_queue()
{
	CTimerCls timer;

	int a[9];
	int i,t=1;
	for(a[1]=1;a[1]<9;a[1]++)
		for(a[2]=1;a[2]<9;a[2]++)
		{
			if(!Check(a,2))	continue;
			for(a[3]=1;a[3]<9;a[3]++)
			{
				if(!Check(a,3))	continue;
				for(a[4]=1;a[4]<9;a[4]++)
				{
					if(!Check(a,4))	continue;
					for(a[5]=1;a[5]<9;a[5]++)
					{
						if(!Check(a,5))	continue;
						for(a[6]=1;a[6]<9;a[6]++)
						{
							if(!Check(a,6))	continue;
							for(a[7]=1;a[7]<9;a[7]++)
							{
								if(!Check(a,7))	continue;
								for(a[8]=1;a[8]<9;a[8]++)
								{
									if(!Check(a,8))	continue;
									else 
									{
										/*printf("第%d种解法：\n",t++);
										for(i=1;i<9;i++)
											printf("第%d个皇后：%d\n",i,a[i]);
										printf("\n\n");*/
}		}	}	}	}	}	}	}	}


int main1()
{
	for_queue();

	//atexit(test1);
	testCount();

	MN_LOG("main", "COrg size %d\n", sizeof(COrg));
	
	for(size_t i = 0; i < 100000; ++i)
		test();
	test1();

	try {
		char* p = debug_new char[64];
		memcpy(p, "this is test!", 16);
		MN_LOG("main", "%s\n",p);
		
		delete[] p;
	} catch (const std::runtime_error& e) {
		MN_LOG("main", "Exception: %s\\n", e.what());
	}
	return 0;
}

int a[20],n,i,j,t=1;////////////////////////////////////////全局变量
int m_nTotalFunNum = 0;
void Try(int i)
{	
	int j,k;
	for(j=1;j<=n;j++)
	{
		a[i]=j;
		if(Check(a,i))///////如果第j列不会与之前的皇后冲突
		{
			if(i<n)/////如果i<n，即还没有找到八个皇后，继续递归
				Try(i+1);
			else ////如果找到了一组解就输出
			{
				//printf("第%d种解法：\n",t++);
				if(++m_nTotalFunNum == 192){
					for(k=1;k<=n;k++)
						printf("第%d个皇后：%d\n",k,a[k]);
					printf("\n\n");
				}

			}
		}
	}
}



void RecursionQueen()
{
	CTimerCls timer;
	//printf("几皇后？n=");
	//scanf("%d",&n);
	n = 8;
	Try(1);
}


#define ulong long long

ulong gcd2(ulong a, ulong b)
{
  if(b == 0)
  	return a;
  	
  return gcd2(b, a % b);
}

ulong gcd3(ulong a, ulong b)
{
 if(a == 0)
 	return b;
 if(b == 0)
  return a;
  
  
 if(a % 2 == 0 && b % 2 == 0)
   return 2 * gcd3(a>>1, b>>1);
 else if(a % 2 == 0)
   return gcd3(a>>1, b);
 else if(b % 2 == 0)
   return gcd3(a, b>>1);
 else
   return gcd3(abs(a-b), min(a,b));
}

void gcd2main(ulong a, ulong b)
{
		CTimerCls timer;
		for(size_t i = 0; i < 10000; ++i)
			gcd2( a,  b);
			//cout<<"gcd2main "<<gcd2( a,  b)<<endl;
}
void gcd3main(ulong a, ulong b)
{
		CTimerCls timer;

		for(size_t i = 0; i < 10000; ++i)
			gcd3( a,  b);
			//cout<<"gcd3main "<<gcd3( a,  b)<<endl;
}
class CNum1
{
public:
	~CNum1()
	{
		int i = 0;
		+i;
	}

	
private:
	int a;
};

void testRect(vector<CWRect2Di>& vtRects, int i )
{
	int x = 10, y = 30, w = 20, h = 20; 
	CWRect2Di temprect = CWRect2Di((int)x, (int)(y + h), (int)(x + w), (int)y);
	vtRects[i] = temprect;
}
int main3 ()
{
	vector<CWRect2Di> vtRects;
	CWRect2Di tmep;
	
	for(int i = 0; i < 3; ++i){
		vtRects.push_back(tmep);
		testRect(vtRects,i);
	}

	for(int i = 0; i < vtRects.size(); ++i)
	{
		cout<<endl<<endl<<i<<"	"<<vtRects[i].left<<"--"<<vtRects[i].bottom<<"--"<<vtRects[i].right<<"--"<<vtRects[i].top<<endl<<endl;
	}

	gcd2main(131211111112, 1455555555582);
	gcd3main(131211111112, 1455555555582);

	void whileQueen();
	cout<<"whileQueen() \n";
	whileQueen();

	cout<<"for_queue() \n";
	for_queue();

	cout<<"RecursionQueen() \n";
	RecursionQueen();

	CQueen queen(8);
	cout<<"CQueen() \n";
	queen.Queen();
		 
	return 0;
}

void whileQueen()
{
	CTimerCls timer;
	int a[256]={0};
	int totalNum = 0;
	int i=1,j,n,t=1;///i表示当前正在搜索第i行的皇后位置
	n = 8;

	while(i>0)
	{
		for(++a[i]; a[i]<=n; ++a[i])
		{
			if(Check(a,i))//如果第i行的皇后与之前的皇后位置上没有冲突，则break跳出循环
				break;
		}
		if(a[i]<=n)//如果a[i]<=n，即上面的for循环是由“break;”跳出来的,即第i行皇后的位置符合条件
		{
			if(i==n)////找到一组解，输出
			{
				/*printf("第%d种解法：\n",t++);
				for(j=1;j<=n;j++)
					printf("第%d个皇后：%d\n",j,a[j]);
				printf("\n\n");*/
				++totalNum;
			}
			else///未找完
			{
				i++;
				a[i]=0;
			}
		}
		else
			i--;////////////回溯
	}

	printf("while queen %d\n", totalNum);
}