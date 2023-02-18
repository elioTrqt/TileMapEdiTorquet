#include "TilePalette.h"
#include <iostream>

TilePalette::TilePalette(vec2f position, const unsigned int& nbColone, const vec2i& butSize, const std::string& tileSet, const vec2i& tileSize)
{
	// On setup la position et on charge la texture
	setPosition(position);

	if (!m_tileSet.loadFromFile(tileSet))
	{
		std::cout << "erreur : texture de TilePalette non charg�" << std::endl;
		exit(EXIT_FAILURE);
	}

	// On positione chaque bouton
	vec2f pos(0, 0);

	int count = 0;
	for (int y = 0; y < (int)m_tileSet.getSize().y / tileSize.y; y++)
	{
		for (int x = 0; x < (int)m_tileSet.getSize().x / tileSize.x; x++)
		{
			pos.x += 16;
			SpriteBouton * b = new SpriteBouton(&m_tileSet, sf::IntRect(x * tileSize.x, y * tileSize.y, tileSize.x, tileSize.y), butSize, pos);
			m_boutonTiles.push_back(b);
			pos.x += butSize.x;
			count++;
			if (count == (int)nbColone)
			{
				count = 0;
				pos.y += butSize.y + 16;
				pos.x -= nbColone * (16 + butSize.x);
			}
			
		}
	}
}

TilePalette::TilePalette(vec2f pos)
{
	setPosition(pos);
}

TilePalette::~TilePalette()
{
	for (int i = 0; i < (int)m_boutonTiles.size(); i++)
	{
		delete m_boutonTiles[i];
	}
}

bool TilePalette::select(const vec2f& pos, int& index)
{
	for (int i = 0; i < (int)m_boutonTiles.size(); i++)
	{
		if (m_boutonTiles[i]->isMouseOver(pos - getPosition())) // la souris est au dessus d'un bouton
		{
			if(m_selectedIndex != -1)
				m_boutonTiles[m_selectedIndex]->push(false);

			// on sauvegarde le textRect et on "pousse" le bouton
			m_selectedIndex = i;
			index = i;
			m_boutonTiles[i]->push(true);
			return true;
		}
	}
	return false;
}

void TilePalette::update(const vec2f& pos)
{
	for (int i = 0; i < (int)m_boutonTiles.size(); i++)
	{
		m_boutonTiles[i]->update(pos - getPosition());
	}
}

void TilePalette::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	for (int i = 0; i < (int)m_boutonTiles.size(); i++)
	{
		target.draw(*m_boutonTiles[i], states);
	}
}

// DERIVED PROPPALETTE

PropPalette::PropPalette(vec2f position, const unsigned int& nbColone, const vec2i& butSize, const std::vector<std::string>& textFiles) : TilePalette(position)
{
	vec2f pos(0, 0);
	int count = 0;

	for (int i = 0; i < (int)textFiles.size(); i++)
	{
		pos.x += 16;
		m_textures.push_back(new sf::Texture());
		m_textures.back()->loadFromFile(textFiles[i]);
		SpriteBouton* b = new SpriteBouton(m_textures[i], sf::IntRect(0, 0, m_textures[i]->getSize().x, m_textures[i]->getSize().y), butSize, pos);
		m_boutonTiles.push_back(b);
		pos.x += butSize.x;
		count++;
		if (count == (int)nbColone)
		{
			count = 0;
			pos.y += butSize.y + 16;
			pos.x -= nbColone * (16 + butSize.x);
		}
		
	}
}

PropPalette::~PropPalette()
{
	for (int i = 0; i < (int)m_textures.size(); i++)
	{
		delete m_textures[i];
	}
}

bool PropPalette::select(const vec2f& pos, int& index)
{
	for (int i = 0; i < (int)m_boutonTiles.size(); i++)
	{
		if (m_boutonTiles[i]->isMouseOver(pos - getPosition())) // la souris est au dessus d'un bouton
		{
			if (m_selectedIndex != -1)
				m_boutonTiles[m_selectedIndex]->push(false);

			// on sauvegarde l'index
			m_selectedIndex = i;
			index = i;
			m_boutonTiles[i]->push(true);
			return true;
		}
	}
	return false;
}

void PropPalette::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	for (int i = 0; i < (int)m_boutonTiles.size(); i++)
	{
		target.draw(*m_boutonTiles[i], states);
	}
}