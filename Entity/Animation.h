#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <fstream>
#include "../Other/Alias.h"

class Animation : public sf::Transformable, public sf::Drawable
{
public:
	Animation(const std::string& textFile, const vec2i& frameSize, const uint32_t& frameCount, const float& speed);
	~Animation() {}

	void update(const sf::Time& elapsed);
	void reset();

	vec2f getSize() const { return vec2f(m_frameSize.x * getScale().x, m_frameSize.y * getScale().y); }

private:
	std::string m_textureFile;
	vec2i m_frameSize;
	uint32_t m_frameCount;

	sf::Texture m_texture;
	sf::Sprite m_sprite;
	std::vector<sf::IntRect> m_framesTab;
	float m_speed = 0.f , m_frameNum = 0;
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	friend void saveAnimation(const Animation& a, std::ofstream& flux);
	friend Animation loadAnimation(std::ifstream& flux);

	friend void saveAnimationTab(const std::vector<Animation>& a, std::ofstream& flux);
	friend std::vector<Animation> loadAnimationTab(std::ifstream& flux);
};

void saveAnimation(const Animation& a, std::ofstream& flux);
Animation loadAnimation(std::ifstream& flux);

void saveAnimationTab(const std::vector<Animation>& a, std::ofstream& flux);
std::vector<Animation> loadAnimationTab(std::ifstream& flux);

#endif
