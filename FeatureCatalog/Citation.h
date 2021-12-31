#pragma once
#include "XML_Item.h"
#include "Date.h"
#include "PresentationFormCode.h"
#include "ResponsibleParty.h"
#include "Series.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

#include <string>

class Citation : public XML_Item
{

public:
	Citation();
	virtual ~Citation();

private:
	std::wstring title = L"";
	std::list<std::wstring> aternateTitle;
	std::list<Date> date;
	std::wstring edition = L"";
	DateExt editionDate;
	std::wstring identifier = L"";
	std::wstring identifierType = L"";
	ResponsibleParty citedResponsibleParty;
	PresentationFormCode presentationForm;
	Series series;
	std::wstring otherCitationDetails = L"";
	std::wstring collectiveTitle = L"";
	std::wstring ISBN = L"";
	std::wstring ISSN = L"";

public:
	void GetContents(pugi::xml_node& node);

	void SetTitle(char* value);
	const std::wstring& GetTitle();

	void SetAternateTitle(char* value);
	void SetAternateTitle(std::list<std::wstring>& value);
	std::list<std::wstring>& GetAternateTitle();

	void SetDate(Date* value);
	void SetDate(std::list<Date>& value);
	std::list<Date>& GetDate();

	void SetEdition(char* value);
	const std::wstring& GetEdition();

	void SetEditionDate(DateExt& value);
	DateExt& GetEditionDate();

	void SetIdentifier(char* value);
	const std::wstring& GetIdentifier();

	void SetIdentifierType(char* value);
	const std::wstring& GetIdentifierType();

	void SetCitedResponsibleParty(ResponsibleParty& value);
	ResponsibleParty& GetCitedResponsibleParty();

	void SetPresentationForm(PresentationFormCode& value);
	PresentationFormCode& GetPresentationForm();
	
	void SetSeries(Series& value);
	Series& GetSeries();

	void SetOtherCitationDetails(char* value);
	const std::wstring& GetOtherCitationDetails();

	void SetCollectiveTitle(char* value);
	const std::wstring& GetCollectiveTitle();

	void SetISBN(char* value);
	const std::wstring& GetISBN();

	void SetISSN(char* value);
	const std::wstring& GetISSN();
};