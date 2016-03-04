#include "ParseDTI.h"
#include <stdlib.h>
#include <stdio.h>

CParseDTI::CParseDTI()
	: m_nTotalDataLen(0)
	, m_nReadDataLen(0)
	, m_nDTIObjIndex(0)
	, m_nUpdateTime(0)
	, m_nContentNum(0)
	, m_pDataBuf(NULL)
	, m_sDataType(0)
	, m_nCurrMeshID(0)
	, m_nCityID(0)
{
}

CParseDTI::~CParseDTI()
{
}

int CParseDTI::ParseDTI(const char* _pcDataBuf, int _nLen, int _nCityID)
{
	int nFreshTime = 0;
	if(_pcDataBuf && _nLen > PARAGRAM_HEAD_LEN)
	{
		// ��¼���ݷ�Χ
		m_pDataBuf = _pcDataBuf;
		m_nTotalDataLen = _nLen;
		m_nReadDataLen = 0;
		m_nDTIObjIndex = 0;
		m_nUpdateTime  = 0;
		m_nCityID = _nCityID;

		if(IsValidPrograms())
		{
			// ѭ������������Ŀ��Ԫ
			do{			
				if(ParseProgram() == false)
					break; // ������ǰ��Ŀ
			}while(m_nReadDataLen < m_nTotalDataLen);
		}

		nFreshTime = m_nUpdateTime;
		//DTI_LOG_DEBUG( "[DTIEngine]","parse end %d !!!\n", m_nDTIObjIndex);
	}
	return nFreshTime;
}

bool CParseDTI::ParseProgram()
{
	bool bRet = false;

	// ������Ŀͷ����ȡmesh no����������� 
	ParseProgramHead(m_pDataBuf + m_nReadDataLen);
	m_nReadDataLen += PARAGRAM_HEAD_LEN; // �ۼ��Ѷ��Ľ�Ŀͷ��С

	//DTI_LOG_DEBUG( "[DTIEngine]","program start-------------");
	////DTI_LOG_DEBUG( "[DTIEngine]","mesh id --%d", m_vtDTIMeshSet[m_nDTIObjIndex]->mMeshID );
	//DTI_LOG_DEBUG( "[DTIEngine]","content nums --%d", m_nContentNum );

	// ������ǰ��Ŀ����������
	ParseContents();

	//DTI_LOG_DEBUG( "[DTIEngine]","program end-------------");
		
	bRet = true;
	m_nDTIObjIndex++;

	return bRet;
}

bool CParseDTI::ParseProgramHead(const char* _pProgramHead)
{
	bool bRet = false;
	m_nContentNum = 0;

	if (_pProgramHead)
	{
		int colLow = (unsigned char)*(_pProgramHead + 8 );
		int colHigh = ((unsigned char)(*(_pProgramHead + 10))& 0xf0) >>4;
		int col = (colHigh<<8) | colLow;
		colLow = col & 0x07;
		colHigh = (col>>3) & 0x1ff;

		int rowLow = (unsigned char)*(_pProgramHead + 9 );
		int rowHigh = (unsigned char)(*(_pProgramHead + 10))& 0x0f;
		int row = (rowHigh<<8) | rowLow;
		rowLow = row & 0x07;
		rowHigh = (row>>3) & 0x1ff;

		m_nCurrMeshID = (rowHigh*10000) + (colHigh * 100) + (rowLow * 10) + colLow;
	//	m_vtDTIMeshSet[m_nDTIObjIndex]->mMeshID  = (rowHigh*10000) + (colHigh * 100) + (rowLow * 10) + colLow;
		m_nContentNum = (_pProgramHead) ? ((*(_pProgramHead + 3)) & 0x3f) : 0;

		bRet =  true;
	}
	return bRet;
}

bool CParseDTI::ParseContents(void)
{
	bool bRet = false;
	if(m_nContentNum > 0)
	{
		for(int i = 0; i < m_nContentNum; ++i)
		{
			ParseOneContent();
		}
		bRet = true;
	}
	return bRet;	
}

void CParseDTI::ParseOneContent(void)
{
	// ������ǰ������ͷ��
	ParseContentHeader();

	// ������ǰ��������������ݶ���
	ParseDataSegments();
}

