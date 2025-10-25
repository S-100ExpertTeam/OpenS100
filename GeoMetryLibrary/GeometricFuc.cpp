#include "stdafx.h"
#include "GeometricFuc.h"
#include "GeoPolygon.h"
#include "GeoPolyline.h"
#include "GeoPoint.h"
#include "ENCCommon.h"

// -10: When there is an end point of a line on the line segment extension to be compared & a tangential line clockwise (the line segment or surface to be compared)
// -11 : When there is an end point of a line on the line segment extension to be compared & a tangent line counterclockwise (the line segment or surface to be compared)
// -20 : When there is an end point of one line on the line segment extension of the comparison criterion & a tangential line clockwise (based on the line segment to find the intersection point)
// -21: When there is an end point of one line on the line segment extension of the comparison criterion & counterclockwise tangent (based on the line segment to find the intersection point)

// -1 : Don't cross.
// 0: If it overlaps,
// 1: Crossing.
// 10: When there is an end point of a line inside the line segment to be compared & a tangential line clockwise (the line segment or surface to be compared)
// 11: When there is an end point of a line inside the line segment to be compared & a tangent line counterclockwise (the line segment or surface to be compared)
// 12: When you reach the end point,
// 20 : Comparison Criteria When there is an end point of a line inside the line segment & a tangential line clockwise (based on the line segment to find the intersection point)
// 21 : When there is an end point of a line inside the comparison reference line segment & a tangent line counterclockwise (based on the line segment to find the intersection point)
// 22: When it reaches the end,

int GeometricFuc::intersect_ccw(double p1x, double p1y, double p2x, double p2y, double p3x, double p3y, double p4x, double p4y)
{
	int r123, r124, r341, r342;

	if(p1x > p2x)
	{
		swap(&p1x, &p2x);
		swap(&p1y, &p2y);
	}
	if(p3x > p4x)
	{
		swap(&p3x, &p4x);
		swap(&p3y, &p4y);
	}

	r123 = ccw(p1x, p1y, p2x, p2y, p3x, p3y);
	r124 = ccw(p1x, p1y, p2x, p2y, p4x, p4y);
	r341 = ccw(p3x, p3y, p4x, p4y, p1x, p1y);
	r342 = ccw(p3x, p3y, p4x, p4y, p2x, p2y);

	if(p1x == p2x && p1y == p2y)
	{
		if(341 * 342 == 0)
			return 1;
		else
			return -1;
	}
	else if(p3x == p4x && p3y == p4y)
	{
		if(123 * 124 == 0)
			return 1;
		else
			return -1;
	}

	if( (r123 * r124 < 0) && (r341 * r342 < 0))
		return 1; // cross

	if(r123 == 0 && r124 == 0) // if same direction
	{
		if(p3x > p2x || p1x > p4x)
			return -1;	// not cross
		//else 
		//	return 0;	// cross
	}

	if(r341 == 0) // If a point is inside the line segment or extension in the direction of the line segment progression,
	{
		if((p3x == p1x && p3y == p1y)
			||  (p4x == p1x && p4y == p1y))
		{
			return 22;
		}
		else if(p3x <= p1x && p1x <= p4x && min(p3y, p4y) <= p1y && max(p3y, p4y) >= p1y) 
		{
			if(r342 == 1) // clockwise
			{
				return 20;
			} 
			else if(r342 == -1) // Counterclockwise.
			{
				return 21;
			}
			else  // Overlap
				return 0;
		}
		else 
		{
			if(r342 == 1) // clockwise
			{
				return -20;
			} 
			else if(r342 == -1) // Counterclockwise
			{
				return -21;
			}
			else// Overlap
				return 0;
		}
	}

	if(r342 == 0) // If a point is inside the line segment or extension in the direction of the line segment progression,
	{
		if((p3x == p2x && p3y == p2y)
			||  (p4x == p2x && p4y == p2y))
		{
			return 22;
		}
		else if(p3x <= p2x && p2x <= p4x && min(p3y, p4y) <= p2y && max(p3y, p4y) >= p2y) 
		{
			if(r341 == 1) // clockwise
			{
				return 20;
			} 
			else if(r341 == -1) // Counterclockwise
			{
				return 21;
			}
			else// Overlap
				return 0;
		}
		else  // If there's an end point on the extension,
		{
			if(r341 == 1) // clockwise
			{
				return -20;
			} 
			else if(r341 == -1) // Counterclockwise
			{
				return -21;
			}
			else// Overlap
				return 0;
		}
	}
	if(r123 == 0) // If a point is inside the line segment or extension in the direction of the line segment start,
	{
		if(		(p1x == p3x && p1y == p3y)
			||	(p2x == p3x && p2y == p3y))
		{
			return 12;
		}
		else if(p1x <= p3x && p3x <= p2x && min(p1y, p2y) <= p3y && max(p1y, p2y) >= p3y) 
		{
			if(r124 == 1) // clockwise
			{
				return 10;
			} 
			else if(r124 == -1) // Counterclockwise
			{
				return 11;
			}
			else// Overlap
				return 0;
		}
		else 
		{
			if(r124 == 1) // clockwise
			{
				return -10;
			} 
			else if(r124 == -1) // Counterclockwise
			{
				return -11;
			}
			else// Overlap
				return 0;
		}
	}
	if(r124 == 0) // If a point is inside the line segment or extension in the direction of the line segment progression,
	{	
		if(		(p1x == p4x && p1y == p4y)
			||	(p2x == p4x && p2y == p4y))
		{
			return 12;
		}
		else if(p1x <= p4x && p4x <= p2x && min(p1y, p2y) <= p4y && max(p1y, p2y) >= p4y) 
		{
			if(r123 == 1) // clockwise
			{
				return 10;
			} 
			else if(r123 == -1) // Counterclockwise
			{
				return 11;
			}
			else// Overlap
				return 0;
		}
		else // Extension line
		{
			if(r123 == 1) // clockwise
			{
				return -10;
			} 
			else if(r123 == -1) // Counterclockwise
			{
				return -11;
			}
			else// Overlap
				return 0;
		}
	}

	return -1; // not cross
}

