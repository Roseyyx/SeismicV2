#include "Renderer.h"
#include "../Log/Log.h"

unsigned long Renderer::Fonts::Default;

void Renderer::Initialize() {
	Renderer::Fonts::Default = Interfaces::Surface->FontCreate();
	
	Interfaces::Surface->SetFontGlyphSet(Renderer::Fonts::Default, "Tahoma", 12, 500, 0, 0, FontFlag_Dropshadow);
	
	LOG("Renderer->Initialized");
}


void Renderer::DrawTexts(int x, int y, Color color, HFont font, const wchar_t* text) {
	Interfaces::Surface->DrawSetTextFont(font);
	Interfaces::Surface->DrawSetTextColor(color.r, color.g, color.b, 255);
	Interfaces::Surface->DrawSetTextPos(x, y);
	Interfaces::Surface->DrawPrintText(text, wcslen(text));
}

void Renderer::DrawTexts(int x, int y, Color color, HFont font, std::wstring text) {
	Interfaces::Surface->DrawSetTextFont(font);
	Interfaces::Surface->DrawSetTextColor(color.r, color.g, color.b, 255);
	Interfaces::Surface->DrawSetTextPos(x, y);
	Interfaces::Surface->DrawPrintText(text.c_str(), text.length());
}