void CParseDTI::ParseContentHeader(void)
{
	// �����������ͣ��ֱ�Ϊ��·��ͨ״��������ʱ�䡢��ͨ���ƺ��¼���Ϣ��ͣ���յ���Ϣ
	const char* pPoint = m_pDataBuf + m_nReadDataLen;
	unsigned char DataType = 0;
	DataType = (*(pPoint + 4)) &0xf0;
	m_sDataType = (DataType >>4 ) & 0x0f;

	// �������ݶ�Ӧ�ĸ���ʱ��
	short hourHigh = (((unsigned char)*(pPoint + 4)) & 0x07) << 2;
	short hourlow = (((unsigned char)*(pPoint + 5)) & 0xc0) >>6;
	short hour = (hourHigh | hourlow) & 0x1f;

	short min = ((unsigned char)*(pPoint + 5)) & 0x3f;
	//m_vtDTIMeshSet[m_nDTIObjIndex]->mFreshTime = hour*60 + min; 
	m_nUpdateTime = hour*  60 + min;
	m_nReadDataLen += CONTENT_HEAD_LEN; // �ۼ��Ѷ��ֽ�

	//DTI_LOG_DEBUG( "[DTIEngine]","update time--%d:%d ------\n", hour, min);
}

bool CParseDTI::IsValidPrograms()
{
	bool bRet = false;  
	if((SEPARATOR == *m_pDataBuf)
		&& (PARAGRAM_HEAD_TYPE == *(m_pDataBuf + 1))
		)
	{
		bRet = true;
	}
	else
	{
		//DTI_LOG_DEBUG( "[DTIEngine]","rece road msg is error!!!!!!\n");
	}
	return bRet;
}

bool CParseDTI::IsValidSegment(void)
{
	bool bRet = false;
	if((DATASEGSE_PARATOR == *(m_pDataBuf + m_nReadDataLen))
		&& ((DATASEGTYPE_CONDITION == *(m_pDataBuf + m_nReadDataLen + 1))
			|| (DATASEGTYPE_EVENT == *(m_pDataBuf + m_nReadDataLen + 1))
			|| (DATASEGTYPE_PARK == *(m_pDataBuf + m_nReadDataLen + 1))
			)
		)
	{
		bRet = true;
	}
	else
	{
		//DTI_LOG_DEBUG( "[DTIEngine]","Curr is not content head!!!!!!\n");
	}
	return bRet;
}

void CParseDTI::ParseDataSegments(void)
{
	// �������ݶ���������
	const char* pSegStartPoint = m_pDataBuf + m_nReadDataLen;
	size_t highPart = ((unsigned char)*(pSegStartPoint + 2) & 0x7f) << 8;
	int segmentContentDataLen = highPart | (unsigned char)*(pSegStartPoint + 3);
	m_nReadDataLen += DATASEG_HEAD_LEN; // �ۼ��Ѷ������ݶ�ͷ��С

	//DTI_LOG_DEBUG( "[DTIEngine]","���ݶ�����:%d ------\n", segmentContentDataLen);
	if (segmentContentDataLen > 0)
	{
		int curSegmentReadLen = 0;
		while(curSegmentReadLen < segmentContentDataLen)
		{
			const char* pCurSegmentContent = m_pDataBuf + m_nReadDataLen + curSegmentReadLen;
			curSegmentReadLen += ParseDataSegmentContent(pCurSegmentContent,segmentContentDataLen);
		}
		m_nReadDataLen += segmentContentDataLen; // �ۼ��Ѷ������ݶ����ݴ�С
	}
}

int CParseDTI::ParseDataSegmentContent(const char* _pSegmentContent,int _nSegContLen)
{
	int nRet = 4;
	const char* pContent = _pSegmentContent + 4; // ����ͷ

	// �������ݶ�����ͷ��4���ֽ�
	// �������ͣ�0����·������ʱ�䣻1��û��·������ʱ��
	unsigned char DataType = ((unsigned char)*_pSegmentContent & 0x80) >>7;

	// ����ʱ�����0ʵʱ��Ϣ�� 1Ԥ����Ϣ
	unsigned char TravelTimeType = ((unsigned char)*_pSegmentContent & 0x40) >>6;

	// �Ƿ�ʹ����ʼ·��������ȫ��·����·����Ϣ
	// 0 ��ʾʹ������·��˳������Ϣ�����ֱ�洢
	// 1 ��ʾ����������·���Ķ�����Ϣ��ͬ������ʹ����ʼ ·������ʾ������Ϣ����ʡ�Ժ���·����Ϣ��ͬ��·����Ϣ
	unsigned char DataFlag = ((unsigned char)*_pSegmentContent & 0x20) >>5;

	// ����loc��
	size_t ContinueLocs =  ((unsigned char)*_pSegmentContent & 0x0f)<<8;
	ContinueLocs |= (unsigned char)*(_pSegmentContent + 1);

	// ��ʼloc
	size_t StartLoc =  ((unsigned char)*(_pSegmentContent + 2) & 0x0f)<<8;
	StartLoc |= (unsigned char)*(_pSegmentContent + 3);

	// loc�ȼ�, ���������ļ�������ʵ�Ĳ�1
	unsigned char LocClass = ((unsigned char)*(_pSegmentContent + 2) >>4) &0x07;
	LocClass += 1;

	//DTI_LOG_DEBUG( "[DTIEngine]","data type:%d\n",DataType);
	//DTI_LOG_DEBUG( "[DTIEngine]","travil time type:%d\n",TravelTimeType);
	//DTI_LOG_DEBUG( "[DTIEngine]","data flag:%d\n",DataFlag);

	//DTI_LOG_DEBUG( "[DTIEngine]","continu loc num:%d\n",ContinueLocs);
	//DTI_LOG_DEBUG( "[DTIEngine]","start loc:%d\n",StartLoc);	
	//DTI_LOG_DEBUG( "[DTIEngine]","loc lev:%d\n",LocClass);
	if (DataType == 1)
	{
		nRet += ParseDataSegmentContentForTypeNotime(pContent, _nSegContLen,
							DataFlag, LocClass, ContinueLocs, StartLoc);
	}
	else
	{
		nRet += ParseDataSegmentContentForTypeHastime(pContent, _nSegContLen,
							DataFlag, LocClass, ContinueLocs, StartLoc);
	}

	return nRet;
}

