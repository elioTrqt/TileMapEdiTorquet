#include "Bouton.hpp"
#include <iostream>

// BASE BOUTON //

Bouton::Bouton(const std::string &text, const std::string &fontFile, const unsigned int &charSize, const sf::Color &charColor, const sf::Color &butColor, const vec2f &position)
{
    setPosition(position);

    m_font.loadFromFile(fontFile);
    m_txt.setFont(m_font);
    m_txt.setString(text);
    m_txt.setCharacterSize(charSize);
    m_txt.setFillColor(charColor);
    m_txt.setLetterSpacing(1.5f);

    m_button.setSize(vec2f(m_txt.getLocalBounds().width + 20.f, m_txt.getLocalBounds().height + 20.f));
    m_button.setFillColor(butColor);
    m_button.setOutlineThickness(1.f);
    m_button.setOutlineColor(sf::Color::Black);
}

void Bouton::Init(const std::string &text, const std::string &fontFile, const unsigned int &charSize, const sf::Color &charColor, const sf::Color &butColor, const vec2f &position)
{
    setPosition(position);

    m_font.loadFromFile(fontFile);
    m_txt.setFont(m_font);
    m_txt.setString(text);
    m_txt.setCharacterSize(charSize);
    m_txt.setFillColor(charColor);
    m_txt.setLetterSpacing(1.5f);

    m_button.setSize(vec2f(m_txt.getLocalBounds().width + 20.f, m_txt.getLocalBounds().height + 20.f));
    m_button.setFillColor(butColor);
    m_button.setOutlineThickness(1.f);
    m_button.setOutlineColor(sf::Color::Black);
}

void Bouton::setSize(const vec2f &size)
{
    float x = size.x / getSize().x;
    float y = size.y / getSize().y;

    scale(x, y);
}

void Bouton::update(const vec2f &pos)
{
    m_button.setOutlineColor(m_isPushed ? sf::Color::Red : sf::Color::Black);
    m_button.setOutlineThickness(isMouseOver(pos) ? 3.f : (m_isPushed ? 2.f : 1.f));
}

void Bouton::push(const bool &op)
{
    m_isPushed = op;
}

void Bouton::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_button, states);

    states.transform.translate(10.f, 5.f);
    target.draw(m_txt, states);
}

// DERIVED SPRITES BOUTONS //

SpriteBouton::SpriteBouton(sf::Texture *texture, const sf::IntRect &textRect, const vec2i &buttonSize, const vec2f &position) : m_textRect(textRect), m_texture(texture)
{
    setPosition(position);
    m_button.setSize(vec2f((float)buttonSize.x, (float)buttonSize.y));
    m_button.setTexture(m_texture);
    m_button.setTextureRect(m_textRect);
    m_button.setOutlineThickness(1.f);
    m_button.setOutlineColor(sf::Color::Black);
}

void SpriteBouton::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(m_button, states);
}

bool select(const vec2f &pos, std::vector<Bouton *> &boutons, int &selected)
{
    for (int i = 0; i < (int)boutons.size(); i++)
    {
        if (boutons[i]->isMouseOver(pos))
        {
            selected = i;
            for (int j = 0; j < (int)boutons.size(); j++)
                boutons[j]->push(false);

            boutons[i]->push(true);
            return true;
        }
    }
    return false;
}
