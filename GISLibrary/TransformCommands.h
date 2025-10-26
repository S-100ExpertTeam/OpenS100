#pragma once
#include "StateCommand.h"

namespace Part9a
{
    class LocalOffset : public StateCommand
    {
    public:
		LocalOffset() = default;
        LocalOffset(double xOffsetMM, double yOffsetMM);
        Enum_CommandType GetType() const override;
		void init() override;
        void execute() override;
        void parse(const std::string& input) override;

    private:
        double xOffsetMM = 0;
        double yOffsetMM = 0;

    public:
        double GetXOffsetMM() const;
		double GetYOffsetMM() const;
    };

    class LinePlacement : public StateCommand
    {
    public:
		LinePlacement() = default;
        LinePlacement(const std::string& linePlacementMode, double offset, double endOffset, bool visibleParts);
        Enum_CommandType GetType() const override;
		void init() override;
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string linePlacementMode = "Relative"; // or Absolute
        double offset = 0.5;
        std::optional<double> endOffset;
        bool visibleParts = false;

    public:
        std::string GetLinePlacementMode() const;
        double GetOffset() const;
        std::optional<double> GetEndOffset() const;
		bool IsVisibleParts() const;
    };

    class AreaPlacement : public StateCommand
    {
    public:
		AreaPlacement() = default;
        AreaPlacement(const std::string& areaPlacementMode);
        Enum_CommandType GetType() const override;
		void init() override;
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string areaPlacementMode = "VisibleParts";

    public:
		std::string GetAreaPlacementMode() const;
    };

    class AreaCRS : public StateCommand
    {
    public:
		AreaCRS() = default;
        AreaCRS(const std::string& areaCRSType);
        Enum_CommandType GetType() const override;
		void init() override;
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string areaCRSType = "GlobalGeometry";
    };

    class Rotation : public StateCommand
    {
    public:
		Rotation() = default;
        Rotation(const std::string& rotationCRS, double rotation);
        Enum_CommandType GetType() const override;
		void init() override;
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string rotationCRS = "PortrayalCRS";
        double rotation = 0.0;

    public:
		std::string GetRotationCRS() const;
		double GetRotation() const;
    };

    class ScaleFactor : public StateCommand
    {
    public:
		ScaleFactor() = default;
        ScaleFactor(double scaleFactor);
        Enum_CommandType GetType() const override;
		void init() override;
        void execute() override;
        void parse(const std::string& input) override;

    private:
        double scaleFactor = 1.0;

	public:
		double GetScaleFactor() const;
    };

}