#pragma once
enum TypeOfDrawingInstruction
{
	nullInstruction = 0,
	pointInstruction = 1,
	lineInstruction = 2,
	areaInstruction = 3,
	coverageInstruction = 4,
	textInstruction = 5,
	augmentedPoint = 6,
	augmentedRay = 7, 
	augmentedPath = 8, 
	augmentedArea = 9,
	patternAreaInstruction = 10
};