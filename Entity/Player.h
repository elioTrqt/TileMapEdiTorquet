#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "Entity.h"

class Player : public Entity
{
public:
	Player(const vec2f& position = vec2f(0, 0), const int& level = 0);
	~Player() {}

	void update(const sf::Time& elapsed);

private:
	static Player m_player;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif