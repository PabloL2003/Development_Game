#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
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

	return ret;
}

// Called before the first frame
bool Scene_Menu::Start()
{
	active = false;
	exit = false;
	menuSettings = false;
	credits = false;
	Enable();

	//Inicializar texturas y música
	background = app->tex->Load("Assets/Textures/MenuScene.png");
	if (background == nullptr)
	{
		LOG("Background no");
	}
	else
	{
		LOG("Background yes");
	}
	settingsBackground = app->tex->Load("Assets/Textures/SettingsMMenuScreen.png");
	creditsBackground = app->tex->Load("Assets/Textures/CreditsScreen.png");

	uint w, h;
	app->win->GetWindowSize(w, h);
	// Buttons 
	// -- Main menu
	playBtn = (GUIControlButton*)app->guiManager->CreateGuiControl(GUIControlType::BUTTON, 1, "Play", { 50,60,250,100 }, this);
	continueBtn = (GUIControlButton*)app->guiManager->CreateGuiControl(GUIControlType::BUTTON, 2, "Continue", { 50, 170, 250, 100 }, this);
	menuOptionsBtn = (GUIControlButton*)app->guiManager->CreateGuiControl(GUIControlType::BUTTON, 3, "Options", { 50, 280, 250, 100 }, this);
	creditsBtn = (GUIControlButton*)app->guiManager->CreateGuiControl(GUIControlType::BUTTON, 4, "Credits", { 50, 390, 250, 100 }, this);
	menuExitBtn = (GUIControlButton*)app->guiManager->CreateGuiControl(GUIControlType::BUTTON, 5, "Exit", { 50, 500, 250, 100 }, this);
	// -- Settings
	returnBtn = (GUIControlButton*)app->guiManager->CreateGuiControl(GUIControlType::BUTTON, 6, "X", { (int)w - 55, 5, 50, 50 }, this);

	// Texturas si queremos

	// Checkboxes
	// -- Settings
	fullscreenCbox = (GUICheckbox*)app->guiManager->CreateGuiControl(GUIControlType::CHECKBOX, 7, "Fullscreen cbox", { 650, 520, 50, 50 }, this);
	vsyncCbox = (GUICheckbox*)app->guiManager->CreateGuiControl(GUIControlType::CHECKBOX, 8, "Vsync cbox", { 500, 620, 50, 50 }, this);

	// Sliders
	// -- Settings
	bgmSlider = (GUISlider*)app->guiManager->CreateGuiControl(GUIControlType::SLIDER, 9, "BGM Slider", { 500, 275, 35, 35 }, this);
	sfxSlider = (GUISlider*)app->guiManager->CreateGuiControl(GUIControlType::SLIDER, 10, "SFX Slider", { 500, 385, 35, 35 }, this);

	// Initial GUI states
	returnBtn->state = GUIControlState::DISABLED;
	fullscreenCbox->state = GUIControlState::DISABLED;
	vsyncCbox->state = GUIControlState::DISABLED;
	bgmSlider->state = GUIControlState::DISABLED;
	sfxSlider->state = GUIControlState::DISABLED;

	bgmSlider->SetValue(app->audio->GetBGMVolume());
	sfxSlider->SetValue(app->audio->GetSFXVolume());
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
		if (!menuSettings && !credits)
		{
			if (playBtn->state == GUIControlState::DISABLED) playBtn->state = GUIControlState::NORMAL;
			pugi::xml_document gameStateFile;
			pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

			if (result)
			{
				if (continueBtn->state == GUIControlState::DISABLED) continueBtn->state = GUIControlState::NORMAL;
			}
			if (menuOptionsBtn->state == GUIControlState::DISABLED) menuOptionsBtn->state = GUIControlState::NORMAL;
			if (creditsBtn->state == GUIControlState::DISABLED) creditsBtn->state = GUIControlState::NORMAL;
			if (menuExitBtn->state == GUIControlState::DISABLED) menuExitBtn->state = GUIControlState::NORMAL;
		}
		else
		{
			if (playBtn->state != GUIControlState::DISABLED) playBtn->state = GUIControlState::DISABLED;
			if (continueBtn->state != GUIControlState::DISABLED) continueBtn->state = GUIControlState::DISABLED;
			if (menuOptionsBtn->state != GUIControlState::DISABLED) menuOptionsBtn->state = GUIControlState::DISABLED;
			if (creditsBtn->state != GUIControlState::DISABLED) creditsBtn->state = GUIControlState::DISABLED;
			if (menuExitBtn->state != GUIControlState::DISABLED) menuExitBtn->state = GUIControlState::DISABLED;
			if (returnBtn->state != GUIControlState::DISABLED) playBtn->state = GUIControlState::DISABLED;
			if (fullscreenCbox->state != GUIControlState::DISABLED) continueBtn->state = GUIControlState::DISABLED;
			if (vsyncCbox->state != GUIControlState::DISABLED) continueBtn->state = GUIControlState::DISABLED;
		}
	}

	if (app->scene->IsEnabled()) app->scene->Disable();

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) return false;

	if (exit) return false;

	// Render background textures

	app->render->DrawTexture(background, app->render->camera.x, app->render->camera.y * -1, NULL);

	if (menuSettings)
	{
		app->render->DrawTexture(settingsBackground, app->render->camera.x, app->render->camera.y * -1, NULL);
	}

	if (credits)
	{
		app->render->DrawTexture(creditsBackground, app->render->camera.x, app->render->camera.y * -1, NULL);
	}

	return true;
}

