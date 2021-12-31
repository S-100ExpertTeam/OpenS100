#include "stdafx.h"
#include "S101SENC.h"
#include <fstream>

S101SENC::S101SENC()
{

}

S101SENC::~S101SENC()
{
	Release();
}


#pragma warning(disable:4244)
bool S101SENC::Open(CString path)
{
	char* cPath = LibMFCUtil::ConvertWCtoC((wchar_t*)std::wstring(path).c_str());
	std::ifstream ifs(path, std::ios::binary | std::ios::ate);
	delete[] cPath;

	std::ifstream::pos_type pos = ifs.tellg();

	int length = pos;
	BYTE* pData = new BYTE[length];
	ifs.seekg(0, std::ios::beg);
	ifs.read((char*)pData, length);
	ifs.close();

	long fLen = length;
	long fPos = 0;

	while (fPos < fLen)
	{
		BYTE* p = pData + fPos;
		if (fPos + SIZEOF_SENCINFO > fLen)
			break;

		int dataLength = *(int*)p;
		SencType dataType = (SencType)*(p += SIZEOF_DATALENGTH);
		int itemCount = *(int*)(p += SIZEOF_DATAID);
		p += SIZEOF_ITEMCOUNT;

		if ((fPos += dataLength) > fLen)
			break;

		int dataSize = dataLength - SIZEOF_SENCINFO;

		switch (dataType)
		{
		case Points:
		{
			s_Point = new stPoint[itemCount];
			s_PointCount = itemCount;
			memcpy(s_Point, p, dataSize);
			break;
		}
		case Curve:
		{
			s_Curve = new stPoint*[itemCount];
			s_CurveCount = itemCount;
			s_CurveEachCount = new int[itemCount];
			memcpy(s_CurveEachCount, p, itemCount * SIZEOF_ITEMCOUNT);
			p += itemCount * SIZEOF_ITEMCOUNT;

			for (int i = 0; i < itemCount; i++)
			{
				int count = s_CurveEachCount[i];
				if (count > 0)
				{
					s_Curve[i] = new stPoint[count];
					memcpy(s_Curve[i], p, count * SIZEOF_INT2DPOINT);
					p += count * SIZEOF_INT2DPOINT;
				}
				else
				{
					s_Curve[i] = nullptr;
				}
			}
			break;
		}
		case CompCurve:
		{
			s_CompCurve = new stCompCurve*[itemCount];
			s_CompCurveCount = itemCount;
			s_CompCurveEachCount = new int[itemCount];
			memcpy(s_CompCurveEachCount, p, itemCount * SIZEOF_ITEMCOUNT);
			p += itemCount * SIZEOF_ITEMCOUNT;

			for (int i = 0; i < itemCount; i++)
			{
				int count = s_CompCurveEachCount[i];
				if (count > 0)
				{
					s_CompCurve[i] = new stCompCurve[count];
					memcpy(s_CompCurve[i], p, count * SIZEOF_COMPCURVEREFINFO);
					p += count * SIZEOF_COMPCURVEREFINFO;
				}
				else
				{
					s_CompCurve[i] = nullptr;
				}
			}
			break;
		}
		case Surface:
		{
			s_Surface = new stSurface*[itemCount];
			s_SurfaceCount = itemCount;
			s_SurfaceEachCount = new int[itemCount];
			memcpy(s_SurfaceEachCount, p, itemCount * SIZEOF_ITEMCOUNT);
			p += itemCount * SIZEOF_ITEMCOUNT;

			for (int i = 0; i < itemCount; i++)
			{
				int count = s_SurfaceEachCount[i];
				if (count > 0)
				{
					s_Surface[i] = new stSurface[count];
					memcpy(s_Surface[i], p, count * SIZEOF_SURFACEREFINFO);
					p += count * SIZEOF_SURFACEREFINFO;
				}
				else
				{
					s_Surface[i] = nullptr;
				}
			}
			break;
		}
		case Feature:
		{
			s_Feature = new stFeature[itemCount];
			s_FeatureCount = itemCount;
			memcpy(s_Feature, p, dataSize);
			break;
		}
		}
	}

	delete[] pData;

	s_CompCurvePoint = new stPoint*[s_CompCurveCount];
	s_CompCurvePointEachCount = new int[s_CompCurveCount];
	for (int i = 0; i < s_CompCurveCount; i++)
	{
		stPoint lastPoint;
		lastPoint.X = MAXINT32;
		lastPoint.Y = MAXINT32;
		std::vector<stPoint> pList;

		auto eachData = s_CompCurve[i];
		int eachCount = s_CompCurveEachCount[i];

		for (int j = 0; j < eachCount; j++)
		{
			int rrid = eachData[j].RRID;
			auto pData = s_Curve[rrid];
			int pCount = s_CurveEachCount[rrid];

			switch (eachData[j].ORNT)
			{
			case 1:
				for (int c = 0; c < pCount; c++)
				{
					stPoint point = pData[c];
					if (lastPoint.X != point.X || lastPoint.Y != point.Y)
					{
						lastPoint = point;
						pList.push_back(point);
					}
				}
				break;
			case 2:
				for (int c = pCount - 1; c >= 0; c--)
				{
					stPoint point = pData[c];
					if (lastPoint.X != point.X || lastPoint.Y != point.Y)
					{
						lastPoint = point;
						pList.push_back(point);
					}
				}
				break;
			}
		}

		if (pList.size() > 0)
		{
			s_CompCurvePoint[i] = new stPoint[pList.size()];
			memcpy(s_CompCurvePoint[i], &pList.at(0), sizeof(stPoint) * pList.size());

			s_CompCurvePointEachCount[i] = (int)pList.size();
			pList.clear();
		}
		else
		{
			s_CompCurvePoint[i] = nullptr;
		}
	}

	vecCurve = new  std::vector<D2D1_POINT_2F>[s_CurveCount];
	vecCompositeCurve = new std::vector<D2D1_POINT_2F>[s_CompCurveCount];
	vecOuterRing = new std::vector<D2D1_POINT_2F>[s_SurfaceCount];

	return true;
}

