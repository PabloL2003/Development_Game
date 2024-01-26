#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Scene_Menu.h"
#include "Scene_Win.h"
#include "EntityManager.h"
#include "Map.h"
#include "Item.h"
#include "Player.h"
#include "Physics.h"
#include "WalkingEnemie.h"
#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"
#include "GUIControl.h"
#include "GUIManager.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");
	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}
	// declare awake of enemies

	enemie = (Flyenem*)app->entityManager->CreateEntity(EntityType::FLYENEM);
	enemie->parameters = config.child("flyenem");

	enemie2 = (Flyenem*)app->entityManager->CreateEntity(EntityType::FLYENEM);
	enemie2->parameters = config.child("flyenem2");

	enemie3 = (Wenem*)app->entityManager->CreateEntity(EntityType::WALKENEM);
	enemie3->parameters = config.child("wenem");

	enemie4 = (Wenem*)app->entityManager->CreateEntity(EntityType::WALKENEM);
	enemie4->parameters = config.child("wenem2");

	enemie5 = (BOSS*)app->entityManager->CreateEntity(EntityType::BOSS);
	enemie5->parameters = config.child("BOSS");


	coin7 = (COIN*)app->entityManager->CreateEntity(EntityType::COIN);
	coin7->parameters = config.child("coin7");

	coin8 = (COIN*)app->entityManager->CreateEntity(EntityType::COIN);
	coin8->parameters = config.child("coin8");

	coin9 = (COIN*)app->entityManager->CreateEntity(EntityType::COIN);
	coin9->parameters = config.child("coin9");

	coin10 = (COIN*)app->entityManager->CreateEntity(EntityType::COIN);
	coin10->parameters = config.child("coin10");

	coin11 = (COIN*)app->entityManager->CreateEntity(EntityType::COIN);
	coin11->parameters = config.child("coin11");

	this->active = false;

	if (config.child("map")) {
		//Get the map name from the config file and assigns the value in the module
		app->map->name = config.child("map").attribute("name").as_string();
		app->map->path = config.child("map").attribute("path").as_string();
	}

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	img = app->tex->Load("Assets/Textures/espada.png");
	
	//Music is commented so that you can add your own music
	app->audio->PlayMusic("Assets/Audio/Music/game-music.wav");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	pauseMenuTexture = app->tex->Load("Assets/Textures/PauseMenu.png");
	settingsBackground = app->tex->Load("Assets/Textures/PauseSettings.png");

	//Get the size of the texture
	app->tex->GetSize(img, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	pause = exit = gameplaySettings = false;

	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	SDL_Rect btPos = { windowW / 2 - 60, windowH / 2 - 10, 120, 20 };

	uint w, h;
	app->win->GetWindowSize(w, h);
	
	// Buttons
	// -- Gameplay screen
	pauseBtn = (GUIControlButton*)app->guiManager->CreateGuiControl(GUIControlType::BUTTON, 1, "PAUSE", { (int)w - 100, (int)20,50,20 }, this);
	// -- Pause screen
	resumeBtn = (GUIControlButton*)app->guiManager->CreateGuiControl(GUIControlType::BUTTON, 2, "RESUME", { (int)w / 2 - 50, (int)h / 2 - 100, 100, 20 }, this);
	settingsBtn = (GUIControlButton*)app->guiManager->CreateGuiControl(GUIControlType::BUTTON, 3, "SETTINGS", { (int)w / 2 - 50, (int)h / 2 - 60,100,20 }, this);
	backToTitleBtn = (GUIControlButton*)app->guiManager->CreateGuiControl(GUIControlType::BUTTON, 4, "MAIN MENU", { (int)w / 2 - 50,(int)h / 2 - 20, 100, 20 }, this);
	exitBtn = (GUIControlButton*)app->guiManager->CreateGuiControl(GUIControlType::BUTTON, 5, "EXIT", { (int)w / 2 - 50,(int)h / 2 + 20,100,20 }, this);
	// -- Settings screen
	gameReturnBtn = (GUIControlButton*)app->guiManager->CreateGuiControl(GUIControlType::BUTTON, 6, "X", { (int)w - 55, 5, 50, 50 }, this);

	// Sliders
	// -- Settings screen
	bgmGameSlider = (GUISlider*)app->guiManager->CreateGuiControl(GUIControlType::SLIDER, 7, "BGM slider", { 500, 275, 35, 35 }, this);
	sfxGameSlider = (GUISlider*)app->guiManager->CreateGuiControl(GUIControlType::SLIDER, 8, "SFX slider", { 500, 385, 35, 35 }, this);

	// Checkboxes
	// -- Settings screen
	fullscreenGameCbox = (GUICheckbox*)app->guiManager->CreateGuiControl(GUIControlType::CHECKBOX, 9, "Fullscreen cbox", { 650, 520, 50, 50 }, this);
	vsyncGameCbox = (GUICheckbox*)app->guiManager->CreateGuiControl(GUIControlType::CHECKBOX, 10, "VSync cbox", { 530, 600, 50, 50 }, this);

	// Initial GUI states
	resumeBtn->state = GUIControlState::DISABLED;
	settingsBtn->state = GUIControlState::DISABLED;
	backToTitleBtn->state = GUIControlState::DISABLED;
	exitBtn->state = GUIControlState::DISABLED;
	pauseBtn->state = GUIControlState::DISABLED;
	gameReturnBtn->state = GUIControlState::DISABLED;
	bgmGameSlider->state = GUIControlState::DISABLED;
	sfxGameSlider->state = GUIControlState::DISABLED;
	fullscreenGameCbox->state = GUIControlState::DISABLED;
	vsyncGameCbox->state = GUIControlState::DISABLED;

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

void Scene::PressPause()
{
	resumeBtn->state = GUIControlState::NORMAL;
	settingsBtn->state = GUIControlState::NORMAL;
	backToTitleBtn->state = GUIControlState::NORMAL;
	exitBtn->state = GUIControlState::NORMAL;
	app->entityManager->Disable();
	pause = true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	Enable();

	/*app->map->Load();*/
	if (!app->entityManager->IsEnabled()) app->entityManager->Enable();

	if (app->scene_menu->IsEnabled())
	{
		app->scene_menu->Disable();
	}

	if (IsEnabled() && !app->scene_menu->IsEnabled())
	{
		if (!gameplaySettings)
		{
			if (pauseBtn->state == GUIControlState::DISABLED) pauseBtn->state = GUIControlState::NORMAL;
		}
	}
	else
	{
		if (pauseBtn->state == GUIControlState::NORMAL) pauseBtn->state = GUIControlState::DISABLED;
		if (resumeBtn->state == GUIControlState::NORMAL) resumeBtn->state = GUIControlState::DISABLED;
		if (backToTitleBtn->state == GUIControlState::NORMAL) backToTitleBtn->state = GUIControlState::DISABLED;
		if (settingsBtn->state == GUIControlState::NORMAL) settingsBtn->state = GUIControlState::DISABLED;
		if (exitBtn->state == GUIControlState::NORMAL) exitBtn->state = GUIControlState::DISABLED;
	}


	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	//Checks to ensure the positions of the enemies reset
	if (enemie->killedPlayer == true)
	{
		enemie3->killedPlayer = true;
		enemie4->killedPlayer = true;
		enemie5->killedPlayer = true;
	}

	if (enemie2->killedPlayer == true)
	{
		enemie3->killedPlayer = true;
		enemie4->killedPlayer = true;
		enemie5->killedPlayer = true;
	}

	if (enemie3->killedPlayer == true)
	{
		enemie->killedPlayer = true;
		enemie2->killedPlayer = true;
		enemie5->killedPlayer = true;
	}

	if (enemie4->killedPlayer == true)
	{
		enemie->killedPlayer = true;
		enemie2->killedPlayer = true;
		enemie5->killedPlayer = true;
	}

	if (enemie5->killedPlayer == true)
	{
		enemie->killedPlayer = true;
		enemie2->killedPlayer = true;
		enemie3->killedPlayer = true;
		enemie4->killedPlayer = true;
	}

	if (pause)
	{
		player->pbody->body->SetActive(false);
		if (app->physics->IsEnabled())
		{
			app->physics->Disable();
		}

		uint w, h;
		app->win->GetWindowSize(w, h);

		app->render->DrawTexture(pauseMenuTexture, app->render->camera.x*-1 + windowW/6, app->render->camera.y * -1 + windowH/4 , NULL);
	}
	else
	{
		player->pbody->body->SetActive(true);
		if (!app->physics->IsEnabled())
		{
			app->physics->Enable();
		}
	}

	if (gameplaySettings)
	{
		app->render->DrawTexture(settingsBackground, app->render->camera.x*-1, app->render->camera.y * -1, NULL);
		LOG("Printing settings menu");
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		PressPause();
	}

	if (enemie5->BOSSHP == 0)
	{
		app->ftb->SceneFadeToBlack(app->scene, app->scene_win, 0.0f);
	}


	if (exit) return false;

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	/*if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;*/

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	player = nullptr;
	enemie = nullptr;
	enemie2 = nullptr;
	enemie3 = nullptr;
	enemie4 = nullptr;
	enemie5 = nullptr;
	return true;
}

bool Scene::SaveState(pugi::xml_node node)
{
	//Introducing the positions of the entities into the save_game xml
	pugi::xml_node playernode = node.append_child("player");

	playernode.append_attribute("x") = player->position.x;
	playernode.append_attribute("y") = player->position.y;

	pugi::xml_node enemynode1 = node.append_child("flyenem1");

	enemynode1.append_attribute("x") = enemie->position.x;
	enemynode1.append_attribute("y") = enemie->position.y;

	pugi::xml_node enemynode2 = node.append_child("flyenem2");

	enemynode2.append_attribute("x") = enemie2->position.x;
	enemynode2.append_attribute("y") = enemie2->position.y;

	pugi::xml_node enemynode3 = node.append_child("wenem1");

	enemynode3.append_attribute("x") = enemie3->position.x;
	enemynode3.append_attribute("y") = enemie3->position.y;

	pugi::xml_node enemynode4 = node.append_child("wenem2");

	enemynode4.append_attribute("x") = enemie4->position.x;
	enemynode4.append_attribute("y") = enemie4->position.y;
	
	pugi::xml_node enemynode5 = node.append_child("BOSS");

	enemynode5.append_attribute("x") = enemie5->position.x;
	enemynode5.append_attribute("y") = enemie5->position.y;
	enemynode5.append_attribute("hp") = enemie5->BOSSHP;

	pugi::xml_node coin7 = node.append_child("coin7");

	coin7.append_attribute("x") = enemie5->position.x;
	coin7.append_attribute("y") = enemie5->position.y;

	pugi::xml_node coin8 = node.append_child("coin8");

	coin8.append_attribute("x") = enemie5->position.x;
	coin8.append_attribute("y") = enemie5->position.y;

	pugi::xml_node coin9 = node.append_child("coin9");

	coin9.append_attribute("x") = enemie5->position.x;
	coin9.append_attribute("y") = enemie5->position.y;

	pugi::xml_node coin10 = node.append_child("coin10");

	coin10.append_attribute("x") = enemie5->position.x;
	coin10.append_attribute("y") = enemie5->position.y;

	pugi::xml_node coin11 = node.append_child("coin11");

	coin11.append_attribute("x") = enemie5->position.x;
	coin11.append_attribute("y") = enemie5->position.y;

	return true;
}

bool Scene::LoadState(pugi::xml_node node)
{
	//Player Load/Save state
	player->position.x = node.child("player").attribute("x").as_int();
	player->position.y = node.child("player").attribute("y").as_int();

	player->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(player->position.x), PIXEL_TO_METERS(player->position.y)), 0);
	player->pbody->body->SetLinearVelocity(b2Vec2(0, 0));

	//Enemies Load/Save state
	enemie->position.x = node.child("flyenem1").attribute("x").as_int();
	enemie->position.y = node.child("flyenem1").attribute("y").as_int();

	enemie->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(enemie->position.x), PIXEL_TO_METERS(enemie->position.y)), 0);
	enemie->pbody->body->SetLinearVelocity(b2Vec2(0, 0));

	enemie2->position.x = node.child("flyenem2").attribute("x").as_int();
	enemie2->position.y = node.child("flyenem2").attribute("y").as_int();

	enemie2->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(enemie2->position.x), PIXEL_TO_METERS(enemie2->position.y)), 0);
	enemie2->pbody->body->SetLinearVelocity(b2Vec2(0, 0));

	enemie3->position.x = node.child("wenem1").attribute("x").as_int();
	enemie3->position.y = node.child("wenem1").attribute("y").as_int();

	enemie3->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(enemie3->position.x), PIXEL_TO_METERS(enemie3->position.y)), 0);
	enemie3->pbody->body->SetLinearVelocity(b2Vec2(0, 0));

	enemie4->position.x = node.child("wenem2").attribute("x").as_int();
	enemie4->position.y = node.child("wenem2").attribute("y").as_int();

	enemie4->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(enemie4->position.x), PIXEL_TO_METERS(enemie4->position.y)), 0);
	enemie4->pbody->body->SetLinearVelocity(b2Vec2(0, 0));

	enemie5->position.x = node.child("BOSS").attribute("x").as_int();
	enemie5->position.y = node.child("BOSS").attribute("y").as_int();
	enemie5->BOSSHP = node.child("BOSS").attribute("hp").as_int();

	enemie5->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(enemie5->position.x), PIXEL_TO_METERS(enemie5->position.y)), 0);
	enemie5->pbody->body->SetLinearVelocity(b2Vec2(0, 0));

	// coins load safe //

	coin7->position.x = node.child("coin7").attribute("x").as_int();
	coin7->position.y = node.child("coin7").attribute("y").as_int();

	coin8->position.x = node.child("coin8").attribute("x").as_int();
	coin8->position.y = node.child("coin8").attribute("y").as_int();

	coin9->position.x = node.child("coin9").attribute("x").as_int();
	coin9->position.y = node.child("coin9").attribute("y").as_int();

	coin10->position.x = node.child("coin10").attribute("x").as_int();
	coin10->position.y = node.child("coin10").attribute("y").as_int();

	coin11->position.x = node.child("coin11").attribute("x").as_int();
	coin11->position.y = node.child("coin11").attribute("y").as_int();


	return true;
	
}