// Called each loop iteration
bool Scene_Menu::PostUpdate()
{
	bool ret = true;

	app->guiManager->Draw();

	return ret;
}

bool Scene_Menu::CleanUp()
{
	background = nullptr;
	settingsBackground = nullptr;
	creditsBackground = nullptr;
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
		app->scene->player->TeleportTo(app->scene->player->spawn);
		app->ftb->SceneFadeToBlack(this, app->scene, 0.0f);
		playBtn->state = GUIControlState::DISABLED;
		continueBtn->state = GUIControlState::DISABLED;
		menuOptionsBtn->state = GUIControlState::DISABLED;
		menuExitBtn->state = GUIControlState::DISABLED;
		creditsBtn->state = GUIControlState::DISABLED;

		break;
	case 2: // Continue btn;
		LOG("Continue button");
		app->LoadRequest();
		app->ftb->SceneFadeToBlack(this, app->scene, 0.0f);
		playBtn->state = GUIControlState::DISABLED;
		continueBtn->state = GUIControlState::DISABLED;
		menuOptionsBtn->state = GUIControlState::DISABLED;
		menuExitBtn->state = GUIControlState::DISABLED;
		creditsBtn->state = GUIControlState::DISABLED;

		break;
	case 3: // Settings btn
		LOG("Settings button");
		menuSettings = true;
		bgmSlider->SetValue(app->audio->GetBGMVolume());
		sfxSlider->SetValue(app->audio->GetSFXVolume());
		playBtn->state = GUIControlState::DISABLED;
		continueBtn->state = GUIControlState::DISABLED;
		menuOptionsBtn->state = GUIControlState::DISABLED;
		creditsBtn->state = GUIControlState::DISABLED;
		menuExitBtn->state = GUIControlState::DISABLED;

		(flags & SDL_WINDOW_FULLSCREEN) ? fullscreenCbox->state = GUIControlState::SELECTED
			: fullscreenCbox->state = GUIControlState::NORMAL;

		returnBtn->state = GUIControlState::NORMAL;
		bgmSlider->state = GUIControlState::NORMAL;
		sfxSlider->state = GUIControlState::NORMAL;

		break;
	case 4: // Credits btn
		LOG("Credits button");
		credits = true;
		playBtn->state = GUIControlState::DISABLED;
		continueBtn->state = GUIControlState::DISABLED;
		menuOptionsBtn->state = GUIControlState::DISABLED;
		creditsBtn->state = GUIControlState::DISABLED;
		menuExitBtn->state = GUIControlState::DISABLED;

		returnBtn->state = GUIControlState::NORMAL;
		break;
	case 5: // Exit btn
		LOG("Exit button");
		exit = true;
		break;
	case 6: // Return btn
		LOG("Exit button");
		if (menuSettings) menuSettings = false;
		if (credits) credits = false;
		playBtn->state = GUIControlState::NORMAL;
		continueBtn->state = GUIControlState::NORMAL;
		menuOptionsBtn->state = GUIControlState::NORMAL;
		creditsBtn->state = GUIControlState::NORMAL;
		menuExitBtn->state = GUIControlState::NORMAL;
		fullscreenCbox->state = GUIControlState::DISABLED;
		vsyncCbox->state = GUIControlState::DISABLED;
		returnBtn->state = GUIControlState::DISABLED;
		bgmSlider->state = GUIControlState::DISABLED;
		sfxSlider->state = GUIControlState::DISABLED;
		break;

	case 7: // Fullscreen cbox
		LOG("Fullscreen checkbox");
		(fullscreenCbox->state == GUIControlState::SELECTED) ? SDL_SetWindowFullscreen(app->win->window, 0)
			: SDL_SetWindowFullscreen(app->win->window, 1);
		break;
	case 8: // Vsync checkbox
		LOG("Vsync checkbox");
		app->vsync = !app->vsync;
		break;

	case 9: // Music slider
		LOG("Music slider");
		app->audio->SetBGMVolume(bgmSlider->value);
		break;

	case 10: // SFX slider
		LOG("SFX slider");
		app->audio->SetSFXVolume(sfxSlider->value);
		break;
	}

	return true;
}
