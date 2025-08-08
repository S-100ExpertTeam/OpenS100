#include "stdafx.h"
#include "GeometryCommands.h"


namespace DrawingInstructions
{
	GeometryCommands::~GeometryCommands()
	{
		delete spatialReference;
		spatialReference = nullptr;

		delete augmentedPoint;
		augmentedPoint = nullptr;

		delete augmentedRay;
		augmentedRay = nullptr;

		delete augmentedPath;
		augmentedPath = nullptr;

		delete polyline;
		polyline = nullptr;

		delete arc3Points;
		arc3Points = nullptr;

		delete arcByRadius;
		arcByRadius = nullptr;

		delete annulus;
		annulus = nullptr;

		delete clearGeometryCmd;
		clearGeometryCmd = nullptr;
	}

	void GeometryCommands::setSpatialReference(const std::string& reference, bool forward)
	{
		delete this->spatialReference;
		this->spatialReference = new SpatialReference(reference, forward);
	}
	void GeometryCommands::setAugmentedPoint(std::string crs, double x, double y)
	{
		delete this->augmentedPoint;
		this->augmentedPoint = new AugmentedPoint(crs, x, y);
	}
	void GeometryCommands::setAugmentedRay(std::string CRSType, double direction, std::string crsLength, double length)
	{
		delete this->augmentedRay;
		this->augmentedRay = new AugmentedRay(CRSType, direction, crsLength, length);
	}
	void GeometryCommands::setAugmentedPath(std::string crsPosition, std::string crsAngle, std::string crsDistance)
	{
		delete this->augmentedPath;
		this->augmentedPath = new AugmentedPath(crsPosition, crsAngle, crsDistance);
	}
	void GeometryCommands::setPolyline(const std::vector<std::string>& points)
	{
		delete this->polyline;
		this->polyline = new Polyline(points);
	}
	void GeometryCommands::setArc3Points(std::string startPointX, std::string startPointY, std::string medianPointX, std::string medianPointY, std::string endPointX, std::string endPointY)
	{
		delete this->arc3Points;
		this->arc3Points = new Arc3Points(startPointX, startPointY, medianPointX, medianPointY, endPointX, endPointY);
	}
	void GeometryCommands::setArcByRadius(std::string centerX, std::string centerY, double radius, double startAngle, double angularDistance)
	{
		delete this->arcByRadius;
		this->arcByRadius = new ArcByRadius(centerX, centerY, radius, startAngle, angularDistance);
	}
	void GeometryCommands::setAnnulus(std::string centerX, std::string centerY, double outerRadius, double innerRadius, double startAngle, double angularDistance)
	{
		delete this->annulus;
		this->annulus = new Annulus(centerX, centerY, outerRadius, innerRadius, startAngle, angularDistance);
	}
	void GeometryCommands::setClearGeometry()
	{
		delete this->clearGeometryCmd;
		this->clearGeometryCmd = new ClearGeometry();
	}

	void GeometryCommands::parse(const std::string& key, std::string value)
	{
		if (key == "SpatialReference")
		{
			//setSpatialReference();
		}
		else if (key == "AugmentedPoint")
		{
			//setAugmentedPoint();
		}
		else if (key == "AugmentedRay")
		{
			//setAugmentedRay();
		}
		else if (key == "AugmentedPath")
		{
			//setAugmentedPath();
		}
		else if (key == "Polyline")
		{
			//setPolyline();
		}
		else if (key == "Arc3Points")
		{
			//setArc3Points();
		}
		else if (key == "ArcByRadius")
		{
			//setArcByRadius();
		}
		else if (key == "Annulus")
		{
			//setAnnulus();
		}
		else if (key == "ClearGeometry")
		{
			//setClearGeometry();
		}
	}
	void GeometryCommands::execute() const
	{
		if (spatialReference) spatialReference->execute();
		if (augmentedPoint) augmentedPoint->execute();
		if (augmentedRay) augmentedRay->execute();
		if (augmentedPath) augmentedPath->execute();
		if (polyline) polyline->execute();
		if (arc3Points) arc3Points->execute();
		if (arcByRadius) arcByRadius->execute();
		if (annulus) annulus->execute();
		if (clearGeometryCmd) clearGeometryCmd->execute();
	}

