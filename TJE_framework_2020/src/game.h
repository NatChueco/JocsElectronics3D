/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef GAME_H
#define GAME_H

#include "includes.h"
#include "camera.h"
#include "utils.h"
#include "animation.h"
#include "texture.h"
#include "Stage.h"
#include "Gamemap.h"
//#include "Entity.h"

struct sPlayer {
	Vector3 pos;
	float yaw;
	float rot_speed = 100;
	float radius = 0.5;
};

class Game
{
public:
	static Game* instance;

	//window
	SDL_Window* window;
	int window_width;
	int window_height;

	//some globals
	long frame;
    float time;
	float elapsed_time;
	int fps;
	bool must_exit;

	//some vars
	Camera* camera; //our global camera
	bool mouse_locked; //tells if the mouse is locked (not seen)

	//stuff
	Mesh* mesh = NULL;
	Texture* texture = NULL;
	Shader* shader = NULL;
	Animation* anim = NULL;
	float angle = 0;
	float mouse_speed = 100.0f;
	FBO* fbo = NULL;

	sPlayer player;

	Mesh* mainCharacter = NULL;
	Texture* texCharacter = NULL;
	Matrix44 model;

	Mesh* escenaMesh = NULL;
	Texture* escenaText = NULL;
	Matrix44 escenaModel;

	bool locked_camera = true;
	Stage* current_stage;
	titleStage* title;
	tutorialStage* tutorial;
	playStage* play;
	endStage* end;

	Mesh* treeMesh = NULL;
	Texture* treeText = NULL;
	Matrix44 treeModel;

	GameMap* map;

	//std::vector<Entity*> static_entities;
	//std::vector<Entity*> dynamic_entities;

	const float tileWidth = 6;
	const float tileHeight = 6;
	//Cielo
	Mesh* skybox;
	Matrix44 skymodel;
	Texture* tex;

	//Suelo
	Mesh* ground_mesh;
	Texture* ground_text;
	Matrix44 groundModel;

	Game( int window_width, int window_height, SDL_Window* window );

	//main functions
	void render( void );
	void update( double dt );

	//events
	void onKeyDown( SDL_KeyboardEvent event );
	void onKeyUp(SDL_KeyboardEvent event);
	void onMouseButtonDown( SDL_MouseButtonEvent event );
	void onMouseButtonUp(SDL_MouseButtonEvent event);
	void onMouseWheel(SDL_MouseWheelEvent event);
	void onGamepadButtonDown(SDL_JoyButtonEvent event);
	void onGamepadButtonUp(SDL_JoyButtonEvent event);
	void onResize(int width, int height);

};


#endif 