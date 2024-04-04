#pragma once

#include "Field.h"

#include <list>

struct TagPair
{
    char tag1[5] = { 0, };
    char tag2[5] = { 0, };
};

class F_FieldControlField :
    public Field
{
public:
    F_FieldControlField();
    F_FieldControlField(const F_FieldControlField& other);
    virtual ~F_FieldControlField();

public:
    unsigned char byte[10];

    std::list<TagPair> tagPairs;

public:
    virtual F_FieldControlField operator=(const F_FieldControlField& other);
    
public:
    void ReadField(BYTE*& buf) {};
    bool WriteField(CFile* file);
    void AddTagPair(char tag1[5], char tag2[5]);
    int GetFieldLength();
};

