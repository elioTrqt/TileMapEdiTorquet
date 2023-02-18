#include "Stairs.h"
#include "Player.h"
#include "../Other/vectorFunctions.hpp"

Stairs::Stairs(const vec2f& pos, const int& level) : Entity(pos, level)
{
    m_collPriority = -1;
    m_anim.push_back(Animation("Textures/Stair1.png", vec2i(64, 96), 1, 0));
    m_upCollider = PolygoneCollider(vec2fTab{vec2f(0, 5), vec2f(64, 5), vec2f(64, 15), vec2f(0, 15)}, vec2f(0, 0));
    m_downCollider = PolygoneCollider(vec2fTab{vec2f(0, 81), vec2f(64, 81), vec2f(64, 91), vec2f(0, 91)}, vec2f(0, 0));
}

Stairs::~Stairs()
{
}

void Stairs::save(std::ofstream& flux) const
{
	flux.write((char*)&m_layer, sizeof(int));
	saveAnimationTab(m_anim, flux);
	savePolygone(m_downCollider, flux);
	savePolygone(m_upCollider, flux);

	saveVec(getPosition(), flux);
}

void Stairs::load(std::ifstream& flux)
{
	flux.read((char*)&m_layer, sizeof(int));
	m_anim = loadAnimationTab(flux);
	m_downCollider = loadPolygone(flux);
	m_upCollider = loadPolygone(flux);
	m_collPriority = -1;

	setPosition(loadVec<float>(flux));
}

void saveStairTab(const std::vector<Stairs>& s, std::ofstream& flux)
{
	size_t n = s.size();
	flux.write((char*)&n, sizeof(size_t));

	for (int i = 0; i < (int)n; i++)
	{
		s[i].save(flux);
	}
}

std::vector<Stairs> loadStairTab(std::ifstream& flux)
{
	size_t n;
	flux.read((char*)&n, sizeof(size_t));

	std::vector<Stairs> s;
	for (int i = 0; i < (int)n; i++)
	{
		s.push_back(Stairs(vec2f(0, 0), 0));
		s[i].load(flux);
	}

	return s;
}
