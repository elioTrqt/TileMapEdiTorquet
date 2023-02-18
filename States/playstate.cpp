#include "playstate.h"
#include "mapeditorstate.h"
#include "../Other/vectorFunctions.hpp"
#include <iostream>
#include <cmath>

CPlayState CPlayState::m_PlayState;

void CPlayState::Init()
{	
	// On met en place les vues
	m_vueMap = new sf::View(sf::FloatRect(0.f, 0.f, (float)CGameEngine::screen->getSize().x, (float)CGameEngine::screen->getSize().y));
	CGameEngine::screen->setView(*m_vueMap);

	// On r�cup�re la tileMap depuis l'�diteur
	m_tMap = new Tilemap(CMapEditorState::Instance()->getTM());

	// On r�cup�re le player
	m_player = new Player();
	m_tMap->setPlayer(m_player);
	m_tMap->spawnPlayer();
}

void CPlayState::Cleanup()
{
	delete m_tMap;
	delete m_player;
}

void CPlayState::Pause()
{
}

void CPlayState::Resume()
{
}

void CPlayState::HandleEvents(CGameEngine* game)
{
	sf::Event event;
	while (CGameEngine::screen->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			game->Quit();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Z)
			{
				k_z = true;
			}
			if (event.key.code == sf::Keyboard::S)
			{
				k_s = true;
			}
			if (event.key.code == sf::Keyboard::Q)
			{
				k_q = true;
			}
			if (event.key.code == sf::Keyboard::D)
			{
				k_d = true;
			}
			if (event.key.code == sf::Keyboard::Escape)
			{
				game->PopState();
			}
			break;
		case sf::Event::KeyReleased:
			if (event.key.code == sf::Keyboard::Z)
			{
				k_z = false;
			}
			if (event.key.code == sf::Keyboard::S)
			{
				k_s = false;
			}
			if (event.key.code == sf::Keyboard::Q)
			{
				k_q = false;
			}
			if (event.key.code == sf::Keyboard::D)
			{
				k_d = false;
			}
			break;
		default :
			break;
		}
	}
}

void CPlayState::Update(CGameEngine* game)
{

	sf::Time elapsed = m_clock.restart();

	vec2f velocity(0, 0);

	if(k_z)
		velocity.y -= 1;
	if (k_d)
		velocity.x += 1;
	if (k_s)
		velocity.y += 1;
	if (k_q)
		velocity.x -= 1;
	if (velocity.x != 0 && velocity.y != 0)
	{
		if (m_player->getVelocity().x != 0)
			velocity.y = 0;
		else
			velocity.x = 0;
	}
	
	normalize(velocity);

	m_player->updateVelocity(velocity);
	m_player->update(elapsed);

	m_tMap->handleCollisions(m_player);
	m_tMap->m_activeLevel = m_player->m_layer;

	vec2f center(m_player->getPosition() + m_player->getSize() / 2.f);
	center.x = std::max(center.x, m_tMap->getPosition().x + m_vueMap->getSize().x / 2.f);
	center.x = std::min(center.x, m_tMap->getPosition().x + m_tMap->getSize().x - m_vueMap->getSize().x / 2.f);
	center.y = std::max(center.y, m_tMap->getPosition().y + m_vueMap->getSize().y / 2.f);
	center.y = std::min(center.y, m_tMap->getPosition().y + m_tMap->getSize().y - m_vueMap->getSize().y / 2.f);

	m_vueMap->setCenter(center);
}

void CPlayState::Draw(CGameEngine* game)
{
	CGameEngine::screen->clear();

	CGameEngine::screen->setView(*m_vueMap);
	CGameEngine::screen->draw(*m_tMap);

	CGameEngine::screen->display();
}
