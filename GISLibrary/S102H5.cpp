#include "stdafx.h"
#include "S102H5.h"

#include "..\LibMFCUtil\LibMFCUtil.h"

S102H5::S102H5()
{

}

S102H5::~S102H5()
{
	
}

bool S102H5::Open(CString _filepath)
{
	//auto path = LibMFCUtil::WStringToString(std::wstring(_filepath));

	//auto fileID = H5Fopen(path.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
	//if (fileID < 0)
	//{
	//	return false;
	//}

	//H5Fclose(fileID);

	return true;
}