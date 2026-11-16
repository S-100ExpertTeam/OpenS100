#pragma once
#include <cstdint>
class NonPrintableCharacter
{
public:
	static const std::uint8_t lexcalLevel0Terminator = ' ';
	static const std::uint8_t lexcalLevel2Terminator = 0x00;
	static const std::uint8_t fieldTerminator = 0x1E;
	static const std::uint8_t unitTerminator = 0x1F;
};

