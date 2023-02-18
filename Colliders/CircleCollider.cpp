#include "CircleCollider.hpp"
#include "PolygoneCollider.hpp"
#include "../Other/vectorFunctions.hpp"
#include <cmath>

bool CircleCollider::isTriggered_SAT(const CircleCollider &oCollider, vec2f &resolveVector) const
{
    float overlap = (m_radius + oCollider.m_radius) - distance(getPosition(), oCollider.getPosition());
    if(overlap >= 0)
    {
        resolveVector = directVector(getPosition(), oCollider.getPosition()) * overlap;
        return true;
    }
    else
    {
        resolveVector = vec2f(0, 0);
        return false;
    }
}

bool CircleCollider::isTriggered_SAT(const PolygoneCollider &oCollider, vec2f &resolveVector) const
{
    float minOverlap = INFINITY;

    vec2f vert = closestVert(oCollider.m_transformedVerts, getPosition());
    vec2f axis = directVector(vert, getPosition());

    vec2f range1 = circleShadow(axis, getPosition(), m_radius);
    vec2f range2 = shadow(axis, oCollider.m_transformedVerts);

    if (!overlap(range1, range2, axis, resolveVector, minOverlap))
    {
        resolveVector = vec2f(0.f, 0.f);
        return false;
    }

    for (uint32_t i = 0; i < oCollider.m_vertsCount; i++)
    {
        vec2f axis = normalVector(oCollider.m_transformedVerts, i); // vecteur normal

        vec2f range1 = shadow(axis, oCollider.m_transformedVerts);
        vec2f range2 = circleShadow(axis, getPosition(), m_radius);

        if (!overlap(range1, range2, axis, resolveVector, minOverlap))
        {
            resolveVector = vec2f(0.f, 0.f);
            return false;
        }
    }
    vec2f centerA = getPosition();
    vec2f centerB = getCenter(oCollider.m_transformedVerts);

    if (dotProduct(centerB - centerA, resolveVector) < 0)
        resolveVector *= -1.f;

    resolveVector = resolveVector * minOverlap;

    return true;
}

void CircleCollider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    sf::CircleShape shape(m_radius);
    shape.setOrigin(m_radius, m_radius);
    shape.setPosition(getPosition());
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineThickness(-1);
    shape.setOutlineColor(sf::Color::White);

    target.draw(shape, states);
}