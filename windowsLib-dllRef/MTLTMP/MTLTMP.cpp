// This is the main DLL file.

#include "stdafx.h"

#include "MTLTMP.h"
#include "spaceIndex/recode.h"

namespace MTLTMP{

// init space recode
void*  SpaceRecode_init()
{
	return ( (void*)new CRecode() );
}

// close space recode
void   SpaceRecode_close(void*  pSpaceRecode)
{
	if(pSpaceRecode)
		delete ( (CRecode*)pSpaceRecode);
}
	

// get code for x,y
long SpaceRecode_GetCodeForPoint(void*  pSpaceRecode,double x, double y)
{
	if(pSpaceRecode)
		return ((CRecode*)pSpaceRecode)->AddPoint(x,y);
	else
		return -1;
}

}