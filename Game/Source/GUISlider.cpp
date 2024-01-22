#include "GUISlider.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"
#include "Textures.h"

GUISlider::GUISlider(uint32 id, SDL_Rect bounds, const char* text) : GUIControl(GUIControlType::SLIDER, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

	length = 130;
	buttonBox = bounds;
	sliderLine = { bounds.x, (int)(bounds.y + bounds.h * 0.5f), length, 2 };
	value = 0;

}

GUISlider::~GUISlider()
{

}

bool GUISlider::Update(float dt)
{
	if (state != GUIControlState::DISABLED)
	{
		app->input->GetMousePosition(mouseX, mouseY);
		GUIControlState previousState = state;

		//Inside the limits of the slider
		if (mouseX >= buttonBox.x && mouseX <= buttonBox.x + buttonBox.w &&
			mouseY >= buttonBox.y && mouseY <= buttonBox.y + buttonBox.h)
		{
			state = GUIControlState::FOCUSED;
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GUIControlState::PRESSED;
				int updateValue = value + mouseX - mousePrevX;
				SetValue(updateValue);
				NotifyObserver();
			}

		}
		else {
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT &&
				previousState == GUIControlState::PRESSED)
			{
				int updateValue = value + mouseX - mousePrevX;
				SetValue(updateValue);
				NotifyObserver();
			}
			else
			{
				if (state != GUIControlState::NORMAL) state = GUIControlState::NORMAL;
			}
		}
		mousePrevX = mouseX;

		app->render->DrawRectangle(sliderLine, 100, 100, 100, 255, true, false);
		switch (state)
		{
		case GUIControlState::NORMAL:
			app->render->DrawRectangle(buttonBox, 0, 0, 255, 255, true, false);
			break;
		case GUIControlState::FOCUSED:
		    app->render->DrawRectangle(buttonBox, 0, 0, 20, 255, true, false);
			break;
		case GUIControlState::PRESSED:
			app->render->DrawRectangle(buttonBox, 0, 255, 0, 255, true, false);
			break;
		}
	}
	return false;
}

void GUISlider::SetValue(int value)
{
	this->value = value;
	if (value <= 0) value = 0;
	else if (value >= length) value = length;

	buttonBox.x = bounds.x + value;
}
