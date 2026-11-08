#pragma once

#include "MBR.h"

#include <string>
#include <memory>

class Scaler;
class Symbol;


class Geometry
{
public:
	Geometry();
	Geometry(const Geometry& other);
	virtual ~Geometry();

public:
	MBR m_mbr;
	std::string id;

	// Opaque per-geometry slot for a renderer cache (for example a Direct2D geometry).
	// The model does not know the concrete type; the renderer attaches and owns it and it
	// is destroyed with the geometry. It is never copied with the geometry.
	struct RenderAttachment
	{
		virtual ~RenderAttachment() = default;
	};
	std::unique_ptr<RenderAttachment> renderAttachment;
	void ResetRenderAttachment() { renderAttachment.reset(); }

public:
	virtual Geometry operator=(const Geometry& other);

public:
	MBR GetMBR();
	std::string GetID();
	std::wstring GetIDAsWString();
	int GetIDAsInt();
	void SetID(std::string value);
	void SetID(std::wstring value);
	void SetID(int value);

public:

	virtual bool ImportFromWkb(unsigned char* value, int size) { return true; };
	virtual bool ExportToWkb(unsigned char** value, int* size) { return true; };
	virtual int WkbSize() { return 0; };

	virtual void SetMBR() {};

	// Function for memory release outside of deconstructor
	virtual void Release() {};

	virtual double GetX() { return 0; };
	virtual double GetY() { return 0; };
	virtual double GetX(int index) { return 0; };
	virtual double GetY(int index) { return 0; };

	virtual std::string ToString() { return ""; }

	virtual int getNumPoint() const { return 0; };

public:
	bool WriteWkb(std::wstring path);
};