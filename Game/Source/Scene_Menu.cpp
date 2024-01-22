#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene_Menu.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "SDL_ttf/include/SDL_ttf.h"
#include "FadeToBlack.h"
#include "Physics.h"
#include "GUIManager.h"

#include "Defs.h"
#include "Log.h"

Scene_Menu::Scene_Menu() : Module()
{
	name.Create("scene_menu");
}

// Destructor
Scene_Menu::~Scene_Menu()
{

}

// Called before render is available
bool Scene_Menu::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene_Menu");
	bool ret = true;

	// Podemos llamar animaciones aquí

	return ret;
}

// Called before the first frame
bool Scene_Menu::Start()
{
	active = false;
	exit = false;
	menuSettings = false;

	//Inicializar texturas y música

	uint w, h;
	app->win->GetWindowSize(w, h);
	// Buttons 
	// -- Main menu
	playBtn = (GUIControlButton*)app->guiManager->CreateGuiControl(GUIControlType::BUTTON, 1, "Play", { 50,350,513,155 }, this);
	continueBtn = (GUIControlButton*)app->guiManager->CreateGuiControl(GUIControlType::BUTTON, 2, "Continue", {600, 350, 513, 155}, this);
	menuOptionsBtn = (GUIControlButton*)app->guiManager->CreateGuiControl(GUIControlType::BUTTON, 3, "Options", { 50, 510, 513, 155 }, this);
	menuExitBtn = (GUIControlButton*)app->guiManager->CreateGuiControl(GUIControlType::BUTTON, 4, "Exit", { 50, 670, 513, 155 }, this);
	// -- Settings
	returnBtn = (GUIControlButton*)app->guiManager->CreateGuiControl(GUIControlType::BUTTON, 5, "X", { (int)w - 55, 5, 50, 50 }, this);

	// Texturas si queremos

	// Checkboxes
	// -- Settings
	fullscreenCbox = (GUICheckbox*)app->guiManager->CreateGuiControl(GUIControlType::CHECKBOX, 6, "Fullscreen cbox", { 550, 755, 50, 50 }, this);
	vsyncCbox = (GUICheckbox*)app->guiManager->CreateGuiControl(GUIControlType::CHECKBOX, 7, "Vsync cbox", { 550, 640, 50, 50 }, this);

	// Sliders
	// -- Settings
	bgmSlider = (GUISlider*)app->guiManager->CreateGuiControl(GUIControlType::SLIDER, 8, "BGM Slider", { 500, 415, 35, 35 }, this);
	sfxSlider = (GUISlider*)app->guiManager->CreateGuiControl(GUIControlType::SLIDER, 10, "SFX Slider", { 500, 535, 35, 35 }, this);

	// Initial GUI states
	continueBtn->state = GUIControlState::DISABLED;
	returnBtn->state == GUIControlState::DISABLED;
	fullscreenCbox->state = GUIControlState::DISABLED;
	vsyncCbox->state = GUIControlState::DISABLED;
	bgmSlider->state = GUIControlState::DISABLED;
	sfxSlider->state = GUIControlState::DISABLED;

	//bgmSlider->SetValue(app->audio->GetBGMVolume());
	//sfxSlider->SetValue(app->audio->GetSFXVolume());
	return true;
}

// Called each loop iteration
bool Scene_Menu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene_Menu::Update(float dt)
{
	if (app->entityManager->IsEnabled()) app->entityManager->Disable();

	if (IsEnabled() && !app->scene->IsEnabled())
	{
		if (!menuSettings)
		{
			if (playBtn->state == GUIControlState::DISABLED) playBtn->state = GUIControlState::NORMAL;
			pugi::xml_document gameStateFile;
			pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

			if (result)
			{
				if (continueBtn->state == GUIControlState::DISABLED) continueBtn->state = GUIControlState::NORMAL;
			}
			if (menuOptionsBtn->state == GUIControlState::DISABLED) playBtn->state = GUIControlState::NORMAL;
			if (menuExitBtn->state == GUIControlState::DISABLED) menuExitBtn->state = GUIControlState::NORMAL;

		}
	}
	else
	{
		if (playBtn->state != GUIControlState::DISABLED) playBtn->state = GUIControlState::DISABLED;
		if (continueBtn->state != GUIControlState::DISABLED) continueBtn->state = GUIControlState::DISABLED;
		if (menuOptionsBtn->state != GUIControlState::DISABLED) menuOptionsBtn->state = GUIControlState::DISABLED;
		if (menuExitBtn->state != GUIControlState::DISABLED) menuExitBtn->state = GUIControlState::DISABLED;
		if (returnBtn->state != GUIControlState::DISABLED) playBtn->state = GUIControlState::DISABLED;
		if (fullscreenCbox->state != GUIControlState::DISABLED) continueBtn->state = GUIControlState::DISABLED;
		if (vsyncCbox->state != GUIControlState::DISABLED) continueBtn->state = GUIControlState::DISABLED;
	}

	if (app->scene->IsEnabled()) app->scene->Disable();

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) return false;

	if (exit) return false;

	return true;
}

// Called each loop iteration
bool Scene_Menu::PostUpdate()
{
	bool ret = true;

	//Render background

	return ret;
}

bool Scene_Menu::CleanUp()
{

	return true;
}

bool Scene_Menu::OnGUIMouseClickEvent(GUIControl* control)
{
	LOG("Event by %d", control->id);
	int flags = SDL_GetWindowFlags(app->win->window);
	switch (control->id)
	{
	case 1: //Play btn
		LOG("Play button");
		app->ftb->SceneFadeToBlack(this, app->scene, 0.0f);
		playBtn->state = GUIControlState::DISABLED;
		continueBtn->state = GUIControlState::DISABLED;
		menuOptionsBtn->state = GUIControlState::DISABLED;
		menuExitBtn->state = GUIControlState::DISABLED;

		break;
	case 2: // Continue btn;
		LOG("Continue button");
	}
	
	return true;
}