#include "stdafx.h"
#include "DisplayPlane.h"

namespace Portrayal
{
	DisplayPlane::DisplayPlane()
	{

	}

	DisplayPlane::~DisplayPlane()
	{

	}

	int DisplayPlane::GetOrder()
	{
		return order;
	}

	void DisplayPlane::SetOrder(int value) 
	{
		order = value;
	}
}