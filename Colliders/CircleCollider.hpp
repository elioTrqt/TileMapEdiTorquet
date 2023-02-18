#ifndef CIRCLECOLLIDER_HPP
#define CIRCLECOLLIDER_HPP

#include "Collider.hpp"

class CircleCollider : public Collider
{
private:
    /// Rayon du cercle
    float m_radius = 0.f;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    CircleCollider() {}
    /**
     * @fn CircleCollider(vec2f position = vec2f(0, 0), float radius = 1)
     * @brief Construct a new CircleCollider object
     * 
     * @param position Rosition du cercle (default = 0)
     * @param radius Rayon du cercle (default = 1)
     */
    CircleCollider(const vec2f &position = vec2f(0, 0), const float &radius = 1) : Collider(position), m_radius(radius) {}
    ~CircleCollider() {}

    /**
     * @fn bool isTriggered(const CircleCollider &oCollider, vec2f &resolveVector) const
     * @brief Teste la collision avec un CircleCollider
     * 
     * @param oCollider L'autre CircleCollider
     * @param resolveVector Vecteur permettant de résoudre la collision (sortie)
     * @return True si il y a une collision, False sinon. Si il y a une collision resolveVec conserve le vecteur permettant
     * de la résoudre (en direction de c2), sinon il est égale à (0, 0)
     */
    bool isTriggered_SAT(const CircleCollider &oCollider, vec2f &resolveVector) const;

    /**
     * @fn bool isTriggered(const PolygoneCollider &oCollider, vec2f &resolveVector) const
     * @brief Teste la collision avec un PolygonCollider
     * @overload
     * 
     * @param oCollider Le PolygonCollider
     * @param resolveVector Vecteur permettant de résoudre la collision (sortie)
     * @return True si il y a une collision, False sinon. Si il y a une collision resolveVec conserve le vecteur permettant
     * de la résoudre (en direction de c2), sinon il est égale à (0, 0)
     */
    bool isTriggered_SAT(const PolygoneCollider &oCollider, vec2f &resolveVector) const;

    friend PolygoneCollider;
};

#endif