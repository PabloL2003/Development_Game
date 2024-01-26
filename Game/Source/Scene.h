#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "FlyingEnemie.h"
#include "WalkingEnemie.h"
#include "Coin.h"
#include "Boss.h"
#include "GUIControl.h"
#include "GUIControlButton.h"
#include "GUICheckbox.h"
#include "GUISlider.h"
#include "GUIControlValueBox.h"

#include <chrono>
using namespace std;

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

	// Manages the menu 
	void PressPause();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool LoadState(pugi::xml_node node);

	bool SaveState(pugi::xml_node node);

	void ResetTimer();

	//Handles multiple GUI event methods
	bool OnGUIMouseClickEvent(GUIControl* control);

	iPoint GetPLayerPosition();
	iPoint GetenemiePosition();
	iPoint Getenemie2Position();
	iPoint Getenemie3Position();
	iPoint Getenemie4Position();

public:
	Player* player;
	Flyenem* enemie;
	Flyenem* enemie2;
	Wenem* enemie3;
	Wenem* enemie4;
	BOSS* enemie5;


	//coins
	COIN* coin7;
	COIN* coin8;
	COIN* coin9;
	COIN* coin10;
	COIN* coin11;
	COIN* coin12;

	// Buttons
	GUIControlButton* pauseBtn;
	GUIControlButton* resumeBtn;
	GUIControlButton* backToTitleBtn;
	GUIControlButton* settingsBtn;
	GUIControlButton* exitBtn;
	// -- Settings
	GUIControlButton* gameReturnBtn;

	// Sliders 
	// -- Settings 
	GUISlider* bgmGameSlider;
	GUISlider* sfxGameSlider;

	// Checkboxes
	// -- Settings
	GUICheckbox* fullscreenGameCbox;
	GUICheckbox* vsyncGameCbox;

	// Value boxes
	GUIControlValueBox* playerDeathsBox;
	GUIControlValueBox* timerBox;


	bool pause;


private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;

	SDL_Texture* pauseMenuTexture;
	SDL_Texture* settingsBackground;
	
	bool exit;
	bool gameplaySettings;

	// Variables for timer
	std::chrono::high_resolution_clock::time_point startTime;
	std::chrono::duration<float> elapsedTime;
};

#endif // __SCENE_H__