#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Item.h"
#include "Player.h"
#include "Physics.h"
#include "WalkingEnemie.h"
#include "BOSS.h"
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

	//BOSS = (BOSS*)app->entityManager->CreateEntity(EntityType::BOSS);
	//BOSS->parameters = config.child("BOSS");

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
	//img = app->tex->Load("Assets/Textures/test.png");
	
	//Music is commented so that you can add your own music
	app->audio->PlayMusic("Assets/Audio/Music/game-music.wav");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(img, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	SDL_Rect btPos = { windowW / 2 - 60, windowH / 2 - 10, 120, 20 };
	gcButton = (GUIControlButton*)app->guiManager->CreateGuiControl(GUIControlType::BUTTON, 1, "MyButton", btPos, this);

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	// Renders the image in the center of the screen 
	//app->render->DrawTexture(img, (int)textPosX, (int)textPosY);
	/*app->map->Load();*/

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	//Checks to ensure the positions of the enemies reset
	if (enemie->killedPlayer == true)
	{
		enemie3->killedPlayer = true;
		enemie4->killedPlayer = true;
	}

	if (enemie2->killedPlayer == true)
	{
		enemie3->killedPlayer = true;
		enemie4->killedPlayer = true;
	}

	if (enemie3->killedPlayer == true)
	{
		enemie->killedPlayer = true;
		enemie2->killedPlayer = true;
	}

	if (enemie4->killedPlayer == true)
	{
		enemie->killedPlayer = true;
		enemie2->killedPlayer = true;
	}
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

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
	LOG("Press Gui Control: %d", control->id);

	return true;
}
