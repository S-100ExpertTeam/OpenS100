#include "stdafx.h"
#include "SENC_CallTemplate.h"

SENC_CallTemplate::SENC_CallTemplate()
{

}

SENC_CallTemplate::~SENC_CallTemplate()
{
	for (auto itor = params.begin(); itor != params.end(); itor++)
		delete *itor;
}