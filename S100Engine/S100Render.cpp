#include "stdafx.h"
#include "S100Render.h"
#include "EnumTypeOfDrawingInstruction.h"
#include "S100DrawingUnitPoint.h"
#include "S100DrawingUnitPolyLine.h"
#include "S100DrawingUnitPolygon.h"
#include "S100DrawingUnitPatternPolygon.h"
#include "S100DrawingUnitTextPoint.h"
#include "S100DrawingUnitAugmentedPath.h"
#include "S100DrawingUnitAugmentedRay.h"
#include "AreaPatternBitmap.h"


S100Render::S100Render()
{
	InitViewFactor();
}


S100Render::~S100Render()
{

}

void S100Render::DrawBitmapOnLine(
	ID2D1RenderTarget *pRenderTarget,
	D2D1_POINT_2F point1,
	D2D1_POINT_2F point2,
	ID2D1Bitmap *pBitmap,
	D2D1_COLOR_F penColor,
	ID2D1StrokeStyle1 *stroke)
{
	D2D1_SIZE_F bitmapSize = pBitmap->GetSize();

	FLOAT rotation = atan2f(point2.y - point1.y, point2.x - point1.x) * (180 / 3.1415f);

	D2D1::Matrix3x2F oldMatrix;
	pRenderTarget->GetTransform(&oldMatrix);

	FLOAT length = sqrtf(pow((point1.x - point2.x), 2.0f) + pow((point2.y - point1.y), 2.0f));

	if (fmodf(length, bitmapSize.width) == 0.0)
	{

	}
	else if (length < 20 && stroke)
	{
		pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		ID2D1SolidColorBrush *pBrush = nullptr;
		pRenderTarget->CreateSolidColorBrush(penColor, &pBrush);
		pRenderTarget->DrawLine(point1, point2, pBrush, 1, stroke);
		SafeRelease(&pBrush);
		//pRenderTarget->SetTransform(oldMatrix);
	}
	else
	{
		pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(rotation, point1));
		D2D1_POINT_2F oldPoint1 = point1;
		FLOAT lastSegmentLength = fmodf(length, bitmapSize.width);

		if (lastSegmentLength == 0.0)
		{
			lastSegmentLength = bitmapSize.width;
		}

		for (int i = 0; i < ceil(length / bitmapSize.width); i++)
		{
			if (i < ceil(length / bitmapSize.width) - 1)
			{
				pRenderTarget->DrawBitmap(
					pBitmap,
					D2D1::RectF(point1.x, point1.y - bitmapSize.height / 2, point1.x + bitmapSize.width, point1.y + bitmapSize.height / 2));
			}
			else
			{
				pRenderTarget->DrawBitmap(
					pBitmap,
					D2D1::RectF(point1.x, point1.y - bitmapSize.height / 2, point1.x + lastSegmentLength, point1.y + bitmapSize.height / 2),
					1,
					D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
					D2D1::RectF(0, 0, lastSegmentLength, bitmapSize.height));
			}

			point1.x += bitmapSize.width;
		}

		//pRenderTarget->SetTransform(oldMatrix);
	}

	pRenderTarget->SetTransform(oldMatrix);
}


void S100Render::DrawBitmapOnPolyline(
	ID2D1RenderTarget *pRenderTarget,
	D2D1_POINT_2F *points,
	int cnt,
	ID2D1Bitmap *pBitmap,
	D2D1_RECT_F viewRect,
	D2D1_COLOR_F penColor,
	ID2D1StrokeStyle1 *stroke)
{
	for (int i = 1; i < cnt; i++)
	{
		DrawBitmapOnLine(pRenderTarget, points[i - 1], points[i], pBitmap, penColor, stroke);
	}
}


bool S100Render::PointInRect(D2D1_POINT_2F point, D2D1_RECT_F rect)
{
	if (point.x < rect.left || point.x > rect.right)
	{
		return false;
	}
	else if (point.y < rect.top || point.y > rect.bottom)
	{
		return false;
	}
	return true;
}


bool S100Render::OverlapLineAndRect(D2D1_POINT_2F point1, D2D1_POINT_2F point2, D2D1_RECT_F rect)
{
	if (PointInRect(point1, rect) || PointInRect(point2, rect))
	{
		return true;
	}
	else if (InterSectLineAndRect(point1, point2, rect))
	{
		return true;
	}
	return false;
}


bool S100Render::InterSectLineAndRect(D2D1_POINT_2F point1, D2D1_POINT_2F point2, D2D1_RECT_F rect)
{
	if (InterSectLine(point1.x, point1.y, point2.x, point2.y, rect.left, rect.top, rect.right, rect.top)) // up
	{
		return true;
	}
	else if (InterSectLine(point1.x, point1.y, point2.x, point2.y, rect.right, rect.top, rect.right, rect.bottom)) //right
	{
		return true;
	}
	else if (InterSectLine(point1.x, point1.y, point2.x, point2.y, rect.left, rect.bottom, rect.right, rect.bottom)) //down
	{
		return true;
	}
	else if (InterSectLine(point1.x, point1.y, point2.x, point2.y, rect.left, rect.top, rect.left, rect.bottom)) //left
	{
		return true;
	}

	return false;
}


