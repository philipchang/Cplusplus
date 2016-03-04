#ifndef _PARSEDTI_H_
#define _PARSEDTI_H_


/***************************************************
 * @File: DTIPaserDef.h
 * @Description: 解析DTI数据
 * @Author: changggy 2014-1-1
 * copy all right
 ***************************************************/

#pragma once

#include "DTIPaserDef.h"

class CParseDTI
{
public:
	CParseDTI();
	~CParseDTI();

public:

	/*
	 * @brief: 解析DTI的主控制函数
	 * @aug: _pcDataBuf， const char*, 指向DTI二进制流首地址
	 *		_nLen, int, 二进制流数据大小
	 *		_nCityID, int, 城市id
	 * @return: void
	 */
	int ParseDTI(const char* _pcDataBuf, int _nLen, int _nCityID);


private:
	// 检查当前来自路况msg是否有效
	bool IsValidPrograms();

	// 解析当前节目
	bool ParseProgram(void);

	// 解析节目头，获取mesh no和内容项个数 
	bool ParseProgramHead(const char* _pProgramHead);

	// 解析当前节目的内容项列
	bool ParseContents(void);

	// 解析当前内容项
	void ParseOneContent(void);

	// 解析当前内容项的项头
	void ParseContentHeader(void);

	// 检查当前数据是否是有效的数据段
	bool IsValidSegment(void);

	// 解析当前数据段
	void ParseDataSegments(void);

	// 解析数据段内容
	int ParseDataSegmentContent(const char* _pSegmentContent,int _nSegContLen);

	// 解析数据段内容，针对没有旅行时间的数据类型
	int ParseDataSegmentContentForTypeNotime(const char* _pSegmentContent,int _nSegContLen,
												unsigned char DataFlag,unsigned char LocClass,
												size_t ContinueLocs,size_t StartLoc);

	// 解析数据段内容，针对有旅行时间的数据类型
	int ParseDataSegmentContentForTypeHastime(const char* _pSegmentContent,int _nSegContLen,
												unsigned char DataFlag,unsigned char LocClass,
												size_t ContinueLocs,size_t StartLoc);

	// 解析扩张信息1
	void ParseExInfo1(const char* _pCont,unsigned char& _DistanceUnit,
						unsigned char& _StartPos,unsigned char& _JamLength);

	// 解析扩张信息2
	void ParseExInfo2(const char* _pCont,int& _TravelTime);

private:
	int		m_nTotalDataLen;			// DTI二进制流总共大小
	int		m_nReadDataLen;				// 当前已读二进制数据大小

	int		m_nDTIObjIndex;				// 当前DTI对象在m_vtDTIMeshSet中的索引
	int		m_nContentNum;				// 当前节目内容项个数
	int		m_nCurrMeshID;				// 当前mesh id
	int		m_nCityID;					// 当前城市id
	int		m_nUpdateTime;				// 路况更新时间,格式HHMM

	const 	char*	m_pDataBuf;			// 指向DTI二进制流首地址
	char    m_sDataType;				// 标示当前DTI数据类型，1、2、3，分别为道路交通状况和旅行时间、交通管制和事件信息、停车诱导信息
	//vector<PDTIObject> m_vtDTIMeshSet;  // 以mesh为单元的DTI对象集合
};

#endif