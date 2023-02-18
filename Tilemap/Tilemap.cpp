#include "Tilemap.hpp"
#include "../Entity/PropTree.h"
#include "../Other/vectorFunctions.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>

Tilemap::Tilemap(const vec2i &tileSize, const std::string &textFile, const vec2i &mapSize, const uint32_t &levels) : m_mapSize(mapSize), m_tileSize(tileSize), m_layerCount(levels)
{
    // On charge la texture
    m_tileSet.loadFromFile(textFile);
    m_tileSetFile = textFile;

    // On setup la base de la tilemap
    m_vertices = sf::VertexArray(sf::Quads, mapSize.x * mapSize.y * 4);
    m_textureNumbers = std::vector<int>(mapSize.x * mapSize.y, 1);
    loadTextureFromTab();

    // on setup le tableau pour le pathfinding
    m_path = std::vector<bool>(mapSize.x * mapSize.y * 4, true);
    m_pathView.resize(mapSize.x * mapSize.y * 4);
    loadPathView();

    for (uint32_t i = 0; i < m_layerCount; i++)
    {
        m_colliders.push_back(std::vector<PolygoneCollider>());
    }
}

Tilemap::Tilemap(std::ifstream& flux)
{
    size_t n;
    flux.read((char*)&n, sizeof(size_t));
    m_tileSetFile.resize(n);
    flux.read((char*)&m_tileSetFile[0], n * sizeof(char));

    m_tileSize = loadVec<int>(flux);
    m_mapSize = loadVec<int>(flux);

    flux.read((char*)&m_layerCount, sizeof(uint32_t));

    flux.read((char*)&m_spawnLayer, sizeof(int));
    m_spawnPoint = loadVec<float>(flux);

    flux.read((char*)&n, sizeof(size_t));
    m_textureNumbers.resize(n);
    for (int i = 0; i < (int)n; i++)
    {
        flux.read((char*)&m_textureNumbers[i], sizeof(int));
    }

    flux.read((char*)&n, sizeof(size_t));
    m_path.resize(n);
    for (int i = 0; i < (int)n; i++)
    {
        bool b;
        flux.read((char*)&b, sizeof(bool));
        m_path[i] = b;
    }

    for (uint32_t i = 0; i < m_layerCount; i++)
    {
        std::vector<PolygoneCollider> p = loadPolygoneTab(flux);
        m_colliders.push_back(p);
    }

    m_decorations = loadEntityTab(flux);
    m_stairs = loadStairTab(flux);

    m_vertices = sf::VertexArray(sf::Quads, m_mapSize.x * m_mapSize.y * 4);
    m_tileSet.loadFromFile(m_tileSetFile);
    loadTextureFromTab();
    loadPathView();
}

