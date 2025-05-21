#pragma once

#include "S100_TM_Instant.h"
#include "IntervalType.h"

#include <optional>

class S100_TM_Period
{
public:
	S100_TM_Period() = default;

public:
	IntervalType closure = IntervalType::none;
	std::optional<S100_TM_Instant> begin;
	std::optional<S100_TM_Instant> end;
};