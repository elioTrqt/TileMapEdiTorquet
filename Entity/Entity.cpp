#include "Entity.h"
#include "Stairs.h"
#include <iostream>
#include "../Other/vectorFunctions.hpp"

Entity::Entity(const vec2f& position, const int& layer, const float& speed) : m_layer(layer), m_speed(speed)
{
	setPosition(position);
}

Entity::~Entity()
{
}

bool Entity::entityCollision(Entity& e)
{
	if (e.m_collPriority != -1 && m_collPriority != -1)
	{
		m_footCollider.setGlobalCoordinates(getPosition(), getScale());
		e.m_footCollider.setGlobalCoordinates(e.getPosition(), e.getScale());
		vec2f resolve;
		if (collision_AABB(&m_footCollider, &(e.m_footCollider), resolve))
		{
			if (m_collPriority > e.m_collPriority)
			{
				e.move(resolve);
			}
			else if (m_collPriority < e.m_collPriority)
			{
				move(-resolve);
			}
			else
			{
				move(vec2f(-resolve.x / 2.f, -resolve.y / 2.f));
				e.move(vec2f(resolve.x / 2.f, resolve.y / 2.f));
			}
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool Entity::entityCollision(Collider& col)
{
	if (m_collPriority != -1)
	{
		m_footCollider.setGlobalCoordinates(getPosition(), getScale());
		vec2f resolve;
		if (collision_AABB(&m_footCollider, &col, resolve))
		{
			move(-resolve);
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool Entity::entityCollision(Stairs& stair)
{
	if (m_collPriority != -1)
	{
		m_footCollider.setGlobalCoordinates(getPosition(), getScale());
		stair.m_upCollider.setGlobalCoordinates(stair.getPosition(), stair.getScale());
		stair.m_downCollider.setGlobalCoordinates(stair.getPosition(), stair.getScale());
		vec2f resolve;

		if (m_layer > stair.m_layer && collision_AABB(&m_footCollider, &(stair.m_downCollider), resolve))
		{
			m_layer--;
			return true;
		}
		else if (m_layer <= stair.m_layer && collision_AABB(&m_footCollider, &(stair.m_upCollider), resolve))
		{
			m_layer++;
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

vec2f Entity::getSize() const
{
	return vec2f(m_anim[m_activeAnim].getSize().x * getScale().x , m_anim[m_activeAnim].getSize().y * getScale().x);
}

void Entity::changeAvtiveAnim(const int &num)
{
	if ((int)m_anim.size() <= num || num < 0)
	{
		std::cout << "Erreur changeActiveAnim() : index out of range" << std::endl;
	}
	else
	{
		m_anim[m_activeAnim].reset();
		m_activeAnim = num;
	}
}

void Entity::save(std::ofstream& flux) const
{
	flux.write((char*)&m_layer, sizeof(int));
	saveAnimationTab(m_anim, flux);
	flux.write((char*)&m_collPriority, sizeof(int));
	if(m_collPriority != -1)
		savePolygone(m_footCollider, flux);
	flux.write((char*)&m_speed, sizeof(float));
	saveVec(getPosition(), flux);
}

void Entity::load(std::ifstream& flux)
{
	flux.read((char*)&m_layer, sizeof(int));
	m_anim = loadAnimationTab(flux);
	flux.read((char*)&m_collPriority, sizeof(int));
	if (m_collPriority != -1)
		m_footCollider = loadPolygone(flux);
	flux.read((char*)&m_speed, sizeof(float));
	setPosition(loadVec<float>(flux));
}

void saveEntityTab(const std::vector<Entity>& e, std::ofstream& flux)
{
	size_t n = e.size();
	flux.write((char*)&n, sizeof(size_t));

	for (int i = 0; i < (int)n; i++)
	{
		e[i].save(flux);
	}
}

std::vector<Entity> loadEntityTab(std::ifstream& flux)
{
	size_t n;
	flux.read((char*)&n, sizeof(size_t));

	std::vector<Entity> e;
	for (int i = 0; i < (int)n; i++)
	{
		e.push_back(Entity());
		e[i].load(flux);
	}

	return e;
}
