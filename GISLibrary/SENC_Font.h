#pragma once

class SENC_Font
{
public:
	SENC_Font();
	~SENC_Font();

public:
	boolean serifs;
	int weight;
	int slant; // 0 : Upright, 1 : Italics
	int proportion;

public:
	void setUpright();
	bool isUpright();

	void setItalics();
	bool isItalics();

	DWRITE_FONT_STYLE getFontStyle();
};