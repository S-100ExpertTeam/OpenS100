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
    virtual ~R_DDR();

public:
    F_FieldControlField f_FieldControlField;
    std::vector<F_DataDescriptiveField> f_DataDescriptiveField;

public:
    bool WriteRecord(CFile* file);

    void AddDDF(F_DataDescriptiveField ddf);

    virtual R_DDR* Clone() const;
};

