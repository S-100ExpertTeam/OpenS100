#include "stdafx.h"
#include "VRID.h"
#include "NonPrintableCharacter.h"

int VRID::GetFieldLength()
{
	//unsigned RCNM;		// <M, b11, 110|120|130|140>
	//						//110(Isolated node) 120(Connected node) 130(Edge) 140(Face)
	//unsigned RCID;		// <M, b14, 1 to 2^32-2>
	//unsigned RVER;// <M, b12, ?>	Record version
	//unsigned RUIN;			// <M, b11, 1|2|3> 1(Insert) 2(Delete) 3(Modify)

	int len = 0;
	len += 1;
	len += 4;
	len += 2;
	len += 1;

	return ++len;
};

bool VRID::Save(CFile *file)
{
	file->Write(&RCNM, 1);
	file->Write(&RCID, 4);
	file->Write(&RVER, 2);
	file->Write(&RUIN, 1);
	file->Write(&NonPrintableCharacter::fieldTerminator, 1); 
	return true;
}
