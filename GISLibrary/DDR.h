#pragma once

class DDR
{
public:
	DDR();
	~DDR();

private:
	int size = 0;
	BYTE* content = nullptr;

public:
	void Set(BYTE* buffer);
	void Delete();

	BYTE* GetContent();
	void Allocate();
	void Allocate(int size);
	void SetByte(int index, BYTE value);
	void SetBytes(int size, BYTE* value);

	int GetSize();
	void SetSize(int value);
};