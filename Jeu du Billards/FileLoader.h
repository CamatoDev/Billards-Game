#pragma once
#include <SFML/Graphics.hpp> 
#include <SFML/Audio.hpp>
#include <iostream>

//fonction pour le chargement des textures 
sf::Texture LoadTexture(sf::Texture& texture, sf::String path) {
	if (!texture.loadFromFile(path)) {
		std::cout << "Erreur chargement de la texture." << std::endl;
	}

	return texture;
}

//Fonction pour le chargement des police d'écriture 
sf::Font LoadFont(sf::String path) {
	sf::Font font;
	if (!font.loadFromFile(path)) {
		std::cout << "Erreur chargement de la police d'écriture." << std::endl;
	}

	return font;
}

//Fonction pour le chargement des sons 
sf::SoundBuffer LoadSourceSound(sf::String path) {
	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile(path)) {
		std::cout << "Erreur chargement su son" << std::endl;
	}

	return buffer;
}
