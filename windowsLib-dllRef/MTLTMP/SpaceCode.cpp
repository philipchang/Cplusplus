#include "SpaceCode.h"
#include "spaceIndex/recode.h"

using namespace MTLTMP;

CSpaceRecode::CSpaceRecode()
{
	pSpaceRecode =  (void*)new CRecode() ;
}

CSpaceRecode::~CSpaceRecode()
{
	if(pSpaceRecode)
		delete ( (CRecode*)pSpaceRecode);
}


long CSpaceRecode::GetCodeForPoint(double x, double y)
{
	if(pSpaceRecode)
		return ((CRecode*)pSpaceRecode)->AddPoint(x,y);
	else
		return -1;
}