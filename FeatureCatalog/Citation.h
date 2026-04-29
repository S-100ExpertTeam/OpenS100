#pragma once
#include "XML_Item.h"
#include "Date.h"
#include "PresentationFormCode.h"
#include "ResponsibleParty.h"
#include "Series.h"

#include <pugixml.hpp>

#include <string>

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

	void SetAternateTitle(char* value);
	void SetAternateTitle(std::list<std::string>& value);
	std::list<std::string>& GetAternateTitle();

	void SetDate(Date* value);
	void SetDate(std::list<Date>& value);
	std::list<Date>& GetDate();

	void SetEdition(char* value);
	const std::string& GetEdition();

	void SetEditionDate(DateExt& value);
	DateExt& GetEditionDate();

	void SetIdentifier(char* value);
	const std::string& GetIdentifier();

	void SetIdentifierType(char* value);
	const std::string& GetIdentifierType();

	void SetCitedResponsibleParty(ResponsibleParty& value);
	ResponsibleParty& GetCitedResponsibleParty();

	void SetPresentationForm(PresentationFormCode& value);
	PresentationFormCode& GetPresentationForm();
	
	void SetSeries(Series& value);
	Series& GetSeries();

	void SetOtherCitationDetails(char* value);
	const std::string& GetOtherCitationDetails();

	void SetCollectiveTitle(char* value);
	const std::string& GetCollectiveTitle();

	void SetISBN(char* value);
	const std::string& GetISBN();

	void SetISSN(char* value);
	const std::string& GetISSN();
};