#pragma once
#include "XML_Item.h"
#include "Date.h"
#include "PresentationFormCode.h"
#include "ResponsibleParty.h"
#include "Series.h"

#include <pugixml.hpp>

#include <string>
#include "StringUtil.h"

class Citation : public XML_Item
{

public:
	Citation();
	virtual ~Citation();

private:
	std::string title = "";
	std::list<std::string> aternateTitle;
	std::list<Date> date;
	std::string edition = "";
	DateExt editionDate;
	std::string identifier = "";
	std::string identifierType = "";
	ResponsibleParty citedResponsibleParty;
	PresentationFormCode presentationForm;
	Series series;
	std::string otherCitationDetails = "";
	std::string collectiveTitle = "";
	std::string ISBN = "";
	std::string ISSN = "";

public:
	void GetContents(pugi::xml_node& node);

	void SetTitle(char* value);
	const std::string& GetTitle();
	std::wstring GetTitleW();

	void SetAternateTitle(char* value);
	void SetAternateTitle(std::list<std::string>& value);
	std::list<std::string>& GetAternateTitle();

	void SetDate(Date* value);
	void SetDate(std::list<Date>& value);
	std::list<Date>& GetDate();

	void SetEdition(char* value);
	const std::string& GetEdition();
	std::wstring GetEditionW();

	void SetEditionDate(DateExt& value);
	DateExt& GetEditionDate();

	void SetIdentifier(char* value);
	const std::string& GetIdentifier();
	std::wstring GetIdentifierW();

	void SetIdentifierType(char* value);
	const std::string& GetIdentifierType();
	std::wstring GetIdentifierTypeW();

	void SetCitedResponsibleParty(ResponsibleParty& value);
	ResponsibleParty& GetCitedResponsibleParty();

	void SetPresentationForm(PresentationFormCode& value);
	PresentationFormCode& GetPresentationForm();
	
	void SetSeries(Series& value);
	Series& GetSeries();

	void SetOtherCitationDetails(char* value);
	const std::string& GetOtherCitationDetails();
	std::wstring GetOtherCitationDetailsW();

	void SetCollectiveTitle(char* value);
	const std::string& GetCollectiveTitle();
	std::wstring GetCollectiveTitleW();

	void SetISBN(char* value);
	const std::string& GetISBN();
	std::wstring GetISBNW();

	void SetISSN(char* value);
	const std::string& GetISSN();
	std::wstring GetISSNW();
};
