#include "gameengine.h"
#include "menustate.h"
#include "../Other/Alias.h"

int main ( int argc, char *argv[] )
{
	CGameEngine game;

	// initialize the engine
	game.Init( "TileMmap EdiTorquet 3000" );

	// load the intro
	game.ChangeState( CMenuState::Instance() );

	// main loop
	while ( game.Running() )
	{
		game.HandleEvents();
		game.Update();
		game.Draw();
	}

	// cleanup the engine
	game.Cleanup();

	return 0;
}
