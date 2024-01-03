#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Module.h"
#include "GUIControl.h"

#include "List.h"

class GUIManager : public Module
{
public:

	//Constructor
	GUIManager();

	//Destructor
	virtual ~GUIManager();

	//Called before the first frame
	bool Start();

	//Called each loop iteration
	bool Update(float dt);

	//Called before quitting
	bool CleanUp();

	//Additional methods
	GUIControl* CreateGuiControl(GUIControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds = { 0,0,0,0 });

public:

	List<GUIControl*> guiControlsList;
	SDL_Texture* texture;

};

#endif // __GUIMANAGER_H__
