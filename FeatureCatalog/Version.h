#pragma once

#include <string>
#include "StringUtil.h"

class Version
{
public:
	Version();
	Version(std::string major, std::string minor, std::string patch, std::string working = "");
	Version(const Version& other);
	virtual ~Version();

public:
	std::string major = "0";
	std::string minor = "0";
	std::string patch = "0";
	std::string working = "0";

	std::string source;

public:
	std::string getMajor() const;
	std::wstring getMajorW() const;
	void setMajor(const std::string& major);
	int getMajorAsInt() const;

	std::string getMinor() const;
	std::wstring getMinorW() const;
	void setMinor(const std::string& minor);
	int getMinorAsInt() const;

	std::string getPatch() const;
	std::wstring getPatchW() const;
	void setPatch(const std::string& patch);
	int getPatchAsInt() const;

	std::string getWorking() const;
	std::wstring getWorkingW() const;
	void setWorking(const std::string& working);

	std::string getSource() const;
	std::wstring getSourceW() const;
	void setSource(const std::string& source);

	Version getMajorMinor();
	Version getMajor();
	bool isEmpty();

	void parseSource(const std::string& source);

public:
	bool operator==(const Version& other) const;
	bool operator<(const Version& other) const;
	bool operator>(const Version& other) const;
	bool operator<=(const Version& other) const;

private:
	void init();
	int convertToInt(const std::string& str) const;
};
