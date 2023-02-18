/*!
* @file Bouton.hpp
* @brief Définit les classes Boutons utilisables avec SFML
*/
#ifndef BOUTON_HPP
#define BOUTON_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "../Other/Alias.h"

/*!
 * @brief Bouton graphique avec un texte, utilisable dans une fenêtre SFML.
 * Les objets Boutons supportent les opérations scale et translate (pas de rotation).
*/
class Bouton : public sf::Drawable, public sf::Transformable
{
public:
    /*!
     * @brief Construit un objet de type Bouton, sa taille est définit en fonction de celle du texte
     * @param text Texte du bouton
     * @param fontFile Chemin du fichier contenant la police
     * @param charSize Taille des charactères
     * @param charColor Couleur du texte
     * @param butColor Couleur du bouton
     * @param position Position du bouton (coin superieur gauche)
    */
    Bouton(const std::string& text, const std::string& fontFile, const unsigned int& charSize, const sf::Color& charColor = sf::Color::Black, const sf::Color& butColor = sf::Color::White, const vec2f& position = vec2f(0.f, 0.f));
    Bouton() {}
    virtual ~Bouton() {}

    void Init(const std::string& text, const std::string& fontFile, const unsigned int& charSize, const sf::Color& charColor = sf::Color::Black, const sf::Color& butColor = sf::Color::White, const vec2f& position = vec2f(0.f, 0.f));

    /*!
     * @brief Calcule la taille d'un bouton
     * @return La Taille
    */
    vec2f getSize() const 
    {
        return vec2f(m_button.getSize().x * getScale().x, m_button.getSize().y * getScale().y);
    }

    /**
     * @brief Set the Size object
     * 
     * @param size taille
     */
    void setSize(const vec2f& size);

    /*!
     * @brief Détermine si un point survole le bouton
     * @param pos Position du curseur
     * @return true si le curseur est au dessus du bouton
    */
    bool isMouseOver(const vec2f& pos) const
    {
        sf::FloatRect bb(getPosition(), getSize());
        return bb.contains(pos);
    }

    /*!
     * @brief Actualise le style du bouton
     * @param pos Position du curseur
    */
    void update(const vec2f& pos);

    /**
     * @brief Change le statue du bouton
     * 
     * @param op on = true, off = false
     */
    void push(const bool& op);

    bool isPushed() {return m_isPushed;}

protected:
    sf::Font m_font;
    sf::Text m_txt;
    sf::RectangleShape m_button;
    bool m_isPushed = false;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};


/*!
 * @brief Bouton graphique avec une texture, d�riv� de la classe Bouton.
 * Les objets SpriteBoutons supportent les op�rations scale et translate (pas de rotation).
*/
class SpriteBouton : public Bouton
{
public :

    /*!
     * @brief Construit un objet de type SpriteBouton
     * @param textureFile Chemin du fichier contenant le tileSet
     * @param textRect Rectangle de texture 
     * @param buttonSize Taille du bouton
     * @param position Position du bouton (coin superieur gauche)
    */
    SpriteBouton(sf::Texture * texture, const sf::IntRect& textRect, const vec2i& buttonSize, const vec2f& position = vec2f(0.f, 0.f));
    ~SpriteBouton() {};

    sf::IntRect m_textRect;

private:
    sf::Texture * m_texture;
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

/**
 * @brief Teste si un bouton est cliqué parmis une liste de Boutons
 * 
 * @param pos Position du curseur
 * @param boutons Liste de boutons clicables
 * @param selected Indice du bouton selectioné (Sortie)
 * @return true si un bouton est selectioné, false sinon.
 */
bool select(const vec2f& pos, std::vector<Bouton*>& boutons, int& selected);

#endif