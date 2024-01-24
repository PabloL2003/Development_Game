#ifndef __SCENE_MENU_H__
#define __SCENE_MENU_H__

#include "Module.h"
#include "Anim.h"
#include "GUIControlButton.h"
#include "GUICheckbox.h"
#include "GUISlider.h"

struct SDL_Texture;

class Scene_Menu : public Module
{
public:

	Scene_Menu();

	// Destructor
	virtual ~Scene_Menu();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

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

	bool OnGUIMouseClickEvent(GUIControl* control);

public:

	// Buttons
	// -- Main menu
	GUIControlButton* playBtn;
	GUIControlButton* continueBtn;
	GUIControlButton* menuOptionsBtn;
	GUIControlButton* creditsBtn;
	GUIControlButton* menuExitBtn;
	GUIControlButton* returnBtn;

	// Sliders
	// -- Settings
	GUISlider* bgmSlider;
	GUISlider* sfxSlider;

	// Checkboxes
	// -- Settings
	GUICheckbox* fullscreenCbox;
	GUICheckbox* vsyncCbox;

	bool exit;

private:

	/*SDL_Texture* background;
	const char* background_texturePath;
	SDL_Texture* settingsBackground;
	SDL_Texture* creditsBackground;*/

	bool menuSettings;
	bool credits;
};
#endif // __SCENE_MENU_H__

