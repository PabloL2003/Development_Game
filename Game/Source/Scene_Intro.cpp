#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene_Menu.h"
#include "Scene_Intro.h"
#include "FadeToBlack.h"
#include "EntityManager.h"
#include "Defs.h"
#include "Log.h"

Scene_Intro::Scene_Intro() : Module()
{
	name.Create("scene_intro");
}

// Destructor
Scene_Intro::~Scene_Intro()
{

}

// Called before render is available
bool Scene_Intro::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene_Intro");
	bool ret = true;


	return ret;
}

// Called before the first frame
bool Scene_Intro::Start()
{
	background = app->tex->Load("Assets/Textures/IntroScreen.png");
	start = high_resolution_clock::now();

	return true;
}

// Called each loop iteration
bool Scene_Intro::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene_Intro::Update(float dt) 
{
	if (app->entityManager->IsEnabled()) app->entityManager->Disable();

	currentTime = high_resolution_clock::now();

	elapsed = duration_cast<milliseconds>(currentTime - start);

	if (elapsed >= milliseconds(3000)) app->ftb->SceneFadeToBlack(this, app->scene_menu, 25.0f);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) return false;

	return true;
}

// Called each loop iteration
bool Scene_Intro::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(background, app->render->camera.x, app->render->camera.y * -1, NULL);

	return ret;
}

// Called before quitting
bool Scene_Intro::CleanUp()
{
	LOG("Freeing Scene_Intro");

	background = nullptr;
	delete background;

	return true;
}