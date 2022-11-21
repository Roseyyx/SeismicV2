#pragma once
#include <cstdint>
#include <d3d9.h>

struct Color {
	int a, r, g, b;
	Color() = default;
	Color(const int r, const int g, const int b, const int a = 255) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	explicit Color(const uint32_t color) {
		this->a = (color >> 24) & 0xff;
		this->r = (color >> 16) & 0xff;
		this->g = (color >> 8) & 0xff;
		this->b = (color & 0xff);
	}

	static Color from_uint(const uint32_t uint) {
		return Color(uint);
	}

	static D3DCOLOR from_color(const Color col) {
		return D3DCOLOR_ARGB(col.a, col.r, col.g, col.b);
	}

	static Color black(const int a = 255) { return { 0, 0, 0, a }; }
	static Color white(const int a = 255) { return { 255, 255, 255, a }; }
	static Color red(const int   a = 255) { return { 255, 0, 0, a }; }
	static Color green(const int a = 255) { return { 0, 255, 0, a }; }
	static Color blue(const int  a = 255) { return { 0, 0, 255, a }; }
};