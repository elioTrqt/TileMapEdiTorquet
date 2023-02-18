#ifndef EDITORSTATE_H
#define EDITORSTATE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "gamestate.h"
#include "../Bouton/Bouton.hpp"
#include "../Tilemap/TilePalette.h"
#include "../Tilemap/Tilemap.hpp"
#include "../Entity/Entity.h"
#include "../Other/Alias.h"

enum editMode{TileM, PropM, StairM, ColliderM, PathM};

class CMapEditorState : public CGameState
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(CGameEngine* game);
	void Update(CGameEngine* game);
	void Draw(CGameEngine* game);

	static CMapEditorState* Instance() {
		return &m_EditorState;
	}

	Tilemap getTM() { return *m_tMap; }
	bool saveTM(const char* file);
	bool loadTM(const char* file);

	sf::View getView() { return *m_vueHUD; }

protected:
	CMapEditorState() { }

private:
	static CMapEditorState m_EditorState;

	sf::View* m_vueHUD = nullptr;
	sf::View* m_vueMap = nullptr;

	Tilemap* m_tMap = nullptr;
	TilePalette* m_tPalette = nullptr;
	PropPalette* m_pPalette = nullptr;
	PropPalette* m_sPalette = nullptr;

	std::vector< Bouton* > m_levelBoutons; // boutons pour changer de layer
	Bouton* m_addRmLevelBoutons[2];
	Bouton* m_colliderBoutons[2]; // remove / send collider
	Bouton* m_modeBouton; // Tile / Prop / Stairs

	std::vector< vec2f> m_actualCollider; // collider en construction
	sf::VertexArray* m_colliderDraw; // dessin du collider en construction
	sf::CircleShape* m_cursor; // curseur allign� sur la grille 
	sf::RectangleShape* m_leftBar = nullptr, *m_topBar = nullptr, *m_highlightTile = nullptr, *m_demiHighlightTile = nullptr; // barres de menu et tile en surbrillance
	Entity* m_actualProp = nullptr;
	Player* m_player;

	bool k_d = false, k_q = false, k_z = false, k_s = false, k_leftClick = false, k_rightClick = false, k_ctrl = false; // touches press�s ou non
	bool m_colliderClosed = false;
	int m_resizeFactor = 0;

	editMode m_mode = TileM;

	void initHUD();
	void mousePressedEvent(const sf::Mouse::Button& button);
	void mousePressedUp();
	void mousePressedLateral();
	void mousePressedMap();
	void keyPressedEvent(const sf::Keyboard::Key& key, CGameEngine* game);
	void addPoint(const vec2f& pos);
};
#endif
