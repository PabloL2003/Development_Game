#ifndef __MODULE_H__
#define __MODULE_H__

#include "SString.h"

#include "PugiXml/src/pugixml.hpp"

class App;
class GUIControl;
class GUIControl;

class Module
{
public:

	Module() : active(false)
	{}

	void Init()
	{
		active = true;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool LoadState(pugi::xml_node node) 
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node node)
	{
		return true;
	}

	virtual bool OnGUIMouseClickEvent(GUIControl* control)
	{
		return true;
	}

	// Switches isEnabled and calls Start() method
	virtual void Enable() {
		if (!active) {
			active = true;
		}
	}

	// Switches isEnabled and calls CleanUp() method
	virtual void Disable() {
		if (active) {
			active = false;
		}
	}

	inline bool IsEnabled() const {
		return active;
	}

	virtual bool OnGuiMouseClickEvent(GUIControl* control)
	{
		return true;
	}

public:

	SString name;
	bool active = true;

};

#endif // __MODULE_H__