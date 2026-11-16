#pragma once
#include <cstdint>

class DDR
{
public:
	DDR();
	~DDR();

private:
	int size = 0;
	std::uint8_t* content = nullptr;

public:
	void Set(std::uint8_t* buffer);
	void Delete();

	std::uint8_t* GetContent();
	void Allocate();
	void Allocate(int size);
	void SetByte(int index, std::uint8_t value);
	void SetBytes(int size, std::uint8_t* value);

	int GetSize();
	void SetSize(int value);
};