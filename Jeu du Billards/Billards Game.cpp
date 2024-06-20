#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cmath>
#include <iostream>

// Fonction pour vérifier la collision avec les boutons
bool ButtonCollision(float cursor_x, float cursor_y, const sf::RectangleShape& box, float box_width, float box_height) {
    return cursor_x >= box.getPosition().x
        && cursor_x < box.getPosition().x + box_width
        && cursor_y >= box.getPosition().y
        && cursor_y < box.getPosition().y + box_height;
}

// Fonction pour écrire du texte dans les fenêtres
void WriteInWindow(sf::Font& font, sf::Text& text, const sf::String& str, float size = 60) {
    text.setFont(font); // On passe la police d'écriture 
    text.setString(str);    // On passe la chaîne de caractère à afficher 
    text.setCharacterSize(size);    // On passe la taille du texte 
    text.setFillColor(size > 30 ? sf::Color::White : sf::Color::Green); // On resiegne la couleur du texte (Blanc si le texte est de taille > 30 et vert dans l'autre)
    text.setStyle(sf::Text::Bold);  // On écrit en gras 
}

// Fonction pour charger une texture
void LoadTexture(sf::Texture& texture, const std::string& path) {
    if (!texture.loadFromFile(path)) {
        std::cerr << "Erreur chargement de la texture." << std::endl;
    }
}

// Fonction pour charger un son
void LoadSourceSound(sf::SoundBuffer& buffer, const std::string& path) {
    if (!buffer.loadFromFile(path)) {
        std::cerr << "Erreur chargement du son" << std::endl;
    }
}

// Fonction pour charger une police d'écriture
void LoadFont(sf::Font& font, const std::string& path) {
    if (!font.loadFromFile(path)) {
        std::cerr << "Erreur chargement de la police d'écriture." << std::endl;
    }
}


// Fonction pour configurer les boules en forme de triangle
static void setupBalls(std::vector<sf::CircleShape>& balls) {
    const float radius = 15.0f;     // Rayon des boules
    const float offsetX = 550.0f;  // Position horizontale du premier rang
    const float offsetY = 350.0f;  // Position verticale du premier rang

    for (int row = 0; row < 5; ++row) {            //Disposition par ligne 
        for (int col = 0; col <= row; ++col) {     // Disposition des colonnes en fonction des lignes 
            sf::CircleShape ball(radius);          // Création d'une boule 
            ball.setOrigin(radius, radius);        // On définit l'origine au centre de la boule

            float x = offsetX + (row * radius * 1.8f);  // le coefficient de multiplication en X
            float y = offsetY + (col * radius * 2.1f) - (row * radius);     // le coefficient de multiplication en Y
            ball.setPosition(x, y);     // On positionne la boule 
            balls.push_back(ball);      // On ajoute la boule à notre vecteur de sf::CircleShape
        }
    }
}


// Fonction pour vérifier les collisions entre les boules et les bords de la table
static void checkCollisionsWithTable(std::vector<sf::CircleShape>& balls, std::vector<sf::Vector2f>& velocities, const sf::FloatRect& tableBounds, sf::Sound& ballCollisionSound) {
    const float radius = balls[0].getRadius();      // On récupère le rayon qui est le même pour toute les boules
    for (size_t i = 0; i < balls.size(); ++i) {     // Pour chaque boule dans notre vecteur 
        sf::Vector2f position = balls[i].getPosition();     // On récupère la position de la boule 
        sf::Vector2f& velocity = velocities[i];             // On récupère la vitesse de déplacement de la boule 
        float borderUp = 35.f;    // Valeur représentant l'écart entre le bord réel de l'image et ce de la table 
        float borderDown = 35.f;    // Valeur représentant l'écart entre le bord réel de l'image et ce de la table 

        // Vérifier collision avec le bord gauche
        if (position.x - radius < tableBounds.left + borderUp) {
            ballCollisionSound.play();      // On joue le son de collision avec la bordure 
            velocity.x = -velocity.x;       // On inverse la vitesse de la boule suivant l'axe X 
            balls[i].setPosition(tableBounds.left + borderUp + radius, position.y);
        }
        // Vérifier collision avec le bord droit
        if (position.x + radius > tableBounds.left + tableBounds.width - borderDown) {
            ballCollisionSound.play();      // On joue le son de collision avec la bordure 
            velocity.x = -velocity.x;       // On inverse la vitesse de la boule suivant l'axe X 
            balls[i].setPosition(tableBounds.left + tableBounds.width - borderDown - radius, position.y);
        }
        // Vérifier collision avec le bord supérieur
        if (position.y - radius < tableBounds.top + borderUp) {
            ballCollisionSound.play();      // On joue le son de collision avec la bordure 
            velocity.y = -velocity.y;       // On inverse la vitesse de la boule suivant l'axe Y
            balls[i].setPosition(position.x, tableBounds.top + borderUp + radius);
        }
        // Vérifier collision avec le bord inférieur
        if (position.y + radius > tableBounds.top + tableBounds.height - borderDown) {
            ballCollisionSound.play();      // On joue le son de collision avec la bordure 
            velocity.y = -velocity.y;       // On inverse la vitesse de la boule suivant l'axe Y
            balls[i].setPosition(position.x, tableBounds.top + tableBounds.height - borderDown - radius);
        }
    }
}