iPoint Scene::GetPLayerPosition() {
	return player->position;
}

iPoint Scene::GetenemiePosition() {
	return enemie->position;
}

iPoint Scene::Getenemie2Position() {
	return enemie2->position;
}

iPoint Scene::Getenemie3Position() {
	return enemie3->position;
}

iPoint Scene::Getenemie4Position() {
	return enemie4->position;
}

bool Scene::OnGUIMouseClickEvent(GUIControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Event by %d", control->id);
	int flags = SDL_GetWindowFlags(app->win->window);
	switch (control->id)
	{
	case 1: // Pause btn
		LOG("Pause button click. PAUSE ENABLED");
		PressPause();

		break;

	case 2: // Resume btn
		LOG("Resume button click. PAUSE DISABLED");
		resumeBtn->state = GUIControlState::DISABLED;
		settingsBtn->state = GUIControlState::DISABLED;
		backToTitleBtn->state = GUIControlState::DISABLED;
		exitBtn->state = GUIControlState::DISABLED;
		app->entityManager->Enable();

		pause = false;
		break;

	case 3: // Settings btn
		LOG("Settings button click");
		bgmGameSlider->SetValue(app->audio->GetBGMVolume());
		sfxGameSlider->SetValue(app->audio->GetSFXVolume());
		gameplaySettings = true;
		pauseBtn->state = GUIControlState::DISABLED;
		resumeBtn->state = GUIControlState::DISABLED;
		settingsBtn->state = GUIControlState::DISABLED;
		backToTitleBtn->state = GUIControlState::DISABLED;
		exitBtn->state = GUIControlState::DISABLED;

		gameReturnBtn->state = GUIControlState::NORMAL;
		bgmGameSlider->state = GUIControlState::NORMAL;
		sfxGameSlider->state = GUIControlState::NORMAL;

		(flags & SDL_WINDOW_FULLSCREEN) ? fullscreenGameCbox->state = GUIControlState::SELECTED
										: fullscreenGameCbox->state = GUIControlState::NORMAL;

		(app->vsync) ? vsyncGameCbox->state = GUIControlState::SELECTED
											: vsyncGameCbox->state = GUIControlState::NORMAL;
		
		break;
	case 4: // Main menu btn
		LOG("Main menu button click");
		pause = false;
		player->isKilled = true;
		app->ftb->SceneFadeToBlack(this, app->scene_menu, 0);
		pauseBtn->state = GUIControlState::DISABLED;
		resumeBtn->state = GUIControlState::DISABLED;
		settingsBtn->state = GUIControlState::DISABLED;
		backToTitleBtn->state = GUIControlState::DISABLED;
		exitBtn->state = GUIControlState::DISABLED;

		break;
	case 5: // Exit btn
		LOG("Exit button click");
		exit = true;
		break;

	case 6: // Return
		pauseBtn->state = GUIControlState::NORMAL;
		resumeBtn->state = GUIControlState::NORMAL;
		settingsBtn->state = GUIControlState::NORMAL;
		backToTitleBtn->state = GUIControlState::NORMAL;
		exitBtn->state = GUIControlState::NORMAL;

		gameReturnBtn->state = GUIControlState::DISABLED;
		bgmGameSlider->state = GUIControlState::DISABLED;
		sfxGameSlider->state = GUIControlState::DISABLED;
		vsyncGameCbox->state = GUIControlState::DISABLED;
		fullscreenGameCbox->state = GUIControlState::DISABLED;

		gameplaySettings = false;
		break;

	case 7: //BGM slider
		LOG("Music slider");
		app->audio->SetBGMVolume(bgmGameSlider->value);
		break;
		
	case 8: // SFX slider
		LOG("SFX slider");
		app->audio->SetSFXVolume(sfxGameSlider->value);
		break;

	case 9: // Fullscreen cbox
		LOG("Fullscreen slider");
		(fullscreenGameCbox->state == GUIControlState::SELECTED) ? SDL_SetWindowFullscreen(app->win->window, 0)
																: SDL_SetWindowFullscreen(app->win->window, 1);
		break;

	case 10: // VSync cbox
		app->vsync = !app->vsync;
		break;
	}
	
	return true;
}
