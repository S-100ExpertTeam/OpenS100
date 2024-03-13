#pragma once
#include "pugixml.hpp"

class xmlParseable {
public:
    virtual ~xmlParseable() = default;
    virtual void GetContents(pugi::xml_node& node) = 0;
    virtual void SaveXmlNode(pugi::xml_node& node) = 0;
};


template<typename T>
class XmlCollection {
     static_assert(std::is_base_of<xmlParseable, T>::value, "T must derive from xmlParseable");
private:
    std::vector<T> items;

public:
    size_t size() const {
        return items.size();
    }
    T& operator[](size_t index) {
        return items[index];
    }
    const T& operator[](size_t index) const {
        return items[index];
    }
    void push_back(const T& item) {
        items.push_back(item);
    }
    void erase(size_t index) {
        if (index < items.size()) {
            items.erase(items.begin() + index);
        }
    }

    typename std::vector<T>::iterator begin() {
        return items.begin();
    }
    typename std::vector<T>::const_iterator begin() const {
        return items.begin();
    }
    typename std::vector<T>::iterator end() {
        return items.end();
    }
    typename std::vector<T>::const_iterator end() const {
        return items.end();
    }

    void parseXmlNode(pugi::xml_node& node , std::string nodeName) {
        for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
        {
            const pugi::char_t* instructionName = instruction.name();
            if (!strcmp(instructionName, nodeName.c_str()))
            {
                T tmp;
                tmp.GetContents(instruction);
                items.push_back(tmp);
            }
            else
            {
                std::wstring content = pugi::as_wide(instructionName);
                content.append(L"is another data");
            }
        }
    }

    void SaveXmlNode(pugi::xml_node& node, std::string nodeName) {
        for (int i = 0; i < items.size(); i++)
        {
            auto temp = node.append_child(nodeName.c_str());
            items[i].SaveXmlNode(temp);
        }
    }
};