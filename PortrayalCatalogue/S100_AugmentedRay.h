#pragma once
#include "S100_AugmentedLineOrArea.h"
#include "S100_LineStyleReference.h"
#include "S100_CompositeLineStyle.h"

class S100_AugmentedRay :
	public S100_AugmentedLineOrArea
{
public:
	S100_AugmentedRay();
	virtual ~S100_AugmentedRay();

private:
	std::string direction;
	std::string length;

public:
	void GetContents(pugi::xml_node node);

	void SetDirection(const std::string& value);
	void SetDirection(const std::wstring& value);
	std::string  GetDirection();
	std::wstring GetDirectionW();

	void SetLength(const std::string& value);
	void SetLength(const std::wstring& value);
	std::string  GetLength();
	std::wstring GetLengthW();
};
