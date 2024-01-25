#ifndef __GUICONTROL_H__
#define __GUICONTROL_H__

#include "Input.h"
#include "Render.h"
#include "Module.h"

#include "Point.h"
#include "SString.h"

#include "SDL/include/SDL.h"

enum class GUIControlType
{
	BUTTON,
	TOGGLE,
	CHECKBOX,
	SLIDER,
	SLIDERBAR,
	COMBOBOX,
	DROPDOWNBOX,
	INPUTBOX,
	VALUEBOX,
	SPINNER

};

enum class GUIControlState
{
	DISABLED,
	NORMAL,
	FOCUSED,
	PRESSED,
	SELECTED
};

class GUIControl
{
public:

	//Constructor
	GUIControl(GUIControlType type, uint32 id) : type(type), id(id), state(GUIControlState::NORMAL) {}

	//Constructor
	GUIControl(GUIControlType type, SDL_Rect bounds, const char* text) :
		type(type),
		state(GUIControlState::NORMAL),
		bounds(bounds),
		text(text)
	{
		color.r = 255; color.g = 255; color.b = 255;
		texture = NULL;
	}

	//Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool Draw(Render* render)
	{
		return true;
	}

	void SetTexture(SDL_Texture* tex)
	{
		texture = tex;
		section = { 0,0,0,0 };
	}

	void SetObserver(Module* module)
	{
		observer = module;
	}

	void NotifyObserver()
	{
		observer->OnGUIMouseClickEvent(this);
	}

public:

	uint32 id;
	GUIControlType type;
	GUIControlState state;

	SString text;
	SDL_Rect bounds;
	SDL_Color color;

	SDL_Texture* texture;
	SDL_Rect section;

	Module* observer;

};
#endif // !__GUICONTROL_H__
