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

	//For each child in config, iterate to find the animation and load the values.
	
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

	//we set the default animation as rightIdle

	currentAnim = &rightIdle;
}


bool Player::Awake() {
	//initialising the animations and the intial positions.
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	InitAnims();

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	//declaring the spawn 
	pbody = app->physics->CreateCircle(position.x, position.y, 16, bodyType::DYNAMIC);
	spawn.x = parameters.attribute("x").as_int();
	spawn.y = parameters.attribute("y").as_int();
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;
	SetSpawnPoint(spawn);

	spawn2.x = 200;
	spawn2.y = 1750;

	saltoFx = app->audio->LoadFx("Assets/Audio/Fx/salto-fx.wav");
	pickSwordFx = app->audio->LoadFx("Assets/Audio/Fx/you-win-street-fighter.wav");
	COINFx = app->audio->LoadFx("Assets/Audio/Fx/coin.wav");
	isKilledFx = app->audio->LoadFx("Assets/Audio/Fx/SPIKES_TRAP_ON.wav");

	return true;
}

void Player::AnimationLogic(float dt) {

	//logic for animations in the case it is rightIdle
	if (currentAnim == &rightIdle)
	{
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			//if the player runs left 
			currentAnim = &leftRun;
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			//if the player runs right
			currentAnim = &rightRun;
		}
		if (jumping == true)
		{
			//if the player jumps
			currentAnim = &rightJump;
		}
	}

	//logic for animations in the case it is leftIdle
	if (currentAnim == &leftIdle)
	{
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			//if the player runs left
			currentAnim = &leftRun;
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			//if the player runs right
			currentAnim = &rightRun;
		}
		if (jumping == true)
		{
			//if the player jumps
			currentAnim = &leftJump;
		}
	}

	//logic for animations in the case it is rightRun
	if (currentAnim == &rightRun)
	{
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
		{
			if (app->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT)
			{
				//if the player does not press any key
				currentAnim = &rightIdle;
			}
			//if the player changes movement direction
			else currentAnim = &leftRun;
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
		{
			//double check if the player changes movement direction
			currentAnim = &leftRun;
		}
		if (jumping == true)
		{
			//if the player jumps
			currentAnim = &rightJump;
		}
	}
	
	//logic for animations in the case it is lefRun
	if (currentAnim == &leftRun)
	{
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
		{
			if (app->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT)
			{
				//if the player does not press any key
				currentAnim = &leftIdle;
			}
			//if the player changes movement direction
			else currentAnim = &rightRun;
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		{
			//double check if the player changes movement direction
			currentAnim = &rightRun;
		}
		if (jumping == true)
		{
			//if the player jumps
			currentAnim = &leftJump;
		}
	}
	//Jump
	

	//logic for animations in the case it is righJump
	if (currentAnim == &rightJump)
	{
		//if the player changes movement direction
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
		{
			currentAnim = &leftJump;
		}
		//return to Idle if not jumping
		if (jumping == false)
		{
			currentAnim = &rightIdle;
		}
		if (isKilled == true)
		{
			//making sure the death animations plays even if jumping
			currentAnim = &rightDeath;
		}
	}

	//logic for animations in the case it is leftJump
	if (currentAnim == &leftJump)
	{
		//if the player changes movement direction
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		{
			currentAnim = &rightJump;
		}
		//return to Idle if not jumping
		if (jumping == false)
		{
			currentAnim = &leftIdle;
		}
	}

	//Death

	//logic for animations in the case it is rightDeath and ensuring it plays 
	if (currentAnim == &rightDeath && currentAnim->HasFinished())
	{
		//return to previous animations 
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

	//Forces application for debug mode, ensuring the player can move freely 
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

	//Jump Logic, preventing the player to jump infinitely
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && jumps > 0)
	{	
		app->audio->PlayFx(saltoFx);
		pbody->body->SetLinearVelocity(b2Vec2(pbody->body->GetLinearVelocity().x, 0.0f));
		pbody->body->ApplyForce(b2Vec2(0, -18.0f*dt), pbody->body->GetWorldCenter(), true);
		jumping = true;
		jumps--;
		
	}

	//Moving left logic
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) 
	{
		//Applying movement dampening in both directions, and ensuring the movement doesn't surpass the limit
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

	//Moving right logic
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		//Applying movement dampening in both directions, and ensuring the movement doesn't surpass the limit
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

	//Applying the dampening when the movement is idle
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE) {
		b2Vec2 velocity = pbody->body->GetLinearVelocity();
		float frictionForce = -velocity.x * idleDampenMultiplier;

		//Applying the dampening in the opposite direction
		pbody->body->ApplyForce(b2Vec2(frictionForce, 0.0f), pbody->body->GetWorldCenter(), true);
	}

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;


}

//Logic of the death of the player
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
	//We first check if the player is dead
	IsDead();

	//Then we perform the necessary updates
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
		
		SetSpawnPoint(spawn2);
		isKilled = true;
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		//We ensure the player does not regain its jumps when collidin with a platform from below
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
		//Enemies spawn reset
		app->scene->enemie->killedPlayer = true;
		app->scene->enemie2->killedPlayer = true;
		app->scene->enemie3->killedPlayer = true;
		app->scene->enemie4->killedPlayer = true;
		break;
	case ColliderType::COIN:
		LOG("Collision COIN");
		app->audio->PlayFx(COINFx);
		Collectedcoins + 1;
		if (!app->debug->debug == false)
		{
			app->debug->debug = true;
		}
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	
	}
}