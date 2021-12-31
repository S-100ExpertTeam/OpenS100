#pragma once

#include <string>
class ShipInfo
{
public:
	ShipInfo();
	virtual ~ShipInfo();

private:
	std::wstring m_name;
	std::wstring m_description;

	double m_latitude;
	double m_longitude;

	double m_length;
	double m_width;
	double m_sog;		// SOG, Speed Over Ground
	double m_cog;		// COG, Course Over Ground
	double m_hdg;		// HDG, Heading

public:
	void SetLength(double _length);
	void SetWidth(double _length);
	void SetPosition(double _longitude, double _latitude);
	void SetMovementStatus(double _sog, double _cog, double _heading);

	void GetLength(double& _length);
	void GetWidth(double& _length);
	void GetPosition(double& _longitude, double& _latitude);
	void GetMovementStatus(double& _sog, double& _cog, double& _heading);
};