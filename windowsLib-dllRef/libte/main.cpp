#include <iostream>
#include <time.h>

#include "SpaceCode.h"
#include "MTLTMP.h"
using namespace MTLTMP;
using namespace std;

void test()
{
	void* pSR =  SpaceRecode_init();

	long c1 = SpaceRecode_GetCodeForPoint(pSR, 114.256, 39.24);
	long c2 = SpaceRecode_GetCodeForPoint(pSR, 114.456, 39.14);
	long c3 = SpaceRecode_GetCodeForPoint(pSR, 115.256, 39.24);

	 double lat = 39;
	 double lon = 115;
	 srand( time(NULL) );
	 for(int i = 0; i < 100; ++i)
	 {
		 lon += rand() % 100 / 100.0;
		 lat += rand() % 100 / 100.0;
		 cout<<lon<<","<<lat<<": "<<SpaceRecode_GetCodeForPoint(pSR, lon,lat)<<endl;
	 }

	cout<<"end test\n\n\n\n";	
	SpaceRecode_close( pSR );
}

int main(void)
{	
	test();

	 CSpaceRecode recode;
	 long c1 = recode.GetCodeForPoint(114.256, 39.24);
	 long c2 = recode.GetCodeForPoint(114.456, 39.14);
	 long c3 = recode.GetCodeForPoint(115.256, 39.24);

	 double lat = 39;
	 double lon = 115;
	 srand( time(NULL) );
	 for(int i = 0; i < 100; ++i)
	 {
		 lon += rand() % 100 / 100.0;
		 lat += rand() % 100 / 100.0;
		 cout<<lon<<","<<lat<<": "<<recode.GetCodeForPoint(lon,lat)<<endl;
	 }

	cout<<"start\n";
	system("pause");
}