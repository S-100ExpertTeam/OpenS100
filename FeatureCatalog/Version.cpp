#include "stdafx.h"
#include "Version.h"

#include <sstream>
#include <vector>

Version::Version()
{
}

Version::Version(const Version& other)
	: major(other.major),
	minor(other.minor),
	patch(other.patch),
	working(other.working),
	source(other.source)
{
}

Version::~Version()
{
}

std::string Version::getMajor() const
{
    return major;
}

void Version::setMajor(const std::string& major)
{
    this->major = major;
}

int Version::getMajorAsInt() const
{
	return convertToInt(major);
}

std::string Version::getMinor() const
{
    return minor;
}

void Version::setMinor(const std::string& minor)
{
    this->minor = minor;
}

int Version::getMinorAsInt() const
{
	return convertToInt(minor);
}

std::string Version::getPatch() const
{
    return patch;
}

void Version::setPatch(const std::string& patch)
{
    this->patch = patch;
}

int Version::getPatchAsInt() const
{
	return convertToInt(patch);
}

std::string Version::getWorking() const
{
    return working;
}

void Version::setWorking(const std::string& working)
{
    this->working = working;
}

std::string Version::getSource() const
{
    return source;
}

void Version::setSource(const std::string& source)
{
	init();
    this->source = source;
    parseSource(source);
}

void Version::init()
{
	major = "";
	minor = "";
	patch = "";
	working = "";
	source = "";
}

void Version::parseSource(const std::string& source)
{
	// Split the source string by '.'
	std::vector<std::string> parts;
	std::string delimiter = ".";
	std::string remaining = source;
	size_t pos = 0;
	std::string token;

	while ((pos = remaining.find(delimiter)) != std::string::npos) {
		token = remaining.substr(0, pos);
		parts.push_back(token);
		remaining = remaining.substr(pos + delimiter.length());
	}

	if (!remaining.empty()) {
		parts.push_back(remaining);
	}

	// Set the variables based on the number of parts
	if (parts.size() >= 1) {
		setMajor(parts[0]);
	}
	if (parts.size() >= 2) {
		setMinor(parts[1]);
	}
	if (parts.size() >= 3) {
		setPatch(parts[2]);
	}
	if (parts.size() >= 4) {
		setWorking(parts[3]);
	}
}

int Version::convertToInt(const std::string& str) const
{
	try {
		return std::stoi(str);
	}
	catch (const std::exception& e) {
		return 0;  
	}
}

bool Version::operator==(const Version& other) const
{
	return (major == other.major &&
		minor == other.minor &&
		patch == other.patch);
}

bool Version::operator<(const Version& other) const
{
	auto thisMajor = getMajorAsInt();
	auto thisMinor = getMinorAsInt();
	auto thisPatch = getPatchAsInt();

	auto otherMajor = other.getMajorAsInt();
	auto otherMinor = other.getMinorAsInt();
	auto otherPatch = other.getPatchAsInt();

	if (thisMajor != otherMajor)
		return thisMajor < otherMajor;

	if (thisMinor != otherMinor)
		return thisMinor < otherMinor;

	return thisPatch < otherPatch;
}