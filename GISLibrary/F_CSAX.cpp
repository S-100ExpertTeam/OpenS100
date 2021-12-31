#include "StdAfx.h"
#include "F_CSAX.h"
#include "CSAX.h"
#include "NonPrintableCharacter.h"

F_CSAX::F_CSAX(void)
{

}

F_CSAX::~F_CSAX(void)
{
	POSITION pos = m_arr.GetHeadPosition();

	while(pos!=NULL)
	{
		CSAX* csax = m_arr.GetNext(pos);
		delete csax;
	}
}

void F_CSAX::ReadField(BYTE *&buf)
{
	while(*buf != 0x1E){
		CSAX *csax = new CSAX();
		csax->m_axty = *(buf++);
		csax->m_axum = *(buf++);	
		m_arr.AddTail(csax);
	}
}

void F_CSAX::ReadField(BYTE *&buf, int loopCnt)
{
	for(int i = 0;i < loopCnt; i++)
	{
		CSAX *csax = new CSAX();

		csax->m_axty = *(buf++);
		csax->m_axum = *(buf++);
			
		m_arr.AddTail(csax);
	}
}

BOOL F_CSAX::Save(CFile *file)
{
	int len = 0;
	POSITION pos = m_arr.GetHeadPosition();
	while(pos != NULL)
	{
		CSAX *csax = m_arr.GetNext(pos);
		file->Write(&csax->m_axty, 1);
		file->Write(&csax->m_axum, 1);
	}
	file->Write(&NonPrintableCharacter::fieldTerminator, 1);
	return TRUE;
}

int F_CSAX::GetFieldLength()
{
	int len = 0;
	POSITION pos = m_arr.GetHeadPosition();
	while(pos != NULL)
	{
		CSAX *csax = m_arr.GetNext(pos);
		len += 1;
		len += 1;
	}
	return ++len;
}