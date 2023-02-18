#include "gameengine.h"
#include "gamestate.h"
#include <iostream>

sf::RenderWindow* CGameEngine::screen;

void CGameEngine::Init(const char* title, int width, int height, bool fullscreen)
{
	// initialize SFML
	screen = new sf::RenderWindow(sf::VideoMode(width, height), title, (fullscreen ? sf::Style::Titlebar | sf::Style::Fullscreen : sf::Style::Titlebar));
	screen->setFramerateLimit(244);
	screen->setKeyRepeatEnabled(true);

	m_fullscreen = fullscreen;
	m_running = true;

	std::cout << "CGameEngine Init" << std::endl;
}

void CGameEngine::Cleanup()
{
	// cleanup the all states
	while ( !states.empty() ) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// switch back to windowed mode so other 
	// programs won't get accidentally resized
	if ( m_fullscreen ) {
		screen->setSize(sf::Vector2u(1600, 900));
	}

	std::cout << "CGameEngine Cleanup" << std::endl;

	// shutdown SDL
	screen->close();
	delete screen;
}

void CGameEngine::ChangeState(CGameState* state) 
{
	// cleanup the current state
	if ( !states.empty() ) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->Init();
}

void CGameEngine::PushState(CGameState* state)
{
	// pause current state
	if ( !states.empty() ) {
		states.back()->Pause();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->Init();
}

void CGameEngine::PopState()
{
	// cleanup the current state
	if ( !states.empty() ) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// resume previous state
	if ( !states.empty() ) {
		states.back()->Resume();
	}
}


void CGameEngine::HandleEvents() 
{
	// let the state handle events
	states.back()->HandleEvents(this);
}

void CGameEngine::Update() 
{
	// let the state update the game
	states.back()->Update(this);
}

void CGameEngine::Draw() 
{
	// let the state draw the screen
	states.back()->Draw(this);
}
