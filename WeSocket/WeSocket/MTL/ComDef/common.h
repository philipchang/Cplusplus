
/*! 
    \file       common.h
    \brief      定义TBT整个工程的基础定义与公共宏，所有TBT文件必须包含此文件
    \details    无
	\author     yxy
*/ 

#ifndef __TBT_BASELIB_COMMON_H__
#define __TBT_BASELIB_COMMON_H__

#include "typedef.h"

//#############################################################################
//    平台设置宏定义
//#############################################################################


//! 交通雷达厂商ID定义
#define TR_USEER  0  // 0是TBT使用， 1是其他厂商使用

#if TR_USEER	
	#if defined(IOS) 
		#define TR_COMPANY_ID "AN_JTLD_IOS_FC"
	#else  
		#define TR_COMPANY_ID "AN_JTLD_ADR_FC"
	#endif
#endif

//#############################################################################
//    常用宏定义
//#############################################################################

#if defined(WIN32) || defined(WINCE)
#define EXPORT_C _declspec(dllexport)       //!< 接口导出宏定义
#else
#define EXPORT_C
#endif

#ifndef _Interface_
#define _Interface_ struct          //!< 接口定义宏
#endif

#if defined(WIN32) || defined(WINCE)
#include <windows.h>
#if !defined(WINCE) && defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC   //!< 只有在PC版才使用，调试是否有内存泄露
#include <stdlib.h>
#include <crtdbg.h>

#endif


#ifdef _DEBUG
    #include <assert.h>
    
    #ifdef ASSERT
        #undef ASSERT
    #endif
    
    #define ASSERT(B)  assert(B)
    
    #ifdef TRACE
        #undef TRACE
    #endif

    #define TRACE(pStr)    OutputDebugString(pStr)
#else
    #ifdef ASSERT
        #undef ASSERT
    #endif

    #define ASSERT(B)  
    
    #ifdef TRACE
        #undef TRACE
    #endif

    #define TRACE(pStr)

#endif
#else
    #ifdef ASSERT
        #undef ASSERT
    #endif

    #define ASSERT(B)  
    
    #ifdef TRACE
        #undef TRACE
    #endif

    #define TRACE(pStr)
#endif

#endif

