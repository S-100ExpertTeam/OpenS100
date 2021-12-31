#include "stdafx.h"
#include "S100_LineStyleBase.h"
S100_LineStyleBase::S100_LineStyleBase()
{

}

S100_LineStyleBase::~S100_LineStyleBase()
{

}

void S100_LineStyleBase::SetType(int value) 
{
	type = value;
}

int S100_LineStyleBase::GetType()
{
	return type;
}