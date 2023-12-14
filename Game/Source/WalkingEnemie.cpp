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



bool Wenem::Update(float dt)
{
	currentAnim->Update();
	app->render->DrawTexture(texture, position.x + 5, position.y - 8, &(currentAnim->GetCurrentFrame()));

	pbody->body->ApplyForce(b2Vec2(0, GRAVITY_Y * dt), pbody->body->GetWorldCenter(), true);

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
	
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