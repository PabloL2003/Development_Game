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

#include "Defs.h"
#include "Log.h"

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
	/*bool retLoad = app->map->Load("Mapa_Dungen_dev.tmx");*/
	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	//img = app->tex->Load("Assets/Textures/test.png");
	
	//Music is commented so that you can add your own music
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

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

	/*if (retLoad)
	{
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateNavigationMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetNavigationMap(w, h, data);

		RELEASE_ARRAY(data);
	}*/

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


	//Probablemente lo meter� en debug
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

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

	return true;
}

bool Scene::SaveState(pugi::xml_node node)
{
	pugi::xml_node playernode = node.append_child("player");

	playernode.append_attribute("x") = player->position.x;
	playernode.append_attribute("y") = player->position.y;
	
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