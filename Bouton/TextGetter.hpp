/**
 * @file TextGetter.hpp
 * @brief Permet de réaliser une entrée texte avec SFML
 */
#ifndef TEXTGETTER_H
#define TEXTGETTER_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Bouton.hpp"

/**
 * @brief Bloc de saisie de texte avec SFML
 * 
 */
class TextGetter : public sf::Drawable, public sf::Transformable
{
public:
	/**
	 * @brief Construct a new TextGetter object
	 * 
	 * @param fontFile Police du texte
	 * @param position Position du bloc
	 */
	TextGetter(const std::string &fontFile, const vec2f& position);
	~TextGetter() { delete m_submit; }

	vec2f getSize() const{
		return vec2f(m_bg.getSize().x * getScale().x, m_bg.getSize().y * getScale().y);
	}

	void setSize(const vec2f& size);

	/**
	 * @brief Actualise le TextGetter
	 * 
	 * @param pos position du curseur
	 * @param txt texte entré
	 */
	void update(const vec2f& pos, const unsigned int& txt = 0);

	/**
	 * @brief Check si le bouton submit est cliqué
	 * 
	 * @param pos position du curseur
	 * @param s chaine de charactères entré (sortie)
	 * @return True si le bouton est cliqué, false sinon
	 */
	bool submit(const vec2f& pos, std::string& s);

private:
	void removeLetter();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	Bouton * m_submit;
	sf::RectangleShape m_bg, m_txtbar;
	sf::Font m_font;
	sf::Text m_displayTxt;
	std::string m_txt;
};

#endif