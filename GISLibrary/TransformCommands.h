#pragma once
#include "StateCommand.h"

namespace DrawingInstruction
{


    class LocalOffset : public StateCommand {
    public:
        LocalOffset(double xOffsetMM, double yOffsetMM);
        void execute() override;
        void parse(const std::string& input) override;

    private:
        double xOffsetMM;
        double yOffsetMM;
    };

    class LinePlacement : public StateCommand {
    public:
        LinePlacement(const std::string& linePlacementMode, double offset, double endOffset, bool visibleParts);
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string linePlacementMode;
        double offset;
        double endOffset;
        bool visibleParts;
    };

    class AreaPlacement : public StateCommand {
    public:
        AreaPlacement(const std::string& areaPlacementMode);
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string areaPlacementMode;
    };

    class AreaCRS : public StateCommand {
    public:
        AreaCRS(const std::string& areaCRSType);
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string areaCRSType;
    };

    class Rotation : public StateCommand {
    public:
        Rotation(const std::string& rotationCRS, double rotation);
        void execute() override;
        void parse(const std::string& input) override;

    private:
        std::string rotationCRS;
        double rotation;
    };

    class ScaleFactor : public StateCommand {
    public:
        ScaleFactor(double scaleFactor);
        void execute() override;
        void parse(const std::string& input) override;

    private:
        double scaleFactor;
    };


class TransformCommands
{
public:
    TransformCommands() = default;
    TransformCommands(const TransformCommands&) = delete;
    TransformCommands& operator=(const TransformCommands&) = delete;
    TransformCommands(TransformCommands&&) = delete;
    TransformCommands& operator=(TransformCommands&&) = delete;
    ~TransformCommands();

    void setLocalOffset(double xOffsetMM, double yOffsetMM);
    void setLinePlacement(const std::string& linePlacementMode, double offset, double endOffset, bool visibleParts);
    void setAreaPlacement(const std::string& areaPlacementMode);
    void setAreaCRS(const std::string& areaCRSType);
    void setRotation(const std::string& rotationCRS, double rotation);
    void setScaleFactor(double scaleFactor);


    void parse(const std::string& key, std::string value);
    void executeCommands() const;


private:
    //Transform commands
    LocalOffset* localOffset = nullptr;
    LinePlacement* linePlacement = nullptr;
    AreaPlacement* areaPlacement = nullptr;
    AreaCRS* areaCRS = nullptr;
    Rotation* rotation = nullptr;
    ScaleFactor* scaleFactor = nullptr;
};


}