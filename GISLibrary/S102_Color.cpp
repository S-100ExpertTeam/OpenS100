#include "stdafx.h"
#include "S102_Color.h"
#include "GISLibrary.h"

S102_Color::S102_Color()
{

}

S102_Color::~S102_Color()
{
	
}

void S102_Color::setColor()
{
	auto s101PC = gisLib->catalogManager.getPC(101);
	if (s101PC) {
		auto colorProfile = s101PC->GetS100PCManager()->GetS100ColorProfile();
		DEPDW[0] = colorProfile->GetColor(L"Day", L"DEPDW");
		DEPMD[0] = colorProfile->GetColor(L"Day", L"DEPMD");
		DEPMS[0] = colorProfile->GetColor(L"Day", L"DEPMS");
		DEPVS[0] = colorProfile->GetColor(L"Day", L"DEPVS");
		DEPIT[0] = colorProfile->GetColor(L"Day", L"DEPIT");

		DEPDW[1] = colorProfile->GetColor(L"Dusk", L"DEPDW");
		DEPMD[1] = colorProfile->GetColor(L"Dusk", L"DEPMD");
		DEPMS[1] = colorProfile->GetColor(L"Dusk", L"DEPMS");
		DEPVS[1] = colorProfile->GetColor(L"Dusk", L"DEPVS");
		DEPIT[1] = colorProfile->GetColor(L"Dusk", L"DEPIT");

		DEPDW[2] = colorProfile->GetColor(L"Night", L"DEPDW");
		DEPMD[2] = colorProfile->GetColor(L"Night", L"DEPMD");
		DEPMS[2] = colorProfile->GetColor(L"Night", L"DEPMS");
		DEPVS[2] = colorProfile->GetColor(L"Night", L"DEPVS");
		DEPIT[2] = colorProfile->GetColor(L"Night", L"DEPIT");

		bSet = true;
	}
}

bool S102_Color::isSet()
{
	return bSet;
}