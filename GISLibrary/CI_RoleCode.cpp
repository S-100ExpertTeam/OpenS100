#include "stdafx.h"
#include <stdexcept>
#include "CI_RoleCode.h"

namespace S100 {

    std::string CI_RoleCodeToString(CI_RoleCode category) {
        switch (category) {
        case CI_RoleCode::author: return "author";
        case CI_RoleCode::coAuthor: return "coAuthor";
        case CI_RoleCode::collaborator: return "collaborator";
        case CI_RoleCode::contributor: return "contributor";
        case CI_RoleCode::custodian: return "custodian";
        case CI_RoleCode::distributor: return "distributor";
        case CI_RoleCode::editor: return "editor";
        case CI_RoleCode::funder: return "funder";
        case CI_RoleCode::mediator: return "mediator";
        case CI_RoleCode::originator: return "originator";
        case CI_RoleCode::owner: return "owner";
        case CI_RoleCode::pointOfContact: return "pointOfContact";
        case CI_RoleCode::principalInvestigator: return "principalInvestigator";
        case CI_RoleCode::processor: return "processor";
        case CI_RoleCode::publisher: return "publisher";
        case CI_RoleCode::resourceProvider: return "resourceProvider";
        case CI_RoleCode::rightsHolder: return "rightsHolder";
        case CI_RoleCode::sponsor: return "sponsor";
        case CI_RoleCode::stakeholder: return "stakeholder";
        case CI_RoleCode::user: return "user";
        default: throw std::invalid_argument("Unknown category");
        }
    }

    CI_RoleCode CI_RoleCodeFromString(const std::string& categoryName) {
        if (categoryName == "author") return CI_RoleCode::author;
        if (categoryName == "coAuthor") return CI_RoleCode::coAuthor;
        if (categoryName == "collaborator") return CI_RoleCode::collaborator;
        if (categoryName == "contributor") return CI_RoleCode::contributor;
        if (categoryName == "custodian") return CI_RoleCode::custodian;
        if (categoryName == "distributor") return CI_RoleCode::distributor;
        if (categoryName == "editor") return CI_RoleCode::editor;
        if (categoryName == "funder") return CI_RoleCode::funder;
        if (categoryName == "mediator") return CI_RoleCode::mediator;
        if (categoryName == "originator") return CI_RoleCode::originator;
        if (categoryName == "owner") return CI_RoleCode::owner;
        if (categoryName == "pointOfContact") return CI_RoleCode::pointOfContact;
        if (categoryName == "principalInvestigator") return CI_RoleCode::principalInvestigator;
        if (categoryName == "processor") return CI_RoleCode::processor;
        if (categoryName == "publisher") return CI_RoleCode::publisher;
        if (categoryName == "resourceProvider") return CI_RoleCode::resourceProvider;
        if (categoryName == "rightsHolder") return CI_RoleCode::rightsHolder;
        if (categoryName == "sponsor") return CI_RoleCode::sponsor;
        if (categoryName == "stakeholder") return CI_RoleCode::stakeholder;
        if (categoryName == "user") return CI_RoleCode::user;
        throw std::invalid_argument("Unknown category name");
    }
}