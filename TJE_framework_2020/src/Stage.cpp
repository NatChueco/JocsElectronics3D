#include "Stage.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"
#include "game.h"

void playStage::render() {
	Game* game = Game::instance;
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	game->camera->enable();

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	//create model matrix for cube
	Matrix44 m;
	m.rotate(game->angle * DEG2RAD, Vector3(0, 1, 0));

	if (game->shader)
	{
		//enable shader
		game->shader->enable();

		//upload uniforms
		game->shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		game->shader->setUniform("u_viewprojection", game->camera->viewprojection_matrix);
		game->shader->setUniform("u_texture", game->texture, 0);
		game->shader->setUniform("u_model", m);
		game->shader->setUniform("u_time", time);

		//do the draw call
		game->mesh->render(GL_TRIANGLES);

		//disable shader
		game->shader->disable();
	}

	//Draw the floor grid
	drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

}