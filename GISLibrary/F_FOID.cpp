#include "stdafx.h"
#include "F_FOID.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_FOID::F_FOID(void)
{
}

F_FOID::F_FOID(const F_FOID& other) : Field(other)
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

F_FOID F_FOID::operator=(const F_FOID& other)
{
	AGEN = other.AGEN;
	FIDN = other.FIDN;
	FIDS = other.FIDS;

	return *this;
}

void F_FOID::ReadField(std::uint8_t*& buf)
{
	AGEN = buf2uint(buf, 2);
	FIDN = buf2uint(buf, 4);
	FIDS = buf2uint(buf, 2);
}

bool F_FOID::WriteField(s100::io::BinaryWriter* file)
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

long long F_FOID::GetName()
{
	return ((long long)AGEN) << 48 | ((long long)FIDN) << 16 | FIDS;
}


