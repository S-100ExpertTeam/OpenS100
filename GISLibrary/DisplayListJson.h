#pragma once

#include <cstdint>
#include <string>
#include <string_view>

class SENC_DisplayList;
class SENC_Instruction;

// The drawing instructions of a portrayal run as JSON text: what is to be drawn, with no
// picture. The text is the same on every platform for the same dataset and catalogues, so two
// builds (or two platforms, or this library and another implementation) are compared by
// comparing text, or its hash.
//
// Field names follow the display list of the web implementation (opens100-web,
// packages/core/src/display-list.ts) where both have the field: type, featureReference,
// viewingGroups, displayPlane, drawingPriority, scaleMinimum, scaleMaximum, symbol, fill,
// textPoint. Where this model holds more (resolved line styles, spatial references, augmented
// geometry) the extra fields are written as they are.
//
// Numbers are written in the shortest form that reads back to the same value, independent of
// the locale. The instructions come in the order of the display list: by drawing priority and
// kind, then in the order the rules emitted them.
namespace GISLibrary
{
	std::string DisplayListToJson(SENC_DisplayList& displayList);
	std::string InstructionToJson(SENC_Instruction& instruction);

	// FNV-1a, 64 bit. Enough to tell two texts apart in a test; not a cryptographic hash.
	std::uint64_t JsonHash(std::string_view json);
}
