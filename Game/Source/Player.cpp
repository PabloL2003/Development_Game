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
#include "Debug.h"

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
    //R.Run
	for (pugi::xml_node node = parameters.child("right_run").child("pushback");
		node; node = node.next_sibling("pushback"))
	{
		rightRun.PushBack({ node.attribute("x").as_int(),
							node.attribute("y").as_int(),
							node.attribute("width").as_int(),
							node.attribute("height").as_int() });
	}
	rightRun.speed = parameters.child("right_run").attribute("animspeed").as_float();
	rightRun.loop = parameters.child("right_run").attribute("loop").as_bool();
	//L.Run
	for (pugi::xml_node node = parameters.child("left_run").child("pushback");
		node; node = node.next_sibling("pushback"))
	{
		leftRun.PushBack({ node.attribute("x").as_int(),
						   node.attribute("y").as_int(),
						   node.attribute("width").as_int(),
						   node.attribute("height").as_int() });
	}
	leftRun.speed = parameters.child("left_run").attribute("animspeed").as_float();
	leftRun.loop = parameters.child("left_run").attribute("loop").as_bool();
	//R.Jump
	for (pugi::xml_node node = parameters.child("right_jump").child("pushback");
		node; node = node.next_sibling("pushback"))
	{
		rightJump.PushBack({ node.attribute("x").as_int(),
							node.attribute("y").as_int(),
							node.attribute("width").as_int(),
							node.attribute("height").as_int() });
	}
	rightJump.speed = parameters.child("right_jump").attribute("animspeed").as_float();
	rightJump.loop = parameters.child("right_jump").attribute("loop").as_bool();
	//L.Jump
	for (pugi::xml_node node = parameters.child("left_jump").child("pushback");
		node; node = node.next_sibling("pushback"))
	{
		leftJump.PushBack({ node.attribute("x").as_int(),
							node.attribute("y").as_int(),
							node.attribute("width").as_int(),
							node.attribute("height").as_int() });
	}
	leftJump.speed = parameters.child("left_jump").attribute("animspeed").as_float();
	leftJump.loop = parameters.child("left_jump").attribute("loop").as_bool();
	//R.Death
	for (pugi::xml_node node = parameters.child("right_death").child("pushback");
		node; node = node.next_sibling("pushback"))
	{
		rightDeath.PushBack({ node.attribute("x").as_int(),
							node.attribute("y").as_int(),
							node.attribute("width").as_int(),
							node.attribute("height").as_int() });
	}
	rightDeath.speed = parameters.child("right_death").attribute("animspeed").as_float();
	rightDeath.loop = parameters.child("right_death").attribute("loop").as_bool();
	//L.Death
	for (pugi::xml_node node = parameters.child("left_death").child("pushback");
		node; node = node.next_sibling("pushback"))
	{
		leftDeath.PushBack({ node.attribute("x").as_int(),
							node.attribute("y").as_int(),
							node.attribute("width").as_int(),
							node.attribute("height").as_int() });
	}
	leftDeath.speed = parameters.child("left_death").attribute("animspeed").as_float();
	leftDeath.loop = parameters.child("left_death").attribute("loop").as_bool();

	currentAnim = &rightIdle;
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

	pbody = app->physics->CreateCircle(position.x, position.y, 16, bodyType::DYNAMIC);
	spawn.x = parameters.attribute("x").as_int();
	spawn.y = parameters.attribute("y").as_int();
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;
	SetSpawnPoint(spawn);

	pickSwordFx = app->audio->LoadFx("Assets/Audio/Fx/you-win-street-fighter.wav");
	isKilledFx = app->audio->LoadFx("Assets/Audio/Fx/SPIKES_TRAP_ON.wav");

	return true;
}

