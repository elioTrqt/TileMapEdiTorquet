#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include <SFML/Graphics.hpp>
#include "gamestate.h"
#include "../Tilemap/Tilemap.hpp"
#include "../Colliders/Colliders.hpp"
#include "../Entity/Player.h"
#include "../Other/Alias.h"

class CPlayState : public CGameState
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(CGameEngine* game);
	void Update(CGameEngine* game);
	void Draw(CGameEngine* game);

	static CPlayState* Instance() {
		return &m_PlayState;
	}

protected:
	CPlayState() { }

private:
	static CPlayState m_PlayState;

	sf::View* m_vueHUD;
	sf::View* m_vueMap;

	sf::Clock m_clock;
	Tilemap* m_tMap;
	Player* m_player;

	bool k_d = false, k_q = false, k_z = false, k_s = false;
};

#endif