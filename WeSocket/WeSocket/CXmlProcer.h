#ifndef _CXML_PROCES_H
#define _CXML_PROCES_H


#include "MTL/XML/tinyxml.h"
#include <cstdlib>
#include <string>
#include "stdafx.h"

class CXmlProcer
{
public:

	/*!
	*  /brief ����xml�ļ���
	*
	*  /param XmlFile xml�ļ�ȫ·����
	*  /return �Ƿ�ɹ���trueΪ�ɹ���false��ʾʧ�ܡ�
	*/
	bool CreateXml(std::string XmlFile);

		
	/*!
	*  /brief ��ӡxml�ļ���
	*
	*  /param XmlFile xml�ļ�ȫ·����
	*  /return �Ƿ�ɹ���trueΪ�ɹ���false��ʾʧ�ܡ�
	*/
	bool PaintXml(std::string XmlFile);
};

#endif