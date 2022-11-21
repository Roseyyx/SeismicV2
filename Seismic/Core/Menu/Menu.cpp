#include "Menu.h"


void Menu::Toggle() {
	if (GetAsyncKeyState(VK_INSERT) & 1)
		Config::MenuSettings::IsOpened = !Config::MenuSettings::IsOpened;
}