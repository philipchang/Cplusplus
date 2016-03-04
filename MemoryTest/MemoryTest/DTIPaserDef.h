/***************************************************
 * @File: DTIPaserDef.h
 * @Description: 解析DTI数据在数据结构图定义
 * @Author: changggy 2014-1-1
 * copy all right
 ***************************************************/
#ifndef _DTIPASERDEF_H_
#define _DTIPASERDEF_H_

#pragma once

// Loc路况数据定义
typedef struct _tagRTTrafficData
{			
	unsigned short			m_usLocID;			// 路链ID号（1-4095）
	unsigned char			m_ucClass;			// 路链等级
	unsigned char			m_ucCondition;		// 路况, 3、2、1、0，分别对应红、黄、绿、不确定
	
	unsigned char			m_ucDistanceUnit;	// 距离单位：0:10m; 1:100m; 2:1km
	unsigned char			m_ucStartPos;		// 起始位置：用与相应路链终点的距离来表示, 有效范围【0,126】* 单位
	
	// 堵塞长度：从堵塞的起始位置到终止位置的长度, 有效范围【0,126】* 单位
	// 当堵塞长度设定值为“127”时，表示堵塞长度为整条路链的长度；
	// 设定值为“0”时表示堵塞长度不确定。
	unsigned char			m_ucJamLength;

	int						m_nTraveltime;	// 路链旅行时间 （单位s)
	int						m_nCityID;		// 当前城市id
}STRTTrafficData,*PRTTrafficData;


const unsigned char PARAGRAM_HEAD_LEN = 11;		// 节目头大小
const unsigned char SEPARATOR = 30;				// 0x1E，节目以及数据项分隔符
const unsigned char PARAGRAM_HEAD_TYPE = 49;	// 0x31，节目数据类型参数

const unsigned char CONTENT_HEAD_LEN = 6;		// 内容项头大小

const unsigned char DATASEG_HEAD_LEN = 4;		// 数据段头大小
const unsigned char DATASEGSE_PARATOR = 31;		// 数据段分隔符
const unsigned char DATASEGTYPE_CONDITION = 64; // 数据段参数1
const unsigned char DATASEGTYPE_EVENT = 65;		// 数据段参数2
const unsigned char DATASEGTYPE_PARK  = 66;		// 数据段参数3

const unsigned char DATASEG_CONTENT_HEAD_LEN = 4;		// 数据段内容头大小

#endif // _DTIPASERDEF_H_