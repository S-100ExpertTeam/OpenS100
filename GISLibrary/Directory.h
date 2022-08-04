#pragma once

class Directory
{
public:
	Directory();
	~Directory();

public:
	char fieldTag[4] = { 0, };
	int fieldLength = 0;
	int fieldPosition = 0;
};