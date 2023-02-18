#include "PolygoneCollider.hpp"
#include "CircleCollider.hpp"
#include "../Other/vectorFunctions.hpp"
#include <cmath>

PolygoneCollider::PolygoneCollider(const vec2fTab &vertices, const vec2f &position, float angle) : Collider(position, angle), m_originalVerts(vertices), m_transformedVerts(vertices), m_vertsCount((uint32_t)vertices.size())
{
    setGlobalCoordinates(position);
}

PolygoneCollider::PolygoneCollider(const uint32_t &vertsCount, const float &centerDist, const vec2f &position, const float &angle) : Collider(position, angle), m_vertsCount(vertsCount)
{
    float teta = float(3.14159 * 2 / m_vertsCount);
    for (uint32_t i = 0; i < m_vertsCount; i++)
    {
        m_originalVerts.push_back(vec2f(centerDist * cosf(teta * i), centerDist * sinf(teta * i)));
        m_transformedVerts.push_back(vec2f(centerDist * cosf(teta * i), centerDist * sinf(teta * i)));
    }
    setGlobalCoordinates(position);
}

PolygoneCollider::PolygoneCollider(const vec2fTab &vertices) : Collider(), m_originalVerts(vertices), m_transformedVerts(vertices), m_vertsCount((uint32_t)vertices.size())
{
    setPosition(getCenter(vertices));
    for(uint32_t i = 0 ; i < m_vertsCount; i++)
    {
        m_originalVerts[i] = vertices[i] - getPosition();
    }
    setGlobalCoordinates(getPosition());
}

bool PolygoneCollider::isTriggered_SAT(PolygoneCollider &oCollider, vec2f &resolveVector) const
{
    //setGlobalCoordinates();
    //oCollider.setGlobalCoordinates();

    float minOverlap = INFINITY;

    for (uint32_t i = 0; i < m_vertsCount; i++) // Pour chaque arrete
    {
        // on calcule le vecteur normal et les "ombres" des Colliders sur l'axe du vecteur

        vec2f axis = normalVector(m_transformedVerts, i);

        vec2f range1 = shadow(axis, m_transformedVerts);
        vec2f range2 = shadow(axis, oCollider.m_transformedVerts);

        // si il y a un espace entre les ombres, il n'y a pas de collision

        if (!overlap(range1, range2, axis, resolveVector, minOverlap))
        {
            resolveVector = vec2f(0.f, 0.f);
            return false;
        }
    }

    // On répète avec le second Collider

    for (uint32_t i = 0; i < oCollider.m_vertsCount; i++)
    {
        vec2f axis = normalVector(oCollider.m_transformedVerts, i);

        vec2f range1 = shadow(axis, oCollider.m_transformedVerts);
        vec2f range2 = shadow(axis, m_transformedVerts);

        if (!overlap(range1, range2, axis, resolveVector, minOverlap))
        {
            resolveVector = vec2f(0, 0);
            return false;
        }
    }

    // On ajuste le sens du vecteur pour etre sur qu'il vas toujours en direction de oCollider

    vec2f centerA = getCenter(m_transformedVerts);
    vec2f centerB = getCenter(oCollider.m_transformedVerts);

    if (dotProduct(centerB - centerA, resolveVector) < 0)
    {
        resolveVector *= -1.f;
    }
    else 
        resolveVector *= 1.f;

    resolveVector = resolveVector * minOverlap;

    return true;
}

