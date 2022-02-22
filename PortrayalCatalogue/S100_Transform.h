#pragma once
#include "S100_Template.h"
#include "S100_Include.h"

#include <vector>

class S100_Transform
{
public:
	S100_Transform();
	virtual ~S100_Transform();

private:
	bool isTransfer;
	// About Rule File
	std::vector<S100_Template*> templates;
	std::vector<S100_Include*> includes;

public:

	void ExcuteIncludeRuleFile();
	void CopyTo(S100_Transform* toObject);

	void SetIsTransfer(bool value);
	bool GetIsTransfer();

	void SetTemplates(S100_Template* value);
	void SetTemplates(std::vector<S100_Template*> value);

	void SetInclude(S100_Include* value);
	void SetIncludes(std::vector<S100_Include*> value);

	S100_Template* GetTemplate(int index);
	std::vector<S100_Template*> GetTemplates();

	S100_Include* GetInclude(int index);
	std::vector<S100_Include*> GetIncludes();
};