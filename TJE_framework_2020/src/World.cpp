#include "World.h"
#include "game.h"
#include "Stage.h"

World::World() {
	Game* game = Game::instance;

	
	title = new titleStage();
	tutorial = new tutorialStage();
	play = new playStage();
	end = new endStage();
	

}
