#include "stdafx.h"
#include "S100ExchangeCatalogue.h"


S100ExchangeCatalogue::S100ExchangeCatalogue(void)
{

}


S100ExchangeCatalogue::~S100ExchangeCatalogue(void)
{
	for (unsigned int i = 0; i < m_nRecords; i++) {
		delete m_pSHPObject[i];
	}
	delete[] m_pSHPObject;
}


bool SHPFile::Open(CString _filepath) 
{

}