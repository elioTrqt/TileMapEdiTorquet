mainOut : main.o Stairs.o vectorFunctions.o Bouton.o TextGetter.o Collider.o CircleCollider.o PolygoneCollider.o Animation.o Entity.o Player.o TilePalette.o gameengine.o menustate.o mapeditorstate.o playstate.o escapestate.o Tilemap.o
	g++ main.o Stairs.o vectorFunctions.o Bouton.o TextGetter.o Collider.o CircleCollider.o PolygoneCollider.o Animation.o Entity.o Player.o TilePalette.o gameengine.o menustate.o mapeditorstate.o playstate.o escapestate.o Tilemap.o -o mainOut -std="c++17" -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

Stairs.o : Entity/Stairs.h Entity/Stairs.cpp Entity/Entity.h Entity/Player.h
	g++ -Wall Entity/Stairs.cpp -c

main.o : States/main.cpp States/gameengine.h States/menustate.h Other/Alias.h
	g++ -Wall States/main.cpp -c

Bouton.o : Bouton/Bouton.hpp Bouton/Bouton.cpp Other/Alias.h
	g++ -Wall Bouton/Bouton.cpp -c 

TextGetter.o : Bouton/Bouton.hpp Bouton/TextGetter.hpp Bouton/TextGetter.cpp Other/Alias.h
	g++ -Wall Bouton/TextGetter.cpp -c 

Collider.o : Colliders/Collider.cpp Colliders/Collider.hpp Colliders/PolygoneCollider.hpp Colliders/CircleCollider.hpp Other/vectorFunctions.hpp Other/Alias.h
	g++ -Wall Colliders/Collider.cpp -c 

CircleCollider.o : Colliders/CircleCollider.cpp Colliders/Collider.hpp Colliders/PolygoneCollider.hpp Colliders/CircleCollider.hpp Other/vectorFunctions.hpp Other/Alias.h
	g++ -Wall Colliders/CircleCollider.cpp -c 

PolygoneCollider.o : Colliders/PolygoneCollider.cpp Colliders/Collider.hpp Colliders/PolygoneCollider.hpp Colliders/CircleCollider.hpp Other/vectorFunctions.hpp Other/Alias.h
	g++ -Wall Colliders/PolygoneCollider.cpp -c 

Animation.o : Entity/Animation.h Entity/Animation.cpp Other/vectorFunctions.hpp Other/Alias.h
	g++ -Wall Entity/Animation.cpp -c 

Entity.o : Entity/Entity.h Entity/Entity.cpp Colliders/Colliders.hpp Colliders/Collider.hpp Colliders/PolygoneCollider.hpp Colliders/CircleCollider.hpp Entity/Stairs.h Other/Alias.h
	g++ -Wall Entity/Entity.cpp -c 

Player.o : Entity/Player.h Entity/Player.cpp Entity/Entity.h Other/Alias.h
	g++ -Wall Entity/Player.cpp -c 

TilePalette.o : Tilemap/TilePalette.h Tilemap/TilePalette.cpp Bouton/Bouton.hpp Other/Alias.h
	g++ -Wall Tilemap/TilePalette.cpp -c 

gameengine.o : States/gameengine.h States/gameengine.cpp States/gamestate.h Other/Alias.h
	g++ -Wall States/gameengine.cpp -c 

menustate.o : States/menustate.h States/menustate.cpp States/gamestate.h Bouton/Bouton.hpp Bouton/TextGetter.hpp States/mapeditorstate.h Other/Alias.h
	g++ -Wall States/menustate.cpp -c 

mapeditorstate.o : States/mapeditorstate.h States/mapeditorstate.cpp States/gamestate.h Bouton/Bouton.hpp Tilemap/TilePalette.h Tilemap/Tilemap.hpp Entity/Entity.h Entity/Props.h Entity/PropBush.h Entity/PropTree.h Other/vectorFunctions.hpp States/playstate.h States/escapestate.h Other/Alias.h
	g++ -Wall States/mapeditorstate.cpp -c

playstate.o : States/playstate.h States/playstate.cpp States/gamestate.h Tilemap/Tilemap.hpp Colliders/Colliders.hpp Colliders/Collider.hpp Colliders/PolygoneCollider.hpp Colliders/CircleCollider.hpp Entity/Player.h States/mapeditorstate.h Other/vectorFunctions.hpp Other/Alias.h
	g++ -Wall States/playstate.cpp -c 

escapestate.o : States/escapestate.h States/escapestate.cpp States/gamestate.h Bouton/Bouton.hpp Bouton/TextGetter.hpp States/mapeditorstate.h Other/Alias.h
	g++ -Wall States/escapestate.cpp -c

Tilemap.o : Tilemap/Tilemap.hpp Tilemap/Tilemap.cpp Colliders/Colliders.hpp Colliders/Collider.hpp Colliders/PolygoneCollider.hpp Colliders/CircleCollider.hpp Entity/Player.h Entity/Entity.h Entity/Stairs.h Other/vectorFunctions.hpp Other/Alias.h
	g++ -Wall Tilemap/Tilemap.cpp -c

vectorFunctions.o : Other/vectorFunctions.hpp Other/vectorFunctions.cpp Other/Alias.h
	g++ -Wall Other/vectorFunctions.cpp -c 





