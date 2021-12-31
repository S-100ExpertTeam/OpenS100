#include "pch.h"
#include "FeatureCodeString.h"

//Operator Overloading: Compare ID values in ascending order.
bool CFeatureCodeString::operator<(CFeatureCodeString& value)
{
	return this->_id < value._id;
}