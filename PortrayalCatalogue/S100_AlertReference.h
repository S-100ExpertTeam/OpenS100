#pragma once
#include "S100_Instruction.h"

//AlertReference[:alertReference[, plan[, monitor]]]
//Specifies alert information to be associated with the geometry of drawing instructions which follow.
//alertReference	
// : A reference to an alert in the alert catalogue.If not provided, clears any previously applied alert reference.
//plan	
// : The viewing group the alert highlight is assigned to when active in route planning.If not provided, no viewing group is assignedand the highlight cannot be disabled.
//monitor	
// : The viewing group the alert highlight is assigned to when active in route monitoring.If not provided, no viewing group is assignedand the highlight cannot be disabled.

class S100_AlertReference : public S100_Instruction
{
public:
	S100_AlertReference();
	virtual ~S100_AlertReference();

public:
	std::wstring alertType;
	std::wstring plan;
	std::wstring monitor;
};
