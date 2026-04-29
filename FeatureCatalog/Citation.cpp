#include "stdafx.h"
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
			Date* date = new Date();
			date->GetContents(instruction);
			SetDate(date);
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
			Series* series = new Series();
			series->GetContents(instruction);
			SetSeries(*series);
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

void Citation::SetTitle(char* value) 
{
	title =value;
}

const std::string& Citation::GetTitle()
{
	return title;
}

void Citation::SetAternateTitle(char* value) 
{
	aternateTitle.push_back(value);
}

void Citation::SetAternateTitle(std::list<std::string>& value)
{
	aternateTitle = value;
}

std::list<std::string>& Citation::GetAternateTitle()
{
	return aternateTitle;
}

void Citation::SetDate(Date* value)
{
	date.push_back(*value);
}
void Citation::SetDate(std::list<Date>& value)
{
	date = value;
}

std::list<Date>& Citation::GetDate()
{
	return date;
}

void Citation::SetEdition(char* value)
{
	edition =value;
}

const std::string& Citation::GetEdition()
{	
	return edition;
}

void Citation::SetEditionDate(DateExt& value)
{
	editionDate = value;
}

DateExt& Citation::GetEditionDate()
{
	return editionDate;
}

void Citation::SetIdentifier(char* value)
{
	identifier =value;
}

const std::string& Citation::GetIdentifier()
{
	return identifier;
}

void Citation::SetIdentifierType(char* value)
{
	identifierType = value;
}

const std::string& Citation::GetIdentifierType()
{
	return identifierType;
}

void Citation::SetCitedResponsibleParty(ResponsibleParty& value)
{
	citedResponsibleParty = value;
}

ResponsibleParty& Citation::GetCitedResponsibleParty()
{
	return citedResponsibleParty;
}

void Citation::SetPresentationForm(PresentationFormCode& value)
{
	presentationForm = value; 
}

PresentationFormCode& Citation::GetPresentationForm()
{
	return presentationForm;
}

void Citation::SetSeries(Series& value)
{
	series = value;
}

Series& Citation::GetSeries()
{
	return series;
}

void Citation::SetOtherCitationDetails(char* value)
{
	otherCitationDetails =value;
}

const std::string& Citation::GetOtherCitationDetails()
{
	return otherCitationDetails;
}

void Citation::SetCollectiveTitle(char* value)
{
	collectiveTitle = value;
}

const std::string& Citation::GetCollectiveTitle()
{
	return collectiveTitle;
}

void Citation::SetISBN(char* value) 
{
	ISBN = value;
}

const std::string& Citation::GetISBN()
{
	return ISBN;
}

void Citation::SetISSN(char* value) 
{
	ISSN = value;
}

const std::string& Citation::GetISSN()
{
	return ISSN;
}