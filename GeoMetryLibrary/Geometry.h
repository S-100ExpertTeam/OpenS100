#pragma once

#include "MBR.h"

#include <string>

class Scaler;
class Symbol;

struct ID2D1Factory1;

class Geometry
{
public:
	Geometry();
	virtual ~Geometry();

public:
	MBR m_mbr;
	std::string id;

public:
	MBR GetMBR();
	std::string GetID();
	std::wstring GetIDAsWString();
	int GetIDAsInt();
	void SetID(std::string value);
	void SetID(std::wstring value);
	void SetID(int value);

public:
	virtual void DrawGeometry(HDC &hDC, Scaler *scaler, double offset = 0) {};
	virtual void DrawTextInfo(HDC &hDC, Scaler *scaler, double offset = 0) {};

	virtual bool ImportFromWkb(unsigned char* value, int size) { return true; };
	virtual bool ExportToWkb(unsigned char** value, int* size) { return true; };
	virtual int WkbSize() { return 0; };

	virtual void SetMBR() {};
	virtual void CreateD2Geometry(ID2D1Factory1* factory) {};

	// Function for memory release outside of deconstructor
	virtual void Release() {};

	virtual double GetX() { return 0; };
	virtual double GetY() { return 0; };
	virtual double GetX(int index) { return 0; };
	virtual double GetY(int index) { return 0; };

	virtual std::string ToString() { return ""; }

	virtual int getNumPoint() const { return 0; };

	virtual Geometry* Clone() const = 0;

public:
	bool WriteWkb(std::wstring path);
};