void Tilemap::resizeTM(const vec2i &resizeSize, const bool &rightDown)
{
    vec2i newSize(m_mapSize.x + resizeSize.x, m_mapSize.y + resizeSize.y);

    if (newSize.x < 0 || newSize.y < 0) // On empeche d'avoir une nouvelle taille negative
    {
        std::cout << "erreur resizeTM() : taille negatif imposible" << std::endl;
        return;
    }

    // On mesure la difference de taille en x et y
    int gapx = newSize.x - m_mapSize.x;
    int gapy = newSize.y - m_mapSize.y;

    if (gapx > 0) // on agrandit la largeur
    {
        std::vector<int>::iterator it;
        std::vector<bool>::iterator it2;

        if (rightDown) // on agrandit à droite
        {
            for (int j = 0; j < newSize.y; j++) // on insère gapx fois 0 au bout de chaque "ligne"
            {
                it = m_textureNumbers.begin();
                int distance = j * newSize.x + m_mapSize.x;
                m_textureNumbers.insert(it + distance, gapx, 1);
            }
        }
        else // on agrandit a gauche
        {
            for (int j = 0; j < newSize.y; j++) // on insère gapx fois 0 au début de chaque "ligne"
            {
                it = m_textureNumbers.begin();
                int distance = j * newSize.x;
                m_textureNumbers.insert(it + distance, gapx, 1);
            }
            for (int j = 0; j < (int)m_decorations.size(); j++)
            {
                m_decorations[j].move(vec2f((float)(gapx * m_tileSize.x), 0.f));
            }
            for (int j = 0; j < (int)m_stairs.size(); j++)
            {
                m_stairs[j].move(vec2f((float)(gapx * m_tileSize.x), 0.f));
            }
            for (uint32_t i = 0; i < m_layerCount; i++)
            {
                for (int j = 0; j < (int)m_colliders[i].size(); j++)
                {
                    m_colliders[i][j].move(vec2f((float)(gapx * m_tileSize.x), 0.f));
                }
            }
        }
       
    }
    else if (gapx < 0) // on rétrécit la largeur
    {    
        std::vector<int>::iterator it;
        if (rightDown) // on rétrécit à droite
        {
            for (int j = 0; j < newSize.y; j++) // on enlève gapx élément au bout de chaque "ligne"
            {
                it = m_textureNumbers.begin();
                int distance = (j + 1) * newSize.x;
                m_textureNumbers.erase(it + distance, it + distance - gapx);
            }
            for (auto it = m_decorations.begin(); it != m_decorations.end();)
            {
                if (it->getPosition().x > m_tileSize.x * newSize.x)
                {
                    it = m_decorations.erase(it);
                }
                else
                    it++;
            }
            for (auto it = m_stairs.begin(); it != m_stairs.end();)
            {
                if (it->getPosition().x > m_tileSize.x * newSize.x)
                {
                    it = m_stairs.erase(it);
                }
                else
                    it++;
            }
            for (uint32_t i = 0; i < m_layerCount; i++)
            {
                for (auto it = m_colliders[i].begin(); it != m_colliders[i].end();)
                {
                    if (it->getPosition().x > getSize().x)
                    {
                        it = m_colliders[i].erase(it);
                    }
                    else
                        it++;
                }
            }
        }
        else // on rétrécit à gauche
        {
            for (int j = 0; j < newSize.y; j++) // on enlève gapx élément au début de chaque "ligne"
            {
                it = m_textureNumbers.begin();
                int distance = j * newSize.x;
                m_textureNumbers.erase(it + distance, it + distance - gapx);
            }
            for (auto it = m_decorations.begin(); it != m_decorations.end();)
            {
                it->move(vec2f((float)(gapx * m_tileSize.x), 0.f));
                if (it->getPosition().x < getPosition().x)
                {
                    it = m_decorations.erase(it);
                }
                else
                    it++;
            }
            for (auto it = m_stairs.begin(); it != m_stairs.end();)
            {
                it->move(vec2f((float)(gapx * m_tileSize.x), 0));
                if (it->getPosition().x < getPosition().x)
                {
                    it = m_stairs.erase(it);
                }
                else
                    it++;
            }
            for (uint32_t i = 0; i < m_layerCount; i++)
            {
                for (auto it = m_colliders[i].begin(); it != m_colliders[i].end();)
                {
                    it->move(vec2f((float)(gapx * m_tileSize.x), 0));
                    if (it->getPosition().x < getPosition().x)
                    {
                        it = m_colliders[i].erase(it);
                    }
                    else
                        it++;
                }
            }
        }
        m_textureNumbers.resize(newSize.x * newSize.y);
        
    }
    if (gapy > 0) // on agrandit en hauteur
    {
        if (rightDown) // on agrandit en bas
                m_textureNumbers.resize(newSize.x * newSize.y, 1);

        else // on agrandit en haut
        {
            std::vector<int>::iterator it;
            it = m_textureNumbers.begin();
            m_textureNumbers.insert(m_textureNumbers.begin(), gapy * newSize.x, 1);

            for (int j = 0; j < (int)m_decorations.size(); j++)
            {
                m_decorations[j].move(vec2f(0.f, (float)(gapy * m_tileSize.y)));
            }
            for (int j = 0; j < (int)m_stairs.size(); j++)
            {
                m_stairs[j].move(vec2f(0.f, (float)(gapy * m_tileSize.y)));
            }
            for (uint32_t i = 0; i < m_layerCount; i++)
            {
                for (int j = 0; j < (int)m_colliders[i].size(); j++)
                {
                    m_colliders[i][j].move(vec2f(0.f, (float)(gapy * m_tileSize.y)));
                }
            }
            
        }
    }
    else if (gapy < 0) // on rétrécit en hauteur
    {
        if (rightDown) // on rétrécit en bas
        {
            m_textureNumbers.erase(m_textureNumbers.begin() + (newSize.x * newSize.y), m_textureNumbers.end());
            m_textureNumbers.resize(newSize.x * newSize.y);

            for (auto it = m_decorations.begin(); it != m_decorations.end();)
            {
                if (it->getPosition().y > m_tileSize.y * newSize.y)
                {
                    it = m_decorations.erase(it);
                }
                else
                    it++;
            }
            for (auto it = m_stairs.begin(); it != m_stairs.end();)
            {
                if (it->getPosition().y > m_tileSize.y * newSize.y)
                {
                    it = m_stairs.erase(it);
                }
                else
                    it++;
            }
            for (uint32_t i = 0; i < m_layerCount; i++)
            {
                for (auto it = m_colliders[i].begin(); it != m_colliders[i].end();)
                {
                    if (it->getPosition().y > getSize().y)
                    {
                        it = m_colliders[i].erase(it);
                    }
                    else
                        it++;
                }
            }
            
        }
        else // on rétrécit en haut
        {
            m_textureNumbers.erase(m_textureNumbers.begin(), m_textureNumbers.begin() - (gapy * newSize.x));
            m_textureNumbers.resize(newSize.x * newSize.y);

            for (auto it = m_decorations.begin(); it != m_decorations.end();)
            {
                it->move(vec2f(0.f, (float)(gapy * m_tileSize.y)));
                if (it->getPosition().y < getPosition().y)
                {
                    it = m_decorations.erase(it);
                }
                else
                    it++;
            }
            for (auto it = m_stairs.begin(); it != m_stairs.end();)
            {
                it->move(vec2f(0.f, (float)(gapy * m_tileSize.y)));
                if (it->getPosition().y < getPosition().y)
                {
                    it = m_stairs.erase(it);
                }
                else
                    it++;
            }
            for (uint32_t i = 0; i < m_layerCount; i++)
            {
                for (auto it = m_colliders[i].begin(); it != m_colliders[i].end();)
                {
                    it->move(vec2f(0.f, (float)(gapy * m_tileSize.y)));
                    if (it->getPosition().y < getPosition().y)
                    {
                        it = m_colliders[i].erase(it);
                    }
                    else
                        it++;
                }
            }
         
        }
    }

    m_mapSize = newSize;

    // on resize le vertexs arrays et on load les textures
    
    m_vertices.resize(newSize.x * newSize.y * 4); 
    loadTextureFromTab();

    m_path.resize(newSize.x* newSize.y * 4);
    m_path.assign(newSize.x* newSize.y * 4, true);
    loadPathView();
}

