#ifndef STAIRS_H
#define STAIRS_H

#include "Entity.h"

class Stairs : public Entity
{
public:
    Stairs(const vec2f& pos, const int& level);
    ~Stairs();

    void save(std::ofstream& flux) const;
    void load(std::ifstream& flux);

    friend Entity;

protected:
    PolygoneCollider m_downCollider, m_upCollider;

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

		target.draw(m_anim[m_activeAnim], states);
        target.draw(m_downCollider, states);
        target.draw(m_upCollider, states);
    }
};

void saveStairTab(const std::vector<Stairs>& e, std::ofstream& flux);
std::vector<Stairs> loadStairTab(std::ifstream& flux);

#endif