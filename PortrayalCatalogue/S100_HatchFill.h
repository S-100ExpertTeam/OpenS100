#pragma once
#include "S100_PatternFill.h"
#include "S100_Hatch.h"

#include <list>

class S100_HatchFill :
	public S100_PatternFill
{
public:
	S100_HatchFill();
	virtual ~S100_HatchFill();

private:
	std::list<S100_Hatch*> hatches;

public:
	void GetContents(pugi::xml_node node);
	
	void SetHatches(S100_Hatch* value);
	void SetHatches(std::list<S100_Hatch*> value);

	S100_Hatch* GetHatches(int index);
	std::list<S100_Hatch*> GetHatches();
};