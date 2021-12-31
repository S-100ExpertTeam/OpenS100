#pragma once
#include "geometry.h"
#include "ENCCommon.h"

#include <list>

class CSymbolManager;
class Symbol;
class ENCArea;
class GeoPolyline;
class ENCPoint;
class ENC_Feature;
class TEParameters;
class TXParameters;
class CGeoObjectMap;
class CAttributeMap;
class CColorMap;
class CSymbolMap;
class CSymbol;

//struct CObjectFindStruct;

namespace S100_SVG_D2D1_DLL
{
	class SVGManager;
	class SVG;
}

class ENCGeometry :
	public Geometry
{
public:
	ENCGeometry();
	virtual ~ENCGeometry();

public:
	/////////////////////////////////////
	// type  1 : DAI
	// type  2 : SVG
	static CGeoObjectMap	*pObject;
	static CAttributeMap	*pAttribute;
	static CColorMap		*pColor;
	static CSymbolMap		*pSymbol;	// Allocation 5 block
										//////////////////////////////////////////
										// CSymbolMap 0 : simpointsymbol.dic
										// - Symplified point symbol (ENC symbol) = point
										// CSymbolMap 1 : pacpointsymbol.dic
										// - Paper chart symbols (Paper)   = point
										// CSymbolMap 2 : linesymbol.dic
										// - Line symbole                  = line
										// CSymbolMap 3 : plainboun.dic
										// - Plain boundaries (ENC symbol) = area
										// CSymbolMap 4 : symboun.dic
										// - Symbolized boundaries (Paper) = area
										//////////////////////////////////////////

	ENC_Feature *m_pFeature;
	int m_simplePLIndex;
	int m_paperPLIndex;
	S100_SVG_D2D1_DLL::SVG* m_svgSymbol;

protected:
	unsigned m_scaleMin;

public:
	void SetPLIndex(int simple, int paper);
	void SetScaleMin(unsigned value);
};