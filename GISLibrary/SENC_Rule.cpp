#include "stdafx.h"
#include "SENC_Rule.h"

SENC_Rule::SENC_Rule()
{

}

SENC_Rule::~SENC_Rule()
{
	for (auto itor = templates.begin(); itor != templates.end(); itor++)
	{
		delete *itor;
	}

	for (unsigned i = 0; i < searchTablePoint.size(); i++)
	{
		for (auto itor = searchTablePoint[i].begin(); itor != searchTablePoint[i].end(); itor++)
		{
			delete itor->second;
		}
	}
	for (unsigned i = 0; i < searchTableCurve.size(); i++)
	{
		for (auto itor = searchTableCurve[i].begin(); itor != searchTableCurve[i].end(); itor++)
		{
			delete itor->second;
		}
	}
	for (unsigned i = 0; i < searchTableSurface.size(); i++)
	{
		for (auto itor = searchTableSurface[i].begin(); itor != searchTableSurface[i].end(); itor++)
		{
			delete itor->second;
		}
	}
}

void SENC_Rule::OrganizeSearchTable()
{
	std::vector<std::unordered_map<std::wstring, std::vector<SENC_Template*>*>>* searchTable;
	for (auto itor = templates.begin(); itor != templates.end(); itor++)
	{
		SENC_Template* t = *itor;
		if (t->type == 1)
		{
			searchTable = &searchTablePoint;
		}
		else if (t->type == 2)
		{
			searchTable = &searchTableCurve;
		}
		else if (t->type == 3)
		{
			searchTable = &searchTableSurface;
		}
		else
		{
			//OutputDebugString(L"Fail to Organized Search Table ( Feature : ");
			//OutputDebugString(t->camelCase.c_str());
			//OutputDebugString(L")\n");
			continue;
		}

		if (searchTable->size() < t->priority)
		{
			searchTable->resize(t->priority);
		}

		std::unordered_map<std::wstring, std::vector<SENC_Template*>*> *searchColumn;
		searchColumn = &(*searchTable)[t->priority - 1];
		std::vector<SENC_Template*> *attrVector = NULL;

		auto ccitor = searchColumn->find(t->camelCase);

		if (ccitor != searchColumn->end())
		{
			attrVector = ccitor->second;
		}
		else
		{
			attrVector = new std::vector<SENC_Template*>();
			searchColumn->insert(std::unordered_map<std::wstring, std::vector<SENC_Template*>*>::value_type(t->camelCase, attrVector));
		}
		attrVector->push_back(t);
	}
}

SENC_Template* SENC_Rule::FindPortrayalObject(int typeOfGeometry, std::wstring camelCase, std::vector<SENC_Attribute*> &attrList)
{
	std::vector<std::unordered_map<std::wstring, std::vector<SENC_Template*>*>>* searchTable;
	if (typeOfGeometry == 1 ||
		typeOfGeometry == 4)
	{
		searchTable = &searchTablePoint;
	}
	else if (typeOfGeometry == 2)
	{
		searchTable = &searchTableCurve;
	}
	else if (typeOfGeometry == 3)
	{
		searchTable = &searchTableSurface;
	}
	else
	{
		//OutputDebugString(L"Fail to search Feature ( Feature : ");
		//OutputDebugString(camelCase.c_str());
		//OutputDebugString(L")\n");

		return 0;
	}

	std::unordered_map<std::wstring, std::vector<SENC_Template*>*> *searchColumn;

	for (int i = (int)searchTable->size() - 1; i >= 0; i--)
	{
		searchColumn = &(*searchTable)[i];
		auto itor = searchColumn->find(camelCase);

		if (itor != searchColumn->end())
		{
			std::vector<SENC_Template*>* templates = itor->second;
			if (templates == NULL)
				continue;

			for (auto titor = templates->rbegin(); titor != templates->rend(); titor++)
			{
				SENC_Template* st = *titor;
				bool same = true;

				std::vector<SENC_Attribute*> matchedList;

				for (auto aitor1 = st->matchAttr.begin(); aitor1 != st->matchAttr.end() && same; aitor1++)
				{
					SENC_Attribute* attr1 = *aitor1;
					SENC_Attribute* attr2 = NULL;

					bool hasValue = false;
					for (auto aitor2 = attrList.begin(); aitor2 != attrList.end(); aitor2++)
					{
						attr2 = *aitor2;
						if (attr2->parentIndex != 0)
							continue;

						if (attr1->camelCase.compare(attr2->camelCase) == 0)
						{
							hasValue = true;
							if (attr1->value.size() == 0 || attr1->value.compare(attr2->value) == 0)
							{
								bool alraedyMatched = false;
								for (auto aitor3 = matchedList.begin(); aitor3 != matchedList.end(); aitor3++)
								{
									if (*aitor3 == attr2)
									{
										alraedyMatched = true;
									}
								}
								if (!alraedyMatched)
								{
									matchedList.push_back(*aitor2);
									same = true;
								}
							}
							else
							{
								same = false;
							}
							break;
						}
					}
					if (!hasValue)
					{
						same = false;
					}
				}

				if (same)
				{
					return st;
				}
			}
		}
		else
		{

		}
	}
	return NULL;
}