#include "TextureChain.h"

Primative::TextureChain::TextureChain() : textures(), counter(0), timeSeperations(0), index(0)
{
}

Primative::TextureChain::TextureChain(const std::vector<unsigned>& textures, const std::vector<float>& seperations) : TextureChain()
{
	assert(textures.size() == seperations.size());
	this->textures = textures;
	timeSeperations = seperations;
}

void Primative::TextureChain::update(float deltaTime)
{
	counter += deltaTime;
}
Unsigned Primative::TextureChain::getTexture()
{
	if (NOT textures.size())
		return 0;
	unsigned res = textures[index];
	if (counter >= timeSeperations[index]) {
		res = textures[index++];
		counter = 0;
		index %= textures.size();
	}
	return res;
}

bool Primative::TextureChain::hasTextures() const
{
	return textures.size();
}
