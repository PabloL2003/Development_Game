#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GUIControl.h"

#include "Point.h"
#include "SString.h"

class GUISlider : public GUIControl
{
public:

	GUISlider(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GUISlider();

	bool Update(float dt);
	void SetValue(int value);

	int mouseX, mouseY, mousePrevX;

	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;

	int length, value;

	SDL_Rect sliderLine, buttonBox;
};
#endif // !__GUISLIDER_H__
