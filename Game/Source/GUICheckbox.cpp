#include "GUICheckbox.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"
#include "Textures.h"

GUICheckbox::GUICheckbox(uint32 id, SDL_Rect bounds, const char* text) : GUIControl(GUIControlType::CHECKBOX, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

}

GUICheckbox::~GUICheckbox()
{

}

bool GUICheckbox::Update(float dt)
{
	if (state != GUIControlState::DISABLED)
	{
		app->input->GetMousePosition(mouseX, mouseY);
		
		//Inside the limits of the checkbox
		if (mouseX >= bounds.x && mouseX <= bounds.x + bounds.w &&
			mouseY >= bounds.y && mouseY <= bounds.y + bounds.h) {

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				NotifyObserver();
				if (state == GUIControlState::SELECTED) state = GUIControlState::NORMAL;
				else if (state == GUIControlState::NORMAL) state = GUIControlState::SELECTED;

			}
		}

		switch (state)
		{
		case GUIControlState::NORMAL:
			app->render->DrawRectangle(bounds, 170, 0, 0, 255, true, false);
			break;
		case GUIControlState::SELECTED:
			app->render->DrawRectangle(bounds, 0, 255, 0, 255, true, false);
			break;
		}
	}
	return false;
}
