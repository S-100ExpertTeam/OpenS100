#include "stdafx.h"
#include "S100_SE_DigitalSignature.h"

namespace S100 {
    void S100_SE_DigitalSignature::GetContents(pugi::xml_node& node)
    {
        Id = node.attribute("id").as_string();
        CertificateRef = node.attribute("certificateRef").as_string();
        Value = node.child_value();
    }

    void S100_SE_DigitalSignature::Save(pugi::xml_node& node)
    {
        if (!Id.empty() || !CertificateRef.empty() || !Value.empty())
        {
            auto child = node.append_child("S100SE:S100_SE_DigitalSignature");
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
