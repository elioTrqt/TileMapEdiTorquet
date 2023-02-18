#include <iostream>
#include <sstream>
#include "mapeditorstate.h"
#include "playstate.h"
#include "escapestate.h"
#include "../Other/vectorFunctions.hpp"
#include "../Entity/Props.h"
#include <fstream>

CMapEditorState CMapEditorState::m_EditorState;

void CMapEditorState::Init()
{
	// Initialisation des vues
	m_vueHUD = new sf::View(sf::FloatRect(0.f, 0.f, (float)CGameEngine::screen->getSize().x, (float)CGameEngine::screen->getSize().y));
	m_vueMap = new sf::View(sf::FloatRect(-128.f, -64.f, (float)CGameEngine::screen->getSize().x, (float)CGameEngine::screen->getSize().y));

	// HUD
	initHUD();

	// Player
	m_player = new Player(vec2f(0, 0), 0);

	// Tilemap
	m_tMap = new Tilemap(vec2i(32, 32), "Textures/Wall_1.png", vec2i(64, 64), 1);
	m_tMap->setPlayer(m_player);

	// Initialisation des formes dessin�s a l'�cran

	m_colliderDraw = new sf::VertexArray(sf::LineStrip);

	m_highlightTile = new sf::RectangleShape(vec2f(32, 32));
	m_highlightTile->setFillColor(sf::Color(255, 0, 0, 40));

	m_demiHighlightTile = new sf::RectangleShape(vec2f(16, 16));
	m_demiHighlightTile->setFillColor(sf::Color(255, 0, 0, 40));

	m_cursor = new sf::CircleShape(2.f);
	m_cursor->setFillColor(sf::Color(255, 0, 0));
	m_cursor->setOrigin(2.f, 2.f);

	std::cout << "CMapEditorState Init" << std::endl;
}

void CMapEditorState::initHUD()
{
	// Barre du haut
	m_topBar = new sf::RectangleShape(vec2f((float)CGameEngine::screen->getSize().x, 64));
	m_topBar->setPosition(0, 0);
	m_topBar->setFillColor(sf::Color::Cyan);
	m_topBar->setOutlineThickness(-1);
	m_topBar->setOutlineColor(sf::Color::Black);

	// Barre laterale
	m_leftBar = new sf::RectangleShape(vec2f(128, (float)CGameEngine::screen->getSize().y));
	m_leftBar->setPosition(0, 0);
	m_leftBar->setFillColor(sf::Color::Cyan);
	m_leftBar->setOutlineThickness(-1);
	m_leftBar->setOutlineColor(sf::Color::Black);

	// Tous les boutons 
	m_levelBoutons.push_back(new Bouton("Layer 0", "Fonts/Retron.ttf", 20, sf::Color::Black, sf::Color::White, vec2f(128 + 16, 16)));
	m_levelBoutons[0]->push(true);

	m_addRmLevelBoutons[0] = new Bouton("Remove Layer", "Fonts/Retron.ttf", 20);
	m_addRmLevelBoutons[0]->setPosition(vec2f(m_vueHUD->getSize().x - m_addRmLevelBoutons[0]->getSize().x - 16, 16));
	m_addRmLevelBoutons[1] = new Bouton("Add Layer", "Fonts/Retron.ttf", 20);
	m_addRmLevelBoutons[1]->setPosition(vec2f(m_addRmLevelBoutons[0]->getPosition().x - m_addRmLevelBoutons[1]->getSize().x - 16, 16));

	m_colliderBoutons[0] = new Bouton("Send Collider", "Fonts/Retron.ttf", 20);
	m_colliderBoutons[0]->setPosition(vec2f(m_vueHUD->getSize().x - m_colliderBoutons[0]->getSize().x - 16, 16));
	m_colliderBoutons[1] = new Bouton("Remove Collider", "Fonts/Retron.ttf", 20);
	m_colliderBoutons[1]->setPosition(vec2f(m_colliderBoutons[0]->getPosition().x - m_colliderBoutons[1]->getSize().x - 16, 16));

	m_modeBouton = new Bouton("Mode", "Fonts/Retron.ttf", 20, sf::Color::Black, sf::Color::White, vec2f(16, 16));

	// Les Palettes
	m_tPalette = new TilePalette(vec2f(0.f, 64.f), 2, vec2i(39, 39), "Textures/Wall_1.png", vec2i(32, 32));

	std::vector<std::string> propList({ "Textures/Tree1.png", "Textures/Tree2.png", "Textures/Tree3.png", "Textures/Bush1.png", "Textures/Bush2.png", "Textures/Bush3.png", "Textures/Bush4.png", "Textures/Bush5.png", "Textures/Bush6.png" });
	m_pPalette = new PropPalette(vec2f(0.f, 64.f), 2, vec2i(39, 39), propList);

	std::vector<std::string> stairsList({ "Textures/Stair1.png" });
	m_sPalette = new PropPalette(vec2f(0.f, 64.f), 2, vec2i(39, 39), stairsList);
}

