#include "CXmlProcer.h"


	/*!
	*  /brief 创建xml文件。
	*
	*  /param XmlFile xml文件全路径。
	*  /return 是否成功。true为成功，false表示失败。
	*/
bool CXmlProcer::CreateXml(std::string XmlFile)
{
	// 定义一个TiXmlDocument类指针
	TiXmlDocument *pDoc = new TiXmlDocument;
	if (NULL==pDoc)
	{
		return false;
	}
	TiXmlDeclaration *pDeclaration = new TiXmlDeclaration("1.0","",(""));
	if (NULL==pDeclaration)
	{
		return false;
	}
	pDoc->LinkEndChild(pDeclaration);
	// 生成一个根节点：MyApp
	TiXmlElement *pRootEle = new TiXmlElement( "MyApp" );
	if (NULL==pRootEle)
	{
		return false;
	}
	pDoc->LinkEndChild(pRootEle);
	// 生成子节点：Messages
	TiXmlElement *pMsg = new TiXmlElement("Messages");
	if (NULL==pMsg)
	{
		return false;
	}
	pRootEle->LinkEndChild(pMsg);
	// 生成子节点：Welcome
	TiXmlElement *pWelcome = new TiXmlElement("Welcome");
	if (NULL==pWelcome)
	{
		return false;
	}
	pMsg->LinkEndChild(pWelcome);
	// 设置Welcome节点的值
	std::string strValue = "Welcome to MyApp";
	TiXmlText *pWelcomeValue = new TiXmlText(strValue);
	pWelcome->LinkEndChild(pWelcomeValue);
	// 生成子节点：Farewell
	TiXmlElement *pFarewell = new TiXmlElement("Farewell");
	if (NULL==pFarewell)
	{
		return false;
	}
	pMsg->LinkEndChild(pFarewell);
	// 设置Farewell节点的值
	strValue = "Thank you for using MyApp";
	TiXmlText *pFarewellValue = new TiXmlText(strValue);
	pFarewell->LinkEndChild(pFarewellValue);
	// 生成子节点：Windows
	TiXmlElement *pWindows = new TiXmlElement("Windows");
	if (NULL==pWindows)
	{
		return false;
	}
	pRootEle->LinkEndChild(pWindows);
	// 生成子节点：Window
	TiXmlElement *pWindow = new TiXmlElement("Window");
	if (NULL==pWindow)
	{
		return false;
	}
	pWindows->LinkEndChild(pWindow);
	// 设置节点Window的值
	pWindow->SetAttribute("name","MainFrame");
	pWindow->SetAttribute("x","5");
	pWindow->SetAttribute("y","15");
	pWindow->SetAttribute("w","400");
	pWindow->SetAttribute("h","250");
	// 生成子节点：Window
	TiXmlElement *pConnection  = new TiXmlElement("Connection");
	if (NULL==pConnection)
	{
		return false;
	}
	pRootEle->LinkEndChild(pConnection);
	// 设置节点Connection的值
	pConnection->SetAttribute("ip","192.168.0.1");
	pConnection->SetAttribute("timeout","123.456000");
	pDoc->SaveFile(XmlFile);

	delete pDoc;
	return true;
}

		
/*!
*  /brief 打印xml文件。
*
*  /param XmlFile xml文件全路径。
*  /return 是否成功。true为成功，false表示失败。
*/
bool CXmlProcer::PaintXml(std::string XmlFile)
{
	// 定义一个TiXmlDocument类指针
	TiXmlDocument *pDoc = new TiXmlDocument();
	if (NULL==pDoc)
	{
		return false;
	}
	pDoc->LoadFile(XmlFile);

	pDoc->Print(  );

	delete pDoc;
	return true;
}