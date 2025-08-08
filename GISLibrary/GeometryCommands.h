#pragma once
#include "StateCommand.h"

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
        bool forward;
    };

    class AugmentedPoint : public StateCommand {
    public:
		AugmentedPoint() = default;
        AugmentedPoint(std::string crs, double x, double y);

		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        std::string crs;
        double x;
        double y;
    };

    class AugmentedRay : public StateCommand {
    public:
		AugmentedRay() = default;
        AugmentedRay(std::string CRSType, double direction, std::string crsLength, double length);

		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        std::string CRSType;
        double direction;
        std::string crsLength;
        double length;
    };

    class AugmentedPath : public StateCommand {
    public:
		AugmentedPath() = default;
        AugmentedPath(std::string crsPosition, std::string crsAngle, std::string crsDistance);

		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        std::string crsPosition;
        std::string crsAngle;
        std::string crsDistance;
    };

    // Polyline class
    class Polyline : public StateCommand {
    public:
		Polyline() = default;
        Polyline(const std::vector<std::string>& points);

		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        std::vector<std::string> points;
    };

    // Arc3Points class
    class Arc3Points : public StateCommand {
    public:
		Arc3Points() = default;
        Arc3Points(std::string startPointX, std::string startPointY, std::string medianPointX, std::string medianPointY, std::string endPointX, std::string endPointY);

		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;


    private:
        std::string startPointX;
        std::string startPointY;
        std::string medianPointX;
        std::string medianPointY;
        std::string endPointX;
        std::string endPointY;
    };

    // ArcByRadius class
    class ArcByRadius : public StateCommand {
    public:
		ArcByRadius() = default;
        ArcByRadius(std::string centerX, std::string centerY, double radius, double startAngle, double angularDistance);

		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        std::string centerX;
        std::string centerY;
        double radius;
        double startAngle;
        double angularDistance;
    };

    // Annulus class
    class Annulus : public StateCommand {
    public:
        Annulus(std::string centerX, std::string centerY, double outerRadius, double innerRadius, double startAngle, double angularDistance);

		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        std::string centerX;
        std::string centerY;
        double outerRadius;
        double innerRadius;
        double startAngle;
        double angularDistance;
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
        void setAugmentedPoint(std::string crs, double x, double y);
        void setAugmentedRay(std::string CRSType, double direction, std::string crsLength, double length);
        void setAugmentedPath(std::string crsPosition, std::string crsAngle, std::string crsDistance);
        void setPolyline(const std::vector<std::string>& points);
        void setArc3Points(std::string startPointX, std::string startPointY, std::string medianPointX, std::string medianPointY, std::string endPointX, std::string endPointY);
        void setArcByRadius(std::string centerX, std::string centerY, double radius, double startAngle, double angularDistance);
        void setAnnulus(std::string centerX, std::string centerY, double outerRadius, double innerRadius, double startAngle, double angularDistance);
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


