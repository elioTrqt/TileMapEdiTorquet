#ifndef ESCAPESTATE_H
#define ESCAPESTATE_H

#include <SFML/Graphics.hpp>
#include "gamestate.h"
#include "../Bouton/Bouton.hpp"
#include "../Bouton/TextGetter.hpp"
#include "../Other/Alias.h"

class CEscapeState : public CGameState
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(CGameEngine* game);
	void Update(CGameEngine* game);
	void Draw(CGameEngine* game);

	static CEscapeState* Instance() {
		return &m_EscapeState;
	}

protected:
	CEscapeState() { }

private:
	static CEscapeState m_EscapeState;

	sf::View m_vue;
	sf::RectangleShape m_bg;
	Bouton* m_choix[3];
	TextGetter* m_saisie = nullptr;

};

#endif