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
	*  /brief 创建xml文件。
	*
	*  /param XmlFile xml文件全路径。
	*  /return 是否成功。true为成功，false表示失败。
	*/
	bool CreateXml(std::string XmlFile);

		
	/*!
	*  /brief 打印xml文件。
	*
	*  /param XmlFile xml文件全路径。
	*  /return 是否成功。true为成功，false表示失败。
	*/
	bool PaintXml(std::string XmlFile);
};

#endif