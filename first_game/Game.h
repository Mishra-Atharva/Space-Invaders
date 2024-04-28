#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>
#include <sstream>


class Game
{
private:

	//Window
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;

	//Player
	int point;
	int life;
	std::string icon;
	sf::RectangleShape player;

	//Enemy
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;
	int maxEnemies;
	float enemySpawnTimer;
	float enemySpawnTimerMax;

	//Bullets
	int maxBullet;
	std::vector<sf::RectangleShape> bullets;
	sf::RectangleShape bullet;

	//Render Check
	bool deleted_enemy;
	bool deleted_bullet;

	//Text
	sf::Font font;
	sf::Text text;

	//Functions
	void initVariables();
	void initWindow();

public:

	//Constructor / Deconstructor
	Game();
	virtual ~Game();
	
	bool renderItems;

	//Functions
	void pollEvents();
	const bool isRunning() const;

	//Game Function
	void initPlayer();
	void playerMovment();

	void spawnBullet();
	void updateBullet();
	void renderBullet();

	void spawnEnemy();
	void updateEnemy();
	void renderEnemy();

	//Update and Render
	void update();
	void render();
};

