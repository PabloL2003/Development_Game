#include "EntityManager.h"
#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "Item.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* entity = nullptr; 

	switch (type)
	{
	case EntityType::PLAYER:
		entity = new Player();
		break;
	case EntityType::ITEM:
		entity = new Item();
		break;
	default:
		break;
	}

	entities.Add(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.Del(item);
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->pendingToDestroy) {

			for (ListItem<PhysBody*>* corpse = pEntity->myBodies.start; corpse != NULL; corpse = corpse->next) {

				app->physics->DestroyObject((PhysBody*)corpse->data);
			}
			pEntity->pendingToDestroy = false;
			DestroyEntity(pEntity);
		}
		if (pEntity->active == false) continue;
		ret = item->data->Update(dt);
	}

	return ret;
}

bool EntityManager::LoadState(pugi::xml_node node) {
	
	ListItem<Entity*>* item;

	bool ret = true;

	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;
		if (pEntity->type == EntityType::ITEM)
		{
			for (ListItem<PhysBody*>* corpse = pEntity->myBodies.start; corpse != NULL; corpse = corpse->next) {
				app->physics->DestroyObject((PhysBody*)corpse->data);
			}
			pEntity->pendingToDestroy = false;
			DestroyEntity(pEntity);
		}

	}

	pEntity = NULL;

	for (item = savedEntities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->type == EntityType::ITEM)
		{
			for (pugi::xml_node itemNode = node.child(pEntity->name.GetString()); itemNode; itemNode = itemNode.next_sibling("item"))
			{
				pEntity = app->entityManager->CreateEntity(EntityType::ITEM);
				pEntity->position.x = itemNode.child("position").attribute("x").as_int();
				pEntity->position.y = itemNode.child("position").attribute("y").as_int();
			}
		}
	}

	return true;
}

bool EntityManager::SaveState(pugi::xml_node node) {

	savedEntities = entities;

	ListItem<Entity*>* item;

	bool ret = true;

	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;
		if (pEntity->type == EntityType::ITEM)
		{
			pugi::xml_node itemNode = node.append_child(pEntity->name.GetString());
			itemNode = itemNode.append_child("position");
			itemNode.append_attribute("x").set_value(pEntity->position.x);
			itemNode.append_attribute("y").set_value(pEntity->position.y);
		}
	}

	return true;
}

void EntityManager::ReSpawn()
{
	
}
