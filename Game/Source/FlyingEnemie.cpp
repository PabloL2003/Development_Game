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
	//For each child in config, iterate to find the animation and load the values.

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
	spawn.x = parameters.attribute("x").as_int();
	spawn.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	InitAnims();

	return true;
}


bool Flyenem::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	isDeadFx = app->audio->LoadFx("Assets/Audio/Fx/bat-fx.wav");
	pbody = app->physics->CreateCircle(position.x, position.y, 7, bodyType::DYNAMIC);
	pbody->body->SetGravityScale(0);
	
	despawn.x = 3000;
	despawn.y = 3000;
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMIE;
	SetSpawnPoint(spawn);

	mouseTileTex = app->tex->Load("Assets/Textures/tiletext.png");

	return true;
}


void Flyenem::MovementLogic(float dt) {

	//Path of the bat's movement
	if (position.x < spawn.x+1)
	{
		pbody->body->SetLinearVelocity(b2Vec2(1.0f, 0));
		
	}

	if (position.x > spawn.x + 120)
	{
		pbody->body->SetLinearVelocity(b2Vec2(-1.0f,0));

	}

}

bool Flyenem::Update(float dt)
{
	KilledPlayer();
	IsDead();

	iPoint destiny = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);
	iPoint origin = app->map->WorldToMap(position.x, position.y);

	currentAnim->Update();
	app->render->DrawTexture(texture, position.x, position.y - 15, &(currentAnim->GetCurrentFrame()));

		/*app->audio->PlayFx(FlyFx); */
		MovementLogic(dt);
	
	
	// In the future, managing dynamic spawn/despawn enemy
	/*if (isKilled)
		return true;*/


		// This big if would implement the pathfinding and the showing of the path whle pressing the mouse button 
		// And also would make th enemies move towards the players if you used the M key 
		// This code has been tested in a different branch but due to what we think were memory leaks 
		// the code would crash 95% of the times that it created the paths, you can see the paths in the video we send

		/*if (app->scene->player->position.x > position.y - 50) {
			iPoint origin = app->map->WorldToMap(position.x+(width / 2),position.y+ (height / 2));
			iPoint destiny = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);
			app->map->pathfinding->CreatePath(origin, destiny);


			if(app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
			{
				const DynArray<iPoint>* movePath = app->map->pathfinding->GetLastPath();

				iPoint enemypos = app->map->MapToWorld(movePath->At(2)->x, movePath->At(2)->y);
				b2Vec2 movepos(PIXEL_TO_METERS(enemypos.x), PIXEL_TO_METERS(enemypos.y));
				pbody->body->SetTransform(movepos, 0);
			}


			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
				for (uint i = 0; i < path->Count(); i++) {
					iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
					app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
				}
			}
		}
		currentAnim->Update();
		app->render->DrawTexture(texture, position.x +27, position.y +2, &(currentAnim->GetCurrentFrame()));*/

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;


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
	if (isKilled == true)
	{
		TeleportTo(despawn);
	}
	isKilled = false;
}

void Flyenem::KilledPlayer()
{
	if (killedPlayer == true)
	{
		TeleportTo(spawn);
	}
	killedPlayer = false;
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
			//If the player position in y is greater than the enemy's
			if (physB->body->GetLinearVelocity().y >= 0.5)
			{
				LOG("ENEMY KILLED");
				isKilled = true;
				app->audio->PlayFx(isDeadFx);
			}

			//If the enemy position in y is greater than the player's
			else if (physB->body->GetLinearVelocity().y < 0.5)
			{
				//Teleport the enemy to its original position
				app->scene->player->isKilled = true;
				killedPlayer = true;
			}

		}
	}
}