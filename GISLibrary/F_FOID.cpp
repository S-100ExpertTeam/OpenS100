#include "stdafx.h"
#include "F_FOID.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_FOID::F_FOID(void)
{
}

F_FOID::F_FOID(const F_FOID& other)
{
	AGEN = other.AGEN;
	FIDN = other.FIDN;
	FIDS = other.FIDS;
}

F_FOID::F_FOID(int AGEN, int FIDN, int FIDS)
{
	this->AGEN = AGEN;
	this->FIDN = FIDN;
	this->FIDS = FIDS;
}

F_FOID::~F_FOID(void)
{

}

void F_FOID::ReadField(BYTE *&buf)
{
	AGEN = buf2uint(buf, 2);
	FIDN = buf2uint(buf, 4);
	FIDS = buf2uint(buf, 2);
}

bool F_FOID::WriteField(CFile* file)
{
	file->Write(&AGEN, 2);
	file->Write(&FIDN, 4);
	file->Write(&FIDS, 2);

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
}

int F_FOID::GetFieldLength()
{
	return 9; // 8 + 1;
}

__int64 F_FOID::GetName()
{
	return ((__int64)AGEN) << 48 | ((__int64)FIDN) << 16 | FIDS;
}