#include "stdafx.h"
#include "CT_Catalogue.h"
#include "CI_Address.h"

namespace S100
{
    void CI_Address::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "cit:deliveryPoint"))
            {
                Deliverypoint = GetCharacterString(instruction);
            }
            else if (!strcmp(instructionName, "cit:city"))
            {
                City = GetCharacterString(instruction);
            }
            else if (!strcmp(instructionName, "cit:administrativeArea"))
            {
                Administrativearea = GetCharacterString(instruction);
            }
            else if (!strcmp(instructionName, "cit:postalCode"))
            {
                Postalcode = GetCharacterString(instruction);
            }
            else if (!strcmp(instructionName, "cit:country"))
            {
                Country = GetCharacterString(instruction);
            }
            else if (!strcmp(instructionName, "cit:electronicMailAddress"))
            {
                Electronicmailaddress.push_back(GetCharacterString(instruction));
            }
            else
            {
                std::wstring content = pugi::as_wide(instructionName);
                content.append(L"is another data");
            }
        }
    }

    void CI_Address::Save(pugi::xml_node& node, std::string nodeName)
    {
        if (!Deliverypoint.empty()|| !City.empty()|| !Administrativearea.empty()|| !Postalcode.empty()|| !Country.empty() || !Electronicmailaddress.empty())
        {
            auto Node = node.append_child(nodeName.c_str());
            
            if (!Deliverypoint.empty())
            {
                auto child = Node.append_child("cit:deliveryPoint");
                SaveCharacterString(child, Deliverypoint);
            }
            if (!City.empty())
            {
                auto child = Node.append_child("cit:city");
                SaveCharacterString(child, City);
            }
            if (!Administrativearea.empty())
            {
                auto child = Node.append_child("cit:Administrativearea");
                SaveCharacterString(child, Administrativearea);
            }
            if (!Postalcode.empty())
            {
                auto child = Node.append_child("cit:Postalcode");
                SaveCharacterString(child, Postalcode);
            }
            if (!Country.empty())
            {
                auto child = Node.append_child("cit:Country");
                SaveCharacterString(child, Country);
            }
            if (!Electronicmailaddress.empty())
            {
                auto child = Node.append_child("cit:electronicMailAddress");
                for (auto item : Electronicmailaddress)
                    SaveCharacterString(child, item);
            }
        }
    }
}