bool S100Render::InterSectLine(
	FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2,
	FLOAT x3, FLOAT y3, FLOAT x4, FLOAT y4)
{
	float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	// If d is zero, there is no intersection
	if (d == 0)
	{
		return false;
	}

	// Get the x and y
	float pre = (x1*y2 - y1 * x2), post = (x3*y4 - y3 * x4);
	float x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
	float y = (pre * (y3 - y4) - (y1 - y2) * post) / d;

	// Check if the x and y coordinates are within both lines
	if (x < min(x1, x2) || x > max(x1, x2) ||
		x < min(x3, x4) || x > max(x3, x4))
	{
		return false;
	}
	if (y < min(y1, y2) || y > max(y1, y2) ||
		y < min(y3, y4) || y > max(y3, y4))
	{
		return false;
	}

	return true;
}

FLOAT S100Render::GetMaxZoom()
{
	FLOAT maxZoom = (FLOAT)(EARTH_2PIR / (PIXEL_MM * 1000.0f * 360.0f));

	return maxZoom;
}

void S100Render::DrawScale(ID2D1RenderTarget *pRenderTarget, ID2D1SolidColorBrush* pBrush, IDWriteTextFormat* pTextFormat)
{
	D2D1::Matrix3x2F oldMatrix;
	pRenderTarget->GetTransform(&oldMatrix);
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	int currentScale = (int)GetCurrentScale();

	CString strScale;
	strScale.Format(_T("1 : %d"), currentScale);

	pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	pRenderTarget->DrawTextW(strScale, strScale.GetLength(), pTextFormat, D2D1::RectF(0, 0, 500, 100), pBrush);

	pRenderTarget->SetTransform(oldMatrix);
}

#pragma warning(disable:4244)
FLOAT S100Render::GetCurrentScale()
{
	return scale;
}

void S100Render::CreateBackgroundGeometry(ID2D1Factory1* pD2Factory)
{
	backgroundLayer.CreateGeometry(pD2Factory, &backgroundSHPFile);
}

#pragma warning(disable:4244)
void S100Render::KeyDown(UINT nChar, CRect& rectView, int dpiX, int dpiY)
{
	FLOAT maxZoom = GetMaxZoom();

	int moveFactor = 50;
	FLOAT offset = 10;

	double zoomFactor = 2.0f;
	if (IsCTRLPressed())
	{
		offset *= 0.01f;
		zoomFactor = 1.1f;
	}

	switch (nChar)
	{
	case VK_LEFT:
		Move(-offset, 0);
		break;
	case VK_RIGHT:
		Move(offset, 0);
		break;
	case VK_UP:
		Move(0, offset);
		break;
	case VK_DOWN:
		Move(0, -offset);
		break;
	case VK_PRIOR:
		ZoomIn(zoomFactor);
		break;
	case VK_NEXT:
		ZoomOut(1.0 / zoomFactor);
		break;
	case VK_HOME:
		RotationClockwise();
		break;
	case VK_END:
		RotationAntiClockwise();
		break;
	case VK_F2:
	{
		int screenWidth = (int)(rectView.Width() * (96.0 / dpiX));
		int screenHeight = (int)(rectView.Height() * (96.0 / dpiY));
		double centerX = screenWidth / 2.0;
		double centerY = screenHeight / 2.0;

		D2D1_POINT_2F chartCenterPoint;
		chartCenterPoint.x = (xmin + xmax) / 2.0f;
		chartCenterPoint.y = -(ymin + ymax) / 2.0f;

		_x = chartCenterPoint.x;
		_y = -chartCenterPoint.y;

		float projectedXmin = ProjectionX(xmin) * FACTOR;
		float projectedYmin = ProjectionX(ymin) * FACTOR;
		float projectedXmax = ProjectionX(xmax) * FACTOR;
		float projectedYmax = ProjectionX(ymax) * FACTOR;

		double earthPixel = rectView.Width() * (360.0 / (xmax - xmin));
		scale = (EARTH_2PIR / PIXEL_MM) / earthPixel;
	}
	break;
	}
}

