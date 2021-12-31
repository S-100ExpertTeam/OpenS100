#include "pch.h"
#include "InformationCodeString.h"

bool CInformationCodeString::operator<(CInformationCodeString & value)
{
	return this->_id < value._id;
}