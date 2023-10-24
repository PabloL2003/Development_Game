#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

void Player::InitAnims() {

	//R.Idle
	for (pugi::xml_node node = parameters.child("right_idle").child("pushback");
		node; node = node.next_sibling("pushback")) {
		rightIdle.PushBack({ node.attribute("x").as_int(),
							node.attribute("y").as_int(),
							node.attribute("width").as_int(),
							node.attribute("height").as_int() });
	}

	//L.Idle
	for (pugi::xml_node node = parameters.child("left_idle").child("pushback");
		node; node = node.next_sibling("pushback")) {
		leftIdle.PushBack({ node.attribute("x").as_int(),
							node.attribute("y").as_int(),
							node.attribute("width").as_int(),
							node.attribute("height").as_int() });
	}
		
}


bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	InitAnims();

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	return true;
}

void Player::AnimationLogic() {

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (pbody->body->GetLinearVelocity().x > 0.01f) 
		{
			
		}
		else if (pbody->body->GetLinearVelocity().x < -0.01f)
		{
			currentAnim = &leftIdle;
		}
	}

}

void Player::MovementLogic() {

	//Gravity application
	pbody->body->ApplyForce(b2Vec2(0, -GRAVITY_Y), pbody->body->GetWorldCenter(), true);

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && jumps > 0)
	{
		pbody->body->SetLinearVelocity(b2Vec2(pbody->body->GetLinearVelocity().x, 0.0f));
		pbody->body->ApplyForce(b2Vec2(0, -225.0f), pbody->body->GetWorldCenter(), true);
		jumps--;
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) 
	{

		if (pbody->body->GetLinearVelocity().x > 0.5f)
		{
			//Opposite direction dampening
			pbody->body->ApplyForce(b2Vec2(-movementDampen, 0.0f), pbody->body->GetWorldCenter(), true);
		}
		else
		{
			if (pbody->body->GetLinearVelocity().x > -maxVel)
				pbody->body->ApplyForce(b2Vec2(-movementForce, 0.0f), pbody->body->GetWorldCenter(), true);
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (pbody->body->GetLinearVelocity().x < -0.5f)
			{
				//Opposite direction dampening
				pbody->body->ApplyForce(b2Vec2(movementDampen, 0.0f), pbody->body->GetWorldCenter(), true);
		}
		else
			{
				if (pbody->body->GetLinearVelocity().x < maxVel)
					pbody->body->ApplyForce(b2Vec2(movementForce, 0.0f), pbody->body->GetWorldCenter(), true);
			}
	}

	/*if (pbody->body->GetLinearVelocity().x < 0.3 && pbody->body->GetLinearVelocity().x >= 0) {
		pbody->body->SetLinearVelocity(b2Vec2(0.0f, -GRAVITY_Y));
	}

	if (pbody->body->GetLinearVelocity().x > -0.3 && pbody->body->GetLinearVelocity().x <= 0) {
		pbody->body->SetLinearVelocity(b2Vec2(0.0f, -GRAVITY_Y));
	}*/

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE)
	{
		if (pbody->body->GetLinearVelocity().x > 0.5f)
			pbody->body->ApplyForce(b2Vec2(-pbody->body->GetLinearVelocity().x * idleDampenMultiplier, 0.0f), pbody->body->GetWorldCenter(), true);

		if (pbody->body->GetLinearVelocity().x < -0.5f)
			pbody->body->ApplyForce(b2Vec2(-pbody->body->GetLinearVelocity().x * idleDampenMultiplier, 0.0f), pbody->body->GetWorldCenter(), true);
	}

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
}

bool Player::Update(float dt)
{
	/*currentAnim->Update();
	app->render->DrawTexture(texture, position.x, position.y, &(currentAnim->GetCurrentFrame()));

	AnimationLogic();*/
	MovementLogic();


	return true;
}

bool Player::CleanUp()
{

	texturePath = nullptr;
	currentAnim = nullptr;

	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		//app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		jumps = 2;
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}