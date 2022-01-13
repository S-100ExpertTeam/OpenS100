#include "stdafx.h"
#include "SENC_Path.h"

SENC_Path::SENC_Path()
{

}

SENC_Path::~SENC_Path()
{
	for (auto data : arc3Pointses)
	{
		delete data;
		data = nullptr;
	}
		
	
	for (auto data : polylines)
	{
		delete data;
		data = nullptr;
	}

	for (auto data : arcByRadiuses)
	{
		delete data;
		data = nullptr;
	}

	for (auto data : annuluses)
	{
		delete data;
		data = nullptr;
	}
		
}