#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <SFML/Graphics.hpp>
#include "../Colliders/Colliders.hpp"
#include "../Entity/Entity.h"
#include "../Entity/Player.h"
#include "../Entity/Stairs.h"
#include "../Other/Alias.h"
#include <vector>
#include <string>
#include <fstream>


class Tilemap : public sf::Drawable, public sf::Transformable
{ 
public:
    Tilemap(const vec2i& tileSize, const std::string& textFile, const vec2i& mapSize, const uint32_t& levels);
    Tilemap(std::ifstream& flux);
    Tilemap() {}
    ~Tilemap() {}

    void resizeTM(const vec2i& newSize, const bool& rightDown);
    void changeTile(const vec2f& pos, const int& textNum);
    void loadTextureFromTab();

    void changePath(const vec2f& pos, const bool& op);
    void loadPathView();

    void setActiveLevel(const int& select);
    void addLevel();
    bool removeLevel();

    void addDecoration(Entity entity);
    void removeDecoration();
    void orderDecorations();

    void addStair(Stairs stair);
    void removeStair();

    void addCollider(const vec2fTab& m_vertices);
    void removeCollider();
    void handleCollisions(Entity* entity);

    vec2f getSize() { return vec2f((float)(m_mapSize.x * m_tileSize.x), (float)(m_mapSize.y * m_tileSize.y)); }

    vec2f getOverflyTile(const vec2f& mousPos) const;
    vec2f getDemiOverflyTile(const vec2f& mousPos) const;

    vec2fTab getRectangle(const vec2fTab& vertices, const unsigned int& i);
    std::vector<PolygoneCollider> getShapeColliders(const vec2fTab& vertices);

    void update(const sf::Time& elapsed);

    void saveToFile(std::ofstream& flux) const;

    void setPlayer(Player* p) { m_player = p; }
    void setSpawn(const vec2f& pos, const int& layer) { m_spawnPoint = pos; m_spawnLayer = layer; }
    void spawnPlayer() { m_player->setPosition(m_spawnPoint); m_player->m_layer = m_spawnLayer; }

    bool m_pathMode = false;
    int m_activeLevel = 0; // level actif (�dition)
    uint32_t m_layerCount = 1;

private:
    Player* m_player = nullptr;
    vec2f m_spawnPoint = vec2f(0, 0);
    int m_spawnLayer = 0;

    vec2i m_mapSize; // taille de la map en nombre de tiles
    vec2i m_tileSize; // taille d'une tile en pixel
    sf::Texture m_tileSet; // texture du tileset
    std::string m_tileSetFile;

    sf::VertexArray m_vertices; // Vertex Arrays de chaque layer
    std::vector<int> m_textureNumbers; // num�ro de tile de chaque layer

    std::vector<bool> m_path; // endroits accessibles pour le pathFinding
    std::vector<sf::RectangleShape> m_pathView; // pour visualiser m_path

    std::vector<Entity> m_decorations; // decorations de la map
    std::vector<Stairs> m_stairs; // escaliers de la map
    
    std::vector<std::vector<PolygoneCollider>> m_colliders; // Colliders de chaque levels
 
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

#endif