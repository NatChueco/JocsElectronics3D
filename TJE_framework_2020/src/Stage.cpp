#include "Stage.h"
#include "game.h"
#include "utils.h"
#include "fbo.h"
#include <cmath>
#include "animation.h"
#include "input.h"
#include "Gamemap.h"

/*
Entity_ arbol;

void playStage::loadMesh() {
	arbol.mesh = Mesh::Get("data/export.obj");
	arbol.model = Matrix44();
	arbol.model.translate(18, 0, 177);
	arbol.texture = Texture::Get("data/export.png");
}
*/

void tutorialStage::render() {
	Game* game = Game::instance;
	//loadMesh();
	setCamera(game->camera, game->model);

	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	game->model = Matrix44();
	game->model.translate(game->player.pos.x, game->player.pos.y, game->player.pos.z);
	game->model.rotate(game->player.yaw * DEG2RAD, Vector3(0, 1, 0));

	//set the camera as default
	game->camera->enable();

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	RenderMesh(game->shader, game->mainCharacter, game->model, game->camera, game->texCharacter);
	RenderMesh(game->shader, game->catCharacter, game->catmodel, game->camera, game->cattexCharacter);
	RenderMesh(game->shader, game->box_mesh, game->boxModel, game->camera, game->box_text);

	drawGrid();
	SDL_GL_SwapWindow(game->window);
}

void tutorialStage::update(float seconds_elapsed) {

	Game* game = Game::instance;

	float speed = seconds_elapsed * 10; //the speed is defined by the seconds_elapsed so it goes constant

	Matrix44 playerRot;
	playerRot.setRotation(game->player.yaw * DEG2RAD, Vector3(0, 1, 0));
	Vector3 playerFront = playerRot.rotateVector(Vector3(0.0f, 0.0f, -1.0f));
	Vector3 playerRight = playerRot.rotateVector(Vector3(1.0f, 0.0f, 0.0f));
	Vector3 playerSpeed;


	if (Input::isKeyPressed(SDL_SCANCODE_W)) playerSpeed = playerSpeed + (playerFront * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_S)) playerSpeed = playerSpeed + (playerFront * -speed);

	if (Input::isKeyPressed(SDL_SCANCODE_Q)) playerSpeed = playerSpeed + (playerRight * -speed);
	if (Input::isKeyPressed(SDL_SCANCODE_E)) playerSpeed = playerSpeed + (playerRight * speed);

	if (Input::isKeyPressed(SDL_SCANCODE_D)) game->player.yaw += game->player.rot_speed * seconds_elapsed;
	if (Input::isKeyPressed(SDL_SCANCODE_A)) game->player.yaw -= game->player.rot_speed * seconds_elapsed;

	Vector3 targetPos = game->player.pos + playerSpeed;
	game->player.pos = targetPos;
}

