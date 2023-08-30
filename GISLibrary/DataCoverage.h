#pragma once
class DataCoverage
{
public:
	DataCoverage();
	virtual ~DataCoverage();

public:
	int minDS = -1;
	int maxDS = -1;

public:
	void setMinDS(int value);
	int getMinDS();

	void setMaxDS(int value);
	int getMaxDS();
};