#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"
#include "PerfTimer.h"
#include "Timer.h"
#include "EntityManager.h"


#include "PugiXml/src/pugixml.hpp"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class Scene;
class Scene_Menu;
class Scene_Intro;
class Scene_Win;
class EntityManager;
class Map;
class PathFinding;
class Physics;
class FadeToBlack;
class GUIManager;
class Debug;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	bool LoadRequest();

	bool SaveRequest();

private:

	// Load config file
	bool LoadConfig();

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	bool LoadFromFile();

	bool SaveFromFile();

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	Scene* scene;
	Scene_Menu* scene_menu;
	Scene_Intro* scene_intro;
	Scene_Win* scene_win;
	EntityManager* entityManager;
	Map* map;
	PathFinding* pathfinding;
	Physics* physics;
	FadeToBlack* ftb;
	GUIManager* guiManager;
	Debug* debug;

	uint32 maxFrameDuration = 0;
	int framecap;
	bool vsync;

private:

	int argc;
	char** args;
	SString gameTitle;
	SString organization;

	List<Module *> modules;

	// xml_document to store the config file and
	// xml_node(s) to read specific branches of the xml
	pugi::xml_document configFile;
	pugi::xml_node configNode;

	uint frames;
	float dt;

    // required variables are provided:
	Timer startupTime;
	PerfTimer frameTime;
	PerfTimer lastSecFrameTime;

	uint64 frameCount = 0;
	uint32 framesPerSecond = 0;
	uint32 lastSecFrameCount = 0;

	float averageFps = 0.0f;
	uint32 secondsSinceStartup = 0;

	bool loadRequest = false;
	bool saveRequest = false;

};

extern App* app;

#endif	// __APP_H__