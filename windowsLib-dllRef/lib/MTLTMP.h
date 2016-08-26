// MTLTMP.h


#pragma once

#ifdef __cplusplus 
extern "C"{
#endif


namespace MTLTMP {

#if !defined(MTLTMP_API)
	#define MTLTMP_API __cdecl
#endif

// init space recode
__declspec(dllexport) void* MTLTMP_API SpaceRecode_init();

// get code for x,y
__declspec(dllexport)  long   MTLTMP_API SpaceRecode_GetCodeForPoint(void* pSpaceRecode, double x, double y);

// close space recode
__declspec(dllexport)  void  MTLTMP_API SpaceRecode_close(void*  pSpaceRecode);

}

#ifdef __cplusplus 
}
#endif