#pragma once
#include "../../Includes/Common.h"
#include "../../sdk/Helpers/Interfaces.h"

enum FontFlags {
	FontFlag_None,
	FontFlag_Italic = 0x001,
	FontFlag_Underline = 0x002,
	FontFlag_Strikeout = 0x004,
	FontFlag_Symbol = 0x008,
	FontFlag_Antialias = 0x010,
	FontFlag_Gaussianblur = 0x020,
	FontFlag_Rotary = 0x040,
	FontFlag_Dropshadow = 0x080,
	FontFlag_Additive = 0x100,
	FontFlag_Outline = 0x200,
	FontFlag_Custom = 0x400,
	FontFlag_Bitmap = 0x800,
};

namespace Renderer
{
	void Initialize();

	void DrawFilledRect(int x, int y, int w, int h, Color color);
	void DrawOutlinedRect(int x, int y, int w, int h, Color color);
	void DrawLine(int x0, int y0, int x1, int y1, Color color);
	void DrawTexts(int x, int y, Color color, HFont font, const wchar_t* text);
	void DrawTexts(int x, int y, Color color, HFont font, std::wstring text);

	namespace Fonts {
		extern unsigned long Default;
	}
};

