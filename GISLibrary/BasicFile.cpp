#include "stdafx.h"
#include "BasicFile.h"
BasicFile::BasicFile()
{

}

BasicFile::~BasicFile()
{

}

std::wstring BasicFile::Get_FcPath()
{
	std::wstring ret(FCPath);
	return ret;
}

std::wstring BasicFile::Get_PortrayalPath()
{
	std::wstring ret(PortrayalPath);
	return ret;
}

void BasicFile::Set_BasicFile(CString fcpath, CString portrayalPath)
{
	FCPath = fcpath;
	PortrayalPath = portrayalPath;
}