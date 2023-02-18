#ifndef PROPBUSH_H
#define PROPBUSH_H

#include "Entity.h"
#include <vector>

class PropBush1 : public Entity
{
public:
	PropBush1(const vec2f& pos, const int& level) : Entity(pos, level) {
		m_anim.push_back(Animation("Textures/Bush1.png", vec2i(22, 19), 1, 0));
		m_collPriority = -1;
	}
	~PropBush1() {}

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(m_anim[m_activeAnim], states);
	}
};

class PropBush2 : public Entity
{
public:
	PropBush2(const vec2f& pos, const int& level) : Entity(pos, level) {
		m_anim.push_back(Animation("Textures/Bush2.png", vec2i(27, 25), 1, 0));
		m_collPriority = -1;
	}
	~PropBush2() {}

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		target.draw(m_anim[m_activeAnim], states);
	}
};

class PropBush3 : public Entity
{
public:
	PropBush3(const vec2f& pos, const int& level) : Entity(pos, level) {
		m_anim.push_back(Animation("Textures/Bush3.png", vec2i(38, 32), 1, 0));
		m_collPriority = -1;
	}
	~PropBush3() {}

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(m_anim[m_activeAnim], states);
	}
};

class PropBush4 : public Entity
{
public:
	PropBush4(const vec2f& pos, const int& level) : Entity(pos, level) {
		m_anim.push_back(Animation("Textures/Bush4.png", vec2i(46, 42), 1, 0));
		m_collPriority = -1;
	}
	~PropBush4() {}

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(m_anim[m_activeAnim], states);
	}
};

class PropBush5 : public Entity
{
public:
	PropBush5(const vec2f& pos, const int& level) : Entity(pos, level) {
		m_anim.push_back(Animation("Textures/Bush5.png", vec2i(39, 45), 1, 0));
		m_collPriority = -1;
	}
	~PropBush5() {}

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(m_anim[m_activeAnim], states);
	}
};

class PropBush6 : public Entity
{
public:
	PropBush6(const vec2f& pos, const int& level) : Entity(pos, level) {
		m_anim.push_back(Animation("Textures/Bush6.png", vec2i(40, 35), 1, 0));
		m_collPriority = -1;
	}
	~PropBush6() {}

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(m_anim[m_activeAnim], states);
	}
};

#endif
