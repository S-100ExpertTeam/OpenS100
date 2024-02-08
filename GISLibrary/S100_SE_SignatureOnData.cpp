#include "stdafx.h"
#include "S100_SE_SignatureOnData.h"

namespace S100 {
    void S100_SE_SignatureOnData::GetContents(pugi::xml_node& node)
    {
        Value = node.child_value();

        Id = node.attribute("id").as_string();
        CertificateRef = node.attribute("certificateRef").as_string();
        DataStatus = DataStatusFromString(node.attribute("dataStatus").as_string());
    }

    void S100_SE_SignatureOnData::Save(pugi::xml_node& node)
    {
        if (!Id.empty() || !CertificateRef.empty() || !Value.empty())
        {
            auto child = node.append_child("S100SE:S100_SE_SignatureOnData");
            if (!Id.empty())
            {
                auto childValue = child.append_attribute("id");
                childValue.set_value(Id.c_str());
            }
            if (!CertificateRef.empty())
            {
                auto childValue = child.append_attribute("certificateRef");
                childValue.set_value(CertificateRef.c_str());
            }
            if (!Value.empty())
            {
                child.text().set(Value.c_str());
            }
        }
    }
}
