#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <fstream>
#include "../Colliders/Colliders.hpp"
#include "Animation.h"
#include "../Other/Alias.h"

class Stairs;

class Entity : public sf::Drawable, public sf::Transformable
{
public:
	Entity(const vec2f& position, const int& layer, const float& speed = 0);
	Entity(){}
	virtual ~Entity();

	bool entityCollision(Entity& e);
	bool entityCollision(Collider& col);
	bool entityCollision(Stairs& st);

	virtual vec2f getSize() const;

	void updateVelocity(const vec2f& velocity) { m_velocity = velocity; }
	vec2f getVelocity() {return m_velocity;}

	void changeAvtiveAnim(const int& num);
	virtual void update(const sf::Time& elapsed) { m_anim[m_activeAnim].update(elapsed); }

	virtual void save(std::ofstream& flux) const;
	virtual void load(std::ifstream& flux);

	int m_layer = 0;

protected:
	 
	std::vector<Animation> m_anim;
	PolygoneCollider m_footCollider;
	int m_collPriority = -1;
	int m_activeAnim = 0;
	float m_speed = 0.f;
	vec2f m_velocity = vec2f(0.f, 0.f);

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(m_anim[m_activeAnim], states);
	}
};

void saveEntityTab(const std::vector<Entity>& e, std::ofstream& flux);
std::vector<Entity> loadEntityTab(std::ifstream& flux);

#endif