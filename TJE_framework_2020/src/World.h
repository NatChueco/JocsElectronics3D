#pragma once

#include "Entity.h"
#include "utils.h"
#include "Stage.h"

class World {
public:
	
	Stage* current_stage;

	titleStage* title;
	tutorialStage* tutorial;
	playStage* play;
	endStage* end;

	World();
	std::vector<Entity*> entities;
	void setInit();
	void setConfiguration();
	void saveGame();


};