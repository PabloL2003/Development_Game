#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	iPoint GetPLayerPosition();
	iPoint GetenemiePosition();
	iPoint Getenemie2Position();
	iPoint Getenemie3Position();
	iPoint Getenemie4Position();

public:
	Player* player;
	Entity* enemie;
	Entity* enemie2;
	Entity* enemie3;
	Entity* enemie4;

	List<PhysBody*> boxes;
	List<PhysBody*> spikes;

private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	

};

#endif // __SCENE_H__