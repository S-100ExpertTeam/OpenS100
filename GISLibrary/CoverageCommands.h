#pragma once
#include "StateCommand.h"

namespace DrawingInstruction
{

    // NumericAnnotation Class
    class NumericAnnotation : public StateCommand {
    public:
        NumericAnnotation(int decimals, const std::string& championChoice, double buffer);

        void execute()  override;
        void parse(const std::string& input) override;

    private:
        int decimals;
        std::string championChoice;
        double buffer;
    };

    // SymbolAnnotation Class
    class SymbolAnnotation : public StateCommand {
    public:
        SymbolAnnotation(const std::string& symbolRef, const std::string& rotationAttribute, const std::string& scaleAttribute,
            const std::string& rotationCRS, double rotationOffset, double rotationFactor,
            double scaleFactor);
         

        void execute()  override;
        void parse(const std::string& input) override;

    private:
        std::string symbolRef;
        std::string rotationAttribute;
        std::string scaleAttribute;
        std::string rotationCRS;
        double rotationOffset;
        double rotationFactor;
        double scaleFactor;
    };

    // CoverageColor Class
    class CoverageColor : public StateCommand {
    public:
        CoverageColor(const std::string& startToken, double startTransparency, const std::string& endToken, double endTransparency, double penWidth);
            
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        std::string startToken;
        double startTransparency;
        std::string endToken;
        double endTransparency;
        double penWidth;
    };

    class LookupEntry : public StateCommand {
    public:
        LookupEntry(const std::string& label, double lower, double upper, const std::string& closure);
        void execute()  override;
        void parse(const std::string& input) override;

    private:
        std::string label;
        double lower;
        double upper;
        std::string closure;
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
			const std::string& rotationCRS, double rotationOffset, double rotationFactor,
			double scaleFactor);
        void setCoverageColor(const std::string& startToken, double startTransparency, const std::string& endToken, double endTransparency, double penWidth);
        void setLookupEntry(const std::string& label, double lower, double upper, const std::string& closure);


        void parse(const std::string& key, std::string value);
        void executeCommands() const;

    private:
        //Coverage commands
		NumericAnnotation* numericAnnotation = nullptr;
		SymbolAnnotation* symbolAnnotation = nullptr;
		CoverageColor* coverageColor = nullptr;
		LookupEntry* lookupEntry = nullptr;
    };




}

