
/*! 
    \file       common.h
    \brief      ����TBT�������̵Ļ��������빫���꣬����TBT�ļ�����������ļ�
    \details    ��
	\author     yxy
*/ 

#ifndef __TBT_BASELIB_COMMON_H__
#define __TBT_BASELIB_COMMON_H__

#include "typedef.h"

//#############################################################################
//    ƽ̨���ú궨��
//#############################################################################


//! ��ͨ�״ﳧ��ID����
#define TR_USEER  0  // 0��TBTʹ�ã� 1����������ʹ��

#if TR_USEER	
	#if defined(IOS) 
		#define TR_COMPANY_ID "AN_JTLD_IOS_FC"
	#else  
		#define TR_COMPANY_ID "AN_JTLD_ADR_FC"
	#endif
#endif

//#############################################################################
//    ���ú궨��
//#############################################################################

#if defined(WIN32) || defined(WINCE)
#define EXPORT_C _declspec(dllexport)       //!< �ӿڵ����궨��
#else
#define EXPORT_C
#endif

#ifndef _Interface_
#define _Interface_ struct          //!< �ӿڶ����
#endif

#if defined(WIN32) || defined(WINCE)
#include <windows.h>
#if !defined(WINCE) && defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC   //!< ֻ����PC���ʹ�ã������Ƿ����ڴ�й¶
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