void CMapEditorState::Cleanup()
{
	delete m_vueHUD;
	delete m_vueMap;

	delete m_tPalette;
	delete m_pPalette;
	delete m_sPalette;
	delete m_tMap;

	delete m_topBar;
	delete m_leftBar;

	delete m_highlightTile;
	delete m_demiHighlightTile;
	delete m_colliderDraw;
	delete m_cursor;

	for (int i = 0; i < (int)m_levelBoutons.size(); i++)
		delete m_levelBoutons[i];

	delete m_colliderBoutons[0];
	delete m_colliderBoutons[1];

	delete m_addRmLevelBoutons[0];
	delete m_addRmLevelBoutons[1];

	delete m_modeBouton;

	delete m_player;

	std::cout << "CMapEditorState Cleanup" << std::endl;
}

void CMapEditorState::Pause()
{
	std::cout << "CMapEditorState Pause" << std::endl;
}

void CMapEditorState::Resume()
{
	std::cout << "CMapEditorState Resume" << std::endl;
}

void CMapEditorState::HandleEvents(CGameEngine* game)
{
	// On r�cup�re la position du curseur sur les 2 vues

	vec2i pos = sf::Mouse::getPosition(*CGameEngine::screen);
	vec2f mousePosHUD = CGameEngine::screen->mapPixelToCoords(pos, *m_vueHUD);
	// vec2f mousePosMap = CGameEngine::screen->mapPixelToCoords(pos, *m_vueMap);

	sf::Event event;

	while (CGameEngine::screen->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			game->Quit();
			break;

		case sf::Event::MouseButtonPressed:
			mousePressedEvent(event.mouseButton.button);
			break;

		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left)
				k_leftClick = false;
			else if (event.mouseButton.button == sf::Mouse::Right)
				k_rightClick = false;
			break;

		case sf::Event::MouseWheelScrolled:
			if (mousePosHUD.x < m_leftBar->getSize().x && mousePosHUD.y > m_topBar->getSize().y) // la souris est sur la tile palette donc on scroll
			{
				int sens = (event.mouseWheelScroll.delta < 0 ? -1 : 1);
				switch (m_mode)
				{
				case 0:
					m_tPalette->move(0.f, 48.f * sens);
					break;
				case 1:
					m_pPalette->move(0.f, 48.f * sens);
					break;
				case 2:
					m_sPalette->move(0.f, 48.f * sens);
					break;
				default:
					break;
				}
			}
			else if (mousePosHUD.y > m_topBar->getSize().y) // la souris est sur la map donc on zoom
			{
				if (event.mouseWheelScroll.delta < 0)
				{
					m_vueMap->zoom(1.1f);
					m_vueMap->setCenter((m_vueMap->getCenter() + mousePosHUD) / 2.f);
				}
				else
				{
					m_vueMap->zoom(0.9f);
					m_vueMap->setCenter((m_vueMap->getCenter() + mousePosHUD) / 2.f);
				}
			}
			break;
		case sf::Event::KeyPressed:
			keyPressedEvent(event.key.code, game);
			break;

		case sf::Event::KeyReleased:
			if (event.key.code == sf::Keyboard::Z)
			{
				k_z = false;
			}
			else if (event.key.code == sf::Keyboard::S)
			{
				k_s = false;
			}
			else if (event.key.code == sf::Keyboard::Q)
			{
				k_q= false;
			}
			else if (event.key.code == sf::Keyboard::D)
			{
				k_d= false;
			}
			else if (event.key.code == sf::Keyboard::P)
			{
				m_resizeFactor = 0;
			}
			else if (event.key.code == sf::Keyboard::M)
			{
				m_resizeFactor = 0;
			}
			else if (event.key.code == sf::Keyboard::LControl)
			{
				k_ctrl = false;
			}
			break;
		default :
			break;
		}
	}
	
}