void Tilemap::changeTile(const vec2f &pos, const int &textNum)
{
    // Le changement s'applique uniquement au layer actif

    if (pos.x < m_tileSize.x * m_mapSize.x && pos.y < m_tileSize.y * m_mapSize.y && pos.x >= 0 && pos.y >= 0) // on vérifie que la tile n'est pas hors de la map
    {
        // On récupère le quad correspondant à la position de la tile
        int indice = ((int)(pos.x / m_tileSize.x) + ((int)(pos.y / m_tileSize.y)) * m_mapSize.x) * 4;
        sf::Vertex *quad = &m_vertices[indice];

        int tu = textNum % (m_tileSet.getSize().x / (int)m_tileSize.x);
        int tv = textNum / (m_tileSet.getSize().x / m_tileSize.x);

        // On applique les nouvelles coordonées de textures
        quad[0].texCoords = vec2f((float)(tu * m_tileSize.x), (float)(tv * m_tileSize.y));
        quad[1].texCoords = vec2f((float)((tu + 1) * m_tileSize.x), (float)(tv * m_tileSize.y));
        quad[2].texCoords = vec2f((float)((tu + 1) * m_tileSize.x), (float)((tv + 1) * m_tileSize.y));
        quad[3].texCoords = vec2f((float)(tu * m_tileSize.x), (float)((tv + 1) * m_tileSize.y));

        // On sauvegarde le numéro de la texture dans m_textureNumbers

        int index = ((int)quad[0].position.x / m_tileSize.x) + m_mapSize.x * ((int)quad[0].position.y / m_tileSize.y);
        m_textureNumbers[index] = textNum;
    }
    else
    {
        std::cout << "changeTile() impossible : tile hors de la map : ";
        printVec(pos);
    }
}

