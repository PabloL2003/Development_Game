#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "FlyingEnemie.h"
#include "WalkingEnemie.h"
#include "BOSS.h"
#include "GUIControl.h"
#include "GUIControlButton.h"

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

	bool LoadState(pugi::xml_node node);

	bool SaveState(pugi::xml_node node);

	//Handles multiple GUI event methods
	bool OnGUIMouseClickEvent(GUIControl* control);

	iPoint GetPLayerPosition();
	iPoint GetenemiePosition();
	iPoint Getenemie2Position();
	iPoint Getenemie3Position();
	iPoint Getenemie4Position();
	iPoint Getenemie5Position();

public:
	Player* player;
	Flyenem* enemie;
	Flyenem* enemie2;
	Wenem* enemie3;
	Wenem* enemie4;
	BOSS* enemie5;
	GUIControlButton* gcButton;


private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	

};

#endif // __SCENE_H__