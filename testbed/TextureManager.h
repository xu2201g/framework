#pragma once

#include "ResourceManager.h"
#include <SFML/Graphics.hpp>

class TextureManager
	:
	public ResourceManager<TextureManager, sf::Texture>
{
public:

	TextureManager()
		:
		ResourceManager("..//..//testbed//assets//textures.cfg")
	{}

	std::unique_ptr<sf::Texture> Load(const std::string& path)
	{
		//create new texture object
		std::unique_ptr<sf::Texture> pTexture(new sf::Texture);

		if (!pTexture->loadFromFile(Utils::GetWorkingDirectory() + path))
		{
			//here would be the place to deallocate memory - not necessary cause it ll
			//be deallocated after the function returned 
			std::cerr << "! Failed to load texture: " << path << std::endl;
			return nullptr;
		}

		return std::move(pTexture);
	}
};