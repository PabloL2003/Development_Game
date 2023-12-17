
#include "WalkingEnemie.h"
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
#include "Player.h"
#include "Map.h"

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
	isDeaddFx = app->audio->LoadFx("Assets/Audio/Fx/Spider_death.wav");
	pbody = app->physics->CreateRectangle(position.x + 17, position.y, width, height, bodyType::DYNAMIC);
	spawn.x = parameters.attribute("x").as_int();
	spawn.y = parameters.attribute("y").as_int();
	despawn.x = spawn.x + 4000;
	despawn.y = spawn.y + 4000;
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMIE;
	SetSpawnPoint(spawn);

	//Killed f

	return true;
}

void Wenem::MovementLogic(float dt) {


	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{

		if (pbody->body->GetLinearVelocity().x > 0.5f)
		{
			pbody->body->ApplyForce(b2Vec2(-movementDampen * dt, 0.0f), pbody->body->GetWorldCenter(), true);
		}
		else
		{
			if (pbody->body->GetLinearVelocity().x > -maxVel)
				pbody->body->ApplyForce(b2Vec2(-movementForce * dt, 0.0f), pbody->body->GetWorldCenter(), true);
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		
		if (pbody->body->GetLinearVelocity().x < -0.5f)
		{
			pbody->body->ApplyForce(b2Vec2(movementDampen * dt, 0.0f), pbody->body->GetWorldCenter(), true);
		}
		else
		{
			if (pbody->body->GetLinearVelocity().x < maxVel)
				pbody->body->ApplyForce(b2Vec2(movementForce * dt, 0.0f), pbody->body->GetWorldCenter(), true);
		}
	}

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;


}




bool Wenem::Update(float dt)
{
	KilledPlayer();
	IsDead();

	/*if (isKilled)
		return true;*/

	currentAnim->Update();
	app->render->DrawTexture(texture, position.x, position.y - 8, &(currentAnim->GetCurrentFrame()));

	pbody->body->ApplyForce(b2Vec2(0, GRAVITY_Y * dt), pbody->body->GetWorldCenter(), true);

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;


	iPoint destiny = app->map->WorldToMap(app->scene->player->position.x,app->scene->player->position.y);
	iPoint origin = app->map->WorldToMap(position.x, position.y);

	/*if (pendingToDelete)
	{
		isKilled = true;
		CleanUp();
	}*/

	if(destiny.DistanceTo(origin)<9){
		MovementLogic(dt);
	}
	

	return true;
}

void Wenem::TeleportTo(iPoint pos)
{
	// Detén la velocidad actual del enemigo
	pbody->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));

	// Establece la posición del cuerpo físico del enemigo en el punto de destino
	pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(pos.x), PIXEL_TO_METERS(pos.y)), 0.0f);

	// Actualiza la posición del enemigo
	position.x = pos.x;
	position.y = pos.y;

}

void Wenem::SetSpawnPoint(iPoint pos)
{
	spawn = pos;
}

void Wenem::KilledPlayer()
{
	if (killedPlayer == true)
	{
		TeleportTo(spawn);
	}
	killedPlayer = false;
}

void Wenem::IsDead()
{
	if (isKilled == true)
	{
		/*app->audio->PlayFx("Assets/Audio/Fx/Spider_death);*/
		TeleportTo(despawn);
	}
	isKilled = false;
}

bool Wenem::CleanUp()
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

void Wenem::OnCollision(PhysBody* physA, PhysBody* physB)
{
	if (physB->ctype == ColliderType::PLAYER)
	{
		LOG("Collision ENEMY");

		if (!app->debug->godMode)
		{
			if (physB->body->GetLinearVelocity().y >= 0.5)
			{
				LOG("ENEMY KILLED");
				isKilled = true;
				app->audio->PlayFx(isDeaddFx);
			}

			else if (physB->body->GetLinearVelocity().y < 0.5)
			{
				app->scene->player->isKilled = true;
				killedPlayer = true;
			}

		}
	}
}
