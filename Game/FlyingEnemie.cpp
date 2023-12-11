#include "FlyingEnemie.h"
#include "../App.h"
#include "../Textures.h"
#include "../Audio.h"
#include "../Input.h"
#include "../Render.h"
#include "../Scene.h"
#include "../Log.h"
#include "../Point.h"
#include "../Physics.h"
#include "../Debug.h"

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
	
}

bool Flyenem::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	//InitAnims();

	return true;
}

//
//void Flyenem::AnimationLogic(float dt) {
//
//	if (/*Player outside pathfinding range*/) {
//
//		if (/*player position < enemie position */)
//		{
//			currentAnim = &flyleftmov;
//		}
//		if (/*player position > enemie position */)
//		{
//			currentAnim = &flyrigthmov;
//		}
//	}
//
//
//
//	if (/*Player inside pathfinding range*/) {
//
//
//		if (/*enem posx -- */) {
//
//			currentAnim = &flyleftmov;
//		}
//		if (/*enem posx ++ */) {
//
//			currentAnim = &flyrigthmov;
//		}
//	}
//}

bool Flyenem::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateCircle(position.x, position.y, 16, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMIE;



	return true;
}


bool Flyenem::Update(float dt)
{
	currentAnim->Update();
	app->render->DrawTexture(texture, position.x + 5, position.y - 8, &(currentAnim->GetCurrentFrame()));

	//AnimationLogic(dt);


	return true;
}


bool Flyenem::CleanUp()
{

	texturePath = nullptr;
	currentAnim = nullptr;

	return true;
}