#include "Boss.h"
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

BOSS::BOSS() : Entity(EntityType::BOSS)
{
	name.Create("BOSS");
}

BOSS::~BOSS() {

}

void BOSS::InitAnims() {

	//R.mov
	for (pugi::xml_node node = parameters.child("BOSSrigthmov").child("pushback");
		node; node = node.next_sibling("pushback"))
	{
		BOSSrigthmov.PushBack({ node.attribute("x").as_int(),
							node.attribute("y").as_int(),
							node.attribute("width").as_int(),
							node.attribute("height").as_int() });
	}
	BOSSrigthmov.speed = parameters.child("BOSSrigthmov").attribute("animspeed").as_float();
	BOSSrigthmov.loop = parameters.child("BOSSrigthmov").attribute("loop").as_bool();

	for (pugi::xml_node node = parameters.child("BOSSleftmov").child("pushback");
		node; node = node.next_sibling("pushback"))
	{
		BOSSleftmov.PushBack({ node.attribute("x").as_int(),
							node.attribute("y").as_int(),
							node.attribute("width").as_int(),
							node.attribute("height").as_int() });
	}
	BOSSleftmov.speed = parameters.child("BOSSleftmov").attribute("animspeed").as_float();
	BOSSleftmov.loop = parameters.child("BOSSleftmov").attribute("loop").as_bool();

	currentAnim = &BOSSleftmov;

}

bool BOSS::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	spawn.x = parameters.attribute("x").as_int();
	spawn.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	InitAnims();

	return true;
}


bool BOSS::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x, position.y, 18, bodyType::DYNAMIC);
	pbody->body->SetGravityScale(-5);

	despawn.x = 3000;
	despawn.y = 3000;
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMIE;
	SetSpawnPoint(spawn);


	return true;
}


void BOSS::MovementLogic(float dt) {

	//movement logic to mimic the player's

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		currentAnim = &BOSSleftmov;
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
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		currentAnim = &BOSSrigthmov;
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
	else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		pbody->body->SetLinearVelocity(b2Vec2(pbody->body->GetLinearVelocity().x, 0.0f));
		pbody->body->ApplyForce(b2Vec2(0, -18.0f * dt), pbody->body->GetWorldCenter(), true);
	}
	else {
		pbody->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	}

}

bool BOSS::Update(float dt)
{
	KilledPlayer();
	IsDead();

	iPoint destiny = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);
	iPoint origin = app->map->WorldToMap(position.x, position.y);

	currentAnim->Update();
	app->render->DrawTexture(texture, position.x, position.y - 15, &(currentAnim->GetCurrentFrame()));

	/*app->audio->PlayFx(FlyFx); */
	if (destiny.DistanceTo(origin) < 25) {
		MovementLogic(dt);
	}
	else if (destiny.DistanceTo(origin) != 21) {
		pbody->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	}



	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;


	return true;
}

void BOSS::TeleportTo(iPoint pos)
{
	// Detén la velocidad actual del enemigo
	pbody->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));

	// Establece la posición del cuerpo físico del enemigo en el punto de destino
	pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(pos.x), PIXEL_TO_METERS(pos.y)), 0.0f);

	// Actualiza la posición del enemigo
	position.x = pos.x;
	position.y = pos.y;

}

void BOSS::SetSpawnPoint(iPoint pos)
{
	spawn = pos;
}

void BOSS::IsDead()
{
	if (isKilled == true)
	{
		TeleportTo(despawn);
	}
	isKilled = false;
}

void BOSS::KilledPlayer()
{
	if (killedPlayer == true)
	{
		TeleportTo(spawn);
	}
	killedPlayer = false;
}

bool BOSS::CleanUp()
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

void BOSS::OnCollision(PhysBody* physA, PhysBody* physB)
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
				BOSSHP -= 1;;
				//Añadir un desplazamiento al player o un timer de invulnerabilidad al boss//
			/*	iPoint newpos;
				newpos.x = app->scene->player->position.x-50;
				newpos.y = app->scene->player->position.y;

				app->scene->player->TeleportTo(newpos);*/
			}
			if (BOSSHP == 0) {
				isKilled = true;
				app->audio->PlayFx(isDeadFx);
			}
			//If the enemy position in y is greater than the player's
			else if (physB->body->GetLinearVelocity().y < 0.5)
			{
				//Teleport the enemy to its original position
				app->scene->player->isKilled = true;
				killedPlayer = true;
				BOSSHP = 3;
			}

		}
	}
}