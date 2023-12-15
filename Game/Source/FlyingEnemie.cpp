#include "FlyingEnemie.h"
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

Flyenem::Flyenem() : Entity(EntityType::FLYENEM)
{
	name.Create("Flyenem");
}

Flyenem::~Flyenem() {

}


void Flyenem::InitAnims() {

	//R.mov
	for (pugi::xml_node node = parameters.child("flyrigthmov").child("pushback");
		node; node = node.next_sibling("pushback"))
	{
		flyrigthmov.PushBack({ node.attribute("x").as_int(),
							node.attribute("y").as_int(),
							node.attribute("width").as_int(),
							node.attribute("height").as_int() });
	}
	flyrigthmov.speed = parameters.child("flyrigthmov").attribute("animspeed").as_float();
	flyrigthmov.loop = parameters.child("flyrigthmov").attribute("loop").as_bool();

	for (pugi::xml_node node = parameters.child("flyleftmov").child("pushback");
		node; node = node.next_sibling("pushback"))
	{
		flyleftmov.PushBack({ node.attribute("x").as_int(),
							node.attribute("y").as_int(),
							node.attribute("width").as_int(),
							node.attribute("height").as_int() });
	}
	flyleftmov.speed = parameters.child("flyleftmov").attribute("animspeed").as_float();
	flyleftmov.loop = parameters.child("flyleftmov").attribute("loop").as_bool();

	currentAnim = &flyleftmov;
	
}

bool Flyenem::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	InitAnims();

	return true;
}


bool Flyenem::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateCircle(position.x, position.y, 10, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMIE;

	mouseTileTex = app->tex->Load("Assets/Textures/tiletext.png");

	return true;
}


bool Flyenem::Update(float dt)
{
	IsDead();

	if (isKilled)
		return true;

	if (true) {
		iPoint origin = app->map->WorldToMap(app->scene->enemie->position.x, app->scene->enemie->position.y);
		iPoint destiny = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);
		/*app->map->pathfinding->CreatePath(iPoint(6,32), iPoint(7,32));*/

		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
			const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
			for (uint i = 0; i < path->Count(); i++) {
				iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
			}
		}
	}
	currentAnim->Update();
	app->render->DrawTexture(texture, position.x - 10, position.y - 40, &(currentAnim->GetCurrentFrame()));

	if (pendingToDelete)
	{
		isKilled = true;
		CleanUp();
	}

	return true;
}

void Flyenem::TeleportTo(iPoint pos)
{
	// Detén la velocidad actual del enemigo
	pbody->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));

	// Establece la posición del cuerpo físico del enemigo en el punto de destino
	pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(pos.x), PIXEL_TO_METERS(pos.y)), 0.0f);

	// Actualiza la posición del enemigo
	position.x = pos.x;
	position.y = pos.y;

}

void Flyenem::SetSpawnPoint(iPoint pos)
{
	spawn = pos;
}

void Flyenem::IsDead()
{
	if (app->scene->player->isKilled)
	{
		TeleportTo(spawn);
	}
}

bool Flyenem::CleanUp()
{
	app->tex->UnLoad(texture);
	texture = nullptr;

	if (pbody)
	{
		app->physics->DestroyBody(pbody);
		pbody = nullptr;
	}

	pendingToDelete = false;
	active = false;

	return true;
}

void Flyenem::OnCollision(PhysBody* physA, PhysBody* physB)
{
	if (physB->ctype == ColliderType::PLAYER)
	{
		LOG("Collision ENEMY");

		if (!app->debug->godMode)
		{
			if (physB->body->GetLinearVelocity().y >= 0.5)
			{
				LOG("ENEMY KILLED");
				pendingToDelete = true;

			}

			else if (physB->body->GetLinearVelocity().y < 0.5)
			{
				app->scene->player->isKilled = true;
			}

		}
	}
}