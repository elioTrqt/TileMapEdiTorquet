#include "TextGetter.hpp"

TextGetter::TextGetter(const std::string &fontFile, const vec2f &position)
{
	setPosition(position);

	m_bg.setSize(vec2f(500.f, 200.f));
	m_bg.setFillColor(sf::Color::Blue);
	m_bg.setOutlineColor(sf::Color::Black);
	m_bg.setOutlineThickness(1.5f);

	m_txtbar.setSize(vec2f(468.f, 75.f));
	m_txtbar.setFillColor(sf::Color::White);
	m_txtbar.setOutlineColor(sf::Color::Black);
	m_txtbar.setOutlineThickness(1.f);
	m_txtbar.setPosition(16.f, 16.f);

	m_txt = "";
	m_font.loadFromFile(fontFile);

	m_displayTxt.setFont(m_font);
	m_displayTxt.setCharacterSize(30);
	m_displayTxt.setString("");
	m_displayTxt.setPosition(25.f, 30.f);
	m_displayTxt.setFillColor(sf::Color::Black);
	
	m_submit = new Bouton("Entrer", fontFile, 25);
	m_submit->setPosition(vec2f(250 - m_submit->getSize().x / 2.f, 125.f));
}

void TextGetter::setSize(const vec2f& size){
	float x = size.x / getSize().x;
    float y = size.y / getSize().y;

    scale(x, y);
}

void TextGetter::update(const vec2f &pos, const unsigned int &txt)
{
	m_submit->update(vec2f((pos.x - getPosition().x) / getScale().x, (pos.y - getPosition().y) / getScale().y));

	if (txt == '\b')
	{
		removeLetter();
	}
	else if (txt != 0 && txt < 128)
	{
		char c = static_cast<char>(txt);
		m_txt += c;
		m_displayTxt.setString(m_txt);
	}
}

bool TextGetter::submit(const vec2f &pos, std::string &s)
{
	if (m_submit->isMouseOver(pos - getPosition()))
	{
		s = m_txt;
		return true;
	}
	else
		return false;
}

void TextGetter::removeLetter()
{
	if (!m_txt.empty())
	{
		m_txt.erase(m_txt.end() - 1);
		m_displayTxt.setString(m_txt);
	}
}

void TextGetter::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(m_bg, states);
	target.draw(m_txtbar, states);
	target.draw(m_displayTxt, states);
	target.draw(*m_submit, states);
}