void Tilemap::loadTextureFromTab()
{
    for (int y = 0; y < m_mapSize.y; y++)
    {
        for (int x = 0; x < m_mapSize.x; x++)
        {
            // on récupere le numéro de la texture et on déduit la position dans le tileSet
            int textNum = m_textureNumbers[x + y * m_mapSize.x];

            int tu = textNum % (m_tileSet.getSize().x / m_tileSize.x);
            int tv = textNum / (m_tileSet.getSize().x / m_tileSize.x);

            // on récupère un pointeur vers le quad correspondant a la tile
            sf::Vertex *quad = &m_vertices[(x + y * m_mapSize.x) * 4];

            // on met a jour la position du quad dans la map
            quad[0].position = vec2f((float)(x * m_tileSize.x), (float)(y * m_tileSize.y));
            quad[1].position = vec2f((float)((x + 1) * m_tileSize.x), (float)(y * m_tileSize.y));
            quad[2].position = vec2f((float)((x + 1) * m_tileSize.x), (float)((y + 1) * m_tileSize.y));
            quad[3].position = vec2f((float)(x * m_tileSize.x), (float)((y + 1) * m_tileSize.y));

            // on met a jour ses coordonées de textures
            quad[0].texCoords = vec2f((float)(tu * m_tileSize.x), (float)(tv * m_tileSize.y));
            quad[1].texCoords = vec2f((float)((tu + 1) * m_tileSize.x), (float)(tv * m_tileSize.y));
            quad[2].texCoords = vec2f((float)((tu + 1) * m_tileSize.x), (float)((tv + 1) * m_tileSize.y));
            quad[3].texCoords = vec2f((float)(tu * m_tileSize.x), (float)((tv + 1) * m_tileSize.y));
        }
    }
    
}

void Tilemap::changePath(const vec2f& pos, const bool& op)
{
    if (pos.x < m_tileSize.x * m_mapSize.x && pos.y < m_tileSize.y * m_mapSize.y && pos.x >= 0 && pos.y >= 0) // on vérifie que la tile n'est pas hors de la map
    {
        // On récupère le quad correspondant à la position de la tile
        int indice = (int)(pos.x / (m_tileSize.x / 2)) + ((int)(pos.y / (m_tileSize.y / 2))) * m_mapSize.x * 2;

        m_path[indice] = op;
        m_pathView[indice].setFillColor(op ? sf::Color::Transparent : sf::Color(255, 255, 255, 100));
    }
    else
    {
        std::cout << "changeTile() impossible : tile hors de la map : ";
        printVec(pos);
    }
}

