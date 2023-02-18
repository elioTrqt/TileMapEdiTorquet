#include "Animation.h"
#include <iostream>
#include "../Other/vectorFunctions.hpp"

Animation::Animation(const std::string& textFile, const vec2i& frameSize, const uint32_t& frameCount, const float& speed) : m_textureFile(textFile), m_frameSize(frameSize), m_frameCount(frameCount), m_speed(speed)
{
	// On charge la texture des frames
	if (!m_texture.loadFromFile(textFile))
	{
		std::cout << "Animation() : texture " << textFile << " non charg�" << std::endl;
	}

	// On setup le rectangle de texture de chaque frame
	for (uint32_t i = 0; i < frameCount ; i++)
	{
		int tu = i % ( (int)m_texture.getSize().x / frameSize.x );
		int tv = i / ((int)m_texture.getSize().x / frameSize.x);

		m_framesTab.push_back(sf::IntRect(tu * frameSize.x, tv * frameSize.y, frameSize.x, frameSize.y));
	}
	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(m_framesTab[0]);
}

void Animation::update(const sf::Time& elapsed)
{
	m_sprite.setTexture(m_texture);

	m_frameNum += m_speed; // *elapsed.asSeconds();
	if (m_frameNum > m_frameCount )
		m_frameNum -= m_frameCount;

	m_sprite.setTextureRect(m_framesTab[(int)m_frameNum]);
}

void Animation::reset()
{
	m_frameNum = 0;
	m_sprite.setTextureRect(m_framesTab[0]);
}

void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(m_sprite, states);
}

void saveAnimation(const Animation& a, std::ofstream& flux)
{
	size_t n = a.m_textureFile.size();
	flux.write((char*)&n, sizeof(size_t));
	flux.write((char*)&a.m_textureFile[0], n * sizeof(char));

	saveVec(a.m_frameSize, flux);

	flux.write((char*)&a.m_frameCount, sizeof(uint32_t));
	flux.write((char*)&a.m_speed, sizeof(float));
}

Animation loadAnimation(std::ifstream& flux)
{
	size_t n;
	flux.read((char*)&n, sizeof(size_t));

	std::string textFile;
	textFile.resize(n);
	flux.read((char*)&textFile[0], n * sizeof(char));

	vec2i frameSize = loadVec<int>(flux);

	uint32_t frameCount;
	flux.read((char*)&frameCount, sizeof(uint32_t));
	float speed;
	flux.read((char*)&speed, sizeof(float));

	return Animation(textFile, frameSize, frameCount, speed);
}

void saveAnimationTab(const std::vector<Animation>& a, std::ofstream& flux)
{
	size_t n = a.size();
	flux.write((char*)&n, sizeof(size_t));

	for (int i = 0; i < (int)n; i++)
	{
		saveAnimation(a[i], flux);
	}
}

std::vector<Animation> loadAnimationTab(std::ifstream& flux)
{
	size_t n;
	flux.read((char*)&n, sizeof(size_t));

	std::vector<Animation> a;
	for (int i = 0; i < (int)n; i++)
	{
		a.push_back(loadAnimation(flux));
	}

	return a;
}
