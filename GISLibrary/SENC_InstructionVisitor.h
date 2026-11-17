#pragma once

class SENC_NullInstruction;
class SENC_PointInstruction;
class SENC_LineInstruction;
class SENC_AreaInstruction;
class SENC_TextInstruction;
class SENC_AugmentedRay;
class SENC_AugmentedPath;

class SENC_LineStyle;
class SENC_LineStyleReference;
class SENC_CompositeLineStyle;

// What is done with a drawing instruction is not part of the instruction: a renderer, an
// exporter or a test visits it. The kinds of instruction are the closed set of S-100 Part 9, so
// a new consumer is a new visitor and the model does not change.
class SENC_InstructionVisitor
{
public:
	virtual ~SENC_InstructionVisitor() = default;

	virtual void Visit(SENC_NullInstruction&) {}
	virtual void Visit(SENC_PointInstruction&) {}
	virtual void Visit(SENC_LineInstruction&) {}
	virtual void Visit(SENC_AreaInstruction&) {}
	virtual void Visit(SENC_TextInstruction&) {}
	virtual void Visit(SENC_AugmentedRay&) {}
	virtual void Visit(SENC_AugmentedPath&) {}
};

// The same for the line styles of a line instruction.
class SENC_LineStyleVisitor
{
public:
	virtual ~SENC_LineStyleVisitor() = default;

	virtual void Visit(SENC_LineStyle&) {}
	virtual void Visit(SENC_LineStyleReference&) {}
	virtual void Visit(SENC_CompositeLineStyle&) {}
};