void Tilemap::loadPathView()
{
    m_pathView.clear();

    for (int y = 0; y < m_mapSize.y * 2; y++)
    {
        for (int x = 0; x < m_mapSize.x * 2; x++)
        {
            // on récupere le booleen de la case et on déduit la position
            bool passable = m_path[x + y * m_mapSize.x * 2];

            sf::RectangleShape r(vec2f(m_tileSize.x / 2.f, m_tileSize.y / 2.f));
            r.setPosition(vec2f(x * m_tileSize.x / 2.f, y * m_tileSize.y / 2.f));
            r.setFillColor(passable ? sf::Color::Transparent : sf::Color(255, 255, 255, 100));
            m_pathView.push_back(r);
        }
    }
}

void Tilemap::setActiveLevel(const int &select)
{
    m_activeLevel = select;
}

void Tilemap::addLevel()
{
    m_layerCount++;
    m_colliders.push_back(std::vector<PolygoneCollider >());
}

bool Tilemap::removeLevel()
{
    if (m_layerCount > 1) // On empèche de supprimer le premier layer
    {
        m_colliders.pop_back();

        for (auto it = m_decorations.begin(); it != m_decorations.end();)
        {
            if (it->m_layer == m_layerCount - 1)
            {
                it = m_decorations.erase(it);
            }
            else
                it++;
        }
        for (auto it = m_stairs.begin(); it != m_stairs.end();)
        {
            if (it->m_layer == m_layerCount - 2)
            {
                it = m_stairs.erase(it);
            }
            else
                it++;
        }

        if (m_activeLevel == m_layerCount - 1)
            m_activeLevel--;

        m_layerCount--;

        return true;
    }
    else
    {
        std::cout << "Impossible de supprimer le premier Layer" << std::endl;
        return false;
    }
}

void Tilemap::addDecoration(Entity entity)
{
    m_decorations.push_back(entity);
    orderDecorations();
}

void Tilemap::removeDecoration()
{
    if (!m_decorations.empty())
    {
        m_decorations.pop_back();
    }
}

void Tilemap::orderDecorations()
{
    std::sort(m_decorations.begin(), m_decorations.end(), [](const Entity& a, const Entity& b) {return a.getPosition().y < b.getPosition().y; });
}

void Tilemap::addStair(Stairs stair)
{
    if (m_activeLevel == m_layerCount - 1)
        std::cout << "Impossible d'ajouter un escalier au dernier level" << std::endl;
    else
        m_stairs.push_back(stair);
}

void Tilemap::removeStair()
{
    if (!m_stairs.empty())
    {
        m_stairs.pop_back();
    }
}

void Tilemap::addCollider(const vec2fTab &vertices)
{
    std::vector<PolygoneCollider > shapes = getShapeColliders(vertices);
    for (int i = 0; i < (int)shapes.size(); i++)
    {
        m_colliders[m_activeLevel].push_back(shapes[i]);
    }
}

void Tilemap::removeCollider()
{
    if (!m_colliders[m_activeLevel].empty())
    {
        m_colliders[m_activeLevel].pop_back();
    }
}

void Tilemap::handleCollisions(Entity *entity)
{
    vec2f resolve;
    for (int i = 0; i < (int)m_colliders[entity->m_layer].size(); i++)
    {
        entity->entityCollision(m_colliders[entity->m_layer][i]);
    }
    for (int i = 0; i < (int)m_decorations.size(); i++)
    {
        if(entity->m_layer == m_decorations[i].m_layer)
            entity->entityCollision(m_decorations[i]);
    }
    for (int i = 0; i < (int)m_stairs.size(); i++)
    {
        entity->entityCollision(m_stairs[i]);
    }
}

vec2f Tilemap::getOverflyTile(const vec2f &mousPos) const
{
    vec2f result;
    result.x = (float)((int)(mousPos.x / m_tileSize.x) * m_tileSize.x);
    if (mousPos.x < 0)
        result.x -= m_tileSize.x;

    result.y = (float)((int)(mousPos.y / m_tileSize.y) * m_tileSize.y);
    if (mousPos.y < 0)
        result.y -= m_tileSize.y;

    return result;
}