void S101SENC::Release()
{
	delete[] s_Point;
	s_Point = nullptr;

	for (int i = 0; i < s_CurveCount; i++)
	{
		delete[] s_Curve[i];
		s_Curve[i] = nullptr;
	}

	delete[] s_Curve;
	s_Curve = nullptr;
	delete[] s_CurveEachCount;
	s_CurveEachCount = nullptr;

	for (int i = 0; i < s_CompCurveCount; i++)
	{
		delete[] s_CompCurve[i];
		s_CompCurve[i] = nullptr;
		delete[] s_CompCurvePoint[i];
		s_CompCurvePoint[i] = nullptr;
	}

	delete[] s_CompCurve;
	s_CompCurve = nullptr;
	delete[] s_CompCurveEachCount;
	s_CompCurveEachCount = nullptr;
	delete[] s_CompCurvePoint;
	s_CompCurvePoint = nullptr;
	delete[] s_CompCurvePointEachCount;
	s_CompCurvePointEachCount = nullptr;

	for (int i = 0; i < s_SurfaceCount; i++)
	{
		delete[] s_Surface[i];
		s_Surface[i] = nullptr;
	}

	delete[] s_Surface;
	s_Surface = nullptr;
	delete[] s_SurfaceEachCount;
	s_SurfaceEachCount = nullptr;

	delete[] s_Feature;
	s_Feature = nullptr;

	delete vecCurve;
	delete vecCompositeCurve;
	delete vecOuterRing;
}

int S101SENC::GetFeatureCount()
{
	return s_FeatureCount;
}

int S101SENC::GetGeometryType(int featureIndex)
{
	switch (GetStFeature(featureIndex)->RRNM)
	{
	case SencType::Points:
		return 110;
	case SencType::Curve:
		return 120;
	case SencType::CompCurve:
		return 125;
	case SencType::Surface:
		return 130;
	default:
		return -1;
	}
}

int S101SENC::GetSpatialReferenceID(int featureID)
{
	return GetStFeature(featureID)->RRID;
}

