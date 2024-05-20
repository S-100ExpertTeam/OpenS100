#pragma once
class S100GML_NameSpace
{
public:
    S100GML_NameSpace() {};
    ~S100GML_NameSpace() {};
public:
    // for namespaces
    std::unordered_map<std::string, std::string> namespaces;
    std::string namespace_s100;
    std::string namespace_xlink;
    std::string namespace_gml;
    std::string namespace_product;
};