vec2f Tilemap::getDemiOverflyTile(const vec2f &mousPos) const
{
    vec2f result;
    result.x = (float)((int)(mousPos.x / (m_tileSize.x / 2.f)) * (m_tileSize.x / 2));
    if (mousPos.x < 0)
        result.x -= m_tileSize.x / 2.f;

    result.y = (float)((int)(mousPos.y / (m_tileSize.y / 2.f)) * (m_tileSize.y / 2));
    if (mousPos.y < 0)
        result.y -= m_tileSize.y / 2.f;

    return result;
}

vec2fTab Tilemap::getRectangle(const vec2fTab &vertices, const unsigned int &i)
{
    int n = (int)vertices.size();
    if (n < 4)
    {
        std::cerr << "erreur getRectangle() : nombre de sommets insuffisant ( < 4 )" << std::endl;
        exit(EXIT_FAILURE);
    }

    // on récupère les 2 sommets de l'arrete ainsi que le sommet qui la precede et celui qui la suit
    // NB : les points du polygone sont définits dans le sens horaire
    // mais on les parcours dans le sens anti horaire pour avoir les colliders a l'interieur du polygone

    vec2f pointX, precX, pointY, suivY;

    pointX = vertices[i];
    precX = (i == 0 ? vertices[n - 1] : vertices[i - 1]);
    pointY = vertices[(i + 1) % n];
    suivY = vertices[(i + 2) % n];

    // NB : les points sont définits dans le sens horaire donc avec notre fonction, le vecteur normal va toujours vers l'interieur du polygone

    vec2f dirVec = directVector(pointX, pointY);
    vec2f normalVec = -1.f * normalVector(pointX, pointY);
    vec2f precDirVec = directVector(pointX, precX);
    vec2f suivDirVec = directVector(pointY, suivY);

    if (dirVec == vec2f(1.f, 0.f)) // le vecteur va vers la droite
    {
        if (normalVec != precDirVec) // tile dispo à gauche ?
            pointX.x -= (m_tileSize.x / 2.f);
        if (normalVec != suivDirVec) // tile dispo à droite ?
            pointY.x += (m_tileSize.x / 2.f);

        vec2f topLeft = vec2f(pointX.x, pointX.y - m_tileSize.y / 2.f);
        vec2f topRight = vec2f(pointY.x, pointY.y - m_tileSize.y / 2.f);
        vec2f botRight = pointY;
        vec2f botLeft = pointX;

        return vec2fTab{topLeft, topRight, botRight, botLeft};
    }
    else if (dirVec == vec2f(-1.f, 0.f)) // le vecteur va vers la gauche
    {
        if (normalVec != precDirVec) // tile dispo à droite ?
            pointX.x += m_tileSize.x / 2.f;
        if (normalVec != suivDirVec) // tile dispo à gauche ?
            pointY.x -= m_tileSize.x / 2.f;

        vec2f topLeft = pointY;
        vec2f topRight = pointX;
        vec2f botRight = vec2f(pointX.x, pointX.y + m_tileSize.y / 2.f);
        vec2f botLeft = vec2f(pointY.x, pointY.y + m_tileSize.y / 2.f);

        return vec2fTab{topLeft, topRight, botRight, botLeft};
    }
    else if (dirVec == vec2f(0.f, 1.f)) // le vecteur va vers le bas
    {
        if (normalVec != precDirVec) // tile dispo en haut ?
            pointX.y -= m_tileSize.y / 2.f;
        if (normalVec != suivDirVec) // tile dispo en bas ?
            pointY.y += m_tileSize.y / 2.f;

        vec2f topLeft = pointX;
        vec2f topRight = vec2f(pointX.x + m_tileSize.x / 2.f, pointX.y);
        vec2f botRight = vec2f(pointY.x + m_tileSize.x / 2.f, pointY.y);
        vec2f botLeft = pointY;

        return vec2fTab{topLeft, topRight, botRight, botLeft};
    }
    else if (dirVec == vec2f(0.f, -1.f)) // le vecteur va vers le haut
    {
        if (normalVec != precDirVec) // tile dispo en bas ?
            pointX.y += m_tileSize.y / 2.f;
        if (normalVec != suivDirVec) // tile dispo en bas ?
            pointY.y -= m_tileSize.y / 2.f;

        vec2f topLeft = vec2f(pointY.x - m_tileSize.x / 2.f, pointY.y);
        vec2f topRight = pointY;
        vec2f botRight = pointX;
        vec2f botLeft = vec2f(pointX.x - m_tileSize.x / 2.f, pointX.y);

        return vec2fTab{topLeft, topRight, botRight, botLeft};
    }
    else
    {
        std::cerr << "erreur getRectangle() : forme non alligné dans le repère orthogonal" << std::endl;
        exit(EXIT_FAILURE);
        return vec2fTab();
    }
}