// Fonction pour vérifier les collisions entre les boules entre elles
static void checkCollisionsBetweenBalls(std::vector<sf::CircleShape>& balls, std::vector<sf::Vector2f>& velocities, sf::Sound& ballCollisionSound) {
    const float radius = balls[0].getRadius();
    const float radiusSquared = radius * 2 * radius * 2;
    const float mass = 18.4f;       // On définit la masse des boules 

    for (size_t i = 0; i < balls.size(); ++i) {
        for (size_t j = i + 1; j < balls.size(); ++j) {
            // Debut du calcul de la distance entre deux boules 
            sf::Vector2f pos1 = balls[i].getPosition();
            sf::Vector2f pos2 = balls[j].getPosition();

            sf::Vector2f diff = pos1 - pos2;
            float distSquared = diff.x * diff.x + diff.y * diff.y;
            // Fin du calcul de la distance entre les boules  

            if (distSquared < radiusSquared) {
                //on joue le son de collision 
                ballCollisionSound.play();
                // Calcul des nouvelles vitesses suivant les lois de la physique 
                sf::Vector2f temp1 = ((mass - mass) * velocities[i] + (2 * mass * velocities[j])) / (mass + mass);
                sf::Vector2f temp2 = ((mass - mass) * velocities[j] + (2 * mass * velocities[i])) / (mass + mass);

                // On passe les nouvelles valeurs de la vitesse de déplacement aux boules respectives 
                velocities[i] = temp1;
                velocities[j] = temp2;

                // Déplacement des boules pour éviter qu'elles restent collées
                float dist = std::sqrt(distSquared);
                float overlap = 0.5f * (dist - radius * 2);
                balls[i].move(-diff / dist * overlap);
                balls[j].move(diff / dist * overlap);
            }
        }
    }
}



// Fonction pour le système de friction qui vas permettre aux boules de s'arrêter au bout d'un moment 
static void applyFriction(std::vector<sf::Vector2f>& velocities, float friction) {
    for (auto& velocity : velocities) {
        velocity.x *= friction; // On multiplie la vitesse de déplacement en X par la force de friction 
        velocity.y *= friction; // On multiplie la vitesse de déplacement en Y par la force de friction

        // Si la vitesse est très petite, on arrête la boule 
        if (std::abs(velocity.x) < 0.01f) velocity.x = 0;
        if (std::abs(velocity.y) < 0.01f) velocity.y = 0;
    }
}

// Initialiser les poches comme des sf::CircleShape
std::vector<sf::CircleShape> pockets;

static void setupPockets(std::vector<sf::CircleShape>& pockets) {
    const float pocketRadius = 13.f;
    const float width = 800.f; // Largeur de la table
    const float height = 400.f; // Hauteur de la table
    // Positions des poches (trous) sur la table
    float pocketWidthMin = 67.5f;
    float pocketWidthMax = 787.5f;
    float pocketHeihtMin = 165.f;
    float pocketWidthMiddle = 427.5f;
    float pocketHeihtMax = 515.f;

    const std::vector<sf::Vector2f> pocketPositions = {
        {pocketWidthMin, pocketHeihtMin + 5.f}, {pocketWidthMax, pocketHeihtMin + 5.f}, {pocketWidthMax, pocketHeihtMax - 5.f}, {pocketWidthMin, pocketHeihtMax - 5.f},
        {pocketWidthMiddle, pocketHeihtMin}, {pocketWidthMiddle, pocketHeihtMax}
    };

    for (const auto& pos : pocketPositions) {
        sf::CircleShape pocket(pocketRadius);
        pocket.setOrigin(pocketRadius, pocketRadius); // Origine au centre
        pocket.setFillColor(sf::Color::Transparent);
        pocket.setPosition(pos);
        pockets.push_back(pocket);
    }
}

