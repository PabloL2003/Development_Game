#include "GUIManager.h"
#include "App.h"
#include "Textures.h"

#include "GUIControlButton.h"
#include "GUICheckbox.h"
#include "GUISlider.h"
#include "Audio.h"

GUIManager::GUIManager() : Module()
{
	name.Create("guiManager");
}

GUIManager::~GUIManager() {}

bool GUIManager::Start()
{
	return true;
}

GUIControl* GUIManager::CreateGuiControl(GUIControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds)
{
	GUIControl* guiControl = nullptr;

	//Call the constructor according to the GUIControlType
	switch (type)
	{
	case GUIControlType::BUTTON:
		guiControl = new GUIControlButton(id, bounds, text);
		break;
	case GUIControlType::CHECKBOX:
		guiControl = new GUICheckbox(id, bounds, text);
		break;
	case GUIControlType::SLIDER:
		guiControl = new GUISlider(id, bounds, text);
		break;
	}

	//Set the observer
	guiControl->observer = observer;

	//Created GUIControls are added into the list of controls
	guiControlsList.Add(guiControl);

	return guiControl;
}

bool GUIManager::Update(float dt)
{
	ListItem<GUIControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		control->data->Update(dt);
		control = control->next;
	}

	return true;
}

bool GUIManager::CleanUp()
{
	ListItem<GUIControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		RELEASE(control);
	}

	return true;

	return false;
}