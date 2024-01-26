#ifndef __SCENE_WIN_H
#define __SCENE_WIN_H

#include "Module.h"
#include "Anim.h"

#include <chrono>
using namespace std::chrono;

struct SDL_Texture;

class Scene_Win : public Module
{
public:

	Scene_Win();

	// Destructor
	virtual ~Scene_Win();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public:

private:

	SDL_Texture* background;

	high_resolution_clock::time_point start;
	high_resolution_clock::time_point currentTime;
	milliseconds elapsed;

	int winFX;

};
#endif // !__SCENE_WIN_H
