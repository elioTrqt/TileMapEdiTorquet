#include "Collider.hpp"
#include "PolygoneCollider.hpp"
#include "CircleCollider.hpp"
#include "../Other/vectorFunctions.hpp"
#include <iostream>

bool collision_SAT(Collider *c1, Collider *c2, vec2f &resolveVec)
{

    PolygoneCollider *pol1 = dynamic_cast<PolygoneCollider *>(c1);
    PolygoneCollider *pol2 = dynamic_cast<PolygoneCollider *>(c2);
    CircleCollider *circ1 = dynamic_cast<CircleCollider *>(c1);
    CircleCollider *circ2 = dynamic_cast<CircleCollider *>(c2);

    if (pol1)
    {
        if (pol2)
        {
            return pol1->isTriggered_SAT(*pol2, resolveVec);
        }
        else if (circ2)
        {
            return pol1->isTriggered_SAT(*circ2, resolveVec);
        }
        else
        {
            std::cerr << "erreur collision_SAT() : collider* c2 indéterminé" << std::endl;
            return EXIT_FAILURE;
        }
    }
    else if (circ1)
    {
        if (pol2)
        {
            return circ1->isTriggered_SAT(*pol2, resolveVec);
        }
        else if (circ2)
        {
            return circ1->isTriggered_SAT(*circ2, resolveVec);
        }
        else
        {
            std::cerr << "erreur collision_SAT() : collider* c2 indéterminé" << std::endl;
            return EXIT_FAILURE;
        }
    }
    else
    {
        std::cerr << "erreur collision_SAT() : collider* c1 indéterminé" << std::endl;
        return EXIT_FAILURE;
    }
}

bool collision_AABB(Collider *c1, Collider *c2, vec2f &resolveVec)
{
    PolygoneCollider *pol1 = dynamic_cast<PolygoneCollider *>(c1);
    PolygoneCollider *pol2 = dynamic_cast<PolygoneCollider *>(c2);
    if(!pol1 || !pol2)
    {
        std::cerr << "erreur collision_AABB() : collider* " <<  (!pol1 ? (!pol2 ? "c1 et c2" : "c1") : "c2") << " indéterminé" << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        return pol1->isTriggered_SAT(*pol2, resolveVec);
    }
}

vec2f shadow(const vec2f& axis, const vec2fTab& vertices)
{
    // projections des points sur la droite du vecteur axis pour trouver le point max et min de "l'ombre"

    vec2f range;
    range.x = dotProduct(axis, vertices[0]);
    range.y = range.x;

    for (int i = 0; i < (int)vertices.size(); i++)
    {
        float temp = dotProduct(axis, vertices[i]);
        range.x = std::min(range.x, temp);
        range.y = std::max(range.y, temp);
    }

    return range;
}

vec2f circleShadow(const vec2f& axis, const vec2f& position, const float& radius)
{
    float temp = dotProduct(axis, position);
    return vec2f(temp - radius, temp + radius);
}

bool overlap(const vec2f& d1, const vec2f& d2, const vec2f& axis, vec2f& resolveVector, float& size)
{
    if (d1.x - d2.y > 0 || d2.x - d1.y > 0)
        return false;
    else
    {
        float temp = std::min(d2.y - d1.x, d1.y - d2.x);
        if (temp < size)
        {
            size = temp;
            resolveVector = axis;
        }
        return true;
    }
}

vec2f getRectInfo(const vec2fTab &vertices, vec2f &size)
{
    vec2f upL = vertices[0];
    int index = 0;
    int n = (int)vertices.size();

    for (int i = 1; i < n; i++)
    {
        if (vertices[i].x < upL.x || vertices[i].y < upL.y)
        {
            upL = vertices[i];
            index = i;
        }
    }
    size.x = distance(vertices[index], vertices[(index + 1) % n]);
    size.y = distance(vertices[(index + 1) % n], vertices[(index + 2) % n]);

    return upL;
}