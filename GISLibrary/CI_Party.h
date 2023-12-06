#pragma once

#include <memory>
#include <vector>
#include <string>
#include "CI_Contact.h"
#include "CI_Organisation.h"
#include "CI_Individual.h"
using namespace std;

namespace S100 {
    class CI_Party
    {
        CI_Organisation Organisation;
        CI_Individual Individual;

    public:
        void GetContents(pugi::xml_node& node);
    };
}