void playStage::render() {
	Game* game = Game::instance;
	//loadMesh();
	setCamera(game->camera, game->model);

	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	game->model = Matrix44();
	game->model.translate(game->player.pos.x, game->player.pos.y, game->player.pos.z);
	game->model.rotate(game->player.yaw * DEG2RAD, Vector3(0, 1, 0));

	//set the camera as default
	game->camera->enable();

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	//create model matrix for cube
	Matrix44 m;
	//m.rotate(game->angle * DEG2RAD, Vector3(0, 1, 0));

	RenderMesh(game->shader, game->skybox, game->skymodel, game->camera, game->tex);
	RenderMesh(game->shader, game->ground_mesh, game->groundModel, game->camera, game->ground_text);
	RenderMesh(game->shader, game->mainCharacter, game->model, game->camera, game->texCharacter);
	//RenderMesh(game->shader, game->escenaMesh, game->escenaModel, game->camera,game->escenaText);


	for (size_t i = 0; i < game->static_entities.size(); i++) {
		RenderMesh(game->shader, game->static_entities[i]->mesh, game->static_entities[i]->model, game->camera, game->static_entities[i]->texture);
	}
	//RenderMesh(game->shader, game->static_entities[0]->mesh, game->static_entities[0]->model, game->camera, game->static_entities[0]->texture);
	

	game->mainCharacter->renderBounding(game->model);
	//game->escenaMesh->renderBounding(game->escenaModel);
	drawText(2, 2, std::to_string(game->player.pos.x), Vector3(1, 1, 1), 2);
	drawText(2, 50, std::to_string(game->player.pos.z), Vector3(1, 1, 1), 2);

	drawGrid();
	SDL_GL_SwapWindow(game->window);
}
void playStage::update(float seconds_elapsed) {
	
	Game* game = Game::instance;

	float speed = seconds_elapsed *10; //the speed is defined by the seconds_elapsed so it goes constant


	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT) || game->mouse_locked) //is left button pressed?
	{
		game->camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
		game->camera->rotate(Input::mouse_delta.y * 0.005f, game->camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
	}
	
	if (!game->locked_camera) {
		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) game->camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) game->camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) game->camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) game->camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);

	}
	else {
		
		//example
		Matrix44 playerRot;
		playerRot.setRotation(game->player.yaw * DEG2RAD, Vector3(0, 1, 0));
		Vector3 playerFront = playerRot.rotateVector(Vector3(0.0f, 0.0f, -1.0f));
		Vector3 playerRight = playerRot.rotateVector(Vector3(1.0f, 0.0f, 0.0f));
		Vector3 playerSpeed;


		if (Input::isKeyPressed(SDL_SCANCODE_W)) playerSpeed = playerSpeed + (playerFront * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S)) playerSpeed = playerSpeed + (playerFront * -speed);

		if (Input::isKeyPressed(SDL_SCANCODE_Q)) playerSpeed = playerSpeed + (playerRight * -speed);
		if (Input::isKeyPressed(SDL_SCANCODE_E)) playerSpeed = playerSpeed + (playerRight * speed);

		if (Input::isKeyPressed(SDL_SCANCODE_D)) game->player.yaw += game->player.rot_speed * seconds_elapsed;
		if (Input::isKeyPressed(SDL_SCANCODE_A)) game->player.yaw -= game->player.rot_speed * seconds_elapsed;

		Vector3 targetPos = game->player.pos + playerSpeed;
		if((targetPos.x <= 183.0 && targetPos.x >=3.0 && targetPos.z <=183.0 && targetPos.z >= 3.0)){ game->player.pos = checkCollision(targetPos); }
		
	}

	//async input to move the camera around

	//(checkTreeCollision(targetPos);

	/*
	for (size_t i = 0; i < game->dynamic_entities.size(); i++) {
		checkCollision(dynamic_entities[i],game->static_entities);
		checkCollision(dynamic_entities[i], game->dynamic_entities);
	}*/

	//checkCollision();

	if (Input::isKeyPressed(SDL_SCANCODE_C)) game->locked_camera = !game->locked_camera;
	//to navigate with the mouse fixed in the middle
	if (game->mouse_locked)
		Input::centerMouse();

}

void RenderMesh(Shader* shader,Mesh* mesh, Matrix44 model, Camera* cam, Texture* tex, float tiling) {
	Vector3 worldPos = model * Vector3(0.0f, 0.0f, 0.0f);
	if (shader && cam->testSphereInFrustum(worldPos,mesh->radius))
	{
		//enable shader
		shader->enable();

		//upload uniforms
		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", cam->viewprojection_matrix);
		if(tex != NULL) shader->setUniform("u_texture", tex, 0);
		shader->setUniform("u_model", model);
		shader->setUniform("u_time", time);
		shader->setUniform("u_texture_tiling", tiling);
		//do the draw call
		mesh->render(GL_TRIANGLES);

		//disable shader
		shader->disable();
	}
}

void setCamera(Camera* cam, Matrix44 model) {
	Vector3 eye = model * Vector3(0.0f, 1.0f, 2.0f);
	Vector3 center = model * Vector3(0.0f, 1.0f, -3.0f);
	cam->lookAt(eye, center, Vector3(0.0f, 1.0f, 0.0f));
}


Vector3 checkCollision(Vector3 target) {
	Game* game = Game::instance;

	Vector3 coll;
	Vector3 collnorm;
	Vector3 pushAway;
	Vector3 returned;

	Vector3 centerCharacter = target + Vector3(0.0, 1.0, 0.0);

	int x = target.x / game->tileWidth;
	int y = target.z / game->tileHeight;

	//eCellType type = game->map->getCell(x-6, y-6).type;

	for (size_t i = 0; i < game->static_entities.size(); i++) {

		if (game->static_entities[i]->mesh->testSphereCollision(game->static_entities[i]->model, centerCharacter, 0.5, coll, collnorm)) {
			pushAway = normalize(coll - centerCharacter) * game->elapsed_time;

			returned = game->player.pos - pushAway;
			returned.y = game->player.pos.y;
			return returned;
		}
	}
	return target;
}

/*
void checkCollision(Entity* entity, std::vector<Entity*> entidades) {
	
	Vector3 coll;
	Vector3 collnorm;
	Entity* current;

	Vector3 centerCharacter = entity->model * Vector3(0, 0, 0);
	Vector3 pushAway;

	for (size_t i = 0; i < entidades.size(); i++) {
		current = entidades[i];

		if (!current->mesh->testSphereCollision(current->model, centerCharacter, 0.5, coll, collnorm)) continue;

		pushAway = normalize(coll - centerCharacter) * game->elapsed_time;

		entity->model.translate(pushAway[0], 0, pushAway[2]);

	}

}*/