#include "stdafx.h"
#include "ColorMapCalculator.h"


nmColorMap::ColorMapCalculator::ColorMapCalculator()
{
}


nmColorMap::ColorMapCalculator::~ColorMapCalculator()
{
}


nmColorMap::ColorMap nmColorMap::ColorMapCalculator::Calculate(ColorMap _colorMap, int _num)
{
	if ((_colorMap.GetColorCount() < 2) || 
		(_num < 2)) {
		ColorMap ret;
		return ret;
	}

	ColorMap input = _colorMap;
	ColorMap output;

	for (int i = 0; i < _num; i++) {
		Color color;
		output.AddColor(color);
	}

	int cntColor = _colorMap.GetColorCount(); // 기존 컬러맵의 색 개수

	double* percentIndicator = new double[cntColor];

	for (int i = 0; i < cntColor; i++) {
		percentIndicator[i] = (double)i / (cntColor - 1);
	}

	for (int i = 0; i < _num; i++) {
		double percent = (double)i / (_num - 1);
		int color1 = 0;
		int color2 = 0;
		for (int j = 0; j < cntColor - 1; j++) {
			if (percent >= percentIndicator[j]) {
				color1 = j;
				color2 = j + 1;
			}
		}
		double newPercent = (percent - percentIndicator[color1]) / ((double)1 / (cntColor - 1));

		output.color[i] = Calculate(_colorMap.color[color1], _colorMap.color[color2], newPercent);
	}

	delete percentIndicator;

	return output;
}


nmColorMap::Color nmColorMap::ColorMapCalculator::Calculate(Color _color1, Color _color2, double _percent)
{
	Color ret;
	int r = (int)(_color1.R() + ((_color2.R() - _color1.R()) * _percent));
	int g = (int)(_color1.G() + ((_color2.G() - _color1.G()) * _percent));
	int b = (int)(_color1.B() + ((_color2.B() - _color1.B()) * _percent));
	
	ret.Set(r, g, b);
	return ret;
}