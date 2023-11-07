#pragma once
#include "PT_Locale.h"
#include "MD_CharacterSetCode.h"

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
	private:
		std::string Name;
		std::string* Scope = nullptr;
		std::vector<std::string> FieldOfApplication;
		std::string VersionNumber;
		time_t VersionDate;
		std::string* Language;
		std::vector<PT_Locale*> Locale;
		MD_CharacterSetCode* CharacterSet;

	public:

		std::string getName() {
			return Name;
		}

		void setName(std::string& name) {
			Name = name;
		}

		std::string* getScope() {
			return Scope;
		}

		void setScope(std::string scope) {
			Scope = new std::string(scope);
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
		}

		time_t getVersionDate() {
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
		}

		std::string* getLanguage() {
			return Language;
		}

		void setLanguage(std::string language) {
			Language = new std::string(language);
		}


		std::vector<PT_Locale*> getLocale() {
			return Locale;
		}

		void setLocale(pugi::xml_node& node) {
			PT_Locale* loc = new PT_Locale();
			loc->GetContents(node);
			Locale.push_back(loc);
		}



	};

}

