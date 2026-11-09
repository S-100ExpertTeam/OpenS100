#include "stdafx.h"
#include "DDR.h"
#include "ISO8211Fuc.h"

DDR::DDR()
{

}

DDR::~DDR()
{
	SetSize(0);
	Delete();
}

void DDR::Set(BYTE* buffer)
{
	size = atoi(buffer, 5);
	buffer -= 5;

	if (size > 0)
	{
		content = new BYTE[size];
		memcpy(content, buffer, size);
	}

	buffer += size;
}

void DDR::Delete()
{
	delete[] content;
	content = nullptr;
}

BYTE* DDR::GetContent()
{
	return content;
}

void DDR::Allocate()
{
	if (GetSize() > 0)
	{
		Allocate(GetSize());
	}
}

void DDR::Allocate(int _size)
{
	SetSize(_size);
	content = new BYTE[GetSize()];
}

void DDR::SetByte(int index, BYTE value)
{
	if (nullptr != content &&
		index >= 0 &&
		index < GetSize())
	{
		content[index] = value;
	}
}

void DDR::SetBytes(int _size, BYTE* value)
{
	Delete();

	if (_size > 0 && nullptr != value)
	{
		Allocate(_size);
		memcpy_s(content, _size, value, _size);
	}
}

int DDR::GetSize()
{
	return size;
}

void DDR::SetSize(int value)
{
	size = value;
}