int GeometricFuc::intersect_ccw(GeoPolyline *objPoly, GeoPolyline *comPoly)
{
	int ret = -1;

	int i = 0, j = 0;

	int objPartIndex = 0;
	int obj2PartIndex = 0;

	for(i = 0; i < objPoly->NumPoints-1 && ret == -1; i++)
	{
		if(objPoly->NumParts > objPartIndex)
		{
			if(objPoly->Parts[objPartIndex + 1] == i+1)
			{
				objPartIndex++;
				continue;
			}
		}

		for(j = 0; j < comPoly->NumPoints-1 && ret == -1; j++)
		{

			if(comPoly->NumParts > obj2PartIndex)
			{
				if(comPoly->Parts[obj2PartIndex + 1] == j+1)
				{
					obj2PartIndex++;
					continue;
				}
			}

			ret = intersect_ccw(objPoly->Points[i].x, objPoly->Points[i].y, objPoly->Points[i+1].x, objPoly->Points[i+1].y,
								comPoly->Points[j].x, comPoly->Points[j].y, comPoly->Points[j+1].x, comPoly->Points[j+1].y);
		}
	}
	return ret;
}

int GeometricFuc::intersect_ccw(GeoPolyline *objPoly, GeoPolygon *comPoly)
{
	int ret = -1;

	int i = 0, j = 0;
	int objPartIndex = 0;
	int obj2PartIndex = 0;

	for(i = 0; i < objPoly->NumPoints-1 && ret < 0; i++)
	{
		if(objPoly->NumParts > objPartIndex)
		{
			if(objPoly->Parts[objPartIndex + 1] == i+1)
			{
				objPartIndex++;
				continue;
			}
		}
		for(j = 0; j < comPoly->NumPoints-1 && ret < 0; j++)
		{			
			if(comPoly->NumParts > obj2PartIndex)
			{
				if(comPoly->Parts[obj2PartIndex + 1] == j+1)
				{
					obj2PartIndex++;
					continue;
				}
			}

			ret = intersect_ccw(objPoly->Points[i].x, objPoly->Points[i].y, objPoly->Points[i+1].x, objPoly->Points[i+1].y,
								comPoly->Points[j].x, comPoly->Points[j].y, comPoly->Points[j+1].x, comPoly->Points[j+1].y);
		}
	}
	if( ret >= 0)
		return 1;
	else 
		return -1;

}