// Fonction pour vérifier si une boule est tombée dans un trou
static void checkBallsInPockets(std::vector<sf::CircleShape>& balls, std::vector<sf::Vector2f>& velocities, const std::vector<sf::CircleShape>& pockets, sf::Sound& inPocket
    , int& player1_score, int& player2_score, bool& turnPlayer1, bool& isCueBallPocketed) {
    const float radius = balls[0].getRadius(); // On récupère le rayon d'une boule (elles ont toutes le même rayon)

    // Parcourt chaque boule
    for (size_t i = 0; i < balls.size(); ++i) {
        sf::Vector2f position = balls[i].getPosition(); // Récupère la position de la boule actuelle

        // Parcourt chaque poche
        for (const auto& pocket : pockets) {
            sf::Vector2f pocketPos = pocket.getPosition(); // Récupère la position de la poche actuelle
            float pocketRadius = pocket.getRadius(); // Récupère le rayon de la poche

            // Vérifie si la distance entre la boule et la poche est inférieure à la somme de leurs rayons
            if (std::hypot(position.x - pocketPos.x, position.y - pocketPos.y) < pocketRadius + radius) {
                inPocket.play(); // Joue le son de la boule tombant dans la poche

                if (i == balls.size() - 1) { // Si c'est la dernière boule (boule blanche)
                    isCueBallPocketed = true;
                }

                // Supprime la boule et sa vélocité des vecteurs respectifs
                balls.erase(balls.begin() + i);
                velocities.erase(velocities.begin() + i);

                // Mise à jour des scores 
                if (!turnPlayer1) {
                    player1_score++;    // Incrémente le score du joueur 1
                }
                else {
                    player2_score++;    // Incrémente le score du joueur 2
                }

                --i; // Ajuste l'indice pour compenser la suppression de la boule
                break; // Sort de la boucle des poches pour passer à la boule suivante
            }
        }
    }
}

// Fonction pour vérifier si une boule est dans une poche
static bool isBallInPocket(const sf::CircleShape& ball) {
    for (const auto& pocket : pockets) {
        float dx = ball.getPosition().x - pocket.getPosition().x;
        float dy = ball.getPosition().y - pocket.getPosition().y;
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance < (ball.getRadius() + pocket.getRadius())) {
            return true;  // Retourne true si la boule est dans une poche
        }
    }
    return false;  // Retourne false si la boule n'est dans aucune poche
}

// Prototype de la fonction menu 
void Menu();


unsigned int width = 1000;  // Largeur de la fenêtre 
unsigned int height = 800;  // Hauteur de la fenêtre 

