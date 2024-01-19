#include "stdafx.h"
#include "F_FieldControlField.h"
#include "NonPrintableCharacter.h"

F_FieldControlField::F_FieldControlField()
{
	byte[0] = 0x30;
	byte[1] = 0x30;
	byte[2] = 0x30;
	byte[3] = 0x30;
	byte[4] = 0x3B;
	byte[5] = 0x26;
	byte[6] = 0x20;
	byte[7] = 0x20;
	byte[8] = 0x20;
	byte[9] = 0x1F;
}

F_FieldControlField::~F_FieldControlField()
{

}

bool F_FieldControlField::WriteField(CFile* file)
{
	file->Write((const void*)&byte[0], 10);

	for (auto i = tagPairs.begin(); i != tagPairs.end(); i++)
	{
		file->Write(&i->tag1[0], 4);
		file->Write(&i->tag2[0], 4);
	}

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
}

void F_FieldControlField::AddTagPair(char tag1[5], char tag2[5])
{
	TagPair tagPair;
	memcpy(&tagPair.tag1, tag1, 4);
	memcpy(&tagPair.tag2, tag2, 4);
	tagPairs.push_back(tagPair);
}

int F_FieldControlField::GetFieldLength()
{
	return (int)(10 + (tagPairs.size() * 8) + 1);
}

F_FieldControlField* F_FieldControlField::Clone() const
{
	F_FieldControlField* fcf = new F_FieldControlField();
	memset(fcf->byte, 0x00, 10);
	memcpy(fcf->byte, byte, 10);
	for (const auto& iter : tagPairs)
	{
		TagPair tagPair;
		memset(&tagPair.tag1, 0x00, 5);
		memcpy(&tagPair.tag1, iter.tag1, 5);
		memset(&tagPair.tag2, 0x00, 5);
		memcpy(&tagPair.tag2, iter.tag2, 5);
		fcf->tagPairs.push_back(tagPair);
	}

	return fcf;
}

