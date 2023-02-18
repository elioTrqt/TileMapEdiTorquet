#include "escapestate.h"
#include "mapeditorstate.h"
#include <iostream>

CEscapeState CEscapeState::m_EscapeState;

void CEscapeState::Init() {
	m_vue = CMapEditorState::Instance()->getView();

	m_choix[0] = new Bouton("Reprendre", "Fonts/Retron.ttf", 30);
	m_choix[0]->setPosition(vec2f(m_vue.getCenter().x - m_choix[0]->getSize().x / 2, m_vue.getCenter().y - 128));
	m_choix[1] = new Bouton("Sauvegarder", "Fonts/Retron.ttf", 30);
	m_choix[1]->setPosition(vec2f(m_vue.getCenter().x - m_choix[1]->getSize().x / 2, m_choix[0]->getPosition().y + m_choix[0]->getSize().y + 32));
	m_choix[2] = new Bouton("Quitter", "Fonts/Retron.ttf", 30);
	m_choix[2]->setPosition(vec2f(m_vue.getCenter().x - m_choix[2]->getSize().x / 2, m_choix[1]->getPosition().y + m_choix[1]->getSize().y + 32));

	m_bg = sf::RectangleShape(vec2f(300, 500));
	m_bg.setPosition(m_vue.getCenter().x - 150, 200);
	m_bg.setFillColor(sf::Color::Cyan);
	m_bg.setOutlineColor(sf::Color::Black);
	m_bg.setOutlineThickness(1.5);
}

void CEscapeState::Cleanup() {
	for (int i = 0; i < 3; i++)
	{
		delete m_choix[i];
	}

	if (m_saisie != nullptr)
		delete m_saisie;
}

void CEscapeState::Pause() {}
void CEscapeState::Resume() {}

void CEscapeState::HandleEvents(CGameEngine* game) {

	vec2i pos = sf::Mouse::getPosition(*CGameEngine::screen);
	vec2f mousePos = CGameEngine::screen->mapPixelToCoords(pos, m_vue);

	sf::Event event;

	while (CGameEngine::screen->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed :
			game->Quit();
			break;
		case sf::Event::KeyPressed :
			if (event.key.code == sf::Keyboard::Escape)
			{
				if (m_saisie == nullptr)
					game->PopState();
				else
				{
					delete m_saisie;
					m_saisie = nullptr;
				}
			}
			break;
		case sf::Event::MouseButtonPressed :
			if (m_saisie == nullptr)
			{
				if (m_choix[0]->isMouseOver(mousePos))
					game->PopState();
				else if (m_choix[1]->isMouseOver(mousePos))
				{
					m_saisie = new TextGetter("Fonts/Retron.ttf", vec2f(550, 350));
				}
				else if (m_choix[2]->isMouseOver(mousePos))
					game->Quit();
			}
			else
			{
				std::string s;
				if (m_saisie->submit(mousePos, s))
				{
					s = "Save/" + s;
					CMapEditorState::Instance()->saveTM(s.c_str());

					delete m_saisie;
					m_saisie = nullptr;
				}
			}
			break;
		case sf::Event::TextEntered :
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

void CEscapeState::Update(CGameEngine* game) {

	vec2i pos = sf::Mouse::getPosition(*CGameEngine::screen);
	vec2f mousePos = CGameEngine::screen->mapPixelToCoords(pos, m_vue);

	if (m_saisie == nullptr)
	{
		for (int i = 0; i < 3; i++)
		{
			m_choix[i]->update(mousePos);
		}
	}
	else
		m_saisie->update(mousePos);
}

void CEscapeState::Draw(CGameEngine* game) {

	CGameEngine::screen->clear();

	CGameEngine::screen->draw(m_bg);

	for (int i = 0; i < 3; i++)
	{
		CGameEngine::screen->draw(*m_choix[i]);
	}
	if (m_saisie != nullptr)
	{
		CGameEngine::screen->draw(*m_saisie);
	}

	CGameEngine::screen->display();
}

