#ifndef __GUICONTROLVALUEBOX_H__
#define __GUICONTROLVALUEBOX_H__

#include "GUIControl.h"
#include "SDL/include/SDL.h"
#include <string>

class GUIControlValueBox : public GUIControl
{
public:
	GUIControlValueBox(uint32 id, SDL_Rect bounds, const char* text);

	virtual ~GUIControlValueBox();

	bool Update(float dt);

	void SetValue(const std::string& value);
	const std::string& GetValue() const;

public:
	std::string value; // Current value displayed
};

#endif // __GUICONTROLVALUEBOX_H__