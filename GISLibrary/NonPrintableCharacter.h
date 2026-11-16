#pragma once
#include <cstdint>
class NonPrintableCharacter
{
public:
	static constexpr std::uint8_t lexcalLevel0Terminator = ' ';
	static constexpr std::uint8_t lexcalLevel2Terminator = 0x00;
	static constexpr std::uint8_t fieldTerminator = 0x1E;
	static constexpr std::uint8_t unitTerminator = 0x1F;
};

