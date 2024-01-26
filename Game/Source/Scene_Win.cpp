#include "Scene_Win.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene_Menu.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"

Scene_Win::Scene_Win() : Module()
{
	name.Create("scene_win");
}

// Destructor
Scene_Win::~Scene_Win()
{

}

// Called before render is available
bool Scene_Win::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene_Win");
	bool ret = true;

	return ret;
}

// Called before the first time
bool Scene_Win::Start()
{
	active = false;
	background = app->tex->Load("Assets/Textures/SceneWin.png");
	return true;
}

bool Scene_Win::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene_Win::Update(float dt)
{
	if (app->entityManager->IsEnabled())
	{
		app->entityManager->Disable();

		start = high_resolution_clock::now();
	}
	currentTime = high_resolution_clock::now();

	elapsed = duration_cast<milliseconds>(currentTime - start);

	if (elapsed >= milliseconds(5000))
	{
		return false;
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		return false;

	return true;
}

// Called each loop iteration
bool Scene_Win::PostUpdate()
{
	bool ret = true;
	app->render->DrawTexture(background, app->render->camera.x*-1, app->render->camera.y * -1, NULL);

	return ret;
}

// Called before quitting
bool Scene_Win::CleanUp()
{
	LOG("Freeing Scene_Win");
	return true;
}