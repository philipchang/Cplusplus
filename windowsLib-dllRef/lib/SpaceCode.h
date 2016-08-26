#pragma once

class __declspec(dllexport) CSpaceRecode
{
public:
	CSpaceRecode();
	~CSpaceRecode();

public:
	long GetCodeForPoint(double x, double y);

private:
	void* pSpaceRecode;
};
