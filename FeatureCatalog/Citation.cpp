#include "stdafx.h"
#include "StringUtil.h"
#include "Citation.h"

Citation::Citation()
{

}

Citation::~Citation()
{

}

void Citation::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const char* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100CI:title"))
		{
			SetTitle((char*)instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:aternateTitle"))
		{
			SetAternateTitle((char*)instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:date"))
		{
			Date* _date = new Date();
			_date->GetContents(instruction);
			SetDate(_date);
		}
		else if (!strcmp(instructionName, "S100CI:edition"))
		{
			SetEdition((char*)instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:editionDate"))
		{
			DateExt* dateext = new DateExt();
			dateext->GetContents(instruction);
			SetEditionDate(*dateext);
		}
		else if (!strcmp(instructionName, "S100CI:identifier"))
		{
			SetIdentifier((char*)instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:identifierType"))
		{
			SetIdentifierType((char*)instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:citedResponsibleParty"))
		{
			ResponsibleParty* party = new ResponsibleParty();
			party->GetContents(instruction);
			SetCitedResponsibleParty(*party);
		}
		else if (!strcmp(instructionName, "S100CI:presentationForm"))
		{
			PresentationFormCode* code = new PresentationFormCode();
			code->GetContents(instruction);
			SetPresentationForm(*code);
		}
		else if (!strcmp(instructionName, "S100CI:series"))
		{
			Series* _series = new Series();
			_series->GetContents(instruction);
			SetSeries(*_series);
		}
		else if (!strcmp(instructionName, "S100CI:otherCitationDetails"))
		{
			SetOtherCitationDetails((char*)instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:collectiveTitle"))
		{
			SetCollectiveTitle((char*)instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:ISBN"))
		{
			SetISBN((char*)instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:ISSN"))
		{
			SetISSN((char*)instruction.child_value());
		}
		else  //case not fixed value
		{
			std::string content = instructionName;
			content.append(" is another data");
		}
	}
}

void Citation::SetTitle(char* _value) 
{
	title =_value;
}

const std::string& Citation::GetTitle()
{
	return title;
}

void Citation::SetAternateTitle(char* _value) 
{
	aternateTitle.push_back(_value);
}

void Citation::SetAternateTitle(std::list<std::string>& _value)
{
	aternateTitle = _value;
}

std::list<std::string>& Citation::GetAternateTitle()
{
	return aternateTitle;
}

void Citation::SetDate(Date* _value)
{
	date.push_back(*_value);
}
void Citation::SetDate(std::list<Date>& _value)
{
	date = _value;
}

std::list<Date>& Citation::GetDate()
{
	return date;
}

void Citation::SetEdition(char* _value)
{
	edition =_value;
}

const std::string& Citation::GetEdition()
{	
	return edition;
}

void Citation::SetEditionDate(DateExt& _value)
{
	editionDate = _value;
}

DateExt& Citation::GetEditionDate()
{
	return editionDate;
}

void Citation::SetIdentifier(char* _value)
{
	identifier =_value;
}

const std::string& Citation::GetIdentifier()
{
	return identifier;
}

void Citation::SetIdentifierType(char* _value)
{
	identifierType = _value;
}

const std::string& Citation::GetIdentifierType()
{
	return identifierType;
}

void Citation::SetCitedResponsibleParty(ResponsibleParty& _value)
{
	citedResponsibleParty = _value;
}

ResponsibleParty& Citation::GetCitedResponsibleParty()
{
	return citedResponsibleParty;
}

void Citation::SetPresentationForm(PresentationFormCode& _value)
{
	presentationForm = _value; 
}

PresentationFormCode& Citation::GetPresentationForm()
{
	return presentationForm;
}

void Citation::SetSeries(Series& _value)
{
	series = _value;
}

Series& Citation::GetSeries()
{
	return series;
}

void Citation::SetOtherCitationDetails(char* _value)
{
	otherCitationDetails =_value;
}

const std::string& Citation::GetOtherCitationDetails()
{
	return otherCitationDetails;
}

void Citation::SetCollectiveTitle(char* _value)
{
	collectiveTitle = _value;
}

const std::string& Citation::GetCollectiveTitle()
{
	return collectiveTitle;
}

void Citation::SetISBN(char* _value) 
{
	ISBN = _value;
}

const std::string& Citation::GetISBN()
{
	return ISBN;
}

void Citation::SetISSN(char* _value) 
{
	ISSN = _value;
}

const std::string& Citation::GetISSN()
{
	return ISSN;
}

std::wstring Citation::GetTitleW()
{
	return toWide(GetTitle());
}

std::wstring Citation::GetEditionW()
{
	return toWide(GetEdition());
}

std::wstring Citation::GetIdentifierW()
{
	return toWide(GetIdentifier());
}

std::wstring Citation::GetIdentifierTypeW()
{
	return toWide(GetIdentifierType());
}

std::wstring Citation::GetOtherCitationDetailsW()
{
	return toWide(GetOtherCitationDetails());
}

std::wstring Citation::GetCollectiveTitleW()
{
	return toWide(GetCollectiveTitle());
}

std::wstring Citation::GetISBNW()
{
	return toWide(GetISBN());
}

std::wstring Citation::GetISSNW()
{
	return toWide(GetISSN());
}
