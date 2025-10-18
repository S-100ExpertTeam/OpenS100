#pragma once

#include "StateCommand.h"

#include "..\\FeatureCatalog\\IntervalType.h"

namespace DrawingInstructions
{

    // NumericAnnotation Class
    class NumericAnnotation : public StateCommand {
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
    class SymbolAnnotation : public StateCommand {
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
    class CoverageColor : public StateCommand {
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

    class LookupEntry : public StateCommand {
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

    class CoverageCommands
    {
    public:
        CoverageCommands() = default;
        CoverageCommands(const CoverageCommands&) = delete;
        CoverageCommands& operator=(const CoverageCommands&) = delete;
        CoverageCommands(CoverageCommands&&) = delete;
        CoverageCommands& operator=(CoverageCommands&&) = delete;
        ~CoverageCommands();


        void setNumericAnnotation(int decimals, const std::string& championChoice, double buffer);
        void setSymbolAnnotation(const std::string& symbolRef, const std::string& rotationAttribute, const std::string& scaleAttribute,
			const GraphicBasePackage::CRSType rotationCRS, double rotationOffset, double rotationFactor,
			double scaleFactor);
        void setCoverageColor(const std::string& startToken, double startTransparency, const std::string& endToken, double endTransparency, double penWidth);
        void setLookupEntry(const std::string& label, double lower, double upper, const IntervalType closure);


        void parse(const std::string& key, std::string value);
        void execute() const;

    private:
        //Coverage commands
		NumericAnnotation* numericAnnotation = nullptr;
		SymbolAnnotation* symbolAnnotation = nullptr;
		CoverageColor* coverageColor = nullptr;
		LookupEntry* lookupEntry = nullptr;
    };




}

