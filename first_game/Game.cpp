#include "Game.h"

void Game::initVariables()
{
	this->window = nullptr;

	this->renderItems = true;
	this->deleted_bullet = false;
	this->deleted_enemy = false;

	this->point = 0;
	this->maxBullet = 10;
	this->life = 3;

	this->maxEnemies = 10;
	this->enemySpawnTimerMax = 1000.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;

	this->font.loadFromFile("Fonts/Copyduck.ttf");
	this->text.setFont(this->font);

	this->icon = "|||";
	std::stringstream ss;
	ss << "Health: " << this->icon << "\nBullets: " << this->maxBullet - this->bullets.size() << "\nPoints : " << this->point;
	std::string bullet_info = ss.str();

	this->text.setCharacterSize(20);
	this->text.setString(bullet_info);
	this->text.setFillColor(sf::Color::White);
	this->text.setPosition(10.f, 10.f);
}

void Game::initWindow()
{
	this->videoMode.height = 600;
	this->videoMode.width = 500;
	this->window = new sf::RenderWindow(this->videoMode, "Game", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(60);
}

Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initPlayer();
}

Game::~Game()
{
	delete this->window;
}

void Game::pollEvents()
{
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
			{
				this->window->close();
				break;
			}
		}
	}
}

const bool Game::isRunning() const
{
	return this->window->isOpen();
}

void Game::spawnEnemy()
{
	this->enemy.setSize(sf::Vector2f(30.f, 30.f));
	this->enemy.setFillColor(sf::Color::Green);
	this->enemy.setPosition(static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)), 0);
	this->enemies.push_back(this->enemy);

}

void Game::updateEnemy()
{
	if (this->enemies.size() < this->maxEnemies)
	{
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else
		{
			this->enemySpawnTimer += 10.f;
		}
	}
	
	for (int i = 0; i < this->enemies.size(); i++)
	{
		bool deleted_enemy = false;

		this->enemies[i].move(0.f, 2.f);

		if (this->enemies[i].getPosition().y > this->window->getSize().y)
		{
			deleted_enemy = true;
		}

		if (deleted_enemy)
		{
			this->enemies.erase(this->enemies.begin() + i);
			this->life--;
			i--;
		}
	}
}

void Game::renderEnemy()
{
	for (auto& e : this->enemies)
	{
		this->window->draw(e);
	}
}

void Game::initPlayer()
{
	this->player.setSize(sf::Vector2f(20.f, 20.f));
	this->player.setFillColor(sf::Color::White);
	this->player.setPosition(static_cast<float>(this->window->getSize().x / 2),static_cast<float>(this->window->getSize().y - this->player.getSize().y));
}

void Game::playerMovment()
{
	static bool shoot = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && this->player.getPosition().x < (this->window->getSize().x - this->player.getSize().x))
	{
		this->player.move(7.f, 0.f);
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && this->player.getPosition().x > 0)
	{
		this->player.move(-7.f, 0);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !shoot)
	{
		this->spawnBullet();
	}

	shoot = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
}

void Game::spawnBullet()
{
	if (this->bullets.size() != this->maxBullet)
	{
		this->bullet.setSize(sf::Vector2f(5.f, 9.f));
		this->bullet.setFillColor(sf::Color::Red);
		this->bullet.setPosition(static_cast<float>(this->player.getPosition().x + this->bullet.getSize().x + 2), static_cast<float>(this->player.getPosition().y));
		this->bullets.push_back(this->bullet);
	}
}

void Game::updateBullet()
{
	for (int i = 0; i < this->bullets.size(); i++)
	{
		bool deleted_bullet = false;

		for (int x = 0; x < this->enemies.size(); x++)
		{
			bool deleted_enemy = false;

			this->bullets[i].move(0.f, -5.f);

			if (this->enemies[x].getGlobalBounds().contains(this->bullets[i].getPosition()))
			{
				this->point++;
				deleted_bullet = true;
				deleted_enemy = true;
			}

			if (deleted_enemy)
			{
				this->enemies.erase(this->enemies.begin() + x);
				break;
			}
		}

		if (this->bullets[i].getPosition().y < 0)
		{
			deleted_bullet = true;
		}

		if (deleted_bullet)
		{
			this->bullets.erase(this->bullets.begin() + i);
			i--;
		}


	}
}

void Game::renderBullet()
{
	for (auto& e : this->bullets)
	{
		if (this->life == 3)
		{
			this->icon = "|||";
		}
		if (this->life == 2)
		{
			this->icon = "||";
		}
		if (this->life == 1)
		{
			this->icon = "|";
		}
		this->window->draw(e);
		std::stringstream ss;
		ss << "Health: " << this->icon << "\nBullets: " << this->maxBullet - this->bullets.size() << "\nPoints : " << this->point;
		std::string bullet_info = ss.str();
		this->text.setString(bullet_info);
	}
}

void Game::update()
{
	this->pollEvents();

	//Updating player
	if (life == 0)
	{
		this->renderItems = false;
	}
	this->updateEnemy();
	this->playerMovment();
	this->updateBullet();
}

void Game::render()
{
	this->window->clear();
	
	//Render
	if (renderItems)
	{
		this->renderEnemy();
		this->renderBullet();
		this->window->draw(this->player);
		this->window->draw(this->text);
	}
	
	if (!renderItems)
	{
		std::stringstream ss;
		ss << "You Lose\n\nPoints: " << this->point;
		std::string bullet_info = ss.str();
		this->text.setString(bullet_info);
		this->text.setFillColor(sf::Color::White);
		this->text.setCharacterSize(30);
		this->text.setPosition(185, 230);
		this->window->draw(this->text);
	}

	this->window->display();
}

