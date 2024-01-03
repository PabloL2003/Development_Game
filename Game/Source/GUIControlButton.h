#ifndef __GUICONTROLBUTTON_H__
#define __GUICONTROLBUTTON_H__

#include "GUIControl.h"

#include "Point.h"
#include "SString.h"

class GUIControlButton : public GUIControl
{
public:

	GUIControlButton(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GUIControlButton();

	//Called each loop iteration
	bool Update(float dt);

private:

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;
};

#endif // __GUICONTROLBUTTON_H__
