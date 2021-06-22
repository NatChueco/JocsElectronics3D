#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"
#include "Stage.h"
#include <cmath>

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

//some globals

Game* Game::instance = NULL;

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = false;

	char* nombre;
	nombre = "data/prueba.txt";
	readFile(nombre);
	

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	//create our camera
	camera = new Camera();
	//camera->lookAt(Vector3(0.f,100.f, 100.f),Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective

	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	escenaText = new Texture();
	escenaText->load("data/export.png");
	escenaMesh = Mesh::Get("data/export.obj");

	box_mesh = Mesh::Get("data/box1.obj");
	box_text = Texture::Get("data/white.png");


	mainCharacter = Mesh::Get("data/pirata_chica1.obj");
	texCharacter = Texture::Get("data/PolygonMinis_Texture_01_A.png");

	Animation* anim = Animation::Get("data/animations/animationchica.skanim");
	anim->assignTime(time);
	anim->skeleton.renderSkeleton(camera, model);

	catCharacter = Mesh::Get("data/Cheshirev2.obj");
	cattexCharacter = Texture::Get("data/CheshireCat_DMA.png");


	Vector3 eye = model * Vector3(0.0f, 3.0f, 5.0f);
	Vector3 center = model * Vector3(0.0f, 1.0f, -3.0f);
	camera->lookAt(eye, center, Vector3(0.0f, 1.0f, 0.0f));

	map = new GameMap();
	map = map->loadGameMap("data/mymap.map");
	map->setViewData();
	loadmap(map);

	// example of shader loading using the shaders manager
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	ground_mesh = new Mesh();
	ground_mesh->createPlane(200);
	ground_text = Texture::Get("data/grass.tga");

	//Sky stuff
	tex = new Texture();
	skybox = Mesh::Get("data/cielo.ASE");
	tex->load("data/cielo.tga");
	// example of shader loading using the shaders manager
	
	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse

	//Mandar captura o todo el codigo con el error****
	//World* mundo = new World();

	title = new titleStage();
	tutorial = new tutorialStage();
	play = new playStage();
	end = new endStage();

	current_stage = play;

	player.pos = Vector3(18, 0, 177);

	//player.pos = Vector3(0, 0, 0);
}

//what to do when the image has to be draw
void Game::render(void)
{
	current_stage->render();
}

void Game::update(double seconds_elapsed)
{
	current_stage->update(seconds_elapsed);
	/*
	float speed = seconds_elapsed * mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant

	//example
	angle += (float)seconds_elapsed * 10.0f;

	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked ) //is left button pressed?
	{
		camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
		camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
	}

	//async input to move the camera around
	if(Input::isKeyPressed(SDL_SCANCODE_LSHIFT) ) speed *= 10; //move faster with left shift
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f,-1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f,0.0f, 0.0f) * speed);

	//to navigate with the mouse fixed in the middle
	if (mouse_locked)
		Input::centerMouse();
	*/
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
		case SDLK_F1: Shader::ReloadAll(); break; 
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
	mouse_speed *= event.y > 0 ? 1.1 : 0.9;
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

void Game::readFile(char* name) {
	string linea;
	
	string nombre = name;

	ifstream fichero(nombre.c_str());
	if (fichero.fail())
	{
		std::cout << "No existe el fichero!" << std::endl;
		exit(1);
	}

	while (getline(fichero, linea))
	{
		std::string stats[15];
		
		int i = 0;

		string del = " ";
		int start = 0;
		int end = linea.find(del);
		while (end != -1) {
			stats[i] = linea.substr(start, end - start);
			i++;

			start = end + del.size();
			end = linea.find(del, start);
		}

		cout << stats[0];

		Entity_* entidad = new Entity_;

		entidad->model.setTranslation(stoi(stats[0]), stoi(stats[1]), stoi(stats[2]));

		entidad->mesh = Mesh::Get(stats[3].c_str());

		entidad->texture = Texture::Get(stats[4].c_str());

		static_entities.push_back(entidad);
	}

	fichero.close();
}

void Game::loadmap(GameMap* map) {
	Game* game = Game::instance;
	for (size_t i = 0; i < map->width; i++) {

		for (size_t j = 0; j < map->height; j++) {

			sCell& cell = map->getCell(i, j);
			int index = (int)cell.type;
			if (index != 0) continue;
			sPropViewData& prop = map->viewData[index];

			Matrix44 model;
			model.translate(i * game->tileWidth, 0.0f, j * game->tileHeight);
			Entity_* entidad = new Entity_;
			entidad->mesh = prop.mesh;
			entidad->texture = prop.texture;
			entidad->model = model;
			game->static_entities.push_back(entidad);

			//RenderMesh(game->shader, prop.mesh, model, game->camera, prop.texture);

		}
	}
}

