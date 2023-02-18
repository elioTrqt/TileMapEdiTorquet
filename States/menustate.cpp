#include <iostream>
#include "menustate.h"
#include "mapeditorstate.h"
#include <string>

CMenuState CMenuState::m_MenuState;

void CMenuState::Init()
{
	// On setup la vue
	m_vue = new sf::View(sf::FloatRect(0.f, 0.f, (float)CGameEngine::screen->getSize().x, (float)CGameEngine::screen->getSize().y));
	CGameEngine::screen->setView(*m_vue);

	// On setup les boutons
	Bouton* b1 = new Bouton("Nouvelle TileMap", "Fonts/Retron.ttf", 40);
	Bouton* b2 = new Bouton("Charger TileMap", "Fonts/Retron.ttf", 40);
	Bouton* b3 = new Bouton("Quitter", "Fonts/Retron.ttf", 40);
	b1->setPosition(vec2f(m_vue->getCenter().x - (b1->getSize().x / 2), m_vue->getCenter().y - b1->getSize().y));
	b2->setPosition(vec2f(m_vue->getCenter().x - (b2->getSize().x / 2), b1->getPosition().y + b1->getSize().y + 32));
	b3->setPosition(vec2f(m_vue->getCenter().x - (b3->getSize().x / 2), b2->getPosition().y + b2->getSize().y + 32));
	m_boutons.push_back(b1);
	m_boutons.push_back(b2);
	m_boutons.push_back(b3);

	std::cout << "CMenuState Init" << std::endl;
}

void CMenuState::Cleanup()
{
	for (int i = 0; i < (int)m_boutons.size(); i++)
	{
		delete m_boutons[i];
	}
	delete m_vue;

	if (m_saisie != nullptr)
		delete m_saisie;

	std::cout << "CMenuState Cleanup" << std::endl;
}

void CMenuState::Pause()
{
	std::cout << "CMenuState Pause" << std::endl;
}

void CMenuState::Resume()
{
	std::cout << "CMenuState Resume" << std::endl;
}

void CMenuState::HandleEvents(CGameEngine* game)
{
	vec2i pos = sf::Mouse::getPosition(*CGameEngine::screen);
	vec2f mousePos = CGameEngine::screen->mapPixelToCoords(pos, *m_vue);

	sf::Event event;
	while (CGameEngine::screen->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed :
			game->Quit();
			break;
		
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				int i;
				if (m_saisie == nullptr && getPressedButton(vec2f((float)event.mouseButton.x, (float)event.mouseButton.y), i))
				{
					switch (i)
					{
					case 0:
						game->ChangeState(CMapEditorState::Instance());
						break;
					case 1:
						m_saisie = new TextGetter("Fonts/Retron.ttf", vec2f(550, 350));
						break;
					case 2:
						game->Quit();
						break;
					default :
						break;
					}
				}
				else if (m_saisie != nullptr)
				{
					std::string s;
					if (m_saisie->submit(vec2f((float)event.mouseButton.x, (float)event.mouseButton.y), s))
					{
						s = "Save/" + s;
						delete m_saisie;
						m_saisie = nullptr;

						game->ChangeState(CMapEditorState::Instance());
						CMapEditorState::Instance()->loadTM(s.c_str());
					}
				}
			}
		case sf::Event::TextEntered:
			if (m_saisie != nullptr)
			{
				m_saisie->update(mousePos, event.text.unicode);
			}
			break;
		
		default :
			break;
		}
	}
}

void CMenuState::Update(CGameEngine* game) 
{
	vec2i pos = sf::Mouse::getPosition(*CGameEngine::screen);
	vec2f mousePos = CGameEngine::screen->mapPixelToCoords(pos, *m_vue);

	for (int i = 0; i < (int)m_boutons.size(); i++)
	{
		m_boutons[i]->update(mousePos);
	}

	if (m_saisie != nullptr)
		m_saisie->update(mousePos);
}

void CMenuState::Draw(CGameEngine* game) 
{
	CGameEngine::screen->clear(sf::Color::Blue);

	for (int i = 0; i < (int)m_boutons.size(); i++)
	{
		CGameEngine::screen->draw(*m_boutons[i]);
	}

	if (m_saisie != nullptr)
		CGameEngine::screen->draw(*m_saisie);

	CGameEngine::screen->display();
}

bool CMenuState::getPressedButton(const vec2f& pos, int& butindex)
{
	for (int i = 0; i < (int)m_boutons.size(); i++)
	{
		if (m_boutons[i]->isMouseOver(pos))
		{
			butindex = i;
			return true;
		}
	}
	
	return false;
}
