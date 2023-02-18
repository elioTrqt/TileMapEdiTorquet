#ifndef VECTORFUNCTION_HPP
#define VECTORFUNCTION_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include "Alias.h"

/**
 * @fn vec2f normalVector(const vec2fTab &vertices, int i)
 * @brief Calcule le vecteur normal à une droite
 *
 * @param vertices Tableau de sommet définissant une forme convexe
 * @param i Numero de l'arrete
 * @return Le vecteur normal a l'arrete (vertices[i], vertices[i+1]), le vecteur est normalisé. 
 */
vec2f normalVector(const vec2fTab &vertices, const int &i);

/**
 * @fn vec2f normalVector(const vec2f &v1, const vec2f &v2)
 * @brief Calcule le vecteur normal à une droite
 * @overload
 *
 * @param p1 Point (x, y)
 * @param p2 Point (x, y)
 * @return Le vecteur normal a la droite (v1, v2), le vecteur est normalisé.
 */
vec2f normalVector(const vec2f &p1, const vec2f &p2);

/**
 * @fn vec2f directVector(const vec2f &v1, const vec2f &v2)
 * @brief Calcule le vecteur directeur d'une droite
 *
 * @param p1 Point (x, y)
 * @param p2 Point (x, y)
 * @return Le vecteur directeur de la droite (v1, v2), le vecteur est normalisé.
 */
vec2f directVector(const vec2f &p1, const vec2f &p2);

/**
 * @fn float distance(const vec2f &v1, const vec2f &v2)
 * @brief Calcule la distance entre 2 points
 *
 * @param p1 Point (x, y)
 * @param p2 Point (x, y)
 * @return La distance entre v1 et v2
 */
float distance(const vec2f &p1, const vec2f &p2);

/**
 * @fn void normalize(vec2f &v)
 * @brief Normalise le vecteur v (longueur de 1)
 *
 * @param v Vecteur (x, y) (entré-sortie)
 */
void normalize(vec2f &v);

/**
 * @fn float dotProduct(const vec2f &v1, const vec2f &v2)
 * @brief Calcule un produit scalaire
 *
 * @param v1 Vecteur ou point
 * @param v2 Vecteur ou point
 * @return Le produit scalaire v1.v2
 */
float dotProduct(const vec2f &v1, const vec2f &v2);

/**
 * @fn vec2f closestVert(const vec2fTab &vertices, const vec2f &v2)
 * @brief Détermine le point le plus proche d'un autre
 * 
 * @param points Tableau de points
 * @param p2 Point
 * @return Le point du tableau points le plus proche de p2 
 */
vec2f closestVert(const vec2fTab &points, const vec2f &p2);

/**
 * @fn vec2f getCenter(const vec2fTab &vertices)
 * @brief Calcule le centre d'un polygone convexe
 * 
 * @param vertices Tableau de sommet définissant une forme convexe
 * @return Le centre de la forme définit par vertices
 */
vec2f getCenter(const vec2fTab &vertices);

void printVec(const vec2f &v);

template <typename T>
void saveVec(const sf::Vector2<T>& vec, std::ofstream& flux)
{
    flux.write((char*)&vec.x, sizeof(T));
    flux.write((char*)&vec.y, sizeof(T));
}

template <typename T>
sf::Vector2<T> loadVec(std::ifstream& flux)
{
    T x, y;
    flux.read((char*)&x, sizeof(T));
    flux.read((char*)&y, sizeof(T));

    return sf::Vector2<T>(x, y);
}

template <typename T>
void saveVecTab(const std::vector<sf::Vector2<T>>& vec, std::ofstream& flux)
{
    size_t n = vec.size();
    flux.write((char*)&n, sizeof(size_t));

    for (int i = 0; i < (int)n; i++)
    {
        saveVec<T>(vec[i], flux);
    }
}

template <typename T>
std::vector<sf::Vector2<T>> loadVecTab(std::ifstream& flux)
{
    size_t n;
    flux.read((char*)&n, sizeof(size_t));

    vec2fTab tab;
    for (int i = 0; i < (int)n; i++)
    {
        tab.push_back(loadVec<T>(flux));
    }

    return tab;
}

#endif