void Player::AnimationLogic(float dt) {


	if (currentAnim == &rightIdle)
	{
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			currentAnim = &leftRun;
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			currentAnim = &rightRun;
		}
		if (jumping == true)
		{
			currentAnim = &rightJump;
		}
	}

	if (currentAnim == &leftIdle)
	{
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			currentAnim = &leftRun;
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			currentAnim = &rightRun;
		}
		if (jumping == true)
		{
			currentAnim = &leftJump;
		}
	}

	if (currentAnim == &rightRun)
	{
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
		{
			if (app->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT)
			{
				currentAnim = &rightIdle;
			}
			else currentAnim = &leftRun;
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
		{
			currentAnim = &leftRun;
		}
		if (jumping == true)
		{
			currentAnim = &rightJump;
		}
	}
	
	if (currentAnim == &leftRun)
	{
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
		{
			if (app->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT)
			{
				currentAnim = &leftIdle;
			}
			else currentAnim = &rightRun;
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		{
			currentAnim = &rightRun;
		}
		if (jumping == true)
		{
			currentAnim = &leftJump;
		}
	}
	//Jump
	
	if (currentAnim == &rightJump)
	{
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
		{
			currentAnim = &leftJump;
		}
		if (jumping == false)
		{
			currentAnim = &rightIdle;
		}
		if (isKilled == true)
		{
			currentAnim = &rightDeath;
		}
	}

	if (currentAnim == &leftJump)
	{
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		{
			currentAnim = &rightJump;
		}
		if (jumping == false)
		{
			currentAnim = &leftIdle;
		}
	}

	//Death

	if (currentAnim == &rightDeath && currentAnim->HasFinished())
	{
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		{
			currentAnim = &rightIdle;
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
		{
			currentAnim = &leftIdle;
		}
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			currentAnim = &rightJump;
		}
	}

	

}

void Player::MovementLogic(float dt) {

	if (app->debug->godMode) {

		pbody->body->ApplyForce(b2Vec2(0, -GRAVITY_Y*dt), pbody->body->GetWorldCenter(), true);

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			pbody->body->SetLinearVelocity(b2Vec2(0.0f, -5.0f));

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_UP)
			pbody->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			pbody->body->SetLinearVelocity(b2Vec2(-5.0f, 0.0f));

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
			pbody->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			pbody->body->SetLinearVelocity(b2Vec2(0.0f, 5.0f));

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_UP)
			pbody->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			pbody->body->SetLinearVelocity(b2Vec2(5.0f, 0.0f));

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
			pbody->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	}
	//Gravity application
	pbody->body->ApplyForce(b2Vec2(0, GRAVITY_Y*dt), pbody->body->GetWorldCenter(), true);

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && jumps > 0)
	{
		pbody->body->SetLinearVelocity(b2Vec2(pbody->body->GetLinearVelocity().x, 0.0f));
		pbody->body->ApplyForce(b2Vec2(0, -18.0f*dt), pbody->body->GetWorldCenter(), true);
		jumping = true;
		jumps--;
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) 
	{

		if (pbody->body->GetLinearVelocity().x > 0.5f)
		{
			//Opposite direction dampening
			pbody->body->ApplyForce(b2Vec2(-movementDampen*dt, 0.0f), pbody->body->GetWorldCenter(), true);
		}
		else
		{
			if (pbody->body->GetLinearVelocity().x > -maxVel)
				pbody->body->ApplyForce(b2Vec2(-movementForce*dt, 0.0f), pbody->body->GetWorldCenter(), true);
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (pbody->body->GetLinearVelocity().x < -0.5f)
		{
			//Opposite direction dampening
			pbody->body->ApplyForce(b2Vec2(movementDampen * dt, 0.0f), pbody->body->GetWorldCenter(), true);
		}
		else
		{
			if (pbody->body->GetLinearVelocity().x < maxVel)
				pbody->body->ApplyForce(b2Vec2(movementForce * dt, 0.0f), pbody->body->GetWorldCenter(), true);
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE) {
		b2Vec2 velocity = pbody->body->GetLinearVelocity();
		float frictionForce = -velocity.x * idleDampenMultiplier;

		// Aplicar fricción en dirección opuesta al movimiento actual
		pbody->body->ApplyForce(b2Vec2(frictionForce, 0.0f), pbody->body->GetWorldCenter(), true);
	}

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;


}

void Player::IsDead()
{
	if (isKilled) 
	{
		currentAnim = &rightDeath;
		app->audio->PlayFx(isKilledFx);
		TeleportTo(spawn);
	}
	isKilled = false;
	
}

void Player::SetSpawnPoint(iPoint pos)
{
	spawn = pos;
}

void Player::TeleportTo(iPoint pos)
{
	if (pbody != nullptr)
	{
		// Detén la velocidad actual del jugador
		pbody->body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));

		// Establece la posición del cuerpo físico del jugador en el punto de destino
		pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(pos.x), PIXEL_TO_METERS(pos.y)), 0.0f);

		// Actualiza la posición del jugador
		position.x = pos.x;
		position.y = pos.y;
	}
}

bool Player::Update(float dt)
{
	IsDead();
	currentAnim->Update();
	app->render->DrawTexture(texture, position.x+5, position.y-8, &(currentAnim->GetCurrentFrame()));

	AnimationLogic(dt);
	MovementLogic(dt);

	return true;
}

bool Player::CleanUp()
{

	texturePath = nullptr;
	currentAnim = nullptr;
	delete pbody;

	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickSwordFx);
		if (app->debug->debug == false)
		{
			app->debug->debug = true;
		}
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		if (pbody->body->GetLinearVelocity().y > 0.0f)
		{
			jumping = false;
			jumps = 1;
		}
		break;
	case ColliderType::SPIKE:
		LOG("Collision SPIKE");
		if (!app->debug->debug)
		{
			isKilled = true;
		}
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	
	}
}