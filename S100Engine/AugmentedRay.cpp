#include "stdafx.h"
#include "AugmentedRay.h"

AugmentedRay::AugmentedRay()
{

}

AugmentedRay::~AugmentedRay()
{
	if (_rotationCRS)
	{
		delete _rotationCRS;
		_rotationCRS = nullptr;
	}
}