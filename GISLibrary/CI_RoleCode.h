#pragma once
#include <string>
#include <stdexcept>

namespace S100 {
    enum class CI_RoleCode
    {
        author = 1,
        coAuthor,
        collaborator,
        contributor,
        custodian,
        distributor,
        editor,
        funder,
        mediator,
        originator,
        owner,
        pointOfContact,
        principalInvestigator,
        processor,
        publisher,
        resourceProvider,
        rightsHolder,
        sponsor,
        stakeholder,
        user
    };

    std::string CI_RoleCodeToString(CI_RoleCode category);
    CI_RoleCode CI_RoleCodeFromString(const std::string& categoryName);
}