int GeometricFuc::overlap(double p1x, double p1y, GeoPolyline *comPoly, bool applyOption)
{
	if (applyOption)
	{
		return -1;
	}
	int r341;

	int ret = -1;

	MBR p_mbr;
	p_mbr.xmax = p1x;
	p_mbr.xmin = p1x;
	p_mbr.ymax = p1y;
	p_mbr.ymin = p1y;
	if (!MBR::CheckOverlap(p_mbr, comPoly->m_mbr))
	{
		return ret;
	}

	int j = 0;

	int obj2PartIndex = 0;

	for (j = 0; j < comPoly->NumPoints - 1 && ret != 1; j++)
	{
		if (comPoly->NumParts > obj2PartIndex)
		{
			if (comPoly->Parts[obj2PartIndex + 1] == j + 1)
			{
				obj2PartIndex++;
				continue;
			}
		}

		double p3x = comPoly->Points[j].x;
		double p3y = comPoly->Points[j].y;
		double p4x = comPoly->Points[j + 1].x;
		double p4y = comPoly->Points[j + 1].y;

		if (p3x > p4x)
		{
			swap(&p3x, &p4x);
			swap(&p3y, &p4y);
		}

		r341 = ccw(p3x, p3y, p4x, p4y, p1x, p1y);

		if (r341 == 0) // If a point is inside the line segment or extension in the direction of the line segment progression,
		{
			if ((p3x == p1x && p3y == p1y)
				|| (p4x == p1x && p4y == p1y))
			{
				ret = 1;
			}
			else if (p3x <= p1x && p1x <= p4x && min(p3y, p4y) <= p1y && max(p3y, p4y) >= p1y)
			{
				ret = 1;
			}
		}
	}
	return ret;
}

int GeometricFuc::overlap(GeoPolyline *objPoly, GeoPolyline *comPoly, bool applyOption)
{
	if (applyOption)
	{
		return -1;
	}
	int ret = -1;
	
	if(!MBR::CheckOverlap(objPoly->m_mbr, comPoly->m_mbr))
	{
		return ret;
	}

	int i = 0, j = 0;
	int objPartIndex = 0;
	int obj2PartIndex = 0;

	for(j = 0; j < comPoly->NumPoints-1; j++)
	{
		if(comPoly->NumParts > obj2PartIndex)
		{
			if(comPoly->Parts[obj2PartIndex + 1] == j+1)
			{
				obj2PartIndex++;
				continue;
			}
		}

		double p1x = objPoly->Points[i].x;
		double p1y = objPoly->Points[i].y;
		double p2x = objPoly->Points[i+1].x;
		double p2y = objPoly->Points[i+1].y;
		double p3x = comPoly->Points[j].x;
		double p3y = comPoly->Points[j].y;
		double p4x = comPoly->Points[j+1].x;
		double p4y = comPoly->Points[j+1].y;

		int r123, r124;

		if(p1x > p2x)
		{
			swap(&p1x, &p2x);
			swap(&p1y, &p2y);
		}
		if(p3x > p4x)
		{
			swap(&p3x, &p4x);
			swap(&p3y, &p4y);
		}
	
		r123 = ccw(p1x, p1y, p2x, p2y, p3x, p3y);
		r124 = ccw(p1x, p1y, p2x, p2y, p4x, p4y);
		
		if(r123 == 0 && r124 == 0)
		{
			if(	p3x <= p1x && p1x <= p4x && min(p3y, p4y) <= p1y && max(p3y, p4y) >= p1y && 
				p3x <= p2x && p2x <= p4x && min(p3y, p4y) <= p2y && max(p3y, p4y) >= p2y)
			{
				if(i+1 == objPoly->NumPoints-1)
				{
					ret = 1;
					break;
				}
				else 
				{
					if(objPoly->NumParts > objPartIndex)
					{
						if(objPoly->Parts[objPartIndex + 1] == i+1)
						{
							objPartIndex++;
							i++;
						}
					}
					i++;
					ret = 0;
				}
			}
			else if(ret == 0)
			{
				ret = -1;
				break;
			}
		}
		else if(ret == 0)
		{
			ret = -1;
			break;
		}
	}
	return ret;
}

int GeometricFuc::overlap(GeoPolyline *objPoly, GeoPolygon *comPoly, bool applyOption)
{
	if (applyOption)
	{
		return -1;
	}
	int ret = -1;

	int i = 0, j = 0;
	for(i = 0; i < objPoly->NumPoints-1 && ret != 1; i++)
	{
		for(j = 0; j < comPoly->NumPoints-1 && ret != 1; j++)
		{
			ret = intersect_ccw(objPoly->Points[i].x, objPoly->Points[i].y, objPoly->Points[i+1].x, objPoly->Points[i+1].y,
								comPoly->Points[j].x, comPoly->Points[j].y, comPoly->Points[j+1].x, comPoly->Points[j+1].y);
		}
	}

	if(ret <= 0)
	{
		for(j = 0; j < comPoly->NumPoints-1 && ret <= 0; j++)
		{
			ret = inside(objPoly->Points[j].x, objPoly->Points[j].y, comPoly);
		}
	}
	return ret;
}

