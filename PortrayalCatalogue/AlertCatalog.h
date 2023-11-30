#pragma once
#include <string>
#include "Messages.h"
#include "Highlights.h"
#include "Alerts.h"

using namespace std;

namespace S100
{
	class AlertCatalog
	{
    public:
		string Version;
		Messages messages;
		Highlights highlights;
		Alerts alerts;

        void AlertCatalog::Open(std::string filePath)
        {
            pugi::xml_document doc;
            pugi::xml_parse_result result = doc.load_file(filePath.c_str());
            pugi::xml_node nodeList = doc.first_child();

            const pugi::char_t* instructionName = nodeList.name();
            if (!strcmp(instructionName, "alertCatalog"))
            {
                GetContents(nodeList);
            }
            else
            {

            }
        }


        void AlertCatalog::GetContents(pugi::xml_node& node)
        {
            auto idAttri = node.attribute("version");
            if (idAttri != nullptr)
            {
                Version = idAttri.value();
            }

            for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
            {
                const pugi::char_t* instructionName = instruction.name();

                if (!strcmp(instructionName, "messages"))
                {
                    Messages ms;
                    ms.GetContents(instruction);
                    messages = ms;
                }
                else if (!strcmp(instructionName, "highlights"))
                {
                    Highlights hl;
                    hl.GetContents(instruction);
                    highlights = hl;
                }
                else if (!strcmp(instructionName, "alerts"))
                {
                    Alerts al;
                    al.GetContents(instruction);
                    alerts = al;
                }
            }
        }




	};

}