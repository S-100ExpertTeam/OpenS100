#include "stdafx.h"
#include "D2DInstructionCache.h"
#include "RenderCacheSlot.h"
#include "SENC_TextInstruction.h"

namespace
{
	struct TextMetricsEntry : GISLibrary::RenderCacheEntry
	{
		DWRITE_TEXT_METRICS metrics{};
	};
}

namespace GISLibrary::D2DInstructionCache
{
	DWRITE_TEXT_METRICS* TextMetrics(SENC_TextInstruction& instruction)
	{
		auto entry = dynamic_cast<TextMetricsEntry*>(instruction.renderCache.get());
		return entry ? &entry->metrics : nullptr;
	}

	DWRITE_TEXT_METRICS& NewTextMetrics(SENC_TextInstruction& instruction)
	{
		auto entry = std::make_unique<TextMetricsEntry>();
		auto& metrics = entry->metrics;
		instruction.renderCache = std::move(entry);
		return metrics;
	}
}