void CMapEditorState::mousePressedEvent(const sf::Mouse::Button& button)
{
	vec2i pos = sf::Mouse::getPosition(*CGameEngine::screen);
	vec2f mousePosHUD = CGameEngine::screen->mapPixelToCoords(pos, *m_vueHUD);
	vec2f mousePosMap = CGameEngine::screen->mapPixelToCoords(pos, *m_vueMap);

	if (button == sf::Mouse::Left)
	{
		if (mousePosHUD.x < m_leftBar->getSize().x && mousePosHUD.y > m_topBar->getSize().y) // la souris est sur la tile palette
		{
			mousePressedLateral();
		}
		else if (mousePosHUD.y < m_topBar->getSize().y) // la souris est sur la barre du haut
		{
			mousePressedUp();
		}
		else
		{
			mousePressedMap();
		}
	}
	else if (button == sf::Mouse::Right)
	{
		if (m_mode == 4)
		{
			k_rightClick = true;
		}
		else if (mousePosHUD.x > m_leftBar->getSize().x && mousePosHUD.y > m_topBar->getSize().y) // On change le point de spawn
		{
			m_tMap->setSpawn(mousePosMap, m_tMap->m_activeLevel);
			m_tMap->spawnPlayer();
		}
	}
}

void CMapEditorState::mousePressedUp()
{
	vec2i pos = sf::Mouse::getPosition(*CGameEngine::screen);
	vec2f mousePosHUD = CGameEngine::screen->mapPixelToCoords(pos, *m_vueHUD);

	int s;
	if (select(mousePosHUD, m_levelBoutons, s)) // Changement de layer
	{
		m_tMap->setActiveLevel(s);
	}
	else if (m_modeBouton->isMouseOver(mousePosHUD)) // Changement de mode PropMode ou TileMode
	{
		m_mode = editMode((m_mode + 1) % 5);
		if (m_mode == 4)
			m_tMap->m_pathMode = true;
		else
			m_tMap->m_pathMode = false;
	}
	else if (m_mode == 3) // mode collisions
	{
		if (m_colliderBoutons[1]->isMouseOver(mousePosHUD)) // remove collider
		{
			if (!m_actualCollider.empty())
			{
				m_actualCollider.clear();
				m_colliderDraw->clear();
				m_colliderClosed = false;
			}
			else
			{
				m_tMap->removeCollider();
			}
		}
		else if (m_colliderBoutons[0]->isMouseOver(mousePosHUD)) // send collider
		{
			if (m_colliderClosed)
			{
				m_tMap->addCollider(m_actualCollider);
				m_actualCollider.clear();
				m_colliderDraw->clear();
				m_colliderClosed = false;
			}
			else
			{
				std::cout << "Collider non ferm� : impossible d'envoyer" << std::endl;
			}
		}
	}
	else // mode dessin
	{
		if (m_addRmLevelBoutons[1]->isMouseOver(mousePosHUD)) // add level
		{
			m_tMap->addLevel();
			std::stringstream txt;
			txt << "Layer " << m_levelBoutons.size();
			m_levelBoutons.push_back(new Bouton(txt.str(), "Fonts/Retron.ttf", 20, sf::Color::Black, sf::Color::White, vec2f(m_levelBoutons.back()->getPosition().x + m_levelBoutons.back()->getSize().x + 16, 16)));
		}
		else if (m_addRmLevelBoutons[0]->isMouseOver(mousePosHUD)) // remove level
		{
			if (m_tMap->removeLevel())
			{
				m_levelBoutons.pop_back();
				if (m_tMap->m_activeLevel == (int)m_levelBoutons.size())
				{
					m_levelBoutons.back()->push(true);
				}
			}
		}
	}
}

