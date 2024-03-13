#include "stdafx.h"
#include "CI_Contact.h"

namespace S100
{
    void CI_Contact::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "cit:phone"))
            {
                CI_Telephone tp;
                tp.GetContents(instruction);
                Phone.push_back(tp);
            }
            else if (!strcmp(instructionName, "cit:address"))
            {
                CI_Address ad;
                ad.GetContents(instruction);
                Address.push_back(ad);
            }
            else if (!strcmp(instructionName, "cit:onlineResource"))
            {
                CI_OnlineResource or ;
                or .GetContents(instruction);
                OnlineResource.push_back(or );
            }
            else if (!strcmp(instructionName, "cit:hoursOfService"))
            {
                HoursOfService.push_back(instruction.child_value());
            }
            else if (!strcmp(instructionName, "cit:contactInstructions"))
            {
                ContactInstructions = std::make_shared<std::string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "cit:contactType"))
            {
                ContactType = std::make_shared<std::string>(instruction.child_value());
            }
        }
    }

    void CI_Contact::Save(pugi::xml_node& node)
    {
        if (!Phone.empty())
        {
            for (int i = 0; i < Phone.size(); i++)
                Phone[i].Save(node,"cit:phone");
        }
        if (!Address.empty())
        {
            for (int i = 0; i < Address.size(); i++)
                Address[i].Save(node, "cit:address");
        }
        if (!OnlineResource.empty())
        {
            for (int i = 0; i < OnlineResource.size(); i++)
                OnlineResource[i].Save(node, "cit:onlineResource");
        }
        if (!HoursOfService.empty())
        {
            for (int i = 0; i < HoursOfService.size(); i++)
            {
                auto child= node.append_child("cit:hoursOfService");
                child.text().set(HoursOfService[i].c_str());
            }
        }
        if (ContactInstructions)
        {
            auto child = node.append_child("cit:contactInstructions");
            child.text().set(ContactInstructions->c_str());
        }
        if (ContactType)
        {
            auto child = node.append_child("cit:contactType");
            child.text().set(ContactType->c_str());
        }
    }
}