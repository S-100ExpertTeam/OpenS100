#pragma once
#include "Command.h"
#include "PointParameter.h"

#include "..\\GeoMetryLibrary\\GeoPoint.h"
#include "..\\S100Engine\\GraphicBasePackage_Enum.h"

namespace DrawingCommand
{
    class SpatialReference : public Command {
    public:
		SpatialReference() = default;
        SpatialReference(const std::string& reference, bool forward);

		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        std::string reference;
        bool forward = true;
    };

    class AugmentedPoint : public Command {
    public:
		AugmentedPoint() = default;
        AugmentedPoint(GraphicBasePackage::CRSType crs, double x, double y);

		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        GraphicBasePackage::CRSType crs = GraphicBasePackage::CRSType::CRSType_None;
        DrawingCommand::Point point;
    };

    class AugmentedRay : public Command {
    public:
		AugmentedRay() = default;
        AugmentedRay(GraphicBasePackage::CRSType CRSType, double direction, GraphicBasePackage::CRSType crsLength, double length);

		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        GraphicBasePackage::CRSType CRSType = GraphicBasePackage::CRSType::CRSType_None;
        double direction = 0.0;
        GraphicBasePackage::CRSType crsLength = GraphicBasePackage::CRSType::CRSType_None;
        double length = 0.0;
    };

    class AugmentedPath : public Command {
    public:
		AugmentedPath() = default;
        AugmentedPath(GraphicBasePackage::CRSType crsPosition, GraphicBasePackage::CRSType crsAngle, GraphicBasePackage::CRSType crsDistance);

		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        GraphicBasePackage::CRSType crsPosition = GraphicBasePackage::CRSType::CRSType_None;
        GraphicBasePackage::CRSType crsAngle = GraphicBasePackage::CRSType::CRSType_None;
        GraphicBasePackage::CRSType crsDistance = GraphicBasePackage::CRSType::CRSType_None;
    };

    // Polyline class
    class Polyline : public Command {
    public:
		Polyline() = default;
        Polyline(const std::vector<DrawingCommand::Point>& points);

		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        std::vector<DrawingCommand::Point> points;
    };

    // Arc3Points class
    class Arc3Points : public Command {
    public:
		Arc3Points() = default;
        Arc3Points(double startPointX, double startPointY, double medianPointX, double medianPointY, double endPointX, double endPointY);

		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;


    private:
        DrawingCommand::Point startPoint;
        DrawingCommand::Point medianPoint;
        DrawingCommand::Point endPoint;
    };

    // ArcByRadius class
    class ArcByRadius : public Command {
    public:
		ArcByRadius() = default;
        ArcByRadius(double centerX, double centerY, double radius, double startAngle, double angularDistance);

		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        DrawingCommand::Point center;
        double radius = 0.0;
        double startAngle = 0.0;
        double angularDistance = 360.0;
    };

    // Annulus class
    class Annulus : public Command {
    public:
		Annulus() = default;
        Annulus(double centerX, double centerY, double outerRadius, double innerRadius, double startAngle, double angularDistance);

		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        DrawingCommand::Point center;
        double outerRadius = 0.0;
        double innerRadius = 0.0;
        double startAngle = 0.0;
        double angularDistance = 360.0;
    };

    class ClearGeometry : public Command {
    public:
        ClearGeometry() = default;

		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;
    };
}


