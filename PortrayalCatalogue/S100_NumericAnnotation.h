#pragma once

#include "Enum_ChampionChoice.h"

#include <optional>

class S100_NumericAnnotation
{
public:
	S100_NumericAnnotation();
	virtual ~S100_NumericAnnotation();

public:
	int decimals = 0;
	double bodySize = 0;
	std::optional<double> buffer;
	Enum_ChampionChoice champion = Enum_ChampionChoice::Unknown;

public:
	void setDecimals(int value);
	int getDecimals();
	
	void setBodySize(double value);
	double getBodySize();
	
	void setBuffer(double value);
	std::optional<double> getBuffer();
	
	void setChampion(Enum_ChampionChoice value);
	Enum_ChampionChoice getChampion();
};

