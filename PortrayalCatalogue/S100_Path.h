#pragma once
#include "S100_Polyline.h"
#include "S100_Arc3Points.h"
#include "S100_ArcByRadius.h"
#include "S100_Annulus.h"

#include <vector>

class S100_Path
{
public:
	S100_Path();
	virtual ~S100_Path();

private:
	std::vector<S100_Polyline> polylines;
	std::vector<S100_Arc3Points> arc3Pointses;
	std::vector<S100_ArcByRadius> arcByRadiuses;
	std::vector<S100_Annulus> annuluses;

public:
	void GetContents(pugi::xml_node node);

	void SetPolylines(S100_Polyline* value);
	void SetPolylines(std::vector<S100_Polyline> value);
	S100_Polyline* GetPolylines(int index);
	std::vector<S100_Polyline> GetPolylines();

	void SetArc3Pointses(S100_Arc3Points* value);
	void SetArc3Pointses(std::vector<S100_Arc3Points> value);
	S100_Arc3Points* GetArc3Pointses(int index);
	std::vector<S100_Arc3Points> GetArc3Pointses();

	void SetArcByRadiuses(S100_ArcByRadius* value);
	void SetArcByRadiuses(std::vector<S100_ArcByRadius> value);
	S100_ArcByRadius* GetArcByRadiuses(int index);
	std::vector<S100_ArcByRadius> GetArcByRadiuses();

	void SetAnnuluses(S100_Annulus* value);
	void SetAnnuluses(std::vector<S100_Annulus> value);
	S100_Annulus* GetAnnuluses(int intdex);
	std::vector<S100_Annulus> GetAnnuluses();
};