bool PolygoneCollider::isTriggered_SAT(const CircleCollider &oCollider, vec2f &resolveVector) const
{
    //setGlobalCoordinates();

    float minOverlap = INFINITY;

    // le seul axe ou projeter les ombres pour le cercle est : (centre du cercle, point du polygone le plus proche)

    vec2f vert = closestVert(m_transformedVerts, oCollider.getPosition());
    vec2f axis = directVector(vert, oCollider.getPosition());

    vec2f range1 = circleShadow(axis, oCollider.getPosition(), oCollider.m_radius);
    vec2f range2 = shadow(axis, m_transformedVerts);

    if (!overlap(range1, range2, axis, resolveVector, minOverlap))
    {
        resolveVector = vec2f(0.f, 0.f);
        return false;
    }

    // verification classique pour chaque arrete du polygone

    for (uint32_t i = 0; i < m_vertsCount; i++)
    {
        vec2f axis = normalVector(m_transformedVerts, i);

        vec2f range1 = shadow(axis, m_transformedVerts);
        vec2f range2 = circleShadow(axis, oCollider.getPosition(), oCollider.m_radius);

        if (!overlap(range1, range2, axis, resolveVector, minOverlap))
        {
            resolveVector = vec2f(0.f, 0.f);
            return false;
        }
    }

    vec2f centerA = getCenter(m_transformedVerts);
    vec2f centerB = oCollider.getPosition();

    if (dotProduct(centerB - centerA, resolveVector) < 0)
        resolveVector *= -1.f;

    resolveVector = resolveVector * minOverlap;

    return true;
}

bool PolygoneCollider::isTriggered_AABB(PolygoneCollider &oCollider, vec2f &resolveVector) const
{

    vec2f size1, size2;
    vec2f pos1 = getRectInfo(m_transformedVerts, size1);
    vec2f pos2 = getRectInfo(oCollider.m_transformedVerts, size2);

    if(pos1.x < (pos2.x + size2.x) && pos2.x < (pos1.x + size1.x) && 
    pos1.y < (pos2.y + size2.y) && pos2.y < (pos1.y + size1.y))
    {
        float overlapX = std::min((pos2.x + size2.x) - pos1.x, (pos1.x + size1.x) - pos2.x);
        float overlapY = std::min((pos2.y + size2.y) - pos1.y, (pos1.y + size1.y) - pos2.y);
        resolveVector.x = (overlapX >= overlapY ? overlapX : 0.f);
        resolveVector.y = (overlapY > overlapX ? overlapY : 0.f);
        return true;
    }
    else
    {
        return false;
    }
}

void PolygoneCollider::setGlobalCoordinates(const vec2f& pos, const vec2f& scale)
{
    for (uint32_t i = 0; i < m_vertsCount; i++)
    {
        m_transformedVerts[i] = vec2f(
            m_originalVerts[i].x * scale.x * cosf(getRotation()) - (m_originalVerts[i].y * scale.y * sinf(getRotation())) + pos.x,
            m_originalVerts[i].x * scale.x * sinf(getRotation()) + (m_originalVerts[i].y * scale.y * cosf(getRotation())) + pos.y);
    }
}

void PolygoneCollider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    sf::VertexArray shape(sf::LineStrip, m_vertsCount + 1);
    for (uint32_t i = 0; i <= m_vertsCount; i++)
    {
        shape[i].position = m_originalVerts[i % m_vertsCount];
        shape[i].color = (sf::Color::Red);
    } 

    target.draw(shape, states);
}

void savePolygone(const PolygoneCollider& col, std::ofstream& flux)
{
    saveVecTab(col.m_originalVerts, flux);
    saveVec(col.getPosition(), flux);
}

PolygoneCollider loadPolygone(std::ifstream& flux)
{
    vec2fTab tab = loadVecTab<float>(flux);
    vec2f pos = loadVec<float>(flux);

    return PolygoneCollider(tab, pos);
}

void savePolygoneTab(const std::vector<PolygoneCollider>& cols, std::ofstream& flux)
{
    size_t n = cols.size();
    flux.write((char*)&n, sizeof(size_t));

    for (int i = 0; i < (int)n; i++)
    {
        savePolygone(cols[i], flux);
    }
}

std::vector<PolygoneCollider> loadPolygoneTab(std::ifstream& flux)
{
    size_t n;
    flux.read((char*)&n, sizeof(size_t));

    std::vector<PolygoneCollider> vec;

    for (int i = 0; i < (int)n; i++)
    {
        PolygoneCollider p = loadPolygone(flux);
        vec.push_back(p);
    }

    return vec;
}

void loadPolygone2(PolygoneCollider& col, std::ifstream& flux)
{
}

void loadPolygoneTab2(std::vector<PolygoneCollider>& cols, std::ifstream& flux)
{
}
