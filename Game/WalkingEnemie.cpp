#include "WalkingEnemie.h"
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

	currentAnim = &wl_idle;
}






bool Wenem::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	//InitAnims();

	return true;
}

bool Wenem::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateCircle(position.x, position.y, 16, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	

	return true;
}

void Wenem::AnimationLogic(float dt) {

	if (/*Player outside pathfinding range*/) {

		if (/*player position < enemie position */)
		{
			currentAnim = &wl_idle;
		}
		if (/*player position > enemie position */)
		{
			currentAnim = &wr_idle;
		}
	}



	if (/*Player inside pathfinding range*/) {


		if(/*enem posx -- */){
		
			currentAnim = &wleftmov;
		}
		if (/*enem posx ++ */) {

			currentAnim = &wrigthmov;
		}
	}
}
