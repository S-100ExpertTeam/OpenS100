#include "stdafx.h"
#include "Text.h"
#include "TextElement.h"

TextPackage::Text::Text()
{

}

TextPackage::Text::~Text()
{
	for (auto i = element.begin(); i != element.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}
}