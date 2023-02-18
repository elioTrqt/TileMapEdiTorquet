#include "vectorFunctions.hpp"
#include <cmath>
#include <iostream>

vec2f normalVector(const vec2fTab &vertices, const int &i)
{
    vec2f v1 = vertices[i], v2 = vertices[(i + 1) % vertices.size()], n;
    n.x = -1 * (v2.y - v1.y);
    n.y = v2.x - v1.x;

    normalize(n);

    return n;
}

vec2f normalVector(const vec2f &p1, const vec2f &p2)
{
    vec2f n;
    n.x = -1 * (p2.y - p1.y);
    n.y = p2.x - p1.x;

    normalize(n);

    return n;
}

vec2f directVector(const vec2f &p1, const vec2f &p2)
{
    vec2f d = p2 - p1;
    normalize(d);
    return d;
}

float distance(const vec2f &p1, const vec2f &p2)
{
    return (float)std::sqrt(std::pow(p2.x - p1.x, 2) + std::pow(p2.y - p1.y, 2));
}

void normalize(vec2f &v)
{
    if (v.x != 0 || v.y != 0)
    {
        float norme = std::sqrt(v.x * v.x + v.y * v.y);
        v /= norme;
    }
}

float dotProduct(const vec2f &v1, const vec2f &v2)
{
    return (v1.x * v2.x + v1.y * v2.y);
}

vec2f closestVert(const vec2fTab &points, const vec2f &v2)
{
    vec2f closestVert = points[0];
    float closestDist = distance(closestVert, v2);

    for (int i = 1; i < (int)points.size(); i++)
    {
        float tempDist = distance(points[i], v2);
        if (closestDist > tempDist)
        {
            closestVert = points[i];
            closestDist = tempDist;
        }
    }

    return closestVert;
}

vec2f getCenter(const vec2fTab &vertices)
{
    vec2f sum(0, 0);
    for(int i = 0 ; i < (int)vertices.size() ; i++)
    {
        sum += vertices[i];
    }
    return (sum / (float)vertices.size());
}

void printVec(const vec2f &vec)
{
    std::cout << "( " << (float)vec.x << " , " << (float)vec.y << " )" << std::endl;
}