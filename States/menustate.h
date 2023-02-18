#ifndef MENUSTATE_H
#define MENUSTATE_H

#include <SFML/Graphics.hpp>
#include "gamestate.h"

#include "../Bouton/Bouton.hpp"
#include "../Bouton/TextGetter.hpp"
#include "../Other/Alias.h"
#include <vector>

class CMenuState : public CGameState
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(CGameEngine* game);
	void Update(CGameEngine* game);
	void Draw(CGameEngine* game);

	static CMenuState* Instance() {
		return &m_MenuState;
	}

	bool getPressedButton(const vec2f& pos, int& butIndex);

protected:
	CMenuState() { }

private:
	static CMenuState m_MenuState;

	std::vector<Bouton*> m_boutons;
	sf::View* m_vue = nullptr;
	TextGetter* m_saisie = nullptr;
};

#endif