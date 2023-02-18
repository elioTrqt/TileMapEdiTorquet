#ifndef TILEPALETTE_H
#define TILEPALETTE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "../Bouton/Bouton.hpp"
#include "../Other/Alias.h"

class TilePalette : public sf::Drawable, public sf::Transformable
{
public:
	TilePalette(vec2f pos, const unsigned int& nbColone, const vec2i& butSize, const std::string& tileSet, const vec2i& tileSize);
	TilePalette(vec2f pos);
	virtual ~TilePalette();

	virtual bool select(const vec2f& pos, int& index);
	void update(const vec2f& pos);

	int getSelected() { return m_selectedIndex; }

protected:
	sf::Texture m_tileSet; // texture du tileSet
	std::vector<SpriteBouton *> m_boutonTiles; // Les boutons 
	int m_selectedIndex = -1;
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class PropPalette : public TilePalette
{
public:
	PropPalette(vec2f pos, const unsigned int& nbColone, const vec2i& butSize, const std::vector<std::string>& textFiles);
	~PropPalette();

	bool select(const vec2f& pos, int& index);

private:
	std::vector<sf::Texture*> m_textures;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

#endif