// Fonction du lancement du Jeu
void GameStart() {
    sf::RenderWindow window(sf::VideoMode(width, height), "Jeu de Billard");

    // Chargement de l'audio de fond
    sf::SoundBuffer main_music;
    LoadSourceSound(main_music, "res/Sounds/fone.ogg");
    sf::Sound mainMusic(main_music);
    mainMusic.setVolume(20.f);
    mainMusic.play();
    mainMusic.setLoop(true);     //On boucle la musique

    //chargement de l'audio de collision de boule 
    sf::SoundBuffer ball_collision;
    LoadSourceSound(ball_collision, "res/Sounds/ball.ogg");
    sf::Sound ballCollisionSound(ball_collision);

    //chargement de l'audio de collision avec la table 
    sf::SoundBuffer table_collision;
    LoadSourceSound(table_collision, "res/Sounds/border.ogg");
    sf::Sound tableCollisionSound(table_collision);
    
    //chargement de l'audio de collision avec les trous 
    sf::SoundBuffer pocket_collision;
    LoadSourceSound(pocket_collision, "res/Sounds/hole.ogg");
    sf::Sound inPocket(pocket_collision);

    // Chargement de la texture du background
    sf::Texture backgroundTexture;
    LoadTexture(backgroundTexture, "res/Textures/simple.jpg");
    sf::Sprite background;
    background.setTexture(backgroundTexture);
    background.setScale(width / 234.f, height / 234.f);

    // Chargement de la texture de la table
    sf::Texture tableTexture;
    LoadTexture(tableTexture, "res/Textures/board.png");

    // Création de la table
    sf::RectangleShape table(sf::Vector2f(800, 400));
    table.setTexture(&tableTexture);
    table.setPosition(30, 140);
    sf::FloatRect tableBounds = table.getGlobalBounds();

    // Chargement de la texture pour la queue de billard
    sf::Texture cue_texture;
    LoadTexture(cue_texture, "res/Textures/cue.png");

    // Initialisation du sprite de la queue de billard
    sf::Sprite cue_sprite;
    cue_sprite.setTexture(cue_texture);
    // On définit l'origine à l'extrémité de la queue (point d'origine pour la rotation)
    cue_sprite.setOrigin(cue_texture.getSize().x, cue_texture.getSize().y);

    bool isAiming = false;
    sf::Vector2f aimStart;
    sf::Vector2f aimEnd;

    sf::VertexArray aimLine(sf::Lines, 2);
    aimLine[0].color = sf::Color::Red;
    aimLine[1].color = sf::Color::Red;

    bool showCue = true; // Indicateur pour montrer ou cacher la queue de billard

    // Chargement des textures pour les boules
    sf::Texture ballTextures[15];
    LoadTexture(ballTextures[0], "res/Textures/10.png");
    LoadTexture(ballTextures[1], "res/Textures/12.png");
    LoadTexture(ballTextures[2], "res/Textures/14.png");
    LoadTexture(ballTextures[3], "res/Textures/8.png");
    LoadTexture(ballTextures[4], "res/Textures/9.png");
    LoadTexture(ballTextures[5], "res/Textures/11.png");
    LoadTexture(ballTextures[6], "res/Textures/5.png");
    LoadTexture(ballTextures[7], "res/Textures/7.png");
    LoadTexture(ballTextures[8], "res/Textures/6.png");
    LoadTexture(ballTextures[9], "res/Textures/4.png");
    LoadTexture(ballTextures[10], "res/Textures/15.png");
    LoadTexture(ballTextures[11], "res/Textures/2.png");
    LoadTexture(ballTextures[12], "res/Textures/9.png");
    LoadTexture(ballTextures[13], "res/Textures/3.png");
    LoadTexture(ballTextures[14], "res/Textures/1.png");

    // Chargement de la texture de la boule blanche 
    sf::Texture cueBallTexture;
    LoadTexture(cueBallTexture, "res/Textures/ball.png");


    // Création des boules
    std::vector<sf::CircleShape> balls;
    setupBalls(balls);

    // Application des textures des boules
    balls[0].setTexture(&ballTextures[0]);
    balls[1].setTexture(&ballTextures[1]);
    balls[2].setTexture(&ballTextures[2]);
    balls[3].setTexture(&ballTextures[3]);
    balls[4].setTexture(&ballTextures[4]);
    balls[5].setTexture(&ballTextures[5]);
    balls[6].setTexture(&ballTextures[6]);
    balls[7].setTexture(&ballTextures[7]);
    balls[8].setTexture(&ballTextures[8]);
    balls[9].setTexture(&ballTextures[9]);
    balls[10].setTexture(&ballTextures[10]);
    balls[11].setTexture(&ballTextures[11]);
    balls[12].setTexture(&ballTextures[12]);
    balls[13].setTexture(&ballTextures[13]);
    balls[14].setTexture(&ballTextures[14]);

    // Création des vecteurs de vitesse pour les boules
    std::vector<sf::Vector2f> velocities(balls.size(), sf::Vector2f(0.0f, 0.0f));  // Initialiser avec une vitesse nulle

    // Création de la bille blanche
    sf::CircleShape cueBall(15.0f);
    cueBall.setTexture(&cueBallTexture);
    cueBall.setOrigin(15.0f, 15.0f);
    cueBall.setPosition(width / 2.f - 350.f, height / 2.f - 50.f);  // Positionner la bille blanche au centre de la table avec un décalage
    balls.push_back(cueBall);   // Ajout dans le vecteur de sf::CircleShape 
    velocities.push_back(sf::Vector2f(0.0f, 0.0f));  // Initialiser la vitesse à zéro pour la bille blanche

    std::vector<sf::CircleShape> pockets;
    setupPockets(pockets);

    //police d'écriture 
    sf::Font font;
    //chargement police
    LoadFont(font, "res/Fonts/times-new-roman.ttf");
    sf::Text text_turn, endText;  // Ajout du texte pour afficher le joueur actuel et le texte de fin de jeu
    WriteInWindow(font, text_turn, " Tour joueur 1", 40);// Initialisation du texte pour afficher le joueur actuel
    sf::Text player1ScoreText, player2ScoreText;  // Textes pour afficher les scores des joueurs
    WriteInWindow(font, player1ScoreText, "Joueur 1 : 0", 30);  // Initialisation du texte pour le score du joueur 1
    WriteInWindow(font, player2ScoreText, "Joueur 2 : 0", 30);  // Initialisation du texte pour le score du joueur 2

    // Positionnement des textes de score
    player1ScoreText.setPosition(100, 80);  // Position en haut à gauche pour le joueur 1
    text_turn.setPosition(300, 80);   //Positionnement du texte qui affiche le joueur 
    player2ScoreText.setPosition(600, 80);  // Position en dessous du score du joueur 1 pour le joueur 2


    const float friction = 0.99f;  // Coefficient de friction

    bool pause = false;  // Variable pour mettre le jeu en Pause
    //bool readyToShoot = true;  // Variable pour indiquer si le joueur peut frapper
    float launchPower = 0.0f;   // Niveau de puissance du coup
    float maxPowerLevel = 8.0f;   // Puissance maximale du coup
    bool increasingPower = true;  // Variable pour augmenter la puissance

    // Variables pour la gestion des scores
    int player1Score = 0;   // Score du joueur 1
    int player2Score = 0;   // Score du joueur 2
    bool turnPlayer1 = true;    // Le joueur actuel (1 ou 2)
    bool isCueBallPocketed = false; // Pour vérifier si la boule blanche dans une poche
    bool gameEnd = false;   // Indicateur de fin de jeu

    //Gestion de fin du jeu 
    sf::Texture buttonTexture;  // Création de la texture des boutons 
    LoadTexture(buttonTexture, "res/Textures/button.png");  // Chargement de la textures des boutons

    //Boutton 
    sf::RectangleShape retry, menu;
    float button_width = 220.f;
    float button_height = 80.f;
    float offset = 10.f;

    //pour le boutton retry 
    retry.setSize(sf::Vector2f(button_width, button_height));
    retry.setPosition(sf::Vector2f(550.0f, 350.0f));
    retry.setTexture(&buttonTexture);
    sf::Text retryText;
    retryText.setPosition(550.0f, 350.0f);
    WriteInWindow(font, retryText, "RETRY");

    //pour le boutton menu 
    menu.setSize(sf::Vector2f(button_width, button_height));
    menu.setPosition(sf::Vector2f(width / 2.f - 350.f, height / 2.f - 50.f));
    menu.setTexture(&buttonTexture);
    sf::Text menuText;
    menuText.setPosition(width / 2.f - 350.f, height / 2.f - 50.f);
    WriteInWindow(font, menuText, "MENU");

    // Initialisation de la bille blanche en attente de lancement
    bool iscueBallLaunched = false; // Variable pour indiquer si le joueur peut frapper
    bool isDirectionValidated = false;  // Indique si la direction a été validée par un clic gauche
    sf::Vector2f cueBallPosition;   // Pour stocker la position actuelle de la bille blanche
    // Position initiale de la bille blanche
    cueBallPosition = sf::Vector2f(width / 2.f - 350.f, height / 2.f - 50.f);
    sf::Vector2f launchDirection;   // Direction du lancement
    balls.back().setPosition(cueBallPosition);
    // Fin de la gestion de fin de jeu 

    sf::Clock clock;    // Création d'une horloge 

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Pour mettre sur pause on appui sur la barre Espace 
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::P)
                    pause = !pause;
            }
            // Si le jeu est terminé 
            if (gameEnd) {
                //Touche souris RETRY
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (ButtonCollision(event.mouseButton.x, event.mouseButton.y, retry, button_width, button_height)) {
                            mainMusic.stop();
                            window.close();
                            //lacer le jeu principal
                            GameStart();
                        }
                    }
                }

                //Touche souris MENU
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (ButtonCollision(event.mouseButton.x, event.mouseButton.y, menu, button_width, button_height)) {
                            mainMusic.stop();
                            window.close();
                            //lacer le menu principal 
                            Menu();
                        }
                    }
                }
            }

            // Détection du clic droit de la souris pour repositioner la boule blanche
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Right) {
                    sf::Vector2f clickPosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    if (isCueBallPocketed) {
                        // Repositionner la boule blanche
                        if (table.getGlobalBounds().contains(clickPosition)) {
                            cueBall.setPosition(clickPosition);
                            balls.push_back(cueBall);
                            velocities.push_back(sf::Vector2f(0.f, 0.f));
                            isCueBallPocketed = false;
                        }
                    }
                }
            }

            // Détection du clic gauche de la souris pour valider la direction
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                isDirectionValidated = true;
                // Capturer la position de la souris
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                sf::Vector2f mousePositionF(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

                // Calculer la direction du tir
                sf::Vector2f direction = mousePositionF - cueBallPosition;
                float length = std::sqrt(std::abs(direction.x * direction.y + direction.y * direction.y));

                // Eviter la division par zéro
                if (length != 0.f) {
                    launchDirection = direction / length;
                }
                else {
                    launchDirection = sf::Vector2f(1.f, 0.f);
                }
                aimStart = cueBallPosition;
                aimEnd = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                isAiming = true;
            }

            // Click gauche relaché
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left && isAiming) {
                    isAiming = false;
                }
            }
        }

        if (isAiming) {
            aimLine[0].position = aimStart;
            aimLine[1].position = aimEnd;
        }

        // Mise à jour des scores et du texte des joueurs
        player1ScoreText.setString("Joueur 1 : " + std::to_string(player1Score));
        player2ScoreText.setString("Joueur 2 : " + std::to_string(player2Score));
        text_turn.setString(turnPlayer1 ? " Tour joueur 1" : " Tour joueur 2");

        // Si on est n'est pas en pause
        if (!pause) {
            if (clock.getElapsedTime().asMilliseconds() > 1000.f / 60.f) {    // On bloque la vitesse du jeu à 60 fps
                // Mettre à jour les positions des boules
                for (size_t i = 0; i < balls.size(); ++i) {
                    balls[i].move(velocities[i]);
                }

                // Vérification les collisions avec les bords
                checkCollisionsWithTable(balls, velocities, tableBounds, tableCollisionSound);

                // Vérification les collisions entre les boules
                checkCollisionsBetweenBalls(balls, velocities, ballCollisionSound);

                // Vérification si les boules sont tombées dans les poches
                checkBallsInPockets(balls, velocities, pockets, inPocket, player1Score, player2Score, turnPlayer1, isCueBallPocketed);

                // On applique la friction
                applyFriction(velocities, friction);

                // Vérifier si le jeu est terminé (toutes les boules sont dans les poches sauf la blanche)
                if (balls.size() == 1) {
                    gameEnd = true;  // Indique que le jeu est terminé
                    if (player1Score > player2Score) {
                        endText.setString("Le joueur 1 a gagne!");  // Affiche que le joueur 1 a gagné
                    }
                    else if (player2Score > player1Score) {
                        endText.setString("Le joueur 2 a gagne!");  // Affiche que le joueur 2 a gagné
                    }
                    else {
                        endText.setString("Match nul!");  // Affiche qu'il y a un match nul
                    }
                    WriteInWindow(font, endText, endText.getString(), 70);  // Met à jour le texte de fin de jeu
                    endText.setPosition(420.f / 2.f, 175.f);   //Positionnement du texte de fin de jeu
                    
                }

                // Gestion du tir du joueur
                if (!iscueBallLaunched) {
                   // Gestion de la puissance du tir
                   if (isDirectionValidated && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                      showCue = false; // Cacher la queue après un tir
                      // Lancer la bille blanche avec la puissance déterminée
                      velocities.back() = launchDirection * launchPower;
                      launchPower = 0.0f;
                      iscueBallLaunched = true;
                      isDirectionValidated = false;
                      turnPlayer1 = !turnPlayer1;
                   }

                    // Mettre à jour la position de la bille blanche
                    cueBallPosition = balls.back().getPosition();

                    // Ajuster la puissance du tir
                    if (increasingPower) {
                        launchPower += 0.1f;
                        if (launchPower >= maxPowerLevel) {
                            increasingPower = false;
                        }
                    }
                    else {
                        launchPower -= 0.1f;
                        if (launchPower <= 0.0f) {
                            increasingPower = true;
                        }
                    }
                }
                else {
                    iscueBallLaunched = true;
                }

                clock.restart();  // Redémarrer l'horloge pour la prochaine itération
            }

            // Vérification si toutes les boules sont arrêtées pour permettre le prochain tour
            bool allBallsStopped = std::all_of(velocities.begin(), velocities.end(), [](const sf::Vector2f& v) {
                return v.x == 0.f && v.y == 0.f;
                });

            if (allBallsStopped) {
                iscueBallLaunched = false;
                showCue = true; // Montrer la queue de billard une fois toutes les boules arrêtées
            }
        }

        window.clear();
        window.draw(background);    // On dessine l'arrière plan 
        window.draw(table);     // On dessine la table 

        // Dessiner toute les boules 
        for (const auto& ball : balls) {
            window.draw(ball);
        }

        // Dessiner toute les poches 
        for (const auto& pocket : pockets) {
            window.draw(pocket);
        }

        // Dessiner la jauge de puissance
        sf::RectangleShape powerBar(sf::Vector2f(60, launchPower * 40));  // Taille de la jauge
        powerBar.setFillColor(sf::Color::Red);
        powerBar.setOutlineThickness(2.f);
        powerBar.setOutlineColor(sf::Color::Black);
        powerBar.setPosition(width - 100, height - 60 - launchPower * 40);  // 
        window.draw(powerBar);
        window.draw(text_turn);  // Dessine le texte du joueur actuel
        window.draw(player1ScoreText);  // Dessine le texte du score du joueur 1
        window.draw(player2ScoreText);  // Dessine le texte du score du joueur 2

        if (isAiming) {
            window.draw(aimLine);
        }
        if (showCue) {
            // Mettre à jour la position et la rotation de la queue
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            sf::Vector2f direction = mousePos - cueBallPosition;
            float angle = - std::atan2(direction.x, direction.y) * 180 / 3.14159f;

            cue_sprite.setPosition(cueBallPosition);
            cue_sprite.setRotation(angle);

            window.draw(cue_sprite);
        }
        if (gameEnd) {
            window.draw(endText);  // Dessine le texte de fin de jeu si le jeu est terminé
            window.draw(retry); // Dessine le bouton pour réessayer
            window.draw(menu);  // Dessine le bouton pour réessayer
            window.draw(retryText); // Dessine le texte du bouton pour réessayer
            window.draw(menuText);  // Dessine le texte du bouton pour réessayer
        }


        window.display();
    }

}