void CMapEditorState::mousePressedLateral()
{
	vec2i pos = sf::Mouse::getPosition(*CGameEngine::screen);
	vec2f mousePosHUD = CGameEngine::screen->mapPixelToCoords(pos, *m_vueHUD);
	vec2f mousePosMap = CGameEngine::screen->mapPixelToCoords(pos, *m_vueMap);

	int x;
	if (m_mode == 0) // Choix d'une nouvelle tile
		m_tPalette->select(mousePosHUD, x);
	else if (m_mode == 1 && m_pPalette->select(mousePosHUD, x)) // Choix d'une nouvelle d�co
	{
		if (m_actualProp)
			delete m_actualProp;
		switch (x)
		{
		case 0:
			m_actualProp = new PropTree1(mousePosMap, m_tMap->m_activeLevel);
			break;
		case 1:
			m_actualProp = new PropTree2(mousePosMap, m_tMap->m_activeLevel);
			break;
		case 2:
			m_actualProp = new PropTree3(mousePosMap, m_tMap->m_activeLevel);
			break;
		case 3:
			m_actualProp = new PropBush1(mousePosMap, m_tMap->m_activeLevel);
			break;
		case 4:
			m_actualProp = new PropBush2(mousePosMap, m_tMap->m_activeLevel);
			break;
		case 5:
			m_actualProp = new PropBush3(mousePosMap, m_tMap->m_activeLevel);
			break;
		case 6:
			m_actualProp = new PropBush4(mousePosMap, m_tMap->m_activeLevel);
			break;
		case 7:
			m_actualProp = new PropBush5(mousePosMap, m_tMap->m_activeLevel);
			break;
		case 8:
			m_actualProp = new PropBush6(mousePosMap, m_tMap->m_activeLevel);
			break;
		default:
			break;
		}
	}
	else if (m_mode == 2 && m_sPalette->select(mousePosHUD, x)) // Choix d'un nouvelle d�escalier
	{
		if (m_actualProp)
			delete m_actualProp;
		switch (x)
		{
		case 0:
			m_actualProp = new Stairs(mousePosMap, m_tMap->m_activeLevel);
			break;
		default:
			break;
		}
	}
}

void CMapEditorState::mousePressedMap()
{
	vec2i pos = sf::Mouse::getPosition(*CGameEngine::screen);
	vec2f mousePosMap = CGameEngine::screen->mapPixelToCoords(pos, *m_vueMap);

	if (m_mode == 3 && !m_colliderClosed) // la souris est sur la map en mode collider
	{
		addPoint(m_tMap->getDemiOverflyTile(mousePosMap)); // on ajoute un point au collider
	}
	else if (m_mode == 4 || (m_mode == 0 && (m_tPalette->getSelected() != -1)))
	{
		k_leftClick = true;
	}
	else if (m_mode == 1 && m_pPalette->getSelected() != -1) // On dessine des d�corations
	{
		switch (m_pPalette->getSelected())
		{
		case 0:
			m_tMap->addDecoration(PropTree1(mousePosMap, m_tMap->m_activeLevel));
			break;
		case 1:
			m_tMap->addDecoration(PropTree2(mousePosMap, m_tMap->m_activeLevel));
			break;
		case 2:
			m_tMap->addDecoration(PropTree3(mousePosMap, m_tMap->m_activeLevel));
			break;
		case 3:
			m_tMap->addDecoration(PropBush1(mousePosMap, m_tMap->m_activeLevel));
			break;
		case 4:
			m_tMap->addDecoration(PropBush2(mousePosMap, m_tMap->m_activeLevel));
			break;
		case 5:
			m_tMap->addDecoration(PropBush3(mousePosMap, m_tMap->m_activeLevel));
			break;
		case 6:
			m_tMap->addDecoration(PropBush4(mousePosMap, m_tMap->m_activeLevel));
			break;
		case 7:
			m_tMap->addDecoration(PropBush5(mousePosMap, m_tMap->m_activeLevel));
			break;
		case 8:
			m_tMap->addDecoration(PropBush6(mousePosMap, m_tMap->m_activeLevel));
			break;
		default:
			break;
		}
	}
	else if (m_mode == 2 && m_sPalette->getSelected() != -1) // On dessine des escaliers
	{
		switch (m_sPalette->getSelected())
		{
		case 0:
			m_tMap->addStair(Stairs(m_tMap->getDemiOverflyTile(mousePosMap), m_tMap->m_activeLevel));
		default:
			break;
		}
	}
}