#pragma warning(disable:4244)
void S100Render::GetMBRfromRenderTarget(D2D1::Matrix3x2F matrix, CRect& rect, FLOAT& xmin, FLOAT& ymin, FLOAT& xmax, FLOAT& ymax)
{
	D2D1_POINT_2F screenLeftBottomPoint;
	D2D1_POINT_2F wgs84LeftBottomPoint;
	screenLeftBottomPoint.x = rect.left;
	screenLeftBottomPoint.y = rect.bottom;
	wgs84LeftBottomPoint = matrix.TransformPoint(screenLeftBottomPoint);
	wgs84LeftBottomPoint.x /= FACTOR;
	wgs84LeftBottomPoint.y /= -FACTOR;

	D2D1_POINT_2F screenTopRightPoint;
	D2D1_POINT_2F wgs84TopRightPoint;
	screenTopRightPoint.x = rect.right;
	screenTopRightPoint.y = rect.top;
	wgs84TopRightPoint = matrix.TransformPoint(screenTopRightPoint);
	wgs84TopRightPoint.x /= FACTOR;
	wgs84TopRightPoint.y /= -FACTOR;

	xmin = InverseProjectionX(wgs84LeftBottomPoint.x);
	ymin = InverseProjectionY(wgs84LeftBottomPoint.y);
	xmax = InverseProjectionX(wgs84TopRightPoint.x);
	ymax = InverseProjectionY(wgs84TopRightPoint.y);

	if (xmin > xmax)
	{
		FLOAT temp = xmin;
		xmin = xmax;
		xmax = temp;
	}

	if (ymin > ymax)
	{
		FLOAT temp = ymin;
		ymin = ymax;
		ymax = temp;
	}

	CString str;
	str.Format(_T("%f, %f, %f, %f\n"), xmin, ymin, xmax, ymax);
	//OutputDebugString(str);
}

#pragma warning(disable:4244)
void S100Render::SetViewFactor(double x, double y, long long zoom)
{
	_x = (x * FACTOR);
	_y = (y * FACTOR);
}


#pragma warning(disable:4244)
void S100Render::ZoomIn(double factor)
{
	scale /= factor;

	if (scale < 1000)
	{
		scale = 1000;
	}
}


#pragma warning(disable:4244)
void S100Render::ZoomOut(double factor)
{
	scale /= factor;

	if (scale > 100000000)
	{
		scale = 100000000;
	}
}


#pragma warning(disable:4244)
void S100Render::Move(double x, double y)
{
	_x += x;
	_y += y;

	if (_x < -180)
	{
		_x = -180;
	}
	else if (_x > 180)
	{
		_x = 180;
	}

	if (_y < -LAT_LIMIT)
	{
		_y = -LAT_LIMIT;
	}
	else if (_y > LAT_LIMIT)
	{
		_y = LAT_LIMIT;
	}
}


void S100Render::RotationClockwise()
{
	_rotation += 30.0f;
}


void S100Render::RotationAntiClockwise()
{
	_rotation -= 30.0f;
}


void S100Render::InitViewFactor()
{
	_x = -150;
	_y = 10;
	_rotation = 0;
	scale = 100000000;
}


void S100Render::SetMapPointAtScreenPoint(FLOAT mx, FLOAT my, int sx, int sy)
{
	mapOffsetX = mx;
	mapOffsetY = my;
	screenOffsetX = sx;
	screenOffsetY = sy;
}


#pragma warning(disable:4244)
void S100Render::GetMapPointByScreenPoint(int sx, int sy, FLOAT* mx, FLOAT* my)
{
	D2D1::Matrix3x2F inverseOfGeometry = geomMatrix;
	D2D1::Matrix3x2F inverseOfScreen = screenMatrix;
	inverseOfGeometry.Invert();
	inverseOfScreen.Invert();

	D2D1::Matrix3x2F inverseMatrix = inverseOfScreen * inverseOfGeometry;
	D2D1_POINT_2F mapPoint = inverseMatrix.TransformPoint(D2D1::Point2F(sx, sy));

	mapPoint.x /= FACTOR;
	mapPoint.y /= FACTOR;

	*mx = InverseProjectionX(mapPoint.x);
	*my = -InverseProjectionY(mapPoint.y);
}


void S100Render::SetOffset(int x, int y)
{
	screenOffsetX = x;
	screenOffsetY = y;
}


#pragma warning(disable:4244)
void S100Render::SetCenterPointByScreen(int x, int y)
{
	D2D1::Matrix3x2F inverseOfGeometry = geomMatrix;
	D2D1::Matrix3x2F inverseOfScreen = screenMatrix;
	inverseOfGeometry.Invert();
	inverseOfScreen.Invert();

	D2D1::Matrix3x2F inverseMatrix = inverseOfScreen * inverseOfGeometry;
	D2D1_POINT_2F mapPoint = inverseMatrix.TransformPoint(D2D1::Point2F(x, y));

	mapPoint.x /= FACTOR;
	mapPoint.y /= FACTOR;

	_x = InverseProjectionX(mapPoint.x);
	_y = -InverseProjectionY(mapPoint.y);

	if (_x < -180)
	{
		_x = -180;
	}
	else if (_x > 180)
	{
		_x = 180;
	}

	if (_y < -LAT_LIMIT)
	{
		_y = -LAT_LIMIT;
	}
	else if (_y > LAT_LIMIT)
	{
		_y = LAT_LIMIT;
	}
}