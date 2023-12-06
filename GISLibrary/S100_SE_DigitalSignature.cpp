#include "stdafx.h"
#include "S100_SE_DigitalSignature.h"

namespace S100 {
    void S100_SE_DigitalSignature::GetContents(pugi::xml_node& node)
    {
        Id = node.attribute("id").as_string();
        CertificateRef = node.attribute("certificateRef").as_string();
        Value = node.child_value();
    }
}