void CMapEditorState::keyPressedEvent(const sf::Keyboard::Key& key, CGameEngine* game)
{
	if (key == sf::Keyboard::Escape)
	{
		game->PushState(CEscapeState::Instance());
	}
	else if (key == sf::Keyboard::Space)
	{
		game->PushState(CPlayState::Instance());
	}
	if (key == sf::Keyboard::LControl) // Collider Mode On / Off
	{
		k_ctrl = true;
	}

	// TileMap Mouvement

	if (key == sf::Keyboard::Z)
	{
		if (k_ctrl)
		{
			switch (m_mode)
			{
			case 1:
				m_tMap->removeDecoration();
				break;
			case 2:
				m_tMap->removeStair();
				break;
			case 3:
				m_tMap->removeCollider();
				break;
			default:
				break;
			}
		}
		else
			k_z = true;
	}
	if (key == sf::Keyboard::S)
	{
		k_s = true;
	}
	if (key == sf::Keyboard::Q)
	{
		k_q = true;
	}
	if (key == sf::Keyboard::D)
	{
		k_d = true;
	}

	// Resize de la TileMap

	if (key == sf::Keyboard::P) // P pour agrandire
	{
		m_resizeFactor = 1;
	}
	if (key == sf::Keyboard::M && m_resizeFactor != 1) // M pour r�tr�cire (P est prioritaire pour esquiver les accidents)
	{
		m_resizeFactor = -1;
	}
	if (key == sf::Keyboard::Up && m_resizeFactor != 0) // On ressize par le haut
	{
		m_tMap->resizeTM(vec2i(0, m_resizeFactor * 10), false);
	}
	if (key == sf::Keyboard::Down && m_resizeFactor != 0) // On ressize par le bas
	{
		m_tMap->resizeTM(vec2i(0, m_resizeFactor * 10), true);
	}
	if (key == sf::Keyboard::Left && m_resizeFactor != 0) // On ressize par la gauche
	{
		m_tMap->resizeTM(vec2i(m_resizeFactor * 10, 0), false);
	}
	if (key == sf::Keyboard::Right && m_resizeFactor != 0) // On ressize par la droite
	{
		m_tMap->resizeTM(vec2i(m_resizeFactor * 10, 0), true);
	}
}

void CMapEditorState::Update(CGameEngine* game)
{
	// On r�cup�re la position de la souris

	vec2i pos = sf::Mouse::getPosition(*CGameEngine::screen);
	vec2f mousePosHUD = CGameEngine::screen->mapPixelToCoords(pos, *m_vueHUD);
	vec2f mousePosMap = CGameEngine::screen->mapPixelToCoords(pos, *m_vueMap);

	// Update de chaque element

	m_player->update(sf::Time::Zero);
	m_tMap->update(sf::Time::Zero);

	m_tPalette->update(mousePosHUD);
	m_pPalette->update(mousePosHUD);
	m_sPalette->update(mousePosHUD);

	m_modeBouton->update(mousePosHUD);

	m_addRmLevelBoutons[0]->update(mousePosHUD);
	m_addRmLevelBoutons[1]->update(mousePosHUD);

	for (int i = 0; i < (int)m_levelBoutons.size(); i++)
		m_levelBoutons[i]->update(mousePosHUD);
	
	switch (m_mode)
	{
	case 0:
		m_highlightTile->setPosition(m_tMap->getOverflyTile(mousePosMap));
		if (k_leftClick && mousePosHUD.x > m_leftBar->getSize().x && mousePosHUD.y > m_topBar->getSize().y)
		{
			m_tMap->changeTile(m_tMap->getOverflyTile(mousePosMap), m_tPalette->getSelected());
		}
		break;
	case 1:
		if (m_actualProp != nullptr)
		{
			m_actualProp->update(sf::Time::Zero);
			m_actualProp->setPosition(mousePosMap);
		}
		break;
	case 2:
		if (m_actualProp != nullptr)
		{
			m_actualProp->update(sf::Time::Zero);
			m_actualProp->setPosition(m_tMap->getDemiOverflyTile(mousePosMap));
		}
		break;
	case 3:
		m_cursor->setPosition(m_tMap->getDemiOverflyTile(mousePosMap));
		m_colliderBoutons[0]->update(mousePosHUD);
		m_colliderBoutons[1]->update(mousePosHUD);
		break;
	case 4:
		m_demiHighlightTile->setPosition(m_tMap->getDemiOverflyTile(mousePosMap));
		if ((k_leftClick || k_rightClick) && mousePosHUD.x > m_leftBar->getSize().x && mousePosHUD.y > m_topBar->getSize().y)
		{
			m_tMap->changePath(m_tMap->getDemiOverflyTile(mousePosMap), (k_leftClick ? false : true));
		}
		break;
	}

	// TileMap Mouvement

	vec2f velocity(0, 0);
	if(k_z)
		velocity.y -= 1;
	if(k_s)
		velocity.y += 1;
	if(k_q)
		velocity.x -= 1;
	if(k_d)
		velocity.x += 1;

	normalize(velocity);
	m_vueMap->move(velocity);
}