// Fonction pour afficher les instructions
void ShowInstructions() {
    sf::RenderWindow window(sf::VideoMode(width, height), "Billards - Comment jouer");

    sf::Texture backgroundTexture;
    LoadTexture(backgroundTexture, "res/Textures/background_window.png");
    sf::Sprite background(backgroundTexture);
    background.scale(width / 1300.f, height / 800.f);

    sf::Font font;
    LoadFont(font, "res/Fonts/times-new-roman.ttf");

    sf::Text instructions, title;
    WriteInWindow(font, title, "Comment Jouer ?", 70.f);
    title.setPosition(width / 4.f, 100.f);
    WriteInWindow(font, instructions, "Instructions du jeu:\n- Déplacez la souris pour viser.\n- Clique gauche de la souris pour valider.\n- La barre rouge représente la jauge de puissance.\n- Appuyez sur espace au moment de votre choix pour frapper la bille\n blanche avec la puissance voulu.\n- Si la boule blanche tombe dans une poche faite un clique droit sur la\n table à l'emplacement de votre choix pour la repositionner.\n- Faites entré toute les boules numérotés dans les poches.\n- celui qui fais entrer le plus de boule à gagné.\n \n Bon Match !", 25);
    instructions.setPosition(100.f, 250.f);
    instructions.setFillColor(sf::Color::White);

    sf::Texture buttonTexture;
    LoadTexture(buttonTexture, "res/Textures/button.png");
    sf::RectangleShape backButton(sf::Vector2f(300.f, 100.f));
    backButton.setPosition(width - 400.f, height - 200.f);
    backButton.setFillColor(sf::Color::Red);
    backButton.setTexture(&buttonTexture);
    sf::Text backButtonText;
    WriteInWindow(font, backButtonText, "Retour", 50);
    backButtonText.setPosition(width - 400.f + 300.f / 4, height - 200.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (ButtonCollision(mousePos.x, mousePos.y, backButton, backButton.getSize().x, backButton.getSize().y)) {
                    window.close();
                    Menu();
                }
            }
        }

        window.clear();
        window.draw(background);
        window.draw(title);
        window.draw(instructions);
        window.draw(backButton);
        window.draw(backButtonText);
        window.display();
    }
}


