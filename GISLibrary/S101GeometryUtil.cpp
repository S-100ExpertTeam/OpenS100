#include "stdafx.h"
#include "S101Creator.h"

SGeometryType S101GeometryUtil::ReadWKBGeometryType(const unsigned char* pabyData)
{
	int nByteOrder = DB2_V72_FIX_BYTE_ORDER(*pabyData);
	if (!(nByteOrder == wkbXDR || nByteOrder == wkbNDR))
		return SGeometryType::none;

	wkbByteOrder eByteOrder = static_cast<wkbByteOrder>(nByteOrder);

	int iRawType = 0;
	memcpy(&iRawType, pabyData + 1, 4);

	if (iRawType == 2000001)
		return SGeometryType::Point;
	else if (iRawType == 2000002)
		return SGeometryType::Curve;
	else if (iRawType == 2000005)
		return SGeometryType::Surface;
	else if (iRawType == 2000007)
		return SGeometryType::MultiPoint;
	else if (iRawType == 3000001)
		return SGeometryType::Point;
	else if (iRawType == 3000002)
		return SGeometryType::Curve;
	else if (iRawType == 3000005)
		return SGeometryType::Surface;
	else if (iRawType == 3000007)
		return SGeometryType::MultiPoint;
	else if (iRawType == 4000001)
		return SGeometryType::Point;
	else if (iRawType == 4000002)
		return SGeometryType::Curve;
	else if (iRawType == 4000005)
		return SGeometryType::Surface;
	else if (iRawType == 4000007)
		return SGeometryType::MultiPoint;

	return SGeometryType::none;
}

