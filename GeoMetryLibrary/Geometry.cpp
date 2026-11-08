#include "stdafx.h"
#include "Geometry.h"

#include "TextCodec.h"
#include "..\\LatLonUtility\\cpp_util.h"

#include <fstream>
#include <iostream>

Geometry::Geometry() 
{

}

Geometry::Geometry(const Geometry& other)
{
	id = other.id;

	m_mbr.xmin = other.m_mbr.xmin;
	m_mbr.ymin = other.m_mbr.ymin;
	m_mbr.xmax = other.m_mbr.xmax;
	m_mbr.ymax = other.m_mbr.xmax;
}

Geometry::~Geometry()
{

}

Geometry Geometry::operator=(const Geometry& other)
{
	id = other.id;

	m_mbr.xmin = other.m_mbr.xmin;
	m_mbr.ymin = other.m_mbr.ymin;
	m_mbr.xmax = other.m_mbr.xmax;
	m_mbr.ymax = other.m_mbr.xmax;

	return *this;
}

MBR Geometry::GetMBR()
{
	return m_mbr;
}

std::string Geometry::GetID()
{
	return id;
}

std::wstring Geometry::GetIDAsWString()
{
	return s100::text::ToWide(GetID());
}

int Geometry::GetIDAsInt()
{
	return cpp_util::stoi(GetID());
}

void Geometry::SetID(std::string value)
{
	this->id = value;
}

void Geometry::SetID(std::wstring value)
{
	SetID(s100::text::ToUtf8(value));
}

void Geometry::SetID(int value)
{
	SetID(std::to_string(value));
}

bool Geometry::WriteWkb(std::wstring path)
{
	unsigned char* wkb = nullptr;
	int size = 0;
	ExportToWkb(&wkb, &size);

	std::ofstream writeFile(path.data());
	if (writeFile.is_open())
	{
		writeFile.write((char*)wkb, size);
		writeFile.close();
	}

	return true;
}