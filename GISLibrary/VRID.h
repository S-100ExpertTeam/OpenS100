#pragma once

class VRID
{
public:
	int RCNM;		// <M, b11, 110|120|130|140>
	//110(Isolated node) 120(Connected node) 130(Edge) 140(Face)
	int RCID;		// <M, b14, 1 to 2^32-2>
	int RVER;// <M, b12, ?>	Record version
	int RUIN;			// <M, b11, 1|2|3> 1(Insert) 2(Delete) 3(Modify)

public:
	//unsigned GetFieldLength(); BOOL Save(CFile *file);
	int GetFieldLength();
	bool Save(CFile *file);
};