std::vector<PolygoneCollider > Tilemap::getShapeColliders(const vec2fTab &vertices)
{
    std::vector<PolygoneCollider > result;
    for (int i = 0; i < (int)vertices.size(); i++)
    {
        vec2fTab carre = getRectangle(vertices, i);
        PolygoneCollider c(carre);
        result.push_back(c);
    }
    return result;
}

void Tilemap::update(const sf::Time& elapsed)
{
    for (int j = 0; j < (int)m_decorations.size(); j++)
    {
        m_decorations[j].update(elapsed);
    }
    for (int j = 0; j < (int)m_stairs.size(); j++)
    {
        m_stairs[j].update(elapsed);
    }
}

void Tilemap::saveToFile(std::ofstream& flux) const
{
    size_t n = m_tileSetFile.size();
    flux.write((char*)&n, sizeof(size_t));
    flux.write((char*)&m_tileSetFile[0], n * sizeof(char));

    saveVec(m_tileSize, flux);
    saveVec(m_mapSize, flux);

    flux.write((char*)&m_layerCount, sizeof(uint32_t));

    flux.write((char*)&m_spawnLayer, sizeof(int));
    saveVec(m_spawnPoint, flux);

    n = m_textureNumbers.size();
    flux.write((char*)&n, sizeof(size_t));
    flux.write((char*)&m_textureNumbers[0], n * sizeof(int));

    n = m_path.size();
    flux.write((char*)&n, sizeof(size_t));
    for (int i = 0; i < (int)n; i++)
    {
        bool b = m_path[i];
        flux.write((char*)&b, sizeof(bool));
    }
        
    for (uint32_t i = 0; i < m_layerCount; i++)
        savePolygoneTab(m_colliders[i], flux);

    saveEntityTab(m_decorations, flux);
    saveStairTab(m_stairs, flux);
}

void Tilemap::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = &m_tileSet;

    target.draw(m_vertices, states);

    bool playerDrawed = false;

    for (int j = 0; j < (int)m_stairs.size(); j++)
    {
        target.draw(m_stairs[j]);
    }

    for (int j = 0; j < (int)m_decorations.size(); j++)
    {
        if (m_decorations[j].getPosition().y + m_decorations[j].getSize().y > m_player->getPosition().y + m_player->getSize().y && !playerDrawed)
        {
            target.draw(*m_player);
            playerDrawed = true;
        }
        target.draw(m_decorations[j]);
    }
    if (!playerDrawed)
    {
        target.draw(*m_player);
    }
    
    for (int i = 0; i < (int)m_colliders[m_activeLevel].size(); i++)
    {
        target.draw(m_colliders[m_activeLevel][i]);
    }

    if (m_pathMode)
    {
        for (int i = 0; i < (int)m_pathView.size(); i++)
        {
            if(!m_path[i])
            target.draw(m_pathView[i]);
        }
    }
}