int CParseDTI::ParseDataSegmentContentForTypeNotime(const char* _pSegmentContent,int _nSegContLen,
													 unsigned char DataFlag,unsigned char LocClass,
													 size_t ContinueLocs,size_t StartLoc)

{
	int nReadDataLen = 0;
	unsigned char Condition = 0;

	/*tagDT_Data data;
	memset( &data, 0, sizeof(tagDT_Data) );

	Time time = Time::getCurrentTime();
	data.m_bOccurYear = time.getYear();
	data.m_bOccurMonth = time.getMonth() + 1;
	data.m_bOccurDay = time.getDayOfMonth();
	data.m_bOccurHour = m_nUpdateTime / 60;
	data.m_bOccurMinute = m_nUpdateTime % 60;
	data.SetDuration( 0 );/* ��������ʱ��---��case������ʱ�� */
	
	for(size_t i = 0; i < ContinueLocs; ++i)
	{
		// pre loc��·��
		size_t currLoc = StartLoc + i;				
		//data.m_bTranState = ((unsigned char)*_pSegmentContent >>4) & 0x0f;
		//DTIDataCache().UpdateStatus( m_nCityID, m_nCurrMeshID, LocClass, currLoc, data );
		//DTI_LOG_DEBUG( "[DTIEngine]","CityID = %d Meshid=%d	 RoadClass=%d	 LinkID=%d	 condition=%d	 travel time=:%d\n",
		//	m_nCityID,m_nCurrMeshID, LocClass, currLoc, data.m_bTranState, 0);

		// next loc��·��		
		if( ++i < ContinueLocs)
		{
			currLoc = StartLoc + i;	
			/*data.m_bTranState = ((unsigned char)*_pSegmentContent & 0x0f);
			//DTIDataCache().UpdateStatus( m_nCityID, m_nCurrMeshID, LocClass, currLoc, data );
			//DTI_LOG_DEBUG( "[DTIEngine]","CityID = %d Meshid=%d	 RoadClass=%d	 LinkID=%d	 condition=%d	 travel time=:%d\n",
				m_nCityID,m_nCurrMeshID, LocClass, currLoc, data.m_bTranState, 0);*/
		}		
		nReadDataLen += 1;
		_pSegmentContent += 1;		
	}
	return nReadDataLen;
}


