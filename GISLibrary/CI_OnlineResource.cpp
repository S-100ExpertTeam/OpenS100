#include "stdafx.h"
#include "CI_OnlineResource.h"

namespace S100
{
    void CI_OnlineResource::GetContents(pugi::xml_node& node)
    {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();

            if (!strcmp(instructionName, "cit:linkage"))
            {
                Linkage = instruction.child_value();
            }
            else if (!strcmp(instructionName, "cit:protocol"))
            {
                Protocol = std::make_shared<std::string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "cit:applicationProfile"))
            {
                ApplicationProfile = std::make_shared<std::string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "cit:name"))
            {
                Name = std::make_shared<std::string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "cit:description"))
            {
                Description = std::make_shared<std::string>(instruction.child_value());
            }
            else if (!strcmp(instructionName, "cit:function"))
            {
                Function = std::make_shared<CI_OnLineFunctionCode>(CI_OnLineFunctionCodeFromString(instruction.child_value()));
            }
            else if (!strcmp(instructionName, "cit:protocolRequest"))
            {
                ProtocolRequest = std::make_shared<std::string>(instruction.child_value());
            }
        }
    }

    void CI_OnlineResource::Save(pugi::xml_node& node,std::string  nodeName)
    {
        auto Node = node.append_child(nodeName.c_str());
        if (!Linkage.empty())
        {
            auto child = Node.append_child("cit:linkage");
            child.text().set(Linkage.c_str());
        }
        if (Protocol)
        {
            auto child = Node.append_child("cit:protocol");
            child.text().set(Protocol->c_str());
        }
        if (ApplicationProfile)
        {
            auto child = Node.append_child("cit:applicationProfile");
            child.text().set(ApplicationProfile->c_str());
        }
        if (Name)
        {
            auto child = Node.append_child("cit:name");
            child.text().set(Name->c_str());
        }
        if (Description)
        {
            auto child = Node.append_child("cit:description");
            child.text().set(Description->c_str());
        }
        if (Function)
        {
            auto child = Node.append_child("cit:function");
            child.text().set(CI_OnLineFunctionCodeToString(*Function).c_str());
        }
        if (ProtocolRequest)
        {
            auto child = Node.append_child("cit:protocolRequest");
            child.text().set(ProtocolRequest->c_str());
        }
       
    }
}
