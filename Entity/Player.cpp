#include "Player.h"
#include "../Other/vectorFunctions.hpp"

Player::Player(const vec2f& position, const int& level) : Entity(position, level)
{
	m_speed = 150.f;

	m_anim.push_back(Animation("Textures/up_idle.png", vec2i(32, 32), 5, 0.04f));
	m_anim.push_back(Animation("Textures/side_idle_rev.png", vec2i(32, 32), 5, 0.04f));
	m_anim.push_back(Animation("Textures/down_idle.png", vec2i(32, 32), 5, 0.04f));
	m_anim.push_back(Animation("Textures/side_idle.png", vec2i(32, 32), 5, 0.04f));

	m_anim.push_back(Animation("Textures/up_walk.png", vec2i(32, 32), 6, 0.04f));
	m_anim.push_back(Animation("Textures/side_walk_rev.png", vec2i(32, 32), 6, 0.04f));
	m_anim.push_back(Animation("Textures/down_walk.png", vec2i(32, 32), 6, 0.04f));
	m_anim.push_back(Animation("Textures/side_walk.png", vec2i(32, 32), 6, 0.04f));

	setPosition(position);

	vec2fTab vertices{ vec2f(12.f, 28), vec2f(21, 28), vec2f(21, 32), vec2f(12, 32) };
	m_footCollider = PolygoneCollider(vertices, vec2f(0, 0));
	m_collPriority = 1;
}

void Player::update(const sf::Time& elapsed)
{
	move(m_velocity * elapsed.asSeconds() * m_speed);

	if (m_velocity.x == 0 && m_velocity.y == 0 && m_activeAnim > 3)
	{
		m_anim[m_activeAnim].reset();
		m_activeAnim -= 4;
	}
	else if (m_velocity.x > 0 && m_activeAnim != 5)
	{
		changeAvtiveAnim(5);
	}
	else if (m_velocity.x < 0 && m_activeAnim != 7)
	{
		changeAvtiveAnim(7);
	}
	else if (m_velocity.y > 0 && m_activeAnim != 6)
	{
		changeAvtiveAnim(6);
	}
	else if (m_velocity.y < 0 && m_activeAnim != 4)
	{
		changeAvtiveAnim(4);
	}
	m_anim[m_activeAnim].update(elapsed);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(m_anim[m_activeAnim], states);
	target.draw(m_footCollider, states);
}