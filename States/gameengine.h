#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "../Other/Alias.h"

class CGameState;

class CGameEngine
{
public:

	void Init(const char* title, int width=1600, int height=900, bool fullscreen=false);
	void Cleanup();

	void ChangeState(CGameState* state);
	void PushState(CGameState* state);
	void PopState();

	void HandleEvents();
	void Update();
	void Draw();

	bool Running() { return m_running; }
	void Quit() { m_running = false; }

	static sf::RenderWindow* screen;
private:
	// the stack of states
	std::vector<CGameState*> states;

	bool m_running;
	bool m_fullscreen;
};

#endif
