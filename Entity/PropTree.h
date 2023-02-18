#ifndef PROPTREE_H
#define PROPTREE_H

#include "Entity.h"
#include <vector>

class PropTree1 : public Entity
{
public:
	PropTree1(const vec2f& pos, const int& level) : Entity(pos, level) {
		Animation a("Textures/Tree1.png", vec2i(113, 139), 1, 0);
		m_anim.push_back(a);
		m_col = PolygoneCollider(vec2fTab{vec2f(50, 130), vec2f(61, 130), vec2f(61, 139), vec2f(50, 139)}, vec2f(0, 0));
		m_footCollider = m_col;
		m_collPriority = 2;
	}
	~PropTree1() {}

private:

	PolygoneCollider m_col;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform  *= getTransform();

		target.draw(m_anim[m_activeAnim], states);
 		target.draw(m_footCollider, states);
	}
};

class PropTree2 : public Entity
{
public:
	PropTree2(const vec2f& pos, const int& level) : Entity(pos, level) {
		m_anim.push_back(Animation("Textures/Tree2.png", vec2i(95, 136), 1, 0));
		m_footCollider = PolygoneCollider(vec2fTab{vec2f(41, 127), vec2f(52, 127), vec2f(52, 136), vec2f(41, 136)}, vec2f(0, 0));
		m_collPriority = 2;
	}
	~PropTree2() {}

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		target.draw(m_anim[m_activeAnim], states);
		target.draw(m_footCollider, states);
	}
};

class PropTree3 : public Entity
{
public:
	PropTree3(const vec2f& pos, const int& level) : Entity(pos, level) {
		m_anim.push_back(Animation("Textures/Tree3.png", vec2i(79, 120), 1, 0));
		m_footCollider = PolygoneCollider(vec2fTab{vec2f(36, 112), vec2f(46, 112), vec2f(46, 120), vec2f(36, 120)}, vec2f(0, 0));
		m_collPriority = 2;
	}
	~PropTree3() {}

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		
		target.draw(m_anim[m_activeAnim], states);
		target.draw(m_footCollider, states);
	}
};


#endif