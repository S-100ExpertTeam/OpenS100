#pragma once
#include <cstdint>
#include "BinaryWriter.h"
#include "Field.h"

struct  THAS;
class F_THAS : public Field
{
public:
	F_THAS();
	F_THAS(const F_THAS& other);
	virtual ~F_THAS();

	std::vector<THAS*> m_arr;

public:
	virtual F_THAS operator=(const F_THAS& other);

public:
	void ReadField(std::uint8_t *&buf);
	void ReadField(std::uint8_t *&buf, int loopCnt);
	bool WriteField(s100::io::BinaryWriter* /*file*/) { return true; }
	int GetFieldLength();

	void Insert(RecordName name, int taui);
	void Insert(int rcnm, int rcid, int taui);
	void Insert(GISLibrary::RCNM rcnm, int rcid, int taui);
};