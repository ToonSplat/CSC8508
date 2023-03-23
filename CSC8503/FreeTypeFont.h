#pragma once
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Maths.h"
#include "Assets.h"
#include "Vector4.h"
#include <map>

using namespace NCL::Maths;
namespace NCL::CSC8503
{
	class FreeTypeFont
	{
		struct Character {
			unsigned int TextureID;
			Vector2i size;
			Vector2i Bearing;
			unsigned int Advance;
		};

	public:
		FreeTypeFont();
		~FreeTypeFont() {}

		void RenderText(std::string text, float x, float y, float scale, Vector4 color);

		std::map<char, Character> Characters;
		unsigned int VAO, VBO;
	};
}