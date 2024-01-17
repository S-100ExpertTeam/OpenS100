#pragma once

#include "Record.h"
#include "F_FieldControlField.h"
#include "F_DataDescriptiveField.h"

#include <vector>

class R_DDR :
    public Record
{
public:
    R_DDR();
    R_DDR(const R_DDR& other);
    virtual ~R_DDR();

public:
    F_FieldControlField f_FieldControlField;
    std::vector<F_DataDescriptiveField> f_DataDescriptiveField;

public:
    virtual R_DDR operator=(const R_DDR& other);

public:
    bool WriteRecord(CFile* file);

    void AddDDF(F_DataDescriptiveField ddf);
};

