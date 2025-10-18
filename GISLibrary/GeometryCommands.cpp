#include "stdafx.h"
#include "GeometryCommands.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

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
	void GeometryCommands::setAugmentedPoint(GraphicBasePackage::CRSType crs, double x, double y)
	{
		delete this->augmentedPoint;
		this->augmentedPoint = new AugmentedPoint(crs, x, y);
	}
	void GeometryCommands::setAugmentedRay(GraphicBasePackage::CRSType CRSType, double direction, GraphicBasePackage::CRSType crsLength, double length)
	{
		delete this->augmentedRay;
		this->augmentedRay = new AugmentedRay(CRSType, direction, crsLength, length);
	}
	void GeometryCommands::setAugmentedPath(GraphicBasePackage::CRSType crsPosition, GraphicBasePackage::CRSType crsAngle, GraphicBasePackage::CRSType crsDistance)
	{
		delete this->augmentedPath;
		this->augmentedPath = new AugmentedPath(crsPosition, crsAngle, crsDistance);
	}
	void GeometryCommands::setPolyline(const std::vector<DrawingInstructions::Point>& points)
	{
		delete this->polyline;
		this->polyline = new Polyline(points);
	}
	void GeometryCommands::setArc3Points(double startPointX, double startPointY, double medianPointX, double medianPointY, double endPointX, double endPointY)
	{
		delete this->arc3Points;
		this->arc3Points = new Arc3Points(startPointX, startPointY, medianPointX, medianPointY, endPointX, endPointY);
	}
	void GeometryCommands::setArcByRadius(double centerX, double centerY, double radius, double startAngle, double angularDistance)
	{
		delete this->arcByRadius;
		this->arcByRadius = new ArcByRadius(centerX, centerY, radius, startAngle, angularDistance);
	}
	void GeometryCommands::setAnnulus(double centerX, double centerY, double outerRadius, double innerRadius, double startAngle, double angularDistance)
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
		StateCommand::init();
		reference.clear();
		forward = true;
	}

	void SpatialReference::execute() 
	{
	}

	void SpatialReference::parse(const std::string& input)
	{
		// SpatialReference:reference[,forward] 
		auto tokens = LatLonUtility::Split(input, ",");
		if (tokens.size() > 0) 
		{
			reference = tokens[0];
		}

		if (tokens.size() > 1) 
		{
			forward = (tokens[1] == "true");
		} 
		else 
		{
			forward = true; // Default value
		}
	}

	AugmentedPoint::AugmentedPoint(GraphicBasePackage::CRSType crs, double x, double y) : crs(crs) 
	{
		point.Set(x, y);
	}

	void AugmentedPoint::init()
	{
		StateCommand::init();
		crs = GraphicBasePackage::CRSType::CRSType_None;
		point.Set(0.0, 0.0); 
	}

	void AugmentedPoint::execute() 
	{
	}

	void AugmentedPoint::parse(const std::string& input)
	{
		// AugmentedPoint:crs,x,y 
		auto tokens = LatLonUtility::Split(input, ",");
		if (tokens.size() == 3) 
		{
			try
			{
				crs = StateCommand::GetCRSTypeFromString(tokens[0]);

				double x = std::stod(tokens[1]);
				double y = std::stod(tokens[2]);
				point.Set(x, y);
			}
			catch (const std::exception& e) 
			{
				init(); // Reset to default values on error
			}
		} 
		else 
		{
			init(); // Reset to default values if not enough tokens
		}
	}

	AugmentedRay::AugmentedRay(GraphicBasePackage::CRSType CRSType, double direction, GraphicBasePackage::CRSType crsLength, double length) : CRSType(CRSType), direction(direction), crsLength(crsLength), length(length) {}

	void AugmentedRay::init()
	{
		StateCommand::init();
		CRSType = GraphicBasePackage::CRSType::CRSType_None;
		direction = 0.0;
		crsLength = GraphicBasePackage::CRSType::CRSType_None;
		length = 0.0;
	}

	void AugmentedRay::execute() 
	{
	}

	void AugmentedRay::parse(const std::string& input)
	{
		// AugmentedRay:crsDirection, direction, crsLength, length
		auto tokens = LatLonUtility::Split(input, ",");
		if (tokens.size() == 4) 
		{
			try
			{
				CRSType = StateCommand::GetCRSTypeFromString(tokens[0]);
				direction = std::stod(tokens[1]);
				crsLength = StateCommand::GetCRSTypeFromString(tokens[2]);
				length = std::stod(tokens[3]);
			}
			catch (const std::exception& e) 
			{
				init(); // Reset to default values on error
			}
		} 
		else 
		{
			init(); // Reset to default values if not enough tokens
		}
	}

	AugmentedPath::AugmentedPath(GraphicBasePackage::CRSType crsPosition, GraphicBasePackage::CRSType crsAngle, GraphicBasePackage::CRSType crsDistance) : crsPosition(crsPosition), crsAngle(crsAngle), crsDistance(crsDistance) {}

	void AugmentedPath::init()
	{
		StateCommand::init();
		crsPosition = GraphicBasePackage::CRSType::CRSType_None;
		crsAngle = GraphicBasePackage::CRSType::CRSType_None;
		crsDistance = GraphicBasePackage::CRSType::CRSType_None;
	}

	void AugmentedPath::execute() 
	{
	}

	void AugmentedPath::parse(const std::string& input)
	{
		// AugmentedPath:crsPosition,crsAngle,crsDistance 
		auto tokens = LatLonUtility::Split(input, ",");
		if (tokens.size() == 3) 
		{
			try
			{
				crsPosition = StateCommand::GetCRSTypeFromString(tokens[0]);
				crsAngle = StateCommand::GetCRSTypeFromString(tokens[1]);
				crsDistance = StateCommand::GetCRSTypeFromString(tokens[2]);
			}
			catch (const std::exception& e) 
			{
				init(); // Reset to default values on error
			}
		} 
		else 
		{
			init(); // Reset to default values if not enough tokens
		}
	}

	Polyline::Polyline(const std::vector<DrawingInstructions::Point>& points) : points(points)
	{

	}

	void Polyline::init()
	{
		StateCommand::init();
		points.clear();
	}

	void Polyline::execute() 
	{
	}

	void Polyline::parse(const std::string& input)
	{
		// Polyline:positionXstart,positionYstart,positionXto,positionYto[,positionXto,positionYto¡¦] 
		auto tokens = LatLonUtility::Split(input, ",");
		if (tokens.size() % 2 == 0 && tokens.size() >= 4) 
		{
			points.clear();
			for (size_t i = 0; i < tokens.size(); i += 2) 
			{
				try
				{
					double x = std::stod(tokens[i]);
					double y = std::stod(tokens[i + 1]);
					points.push_back(DrawingInstructions::Point(x, y));
				}
				catch (const std::exception& e) 
				{
					init(); // Reset to default values on error
					return;
				}
			}
		} 
		else 
		{
			init(); // Reset to default values if not enough tokens
		}
	}

	Arc3Points::Arc3Points(double startPointX, double startPointY, double medianPointX, double medianPointY, double endPointX, double endPointY)
	{
		startPoint.Set(startPointX, startPointY);
		medianPoint.Set(medianPointX, medianPointY);
		endPoint.Set(endPointX, endPointY);
	}

	void Arc3Points::init()
	{
		StateCommand::init();
		startPoint.Set(0.0, 0.0);
		medianPoint.Set(0.0, 0.0);
		endPoint.Set(0.0, 0.0);
	}

	void Arc3Points::execute() 
	{
	}

	void Arc3Points::parse(const std::string& input)
	{
		// Arc3Points:startPointX,startPointY,medianPointX,medianPointY,endPointX,endPointY 
		auto tokens = LatLonUtility::Split(input, ",");
		if (tokens.size() == 6) 
		{
			try
			{
				double startPointX = std::stod(tokens[0]);
				double startPointY = std::stod(tokens[1]);
				double medianPointX = std::stod(tokens[2]);
				double medianPointY = std::stod(tokens[3]);
				double endPointX = std::stod(tokens[4]);
				double endPointY = std::stod(tokens[5]);

				startPoint.Set(startPointX, startPointY);
				medianPoint.Set(medianPointX, medianPointY);
				endPoint.Set(endPointX, endPointY);
			}
			catch (const std::exception& e) 
			{
				init(); // Reset to default values on error
			}
		} 
		else 
		{
			init(); // Reset to default values if not enough tokens
		}
	}

	ArcByRadius::ArcByRadius(double centerX, double centerY, double radius, double startAngle, double angularDistance) : radius(radius), startAngle(startAngle), angularDistance(angularDistance)
	{
		center.Set(centerX, centerY);
	}

	void ArcByRadius::init()
	{
		StateCommand::init();
		center.Set(0.0, 0.0);
		radius = 0.0;
		startAngle = 0.0;
		angularDistance = 360.0;
	}

	void ArcByRadius::execute() 
	{
	}

	void ArcByRadius::parse(const std::string& input)
	{
		// ArcByRadius:centerX,centerY,radius[,startAngle,angularDistance] 
		auto tokens = LatLonUtility::Split(input, ",");
		if (tokens.size() >= 3 && tokens.size() <= 5) 
		{
			try
			{
				double centerX = std::stod(tokens[0]);
				double centerY = std::stod(tokens[1]);
				radius = std::stod(tokens[2]);
				center.Set(centerX, centerY);
				if (tokens.size() > 3) 
				{
					startAngle = std::stod(tokens[3]);
				} 
				else 
				{
					startAngle = 0.0; // Default value
				}
				if (tokens.size() > 4) 
				{
					angularDistance = std::stod(tokens[4]);
				} 
				else 
				{
					angularDistance = 360.0; // Default value
				}
			}
			catch (const std::exception& e) 
			{
				init(); // Reset to default values on error
			}
		} 
		else 
		{
			init(); // Reset to default values if not enough tokens
		}
	}

	Annulus::Annulus(double centerX, double centerY, double outerRadius, double innerRadius, double startAngle, double angularDistance) :outerRadius(outerRadius), innerRadius(innerRadius), startAngle(startAngle), angularDistance(angularDistance) 
	{
		center.Set(centerX, centerY);
	}

	void Annulus::init()
	{
		StateCommand::init();
		center.Set(0.0, 0.0);
		outerRadius = 0.0;
		innerRadius = 0.0;
		startAngle = 0.0;
		angularDistance = 360.0;
	}

	void Annulus::execute() 
	{
	}

	void Annulus::parse(const std::string& input)
	{
		// Annulus:centerX,centerY,outerRadius[,innerRadius[,startAngle,angularDistance]] 
		auto tokens = LatLonUtility::Split(input, ",");
		if (tokens.size() >= 3 && tokens.size() <= 6) 
		{
			try
			{
				double centerX = std::stod(tokens[0]);
				double centerY = std::stod(tokens[1]);
				outerRadius = std::stod(tokens[2]);
				center.Set(centerX, centerY);
				if (tokens.size() > 3) 
				{
					innerRadius = std::stod(tokens[3]);
				} 
				else 
				{
					innerRadius = 0.0; // Default value
				}
				if (tokens.size() > 4) 
				{
					startAngle = std::stod(tokens[4]);
				} 
				else 
				{
					startAngle = 0.0; // Default value
				}
				if (tokens.size() > 5) 
				{
					angularDistance = std::stod(tokens[5]);
				} 
				else 
				{
					angularDistance = 360.0; // Default value
				}
			}
			catch (const std::exception& e) 
			{
				init(); // Reset to default values on error
			}
		} 
		else 
		{
			init(); // Reset to default values if not enough tokens
		}
	}

	void ClearGeometry::init()
	{
		StateCommand::init();
	}

	void ClearGeometry::execute() 
	{
	}

	void ClearGeometry::parse(const std::string& input)
	{
		// ClearGeometry 
	}

}