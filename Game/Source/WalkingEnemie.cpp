#include "WalkingEnemie.h"
#include "DynArray.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Debug.h"
#include "Map.h"
#include "pathfinding.h"

Wenem::Wenem() : Entity(EntityType::WALKENEM)
{
	name.Create("Wenem");
}

Wenem::~Wenem() {

}


void Wenem::InitAnims() {

	//R.Idle
	for (pugi::xml_node node = parameters.child("wr_idle").child("pushback");
		node; node = node.next_sibling("pushback")) {
		wr_idle.PushBack({ node.attribute("x").as_int(),
							node.attribute("y").as_int(),
							node.attribute("width").as_int(),
							node.attribute("height").as_int() });
	}

	//L.Idle
	for (pugi::xml_node node = parameters.child("wl_idle").child("pushback");
		node; node = node.next_sibling("pushback")) {
		wl_idle.PushBack({ node.attribute("x").as_int(),
							node.attribute("y").as_int(),
							node.attribute("width").as_int(),
							node.attribute("height").as_int() });
	}
	//R.Run

	for (pugi::xml_node node = parameters.child("wrigthmov").child("pushback");
		node; node = node.next_sibling("pushback"))
	{
		wrigthmov.PushBack({ node.attribute("x").as_int(),
							node.attribute("y").as_int(),
							node.attribute("width").as_int(),
							node.attribute("height").as_int() });
	}
	wrigthmov.speed = parameters.child("wrigthmov").attribute("animspeed").as_float();
	wrigthmov.loop = parameters.child("wrigthmov").attribute("loop").as_bool();


	//L.Run
	for (pugi::xml_node node = parameters.child("wleftmov").child("pushback");
		node; node = node.next_sibling("pushback"))
	{
		wleftmov.PushBack({ node.attribute("x").as_int(),
						   node.attribute("y").as_int(),
						   node.attribute("width").as_int(),
						   node.attribute("height").as_int() });
	}
	wleftmov.speed = parameters.child("wleftmov").attribute("animspeed").as_float();
	wleftmov.loop = parameters.child("wleftmov").attribute("loop").as_bool();

	currentAnim = &wleftmov;
}


bool Wenem::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	InitAnims();

	return true;
}

bool Wenem::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateCircle(position.x + 17, position.y, 10, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMIE;

	return true;
}

void Wenem::TeleportTo(iPoint position)
{

}


bool Wenem::Update(float dt)
{
	currentAnim->Update();
	app->render->DrawTexture(texture, position.x + 5, position.y - 8, &(currentAnim->GetCurrentFrame()));

	pbody->body->ApplyForce(b2Vec2(0, GRAVITY_Y * dt), pbody->body->GetWorldCenter(), true);

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	//pathfinding

	iPoint entityTile = app->map->WorldToMap(METERS_TO_PIXELS(this->pbody->body->GetPosition().x), METERS_TO_PIXELS(this->pbody->body->GetPosition().y));
	iPoint playerTile = app->map->WorldToMap(METERS_TO_PIXELS(app->scene->player->pbody->body->GetPosition().x), METERS_TO_PIXELS(app->scene->player->pbody->body->GetPosition().y));

	app->pathfinding->CreatePath(entityTile, playerTile);
	pathToPlayer.Clear();


	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); i++)
	{
		pathToPlayer.PushBack(iPoint(path->At(i)->x, path->At(i)->y));
	}

	//codigo de debug

	if (pathToPlayer.Count() > 1 && app->scene->player->position.y > 80)
	{
		int directionX = pathToPlayer.At(1)->x - pathToPlayer.At(0)->x;

		if (directionX > 0)
		{
			if (pbody->body->GetLinearVelocity().x < speedCap)
			{
				if (currentAnim != &wrigthmov) currentAnim = &wrigthmov;
				pbody->body->ApplyForce(b2Vec2(2.0f, 0.0f), pbody->body->GetWorldCenter(), true);
			}
		}
		else if (directionX < 0)
		{
			if (pbody->body->GetLinearVelocity().x > - speedCap)
			{
				if (currentAnim != &wleftmov) currentAnim = &wleftmov;
				pbody->body->ApplyForce(b2Vec2(-2.0f, 0.0f), pbody->body->GetWorldCenter(), true);
			}
		}
	}

	
	return true;
}

bool Wenem::CleanUp()
{
	delete pbody;
	texturePath = nullptr;
	currentAnim = nullptr;

	return true;
}

void Wenem::OnCollision(PhysBody* physA, PhysBody* physB)
{
	if (physB->ctype == ColliderType::PLAYER)
	{
		LOG("Collision ENEMY");

		if (!app->debug->godMode) 
		{
			if (physB->body->GetLinearVelocity().y > 0)
			{
				isDead = true;
			}

			else 
			{
				app->scene->player->isKilled = true;
			}

		}
	}
}