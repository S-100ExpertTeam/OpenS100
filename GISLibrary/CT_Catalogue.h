#pragma once

namespace S100
{
	class CT_Catalogue
	{
    private:
		std::string Name;
		std::string* Scope = nullptr;
		std::vector<std::string> FieldOfApplication;
		std::string VersionNumber;
		time_t VersionDate;
		std::string* Language;
		//Locale;
		//CharacterSet;



    public:
		std::string CharacterString(pugi::xml_node& node)
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


        std::string getName()  {
            return Name;
        }

        void setName( std::string& name) {
            Name = name;
        }

        std::string* getScope()  {
            return Scope;
        }

        void setScope(std::string scope) {
            Scope = new std::string(scope);
        }

        std::vector<std::string> getFieldOfApplication() {
            return FieldOfApplication;
        }

        void setFieldOfApplication( std::string fieldOfApplication) {
            FieldOfApplication.push_back(fieldOfApplication);
        }

        std::string getVersionNumber()  {
            return VersionNumber;
        }

        void setVersionNumber( std::string versionNumber) {
            VersionNumber = versionNumber;
        }

        time_t getVersionDate()  {
            return VersionDate;
        }

        void setVersionDate(time_t date) {

            VersionDate = date;
        }

        std::string* getLanguage()  {
            return Language;
        }

        void setLanguage( char* language) {
            if (Language) {
                *Language = std::string(language);
            }
        }

	};

}

