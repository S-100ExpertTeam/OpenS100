#pragma once

#include "Command.h"

#include "..\\FeatureCatalog\\IntervalType.h"

namespace DrawingInstructions
{

    // NumericAnnotation Class
    class NumericAnnotation : public Command {
    public:
		NumericAnnotation() = default;
        NumericAnnotation(int decimals, const std::string& championChoice, double buffer);

		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        int decimals = 0;
        std::string championChoice = "ChampionChoice";
        double buffer = 0.0;
    };

    // SymbolAnnotation Class
    class SymbolAnnotation : public Command {
    public:
		SymbolAnnotation() = default;
        SymbolAnnotation(const std::string& symbolRef, const std::string& rotationAttribute, const std::string& scaleAttribute,
            const GraphicBasePackage::CRSType rotationCRS, double rotationOffset, double rotationFactor,
            double scaleFactor);
         
		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        std::string symbolRef;
        std::string rotationAttribute;
        std::string scaleAttribute;
        GraphicBasePackage::CRSType rotationCRS = GraphicBasePackage::CRSType::CRSType_None;
        double rotationOffset = 0.0;
        double rotationFactor = 1.0;
        double scaleFactor = 1.0;
    };

    // CoverageColor Class
    class CoverageColor : public Command {
    public:
		CoverageColor() = default;
        CoverageColor(const std::string& startToken, double startTransparency, const std::string& endToken, double endTransparency, double penWidth);
            
		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        std::string startToken;
        double startTransparency = 0.0;
        std::string endToken;
        double endTransparency = 0.0;
        double penWidth = 0.0;
    };

    class LookupEntry : public Command {
    public:
		LookupEntry() = default;
        LookupEntry(const std::string& label, double lower, double upper, const IntervalType closure);

		void init() override;
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        std::string label;
        double lower = 0.0;
        double upper = 0.0;
        IntervalType closure = IntervalType::none;
    };
}