int CParseDTI::ParseDataSegmentContentForTypeHastime(const char* _pSegmentContent,int _nSegContLen,
													  unsigned char DataFlag,unsigned char LocClass,
													  size_t ContinueLocs,size_t StartLoc)
{
	int nReadDataLen = 0;
	int TravelTime = 0;
	unsigned char ShareCondition = 0; 

	unsigned char DistanceUnit = 0;
	unsigned char StartPos = 0;
	unsigned char JamLength = 0;

	if(DataFlag == 1) // locs����һ��״̬
	{
		ShareCondition = ((unsigned char)*_pSegmentContent >>6) & 0x03;
		TravelTime = ((unsigned char)*_pSegmentContent & 0x3f);
		nReadDataLen += 1;
		_pSegmentContent += 1;

		if (TravelTime == 60) // ����������Ϣ1-����
		{
			ParseExInfo1(_pSegmentContent + 1, DistanceUnit, StartPos, JamLength);
			nReadDataLen += 2;
			_pSegmentContent += 2;
			TravelTime *= 10; // ת��Ϊ��λs
		} 
		else if (TravelTime == 61) // ����������Ϣ1-�����������Ϣ2-ʱ��
		{
			ParseExInfo1(_pSegmentContent + 1, DistanceUnit, StartPos, JamLength);
			ParseExInfo2(_pSegmentContent + 3, TravelTime);
			nReadDataLen += 3;
			_pSegmentContent += 3;
		}
		else
		{
			TravelTime *=  10;
		}
	}

	// ����data
	/*tagDT_Data data;
	memset( &data, 0, sizeof(tagDT_Data) );

	Time time = Time::getCurrentTime();
	data.m_bOccurYear = time.getYear();
	data.m_bOccurMonth = time.getMonth() + 1;
	data.m_bOccurDay = time.getDayOfMonth();

	data.m_bOccurHour = m_nUpdateTime / 60;
	data.m_bOccurMinute = m_nUpdateTime % 60;*/

	for(size_t i = 0; i < ContinueLocs; ++i)
	{
		size_t currLoc = StartLoc + i;
	/*	STRTTrafficData tmpTrafficData;
		PRTTrafficData pNewTrafficData = &tmpTrafficData;//new STRTTrafficData();
		memset(pNewTrafficData, 0x00, sizeof(STRTTrafficData));*/
	//	m_vtDTIMeshSet[m_nDTIObjIndex]->mTrafficDataSet.push_back(pNewTrafficData);
		
		if (DataFlag == 0)
		{
			ShareCondition = ((unsigned char)*_pSegmentContent >>6) & 0x03;
			TravelTime = ((unsigned char)*_pSegmentContent & 0x3f);
			nReadDataLen += 1;
			_pSegmentContent += 1;

			if (TravelTime == 60) // ����������Ϣ1-����
			{
				ParseExInfo1(_pSegmentContent, DistanceUnit, StartPos, JamLength);
				nReadDataLen += 2;
				_pSegmentContent += 2;
				TravelTime *= 10; // ת��Ϊ��λs
			} 
			else if (TravelTime == 61) // ����������Ϣ1-�����������Ϣ2-ʱ��
			{
				ParseExInfo1(_pSegmentContent, DistanceUnit, StartPos, JamLength);
				ParseExInfo2(_pSegmentContent + 2, TravelTime);
				nReadDataLen += 3;
				_pSegmentContent += 3;
			}
			else
			{
				TravelTime *=  10;
			}
		}

		/*pNewTrafficData->m_ucClass = LocClass;
		pNewTrafficData->m_ucCondition = ShareCondition;
		pNewTrafficData->m_usLocID = currLoc;
		pNewTrafficData->m_nTraveltime = TravelTime;
		pNewTrafficData->m_ucDistanceUnit =  DistanceUnit;
		pNewTrafficData->m_ucStartPos = StartPos;
		pNewTrafficData->m_ucJamLength = JamLength;
		pNewTrafficData->m_nCityID = m_nCityID;*/
				
	//	data.m_bTranState = ShareCondition;		
		//data.SetDuration( TravelTime );/* ��������ʱ�� */
	//	//DTIDataCache().UpdateStatus( m_nCityID, m_nCurrMeshID, LocClass, currLoc, data );

		printf( "[DTIEngine] CityID = %d Meshid=%d	 RoadClass=%d	 LinkID=%d	 condition=%d	 travel time=:%d\n",
			m_nCityID,m_nCurrMeshID, LocClass, currLoc, ShareCondition, TravelTime);
	}

	return nReadDataLen;
}

void CParseDTI::ParseExInfo1(const char* _pCont,unsigned char& _DistanceUnit,
							 unsigned char& _StartPos,unsigned char& _JamLength)
{
	if (_pCont)
	{
		_DistanceUnit = ((unsigned char)*_pCont & 0x80)>>6;
		_DistanceUnit |= (((unsigned char)*(_pCont+1)>>7) & 0x01);

		_StartPos = (unsigned char)*_pCont & 0x7f;
		_JamLength= (unsigned char)*(_pCont + 1) & 0x7f;

		//DTI_LOG_DEBUG( "[DTIEngine]","ParseExInfo1\n dist unit ��%d\n_JamLength��%d\n _StartPos��%d\n",_DistanceUnit,_JamLength,_StartPos);
	}
}

void CParseDTI::ParseExInfo2(const char* _pCont,int& _TravelTime)
{
	if (_pCont)
	{
		unsigned char timeUnit = ((unsigned char)*_pCont>>7) & 0x01;

		_TravelTime = ((unsigned char)*_pCont & 0x7f);

		_TravelTime *= ((timeUnit==0) ? 10 : 60); // ��λs

		//DTI_LOG_DEBUG( "[DTIEngine]","ParseExInfo2\ntime unit��%d\ntravl time��%d\n",timeUnit,_TravelTime);
	}
}