void CMapEditorState::Draw(CGameEngine* game)
{
	CGameEngine::screen->clear(sf::Color::Black);

	CGameEngine::screen->setView(*m_vueMap);

	// On dessine la TileMap d'abord
	CGameEngine::screen->draw(*m_tMap);

	// On dessine le curseur actuel
	switch (m_mode)
	{
	case 0:
		CGameEngine::screen->draw(*m_highlightTile);

		CGameEngine::screen->setView(*m_vueHUD);
		CGameEngine::screen->draw(*m_leftBar);

		CGameEngine::screen->draw(*m_tPalette);
		break;
	case 1:
		if (m_actualProp != nullptr)
			CGameEngine::screen->draw(*m_actualProp);

		CGameEngine::screen->setView(*m_vueHUD);
		CGameEngine::screen->draw(*m_leftBar);

		CGameEngine::screen->draw(*m_pPalette);
		break;
	case 2:
		if (m_actualProp != nullptr)
			CGameEngine::screen->draw(*m_actualProp);

		CGameEngine::screen->setView(*m_vueHUD);
		CGameEngine::screen->draw(*m_leftBar);

		CGameEngine::screen->draw(*m_sPalette);
		break;
	case 3:
		CGameEngine::screen->draw(*m_cursor);
		CGameEngine::screen->draw(*m_colliderDraw);

		CGameEngine::screen->setView(*m_vueHUD);
		CGameEngine::screen->draw(*m_leftBar);
		break;
	case 4:
		CGameEngine::screen->draw(*m_demiHighlightTile);

		CGameEngine::screen->setView(*m_vueHUD);
		CGameEngine::screen->draw(*m_leftBar);
		break;
	default:
		break;
	}

	// On dessine le menu par dessus
	

	CGameEngine::screen->draw(*m_topBar);
	for (int i = 0; i < (int)m_levelBoutons.size(); i++)
		CGameEngine::screen->draw(*m_levelBoutons[i]);

	if (m_mode == 3)
	{
		CGameEngine::screen->draw(*m_colliderBoutons[0]);
		CGameEngine::screen->draw(*m_colliderBoutons[1]);
	}
	else
	{
		CGameEngine::screen->draw(*m_addRmLevelBoutons[0]);
		CGameEngine::screen->draw(*m_addRmLevelBoutons[1]);
	}

	CGameEngine::screen->draw(*m_modeBouton);

	CGameEngine::screen->display();
}

bool CMapEditorState::saveTM(const char* file)
{
	std::ofstream flux(file, std::ios::out | std::ios::binary);
	if (flux)
	{
		m_tMap->saveToFile(flux);
		std::cout << "Tilamap Saved to " << file << std::endl;
		return true;
	}
	else
	{
		std::cout << "Cannot open " << file << std::endl;
		return false;
	}
}

bool CMapEditorState::loadTM(const char* file)
{
	std::ifstream flux(file, std::ios::in | std::ios::binary);
	if (flux)
	{
		delete m_tMap;
		m_tMap = new Tilemap(flux);
		m_tMap->setPlayer(m_player);

		std::cout << "Tilamap Loaded from " << file << std::endl;

		for (uint32_t i = 1; i < m_tMap->m_layerCount; i++)
		{
			std::stringstream txt;
			txt << "Layer " << m_levelBoutons.size();
			m_levelBoutons.push_back(new Bouton(txt.str(), "Fonts/Retron.ttf", 20, sf::Color::Black, sf::Color::White, vec2f(m_levelBoutons.back()->getPosition().x + m_levelBoutons.back()->getSize().x + 16, 16)));
		}

		return true;
	}
	else
	{
		std::cout << "Cannot open " << file << std::endl;
		return false;
	}
}

void CMapEditorState::addPoint(const vec2f& pos)
{
	if (pos.x < 0 || pos.y < 0 || pos.x > m_tMap->getSize().x || pos.y > m_tMap->getSize().y)
	{
		std::cout << "Ajout de Collider point impossible : tile hors de la map (addPoint())" << std::endl;
	}
	else
	{
		if (m_actualCollider.empty())
		{
			m_actualCollider.push_back(pos);
			m_colliderDraw->append(sf::Vertex(pos, sf::Color(255, 0, 0)));
		}
		else if (m_actualCollider.back() != pos && m_actualCollider[0] != pos && (m_actualCollider.back().x == pos.x || m_actualCollider.back().y == pos.y))
		{
			m_actualCollider.push_back(pos);
			m_colliderDraw->append(sf::Vertex(pos, sf::Color(255, 0, 0)));
		}
		else if (m_actualCollider[0] == pos && (m_actualCollider.back().x == m_actualCollider[0].x || m_actualCollider.back().y == m_actualCollider[0].y))
		{
			m_colliderDraw->append(sf::Vertex(pos, sf::Color(255, 0, 0)));
			m_colliderClosed = true;
		}
		
	}
}