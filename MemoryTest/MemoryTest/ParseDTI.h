#ifndef _PARSEDTI_H_
#define _PARSEDTI_H_


/***************************************************
 * @File: DTIPaserDef.h
 * @Description: ����DTI����
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
	 * @brief: ����DTI�������ƺ���
	 * @aug: _pcDataBuf�� const char*, ָ��DTI���������׵�ַ
	 *		_nLen, int, �����������ݴ�С
	 *		_nCityID, int, ����id
	 * @return: void
	 */
	int ParseDTI(const char* _pcDataBuf, int _nLen, int _nCityID);


private:
	// ��鵱ǰ����·��msg�Ƿ���Ч
	bool IsValidPrograms();

	// ������ǰ��Ŀ
	bool ParseProgram(void);

	// ������Ŀͷ����ȡmesh no����������� 
	bool ParseProgramHead(const char* _pProgramHead);

	// ������ǰ��Ŀ����������
	bool ParseContents(void);

	// ������ǰ������
	void ParseOneContent(void);

	// ������ǰ���������ͷ
	void ParseContentHeader(void);

	// ��鵱ǰ�����Ƿ�����Ч�����ݶ�
	bool IsValidSegment(void);

	// ������ǰ���ݶ�
	void ParseDataSegments(void);

	// �������ݶ�����
	int ParseDataSegmentContent(const char* _pSegmentContent,int _nSegContLen);

	// �������ݶ����ݣ����û������ʱ�����������
	int ParseDataSegmentContentForTypeNotime(const char* _pSegmentContent,int _nSegContLen,
												unsigned char DataFlag,unsigned char LocClass,
												size_t ContinueLocs,size_t StartLoc);

	// �������ݶ����ݣ����������ʱ�����������
	int ParseDataSegmentContentForTypeHastime(const char* _pSegmentContent,int _nSegContLen,
												unsigned char DataFlag,unsigned char LocClass,
												size_t ContinueLocs,size_t StartLoc);

	// ����������Ϣ1
	void ParseExInfo1(const char* _pCont,unsigned char& _DistanceUnit,
						unsigned char& _StartPos,unsigned char& _JamLength);

	// ����������Ϣ2
	void ParseExInfo2(const char* _pCont,int& _TravelTime);

private:
	int		m_nTotalDataLen;			// DTI���������ܹ���С
	int		m_nReadDataLen;				// ��ǰ�Ѷ����������ݴ�С

	int		m_nDTIObjIndex;				// ��ǰDTI������m_vtDTIMeshSet�е�����
	int		m_nContentNum;				// ��ǰ��Ŀ���������
	int		m_nCurrMeshID;				// ��ǰmesh id
	int		m_nCityID;					// ��ǰ����id
	int		m_nUpdateTime;				// ·������ʱ��,��ʽHHMM

	const 	char*	m_pDataBuf;			// ָ��DTI���������׵�ַ
	char    m_sDataType;				// ��ʾ��ǰDTI�������ͣ�1��2��3���ֱ�Ϊ��·��ͨ״��������ʱ�䡢��ͨ���ƺ��¼���Ϣ��ͣ���յ���Ϣ
	//vector<PDTIObject> m_vtDTIMeshSet;  // ��meshΪ��Ԫ��DTI���󼯺�
};

#endif