int GeometricFuc::overlap(GeoPolygon *objPoly, GeoPolygon *comPoly, bool applyOption)
{
	if (applyOption)
	{
		return -1;
	}

	int ret = -1;

	int i = 0, j = 0;
	for(i = 0; i < objPoly->NumPoints-1 && ret != 1; i++)
	{
		for(j = 0; j < comPoly->NumPoints-1 && ret != 1; j++)
		{
			ret = intersect_ccw(objPoly->Points[i].x, objPoly->Points[i].y, objPoly->Points[i+1].x, objPoly->Points[i+1].y,
								comPoly->Points[j].x, comPoly->Points[j].y, comPoly->Points[j+1].x, comPoly->Points[j+1].y);
		}
	}

	if(ret <= 0)
	{
		for(j = 0; j < comPoly->NumPoints-1 && ret <= 0; j++)
		{
			ret = inside(objPoly->Points[j].x, objPoly->Points[j].y, comPoly);
		}
	}
	return ret;
}

// -1 : Counterclockwise
//  0 : Straight.
//  1 : clockwise
int GeometricFuc::ccw(double ax, double ay, double bx, double by, double cx, double cy)
{
	double l;
	l = bx*cy - ay*bx - ax*cy 
		 - by*cx + ax*by + ay*cx;
	
	//l = bx*cy;
	//l -= ay*bx;
	//l -= ax*cy;
	//l -= by*cx;
	//l += ax*by;
	//l += ay*cx;

	if(l > 0)
		return 1; // cw Direction
	else if(l < 0)
		return -1; // ccw Direction
	else 
		return 0; // in a straight line,
}
	

//  -1 : outside
//  1 : inside
int GeometricFuc::inside(GeoPoint *point, GeoPolygon *poly, bool applyOption)
{
	if (applyOption)
	{
		return -1;
	}
	
	return inside(point->x, point->y, poly);
}

//  -1 : outside
//   1 : inside
int GeometricFuc::inside(double x, double y, GeoPolygon *poly, bool applyOption)
{
	if (applyOption)
	{
		return -1;
	}

	int i;
	int count = 0;
	int ic;
	count = 0;

	int lastExteriorIdx = 0;
	if(poly->NumParts > 1)
	{
		lastExteriorIdx = poly->Parts[1]-1;
	}
	else
	{
		lastExteriorIdx = poly->NumPoints-1;
	}

	int obj2PartIndex = 0;

	int entryIc = -1;

	int icList[100000];
	int listIndex = 0;
	for(i = 0; i < poly->NumPoints-1; i++)
	{
		if(poly->NumParts > obj2PartIndex+1)
		{
			if(poly->Parts[obj2PartIndex+1] == i+1)
			{
				obj2PartIndex++;
				continue;
			}
		}

		double max_px = max(max(poly->Points[i].x, poly->Points[i+1].x) + 1, x + 1);
		
		ic = intersect_ccw(x, y, max_px, y, poly->Points[i].x, poly->Points[i].y, poly->Points[i+1].x, poly->Points[i+1].y);
		icList[listIndex++] = ic;
		
		if(ic == -1)
		{
		}
		if(ic == 0)
		{
			if(x > min(poly->Points[i].x, poly->Points[i+1].x) )
			{
				count = 0;
				break;
			}
		}
		else if(ic==1)
		{
			count++;
		}
		else if(ic >= 20 || ic == 12)
		{
			count = 0; 
			break;
		}
		else if(ic == 10) // Go counterclockwise (enter/out)
		{
			if(entryIc == 11)
			{
				count++;
				entryIc = -1;
			}
			else if(lastExteriorIdx < i)
			{
				entryIc = ic;
			}
			else 
				entryIc = ic;
		}	
		else if(ic == 11) //Clockwise (entry/out)
		{
			if(entryIc == 10)
			{
				count++;
				entryIc = -1;
			}
			else if(lastExteriorIdx < i)
			{
				entryIc = ic;
			}
			else
				entryIc = ic;
		}
	}
	if(count & 1)
		return 1;
	else 
		return -1;
}

void swap(double *var1, double *var2)
{
	double temp = *var1;
	*var1 = *var2;
	*var2 = temp;
}