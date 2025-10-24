#include "stdafx.h"
#include "GeometryCommands.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

namespace DrawingCommand
{
	SpatialReference::SpatialReference(const std::string& reference, bool forward) : reference(reference), forward(forward) {}

	void SpatialReference::init()
	{
		Command::init();
		reference.clear();
		forward = true;
	}

	void SpatialReference::execute() 
	{
	}

	void SpatialReference::parse(const std::string& input)
	{
		setPresent();
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
		Command::init();
		crs = GraphicBasePackage::CRSType::CRSType_None;
		point.Set(0.0, 0.0); 
	}

	void AugmentedPoint::execute() 
	{
	}

	void AugmentedPoint::parse(const std::string& input)
	{
		setPresent();
		// AugmentedPoint:crs,x,y 
		auto tokens = LatLonUtility::Split(input, ",");
		if (tokens.size() == 3) 
		{
			try
			{
				crs = Command::GetCRSTypeFromString(tokens[0]);

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
		Command::init();
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
		setPresent();
		// AugmentedRay:crsDirection, direction, crsLength, length
		auto tokens = LatLonUtility::Split(input, ",");
		if (tokens.size() == 4) 
		{
			try
			{
				CRSType = Command::GetCRSTypeFromString(tokens[0]);
				direction = std::stod(tokens[1]);
				crsLength = Command::GetCRSTypeFromString(tokens[2]);
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
		Command::init();
		crsPosition = GraphicBasePackage::CRSType::CRSType_None;
		crsAngle = GraphicBasePackage::CRSType::CRSType_None;
		crsDistance = GraphicBasePackage::CRSType::CRSType_None;
	}

	void AugmentedPath::execute() 
	{
	}

	void AugmentedPath::parse(const std::string& input)
	{
		setPresent();
		// AugmentedPath:crsPosition,crsAngle,crsDistance 
		auto tokens = LatLonUtility::Split(input, ",");
		if (tokens.size() == 3) 
		{
			try
			{
				crsPosition = Command::GetCRSTypeFromString(tokens[0]);
				crsAngle = Command::GetCRSTypeFromString(tokens[1]);
				crsDistance = Command::GetCRSTypeFromString(tokens[2]);
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

	Polyline::Polyline(const std::vector<DrawingCommand::Point>& points) : points(points)
	{

	}

	void Polyline::init()
	{
		Command::init();
		points.clear();
	}

	void Polyline::execute() 
	{
	}

	void Polyline::parse(const std::string& input)
	{
		setPresent();
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
					points.push_back(DrawingCommand::Point(x, y));
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
		Command::init();
		startPoint.Set(0.0, 0.0);
		medianPoint.Set(0.0, 0.0);
		endPoint.Set(0.0, 0.0);
	}

	void Arc3Points::execute() 
	{
	}

	void Arc3Points::parse(const std::string& input)
	{
		setPresent();
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
		Command::init();
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
		setPresent();
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
		Command::init();
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
		setPresent();
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
		Command::init();
	}

	void ClearGeometry::execute() 
	{
	}

	void ClearGeometry::parse(const std::string& input)
	{
		setPresent();
		// ClearGeometry 
	}

}