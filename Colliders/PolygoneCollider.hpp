#ifndef POLYGONECOLLIDER_HPP
#define POLYGONECOLLIDER_HPP

#include <vector>
#include "Collider.hpp"
#include <fstream>

class PolygoneCollider : public Collider
{
private:
    /// Tableau de sommet définissant le polygone convexe sans transformations
    vec2fTab m_originalVerts;
    /// Tableau de sommet définissant le polygone convexe après les transformations appliquées
    vec2fTab m_transformedVerts;
    /// Nombre de sommets du polygone
    uint32_t m_vertsCount = 0;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    PolygoneCollider() {}
    /**
     * @fn PolygoneCollider(const vec2fTab &vertices, const vec2f &position, float angle = 0)
     * @brief Construct a new PolygoneCollider object
     * 
     * @param vertices Tableau de sommet définissant une forme convexe (positions relatives au centre de la forme)
     * @param position Position du polygone
     * @param angle Angle de rotation
     */
    PolygoneCollider(const vec2fTab &vertices, const vec2f &position, float angle = 0);

    /**
     * @fn PolygoneCollider(const vec2fTab &vertices)
     * @brief Construct a new PolygoneCollider object
     * 
     * @param vertices Tableau de sommet définissant une forme convexe (positions global dans l'espace)
     */
    PolygoneCollider(const vec2fTab &vertices);

    /**
     * @fn PolygoneCollider(const int &vertsCount, const float &centerDist, const vec2f &position, const float &angle = 0)
     * @brief Construct a new PolygoneCollider object dont la forme est un polygone convexe régulier
     * @overload
     * 
     * @param vertsCount Nombre de sommets
     * @param centerDist Distance entre le centre et chaque sommet
     * @param position Position du polygone
     * @param angle Angle du polygone (default = 0)
     */
    PolygoneCollider(const uint32_t &vertsCount, const float &centerDist, const vec2f &position, const float &angle = 0);
    ~PolygoneCollider() {}

    /**
     * @fn void setGlobalCoordinates()
     * @brief Calcule les coordonées des sommets avec les transformations
     * 
     * @param pos Position du Collider
     * @param sc Echelle du Collider
     */
    void setGlobalCoordinates(const vec2f& pos, const vec2f& sc = vec2f(1, 1));

    /**
     * @fn bool isTriggered_SAT(const PolygoneCollider &oCollider, vec2f &resolveVector) const
     * @brief Teste la collision avec un PolygonCollider avec l'algorithme SAT
     * 
     * @param oCollider L'autre PolygonCollider
     * @param resolveVector Vecteur permettant de résoudre la collision (sortie)
     * @return True si il y a une collision, False sinon. Si il y a une collision resolveVec conserve le vecteur permettant
     * de la résoudre (en direction de c2), sinon il est égale à (0, 0)
     */
    bool isTriggered_SAT(PolygoneCollider &oCollider, vec2f &resolveVector) const;

    /**
     * @fn bool isTriggered_SAT(const CircleCollider &oCollider, vec2f &resolveVector) const
     * @brief Teste la collision avec un CircleCollider avec l'algorithme SAT
     * @overload
     * 
     * @param oCollider Le CircleCollider
     * @param resolveVector Vecteur permettant de résoudre la collision (sortie)
     * @return True si il y a une collision, False sinon. Si il y a une collision resolveVec conserve le vecteur permettant
     * de la résoudre (en direction de c2), sinon il est égale à (0, 0)
     */
    bool isTriggered_SAT(const CircleCollider &oCollider, vec2f &resolveVector) const;

    /**
     * @fn bool isTriggered_AABB(const PolygoneCollider &oCollider, vec2f &resolveVector) const
     * @brief Teste la collision avec un PolygonCollider avec l'algorithme AABB, 
     * les 2 Colliders doivent être des rectangles allignés
     * 
     * @param oCollider L'autre PolygonCollider (quoi doit être un rectangle alligné)
     * @param resolveVector Vecteur permettant de résoudre la collision (sortie)
     * @return True si il y a une collision, False sinon. Si il y a une collision resolveVec conserve le vecteur permettant
     * de la résoudre (en direction de c2), sinon il est égale à (0, 0)
     */
    bool isTriggered_AABB(PolygoneCollider &oCollider, vec2f &resolveVector) const;

    friend CircleCollider;

    friend void savePolygone(const PolygoneCollider& col, std::ofstream& flux);
    friend PolygoneCollider loadPolygone(std::ifstream& flux);

    friend void savePolygoneTab(const std::vector<PolygoneCollider>& cols, std::ofstream& flux);
    friend std::vector< PolygoneCollider> loadPolygoneTab(std::ifstream& flux);
};

void savePolygone(const PolygoneCollider& col, std::ofstream& flux);
PolygoneCollider loadPolygone(std::ifstream& flux);

void savePolygoneTab(const std::vector<PolygoneCollider>& cols, std::ofstream& flux);
std::vector< PolygoneCollider> loadPolygoneTab(std::ifstream& flux);

void loadPolygone2(PolygoneCollider& col, std::ifstream& flux);
void loadPolygoneTab2(std::vector< PolygoneCollider>& cols, std::ifstream& flux);

#endif