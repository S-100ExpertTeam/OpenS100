#pragma once

#include<string>

class BasicFile
{
public:
	BasicFile();
	virtual ~BasicFile();

private:
	CString FCPath;
	CString PortrayalPath;

public:
	std::wstring Get_FcPath();
	std::wstring Get_PortrayalPath();
	void Set_BasicFile(CString fcpath, CString portrayalPath);
};