// Menu principal 
void Menu() {

    //Définition de la fenêtre du menu principale 
    sf::RenderWindow mainMenu(sf::VideoMode(width, height), "Jeu de Billard");

    //police d'écriture 
    sf::Font font;
    //chargement police
    LoadFont(font, "res/Fonts/times-new-roman.ttf");

    sf::Text titleText;
    WriteInWindow(font, titleText, "Jeu de Billard", 70);   // Initialisation du texte pour afficher le titre
    titleText.setPosition(300, 80); // Positionnement des textes du titre 

    //chargement de la texture de l'arrière plan 
    sf::Texture backgroundTexture;
    LoadTexture(backgroundTexture, "res/Textures/intro.jpg");
    sf::Sprite background;
    background.setTexture(backgroundTexture);

    //Boutton 
    sf::RectangleShape play, option, quit;
    float button_width = 450.f;
    float button_height = 90.f;
    float offsetHeight = 30.f;
    float offsetWidth = 130.f;
    sf::Texture buttonTexture;
    LoadTexture(buttonTexture, "res/Textures/button.png");

    //Pour le son
    sf::SoundBuffer mainMenuSoundbuffer;
    //chargement du song 
    LoadSourceSound(mainMenuSoundbuffer, "res/Sounds/intro.ogg");
    sf::Sound music;
    music.setBuffer(mainMenuSoundbuffer);
    music.setLoop(true);
    music.setVolume(40.f);
    music.play();

    //pour le boutton play 
    play.setOrigin(button_width / 2.f, button_height / 2.f);
    play.setSize(sf::Vector2f(button_width, button_height));
    play.setPosition(sf::Vector2f(width / 2.f, height / 2.f));
    play.setTexture(&buttonTexture);
    sf::Text playText;
    playText.setPosition(width / 2.f - button_width / 2.f + offsetWidth, height / 2.f - button_height / 2.f);
    WriteInWindow(font, playText, "JOUER");

    //pour le boutton option 
    option.setOrigin(button_width / 2.f, button_height / 2.f);
    option.setSize(sf::Vector2f(button_width, button_height));
    option.setPosition(sf::Vector2f(width / 2.f, height / 2.f + offsetHeight + button_height));
    option.setTexture(&buttonTexture);
    sf::Text optionText;
    optionText.setPosition(width / 2.f - button_width / 2.f + offsetWidth / 4.f, height / 2.f - button_height / 2.f + offsetHeight + button_height);
    WriteInWindow(font, optionText, "Comment jouer");

    //pour le boutton quitter 
    quit.setOrigin(button_width / 2.f, button_height / 2.f);
    quit.setSize(sf::Vector2f(button_width, button_height));
    quit.setPosition(sf::Vector2f(width / 2.f, height / 2.f + button_height * 2.f + offsetHeight * 2.f));
    quit.setTexture(&buttonTexture);
    sf::Text quitText;
    quitText.setPosition(width / 2.f - button_width / 2.f + offsetWidth / 1.5f, height / 2.f - button_height / 2.f + button_height * 2.f + offsetHeight * 2.f);
    WriteInWindow(font, quitText, "QUITTER");

    //gestion du menu principal 
    while (mainMenu.isOpen()) {
        sf::Event events;
        while (mainMenu.pollEvent(events)) {
            if (events.type == sf::Event::Closed)
                mainMenu.close();

            //Touche souris PLAY
            if (events.type == sf::Event::MouseButtonPressed) {
                if (events.mouseButton.button == sf::Mouse::Left) {
                    if (ButtonCollision(events.mouseButton.x + button_width / 2, events.mouseButton.y + button_height / 2, play, button_width, button_height)) {
                        /*astuce debug std::cout << "ok" << std::endl;*/
                        music.stop();
                        mainMenu.close();
                        //lacer le jeu principal
                        GameStart();
                    }
                }
            }
            //Touche souris OPTION
            if (events.type == sf::Event::MouseButtonPressed) {
                if (events.mouseButton.button == sf::Mouse::Left) {
                    if (ButtonCollision(events.mouseButton.x + button_width / 2, events.mouseButton.y + button_height / 2, option, button_width, button_height)) {
                        music.stop();
                        mainMenu.close();
                        //lacer le menu des options
                        ShowInstructions();
                    }
                }
            }
            //Touche souris QUIT
            if (events.type == sf::Event::MouseButtonPressed) {
                if (events.mouseButton.button == sf::Mouse::Left) {
                    if (ButtonCollision(events.mouseButton.x + button_width / 2, events.mouseButton.y + button_height / 2, quit, button_width, button_height)) {
                        /*astuce debug std::cout << "ok" << std::endl;*/
                        //quitter le jeu 
                        mainMenu.close();
                    }
                }
            }
            //Touche clavier  
            if (events.type == sf::Event::KeyPressed) {
                if (events.key.code == sf::Keyboard::Enter) {
                    mainMenu.close();
                    //lacer le jeu principal 
                    GameStart();
                }
            }
        }

        mainMenu.clear();
        mainMenu.draw(background);
        mainMenu.draw(titleText);
        mainMenu.draw(play);
        mainMenu.draw(playText);
        mainMenu.draw(option);
        mainMenu.draw(optionText);
        mainMenu.draw(quit);
        mainMenu.draw(quitText);
        mainMenu.display();
    }
}

int main() {
    // Lancement du menu principal
    Menu();
    return 0;
}