S101SENC::stFeature* S101SENC::GetStFeature(int featureID)
{
	return &s_Feature[featureID];
}

S101SENC::stPoint* S101SENC::GetStPoint(int featureID)
{
	return &s_Point[GetSpatialReferenceID(featureID)];
}

int S101SENC::GetStCurve(int featureID, S101SENC::stPoint** outputCurve)
{
	*outputCurve = s_Curve[GetSpatialReferenceID(featureID)];
	return s_CurveEachCount[GetSpatialReferenceID(featureID)];
}

int S101SENC::GetStCompCurve(int featureID, S101SENC::stPoint** outputCompCurve)
{
	*outputCompCurve = s_CompCurvePoint[GetSpatialReferenceID(featureID)];
	return s_CompCurvePointEachCount[GetSpatialReferenceID(featureID)];
}

#pragma warning(disable:4244)
D2D1_POINT_2F S101SENC::GetPoint(int featureID)
{
	D2D1_POINT_2F d2Point;
	d2Point.x = GetStPoint(featureID)->X * COMF;
	d2Point.y = GetStPoint(featureID)->Y * COMF;
	return d2Point;
}

#pragma warning(disable:4838)
#pragma warning(disable:4244)
void S101SENC::GetCurve(int featureID, std::vector<D2D1_POINT_2F>& points)
{
	stPoint* stPoints = nullptr;
	int pointCount = GetStCurve(featureID, &stPoints);

	for (int i = 0; i < pointCount; i++)
	{
		D2D1_POINT_2F d2Point = { stPoints[i].X * COMF, stPoints[i].Y * COMF };
		points.push_back(d2Point);
	}
}

#pragma warning(disable:4838)
#pragma warning(disable:4244)
void S101SENC::GetCompositeCurve(int featureID, std::vector<D2D1_POINT_2F>& points)
{
	stPoint* stPoints = nullptr;
	int pointCount = GetStCompCurve(featureID, &stPoints);

	for (int i = 0; i < pointCount; i++)
	{
		D2D1_POINT_2F d2Point = { stPoints[i].X * COMF, stPoints[i].Y * COMF };
		points.push_back(d2Point);
	}
}

int S101SENC::GetInnerRingCount(int featureID)
{
	return s_SurfaceEachCount[GetSpatialReferenceID(featureID)] - 1;
}

#pragma warning(disable:4838)
#pragma warning(disable:4244)
void S101SENC::GetOuterRing(int featureID, std::vector<D2D1_POINT_2F>& points)
{
	stPoint* ringPoints = nullptr;
	int pointCount = 0;
	if (s_Surface[GetSpatialReferenceID(featureID)][0].RRNM == 120)
	{
		pointCount = GetStCurve(featureID, &ringPoints);
	}
	else if (s_Surface[GetSpatialReferenceID(featureID)][0].RRNM == 125)
	{
		pointCount = GetStCompCurve(featureID, &ringPoints);
	}

	for (int i = 0; i < pointCount; i++)
	{
		D2D1_POINT_2F d2Point = { ringPoints[i].X * COMF, ringPoints[i].Y * COMF };
		points.push_back(d2Point);
	}
}

#pragma warning(disable:4838)
#pragma warning(disable:4244)
void S101SENC::GetInnerRing(int featureID, int innerRingIndex, std::vector<D2D1_POINT_2F>& points)
{
	stPoint* ringPoints = nullptr;
	int pointCount = 0;

	if (s_Surface[GetSpatialReferenceID(featureID)][innerRingIndex - 1].RRNM == 120)
	{
		pointCount = GetStCurve(featureID, &ringPoints);
	}
	else if (s_Surface[GetSpatialReferenceID(featureID)][innerRingIndex - 1].RRNM == 125)
	{
		pointCount = GetStCompCurve(featureID, &ringPoints);
	}

	for (int i = 0; i < pointCount; i++)
	{
		D2D1_POINT_2F d2Point = { ringPoints[i].X * COMF, ringPoints[i].Y * COMF };
		points.push_back(d2Point);
	}
}