/***************************************************
 * @File: DTIPaserDef.h
 * @Description: ����DTI���������ݽṹͼ����
 * @Author: changggy 2014-1-1
 * copy all right
 ***************************************************/
#ifndef _DTIPASERDEF_H_
#define _DTIPASERDEF_H_

#pragma once

// Loc·�����ݶ���
typedef struct _tagRTTrafficData
{			
	unsigned short			m_usLocID;			// ·��ID�ţ�1-4095��
	unsigned char			m_ucClass;			// ·���ȼ�
	unsigned char			m_ucCondition;		// ·��, 3��2��1��0���ֱ��Ӧ�졢�ơ��̡���ȷ��
	
	unsigned char			m_ucDistanceUnit;	// ���뵥λ��0:10m; 1:100m; 2:1km
	unsigned char			m_ucStartPos;		// ��ʼλ�ã�������Ӧ·���յ�ľ�������ʾ, ��Ч��Χ��0,126��* ��λ
	
	// �������ȣ��Ӷ�������ʼλ�õ���ֹλ�õĳ���, ��Ч��Χ��0,126��* ��λ
	// �����������趨ֵΪ��127��ʱ����ʾ��������Ϊ����·���ĳ��ȣ�
	// �趨ֵΪ��0��ʱ��ʾ�������Ȳ�ȷ����
	unsigned char			m_ucJamLength;

	int						m_nTraveltime;	// ·������ʱ�� ����λs)
	int						m_nCityID;		// ��ǰ����id
}STRTTrafficData,*PRTTrafficData;


const unsigned char PARAGRAM_HEAD_LEN = 11;		// ��Ŀͷ��С
const unsigned char SEPARATOR = 30;				// 0x1E����Ŀ�Լ�������ָ���
const unsigned char PARAGRAM_HEAD_TYPE = 49;	// 0x31����Ŀ�������Ͳ���

const unsigned char CONTENT_HEAD_LEN = 6;		// ������ͷ��С

const unsigned char DATASEG_HEAD_LEN = 4;		// ���ݶ�ͷ��С
const unsigned char DATASEGSE_PARATOR = 31;		// ���ݶηָ���
const unsigned char DATASEGTYPE_CONDITION = 64; // ���ݶβ���1
const unsigned char DATASEGTYPE_EVENT = 65;		// ���ݶβ���2
const unsigned char DATASEGTYPE_PARK  = 66;		// ���ݶβ���3

const unsigned char DATASEG_CONTENT_HEAD_LEN = 4;		// ���ݶ�����ͷ��С

#endif // _DTIPASERDEF_H_