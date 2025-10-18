#pragma once
#include "StateCommand.h"
#include "PointParameter.h"

#include "..\\GeoMetryLibrary\\GeoPoint.h"
#include "..\\S100Engine\\GraphicBasePackage_Enum.h"

namespace DrawingInstructions
{
    class SpatialReference : public StateCommand {
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

    class AugmentedPoint : public StateCommand {
    public:
		AugmentedPoint() = default;
        AugmentedPoint(GraphicBasePackage::CRSType crs, double x, double y);

		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        GraphicBasePackage::CRSType crs = GraphicBasePackage::CRSType::CRSType_None;
        DrawingInstructions::Point point;
    };

    class AugmentedRay : public StateCommand {
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

    class AugmentedPath : public StateCommand {
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
    class Polyline : public StateCommand {
    public:
		Polyline() = default;
        Polyline(const std::vector<DrawingInstructions::Point>& points);

		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        std::vector<DrawingInstructions::Point> points;
    };

    // Arc3Points class
    class Arc3Points : public StateCommand {
    public:
		Arc3Points() = default;
        Arc3Points(double startPointX, double startPointY, double medianPointX, double medianPointY, double endPointX, double endPointY);

		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;


    private:
       DrawingInstructions::Point startPoint;
       DrawingInstructions::Point medianPoint;
       DrawingInstructions::Point endPoint;
    };

    // ArcByRadius class
    class ArcByRadius : public StateCommand {
    public:
		ArcByRadius() = default;
        ArcByRadius(double centerX, double centerY, double radius, double startAngle, double angularDistance);

		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        DrawingInstructions::Point center;
        double radius = 0.0;
        double startAngle = 0.0;
        double angularDistance = 360.0;
    };

    // Annulus class
    class Annulus : public StateCommand {
    public:
		Annulus() = default;
        Annulus(double centerX, double centerY, double outerRadius, double innerRadius, double startAngle, double angularDistance);

		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        DrawingInstructions::Point center;
        double outerRadius = 0.0;
        double innerRadius = 0.0;
        double startAngle = 0.0;
        double angularDistance = 360.0;
    };

    class ClearGeometry : public StateCommand {
    public:
        ClearGeometry() = default;

		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;
    };

	class GeometryCommands
	{
		public:
		GeometryCommands() = default;
		GeometryCommands(const GeometryCommands&) = delete;
		GeometryCommands& operator=(const GeometryCommands&) = delete;
		GeometryCommands(GeometryCommands&&) = delete;
		GeometryCommands& operator=(GeometryCommands&&) = delete;
		~GeometryCommands();

        void setSpatialReference(const std::string& reference, bool forward);
        void setAugmentedPoint(GraphicBasePackage::CRSType crs, double x, double y);
        void setAugmentedRay(GraphicBasePackage::CRSType, double direction, GraphicBasePackage::CRSType crsLength, double length);
        void setAugmentedPath(GraphicBasePackage::CRSType crsPosition, GraphicBasePackage::CRSType crsAngle, GraphicBasePackage::CRSType crsDistance);
        void setPolyline(const std::vector<DrawingInstructions::Point>& points);
        void setArc3Points(double startPointX, double startPointY, double medianPointX, double medianPointY, double endPointX, double endPointY);
        void setArcByRadius(double centerX, double centerY, double radius, double startAngle, double angularDistance);
        void setAnnulus(double centerX, double centerY, double outerRadius, double innerRadius, double startAngle, double angularDistance);
        void setClearGeometry();

        void parse(const std::string& key, std::string value);
		void execute() const;
	private:
		//Geometry Commands
		SpatialReference* spatialReference = nullptr;
		AugmentedPoint* augmentedPoint = nullptr;
		AugmentedRay* augmentedRay = nullptr;
		AugmentedPath* augmentedPath = nullptr;
		Polyline* polyline = nullptr;
		Arc3Points* arc3Points = nullptr;
		ArcByRadius* arcByRadius = nullptr;
		Annulus* annulus = nullptr;
		ClearGeometry* clearGeometryCmd = nullptr;
	};


}


