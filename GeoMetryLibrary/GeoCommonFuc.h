#pragma once

#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif

class MBR;

static double ex = M_PI / 180.0;
static double limit = -85.0;
static double M_PI_D4 = M_PI / 4.0;
static double EARTH_RADIUS = 6378.1;
static unsigned DOTPERCM = 38;

void projection(double &x, double &y);
void projection(float &x, float &y);
void projection(MBR& mbr);
void inverseProjection(double& x, double& y);
void inverseProjection(MBR& mbr);
unsigned getClippingLocation(const MBR *mbr, const double &x, const double &y);
