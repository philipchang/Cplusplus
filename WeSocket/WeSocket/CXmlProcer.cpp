#include "CXmlProcer.h"


	/*!
	*  /brief ����xml�ļ���
	*
	*  /param XmlFile xml�ļ�ȫ·����
	*  /return �Ƿ�ɹ���trueΪ�ɹ���false��ʾʧ�ܡ�
	*/
bool CXmlProcer::CreateXml(std::string XmlFile)
{
	// ����һ��TiXmlDocument��ָ��
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
	// ����һ�����ڵ㣺MyApp
	TiXmlElement *pRootEle = new TiXmlElement( "MyApp" );
	if (NULL==pRootEle)
	{
		return false;
	}
	pDoc->LinkEndChild(pRootEle);
	// �����ӽڵ㣺Messages
	TiXmlElement *pMsg = new TiXmlElement("Messages");
	if (NULL==pMsg)
	{
		return false;
	}
	pRootEle->LinkEndChild(pMsg);
	// �����ӽڵ㣺Welcome
	TiXmlElement *pWelcome = new TiXmlElement("Welcome");
	if (NULL==pWelcome)
	{
		return false;
	}
	pMsg->LinkEndChild(pWelcome);
	// ����Welcome�ڵ��ֵ
	std::string strValue = "Welcome to MyApp";
	TiXmlText *pWelcomeValue = new TiXmlText(strValue);
	pWelcome->LinkEndChild(pWelcomeValue);
	// �����ӽڵ㣺Farewell
	TiXmlElement *pFarewell = new TiXmlElement("Farewell");
	if (NULL==pFarewell)
	{
		return false;
	}
	pMsg->LinkEndChild(pFarewell);
	// ����Farewell�ڵ��ֵ
	strValue = "Thank you for using MyApp";
	TiXmlText *pFarewellValue = new TiXmlText(strValue);
	pFarewell->LinkEndChild(pFarewellValue);
	// �����ӽڵ㣺Windows
	TiXmlElement *pWindows = new TiXmlElement("Windows");
	if (NULL==pWindows)
	{
		return false;
	}
	pRootEle->LinkEndChild(pWindows);
	// �����ӽڵ㣺Window
	TiXmlElement *pWindow = new TiXmlElement("Window");
	if (NULL==pWindow)
	{
		return false;
	}
	pWindows->LinkEndChild(pWindow);
	// ���ýڵ�Window��ֵ
	pWindow->SetAttribute("name","MainFrame");
	pWindow->SetAttribute("x","5");
	pWindow->SetAttribute("y","15");
	pWindow->SetAttribute("w","400");
	pWindow->SetAttribute("h","250");
	// �����ӽڵ㣺Window
	TiXmlElement *pConnection  = new TiXmlElement("Connection");
	if (NULL==pConnection)
	{
		return false;
	}
	pRootEle->LinkEndChild(pConnection);
	// ���ýڵ�Connection��ֵ
	pConnection->SetAttribute("ip","192.168.0.1");
	pConnection->SetAttribute("timeout","123.456000");
	pDoc->SaveFile(XmlFile);

	delete pDoc;
	return true;
}

		
/*!
*  /brief ��ӡxml�ļ���
*
*  /param XmlFile xml�ļ�ȫ·����
*  /return �Ƿ�ɹ���trueΪ�ɹ���false��ʾʧ�ܡ�
*/
bool CXmlProcer::PaintXml(std::string XmlFile)
{
	// ����һ��TiXmlDocument��ָ��
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