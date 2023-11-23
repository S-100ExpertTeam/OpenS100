#pragma once

#include "MD_CharacterSetCode.h"
#include "PT_Locale.h"

#include <memory>
#include <string>
#include "pugixml.hpp"




namespace S100
{
	inline std::string CharacterString(pugi::xml_node& node)
	{
		for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
		{
			const pugi::char_t* instructionName = instruction.name();

			if (!strcmp(instructionName, "gco:CharacterString"))
			{
				return instruction.child_value();
			}
		}
		return "";
	}

	class CT_Catalogue
	{
	public:
		std::string Name;
		std::shared_ptr<std::string> Scope;
		std::vector<std::string> FieldOfApplication;
		std::string VersionNumber;
		S100_Date VersionDate;
		std::shared_ptr<std::string>  Language;
		std::vector<std::shared_ptr<PT_Locale>> Locale;
		MD_CharacterSetCode CharacterSet;
	
	/*
		std::string getName() {
			return Name;
		}

		void setName(std::string& name) {
			Name = name;
		}

		std::shared_ptr<std::string> getScope() {
			return Scope;
		}

		void setScope(const std::string& scope) {
			Scope = std::make_shared<std::string>(scope);
		}

		std::vector<std::string> getFieldOfApplication() {
			return FieldOfApplication;
		}

		void setFieldOfApplication(std::string fieldOfApplication) {
			FieldOfApplication.push_back(fieldOfApplication);
		}

		std::string getVersionNumber() {
			return VersionNumber;
		}

		void setVersionNumber(std::string versionNumber) {
			VersionNumber = versionNumber;
		}*/

		/*time_t getVersionDate() {
			return VersionDate;
		}

		void setVersionDate(std::string date) {
			CString cstrDate(date.c_str());
			COleDateTime oleDateTime;
			if (oleDateTime.ParseDateTime(cstrDate, VAR_DATEVALUEONLY))
			{
				SYSTEMTIME st;
				if (oleDateTime.GetAsSystemTime(st))
				{
					tm tmDateTime;
					tmDateTime.tm_year = st.wYear - 1900;  
					tmDateTime.tm_mon = st.wMonth - 1;     
					tmDateTime.tm_mday = st.wDay;
					tmDateTime.tm_hour = st.wHour;
					tmDateTime.tm_min = st.wMinute;
					tmDateTime.tm_sec = st.wSecond;
					tmDateTime.tm_isdst = -1;

					VersionDate = mktime(&tmDateTime);
				}
			}
		}*/

		inline std::shared_ptr<std::string>  getLanguage() {
			return Language;
		}

		inline void setLanguage(std::string language) {
			Language = std::make_shared<std::string>(language);
		}


		inline std::vector<std::shared_ptr<PT_Locale>> getLocale() {
			return Locale;
		}

		inline void setLocale(pugi::xml_node& node) {
			auto loc = std::make_shared<PT_Locale>();
			loc->GetContents(node);
			Locale.push_back(loc);
		}
	};
}