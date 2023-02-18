#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include <SFML/Graphics.hpp>
#include "../Other/Alias.h"

class PolygoneCollider;
class CircleCollider;

class Collider : public sf::Transformable, public sf::Drawable
{
protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {}

public:
    Collider(const vec2f& position, const float& angle = 0) { setPosition(position); setRotation(angle); }
    Collider() {}
    virtual ~Collider() {}

    /**
     * @fn bool collision_SAT(Collider *c1, Collider *c2, vec2f &resolveVec)
     * @brief Teste la collision entre 2 Colliders quelquonques avec l'algorithme SAT
     * 
     * @param c1 Collider de type quelquonque
     * @param c2 Collider de type quelquonque
     * @param resolveVec Vecteur permettant de résoudre la collision (sortie)
     * @return True si il y a une collision, False sinon. Si il y a une collision resolveVec conserve le vecteur permettant
     * de la résoudre (en direction de c2), sinon il est égale à (0, 0)
     */
    friend bool collision_SAT(Collider *c1, Collider *c2, vec2f &resolveVec);

    /*
     * @fn bool collision_AABB(Collider *c1, Collider *c2, vec2f &resolveVec)
     * @brief Teste la collision entre 2 Colliders * avec l'algorithme AABB, les colliders doivent être des rectangles allignés
     * 
     * @param c1 Collider de type PolygoneCollider (un rectangles alligné)
     * @param c2 Collider de type PolygoneCollider (un rectangles alligné)
     * @param resolveVec Vecteur permettant de résoudre la collision (sortie)
     * @return True si il y a une collision, False sinon. Si il y a une collision resolveVec conserve le vecteur permettant
     * de la résoudre (en direction de c2), sinon il est égale à (0, 0)
     */
    friend bool collision_AABB(Collider *c1, Collider *c2, vec2f &resolveVec);
};

/**
 * @fn vec2f range(const vec2f &axis, const vec2fTab &vertices)
 * @brief Détermine l'ombre de vertices sur une droite
 *
 * @param axis Vecteur directeur de la droite
 * @param vertices Tableau de sommet définissant une forme convexe
 * @return Le min et le max des projetés orthogonaux de chaque sommet de vertices sur la droite définit par le vecteur directeur axis
 */
vec2f shadow(const vec2f &axis, const vec2fTab &vertices);

/**
 * @fn vec2f circleRange(const vec2f &axis, const vec2f &position, float radius)
 * @brief Détermine l'ombre d'un cercle sur une droite
 * 
 * @param axis Vecteur directeur de la droite
 * @param position Centre du cercle
 * @param radius Rayon du cercle
 * @return Le min et le max des projetés orthogonaux de "chaque point du cercle" sur la droite définit par le vecteur directeur axis
 */
vec2f circleShadow(const vec2f &axis, const vec2f &position, const float &radius);

/**
 * @fn bool overlap(const vec2f &v1, const vec2f &v2, const vec2f &axis, vec2f &resolveAxis, float &size)
 * @brief Calcule le chevauchement de 2 droites (plan 1D)
 * 
 * @param d1 Droite (x, y)
 * @param d2 Droite (x, y)
 * @param axis Vecteur directeur du plan à 1 dimension
 * @param resolveVector Vecteur directeur du plan ou le chevauchement minimum à été trouvé (précédemment calculé) (sortie)
 * @param size Taille du chevauchement minimum (précédemment calculés) (entré-sortie)
 * @return True si il y a un chevauchement, False sinon. Si il y a un chevauchement plus petit que size,
 * resolveAxis conserve le vecteur axis, et size la taille du chevauchement.
 */
bool overlap(const vec2f &d1, const vec2f &d2, const vec2f &axis, vec2f &resolveVector, float &size);

/**
 * @fn vec2f getRectInfo(const vec2fTab &vertices, vec2f &size)
 * @brief calcule le coin superieur gauche d'un rectangle et sa taille
 * 
 * @param vertices Tableau de sommet définissant un rectangle
 * @param size Taille du rectangle (sortie)
 * @return Le coin superieur gauche. Et size conserve la taille du rectangle
 */
vec2f getRectInfo(const vec2fTab &vertices, vec2f &size);

#endif