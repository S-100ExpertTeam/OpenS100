#pragma once
// Whole-number types of the device space, the library's own: nothing here is a type of an
// operating system.

// A rectangle of the device in pixels.
struct ScreenRect
{
	long left = 0;
	long top = 0;
	long right = 0;
	long bottom = 0;
};

// A point of two whole numbers: a pixel of the device, or a position in the whole-number
// coordinates of a dataset.
struct LongPoint
{
	long x = 0;
	long y = 0;
};
