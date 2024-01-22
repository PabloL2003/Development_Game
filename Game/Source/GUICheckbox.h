#ifndef __GUICHECKBOX_H__
#define __GUICHECKBOX_H__

#include "GUIControl.h"

#include "Point.h"
#include "SString.h"

class GUICheckbox : public GUIControl
{
public:

	GUICheckbox(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GUICheckbox();

	bool Update(float dt);

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;
};
#endif // !__GUICHECKBOX_H__
