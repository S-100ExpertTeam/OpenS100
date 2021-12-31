#include "StdAfx.h"
#include "ShipInfo.h"
ShipInfo::ShipInfo(void)
{
	m_latitude = 0.;
	m_longitude = 0.;

	m_length = 0.;
	m_width = 0.;
	m_sog = 0.;		// speed measured by GPS.(SOG, Speed Over Ground)
	m_cog = 0.;		// COG, Course Over Ground
	m_hdg = 0.;		// HDG, Heading
}

ShipInfo::~ShipInfo(void) 
{

}

void ShipInfo::SetLength(double _length)
{
	m_length = _length;
}
void ShipInfo::SetWidth(double _width)
{
	m_width = _width;

}
void ShipInfo::SetPosition(double _longitude, double _latitude)
{
	m_latitude = _latitude;
	m_longitude = _longitude;
}

void ShipInfo::SetMovementStatus(double _sog, double _cog, double _heading)
{
	m_sog = _sog;
	m_cog = _cog;
	m_hdg = _heading;
}

void ShipInfo::GetLength(double &_length)
{
	_length = m_length;

}

void ShipInfo::GetWidth(double &_width)
{
	_width = m_width;
}

void ShipInfo::GetPosition(double &_longitude, double &_latitude)
{
	_latitude = m_latitude;
	_longitude = m_longitude;
}

void ShipInfo::GetMovementStatus(double& _sog, double& _cog, double& _heading)
{
	_sog = m_sog;
	_cog = m_cog;
	_heading = m_hdg;
}