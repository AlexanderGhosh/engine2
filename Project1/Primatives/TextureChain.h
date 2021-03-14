#pragma once
#include "../Utils/General.h"

namespace Primative {
	class TextureChain
	{
	private:
		std::vector<unsigned> textures;
		std::vector<float> timeSeperations;
		int index;
		float counter;
	public:
		TextureChain();
		TextureChain(const std::vector<unsigned>& textures, const std::vector<float>& seperations);
		~TextureChain() = default;
		void update(float deltaTime);
		Unsigned getTexture();
		bool hasTextures() const;
	};
}

