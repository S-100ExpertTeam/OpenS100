#pragma once
#include "LibMFCUtil.h"

#include <vector>
#include <string>

class LIBMFC_API StringTokenizer
{
public:
	StringTokenizer(const std::string& inputstring, const std::string& seperator);
	virtual ~StringTokenizer();

private:
	std::string _input;
	std::string _delimiter;
	std::vector<std::string> token;
	std::vector<std::string>::iterator index;

public:
	size_t countTokens(); //token num
	bool hasMoreTokens(); //Checking the existence of "token.
	std::string nextToken();  //next token
	void split();   //Divide the string into seperators and save it in vector.
};