	SpatialReference::SpatialReference(const std::string& reference, bool forward) : reference(reference), forward(forward) {}

	void SpatialReference::init()
	{
		reference = "";
		forward = false;
	}

	void SpatialReference::execute() 
	{
	}

	void SpatialReference::parse(const std::string& input)
	{
	}

	AugmentedPoint::AugmentedPoint(std::string crs, double x, double y) : crs(crs), x(x), y(y) {}

	void AugmentedPoint::init()
	{
		crs = "";
		x = 0.0;
		y = 0.0;
	}

	void AugmentedPoint::execute() 
	{
	}

	void AugmentedPoint::parse(const std::string& input)
	{
	}

	AugmentedRay::AugmentedRay(std::string CRSType, double direction, std::string crsLength, double length) : CRSType(CRSType), direction(direction), crsLength(crsLength), length(length) {}

	void AugmentedRay::init()
	{
		CRSType = "";
		direction = 0.0;
		crsLength = "";
		length = 0.0;
	}

	void AugmentedRay::execute() 
	{
	}

	void AugmentedRay::parse(const std::string& input)
	{
	}

	AugmentedPath::AugmentedPath(std::string crsPosition, std::string crsAngle, std::string crsDistance) : crsPosition(crsPosition), crsAngle(crsAngle), crsDistance(crsDistance) {}

	void AugmentedPath::init()
	{
		crsPosition = "";
		crsAngle = "";
		crsDistance = "";
	}

	void AugmentedPath::execute() 
	{
	}

	void AugmentedPath::parse(const std::string& input)
	{
	}

	Polyline::Polyline(const std::vector<std::string>& points) : points(points) {}

	void Polyline::init()
	{
		points.clear();
	}

	void Polyline::execute() 
	{
	}

	void Polyline::parse(const std::string& input)
	{
	}

	Arc3Points::Arc3Points(std::string startPointX, std::string startPointY, std::string medianPointX, std::string medianPointY, std::string endPointX, std::string endPointY) : startPointX(startPointX), startPointY(startPointY), medianPointX(medianPointX), medianPointY(medianPointY), endPointX(endPointX), endPointY(endPointY) {}

	void Arc3Points::init()
	{
		startPointX = "";
		startPointY = "";
		medianPointX = "";
		medianPointY = "";
		endPointX = "";
		endPointY = "";
	}

	void Arc3Points::execute() 
	{
	}

	void Arc3Points::parse(const std::string& input)
	{
	}

	ArcByRadius::ArcByRadius(std::string centerX, std::string centerY, double radius, double startAngle, double angularDistance) : centerX(centerX), centerY(centerY), radius(radius), startAngle(startAngle), angularDistance(angularDistance) {}

	void ArcByRadius::init()
	{
		centerX = "";
		centerY = "";
		radius = 0.0;
		startAngle = 0.0;
		angularDistance = 0.0;
	}

	void ArcByRadius::execute() 
	{
	}

	void ArcByRadius::parse(const std::string& input)
	{
	}

	Annulus::Annulus(std::string centerX, std::string centerY, double outerRadius, double innerRadius, double startAngle, double angularDistance) : centerX(centerX), centerY(centerY), outerRadius(outerRadius), innerRadius(innerRadius), startAngle(startAngle), angularDistance(angularDistance) {}

	void Annulus::init()
	{
		centerX = "";
		centerY = "";
		outerRadius = 0.0;
		innerRadius = 0.0;
		startAngle = 0.0;
		angularDistance = 0.0;
	}

	void Annulus::execute() 
	{
	}

	void Annulus::parse(const std::string& input)
	{
	}

	void ClearGeometry::init()
	{
	}

	void ClearGeometry::execute() 
	{
	}

	void ClearGeometry::parse(const std::string& input)
	{
	}

}