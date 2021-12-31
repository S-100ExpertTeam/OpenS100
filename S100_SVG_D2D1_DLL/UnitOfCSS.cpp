#include "stdafx.h"
#include "UnitOfCSS.h"
#include "CSSStrokeLinecap.h"
#include "CSSStrokeLinejoin.h"
#include "CSSDisplay.h"
#include "CSSFill.h"
#include "CSSStroke.h"
#include "CSSStrokeWidth.h"
#include "CSSOpacity.h"

namespace S100_SVG_D2D1_DLL
{
	UnitOfCSS::UnitOfCSS() 
	{

	}

	UnitOfCSS::~UnitOfCSS()
	{
		std::list<CSSStyle*>::iterator itor;
		for (itor = styles.begin(); itor != styles.end(); itor++)
		{

			switch ((*itor)->styleType)
			{
			case 1:
				delete (CSSFill*)(*itor);
				break;
			case 2:
				delete (CSSStroke*)(*itor);
				break;
			case 3:
				delete (CSSStrokeWidth*)(*itor);
				break;
			case 4:
				delete (CSSStrokeLinecap*)(*itor);
				break;
			case 5:
				delete (CSSStrokeLinejoin*)(*itor);
				break;
			case 6:
				delete (CSSDisplay*)(*itor);
				break;
			case 7:
				delete (CSSOpacity*)(*itor);
				break;
			}
		}
	}
}