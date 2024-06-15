#pragma once
#include <SFML/Graphics.hpp> 

//Fonction pour créer des rectangles 
sf::RectangleShape CreateRect(int posX, int posY, float width, float height, sf::Color color, float outlineThicknessSize, sf::Color outlineColor) {
	sf::RectangleShape rect(sf::Vector2f(width, height));
	rect.setPosition(posX, posY);
	rect.setOrigin(width / 2.f, height / 2.f);
	rect.setOutlineThickness(outlineThicknessSize);
	rect.setFillColor(color);
	rect.setOutlineColor(outlineColor);

	return rect;
}
sf::RectangleShape CreateRect(int posX, int posY, float width, float height, sf::Color color) {
	sf::RectangleShape rect(sf::Vector2f(width, height));
	rect.setPosition(posX, posY);
	rect.setOrigin(width / 2.f, height / 2.f);
	rect.setFillColor(color);

	return rect;
}
sf::RectangleShape CreateRect(int posX, int posY, float width, float height) {
	sf::RectangleShape rect(sf::Vector2f(width, height));
	rect.setPosition(posX, posY);
	rect.setOrigin(width / 2.f, height / 2.f);

	return rect;
}

//Fonction pour creer des balle 
sf::CircleShape CreateBall(float rayon, float posX, float posY, sf::Color color, sf::Texture& texture) {
	sf::CircleShape ball(rayon);
	ball.setOrigin(rayon, rayon);
	ball.setFillColor(color);
	ball.setPosition(posX, posY);
	ball.setTexture(&texture);

	return ball;
}
