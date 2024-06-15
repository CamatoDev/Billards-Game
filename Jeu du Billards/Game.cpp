//#include <SFML/Graphics.hpp> *
//#include <iostream>
//#include "Shape.h"
//#include "FileLoader.h"
//
//
//unsigned int width = 1200;
//unsigned int height = 800;
//
////Déclaration des prototype des fonction 
//void CollisionTable(sf::CircleShape& ball, float rayon, sf::Vector2f& ball_speed, float table_width, float table_height);
//
////fonction du lancement du jeu 
//void Game() {
//	//définition de la fenêtre 
//	sf::RenderWindow gameWindow(sf::VideoMode(width, height), "BILLARDS");
//	 
//	//Définition des limite de la table 
//	float table_width = 900.f;
//	float table_height = 450.f;
//	sf::RectangleShape table = CreateRect(width / 1.7f, height / 2.f, table_width, table_height);
//	sf::Texture tableTexture;
//	LoadTexture(tableTexture, "res/Textures/board.png");
//	table.setTexture(&tableTexture, false);
//
//	//Définition d'une horloge 
//	sf::Clock clock;
//
//	//Définition des paramètre des balles 
//	sf::Texture ballTexture;
//	LoadTexture(ballTexture, "res/Textures/ball.png");
//	float rayon = 15.f;
//	float mass = 2.f;
//	sf::Vector2f ball_speed;
//	float speed_const = 0.f;
//	ball_speed.x = speed_const;
//	ball_speed.y = speed_const;
//	
//	//Ball blanche
//	sf::CircleShape white_ball = CreateBall(rayon, width / 3.f, height / 2.f, sf::Color::White, ballTexture);
//	//Ball noir 
//	sf::CircleShape black_ball = CreateBall(rayon, width - 300.f, height / 2.f, sf::Color::Black, ballTexture);
//	/*white_ball.setOrigin(rayon, rayon);
//	white_ball.setFillColor(sf::Color::White);
//	white_ball.setPosition(width / 4.f, height /2.f);*/
//	
//	
//
//	//pour savoir si le jeu est sur pause 
//	bool pause = false;
//
//
//	while (gameWindow.isOpen())
//	{
//		sf::Event event;
//		while (gameWindow.pollEvent(event))
//		{
//			if (event.type == sf::Event::Closed)
//				gameWindow.close();
//
//			//Pour mettre sur pause 
//			if (event.type == sf::Event::KeyPressed) {
//				if (event.key.code == sf::Keyboard::Space)
//					pause = !pause;
//			}
//
//			//Pour tirer la boule blanche
//			if (event.type == sf::Event::KeyPressed) {
//				if (event.key.code == sf::Keyboard::T) {
//					speed_const = 10.f;
//					ball_speed.x = speed_const;
//					ball_speed.y = speed_const;
//				}
//			}
//		}
//
//		//Vérification des colision avec la table 
//		CollisionTable(white_ball, rayon, ball_speed, table_width, table_height);
//		CollisionTable(black_ball, rayon, ball_speed, table_width, table_height);
//
//		if (!pause) {
//			//On applique les mouvement en fonction du temps 
//			if (clock.getElapsedTime().asMilliseconds() > 1000.f / 60.f) {
//				white_ball.move(ball_speed);
//				if(ball_speed.x > 0 && ball_speed.y > 0) {
//					ball_speed.x -= 0.1f;
//					ball_speed.y -= 0.1f;
//				}				
//				clock.restart();
//			}
//		}
//
//		gameWindow.clear();
//		gameWindow.draw(table);
//		gameWindow.draw(white_ball);
//		gameWindow.draw(black_ball);
//		gameWindow.display();
//	}
//}
//
//
//int main()
//{
//	Game();
//	return 0;
//}
//
//void CollisionTable(sf::CircleShape& ball, float rayon, sf::Vector2f& ball_speed, float table_width, float table_height) {
//	if (ball.getPosition().x + rayon + ball_speed.x >= width / 2.f - table_width / 2.f + table_width || ball.getPosition().x - rayon + ball_speed.x <= width / 2.f - table_width / 2.f) {
//		ball_speed.x = -ball_speed.x;
//	}
//
//	if (ball.getPosition().y + rayon + ball_speed.y >= height / 2.f - table_height / 2.f + table_height || ball.getPosition().y - rayon + ball_speed.y <= height / 2.f - table_height / 2.f) {
//		ball_speed.y = -ball_speed.y;
//	}
//}
//
//
//
////float x1 = white_ball.getPosition().x;
////float y1 = white_ball.getPosition().y;
////float x2 = black_ball.getPosition().x;
////float y2 = black_ball.getPosition().y;
////sf::Vector2f speed_f;
////sf::Vector2f speed2_f;
////
////float cx = (x1 - x2);
////float cy = (y1 - y2);
////float cr = rayon + rayon;
////
////if ((cx * cx + cy * cy) <= cr * cr) {
////	speed_f = ((mass - mass) * ball_speed + (2 * mass * ball_speed)) / (mass + mass);
////	speed2_f = ((mass - mass) * ball_speed + (2 * mass * ball_speed)) / (mass + mass);
////
////	ball_speed = speed_f;
////	ball_speed = speed2_f;
////}