#pragma once

#include <list>

class Layer;
class ENC_Feature;
class MBR;
struct FindedFeature
{
public:
	FindedFeature() {}
	virtual ~FindedFeature() {}

public:
	Layer* layer;
	ENC_Feature* fe;
	MBR* mbr;
};

typedef std::list<FindedFeature> ListFindedFeature;