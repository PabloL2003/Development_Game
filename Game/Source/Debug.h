#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "Module.h"
#include "Point.h"
#include "DynArray.h"
#include "List.h"

#include <chrono>
using namespace std::chrono;

struct SDL_Texture;

class Debug : public Module {
public:

	Debug();

	~Debug();

	bool Start() override;

	bool Update(float dt);

	bool PostUpdate();

	void DebugDraw();

	void DrawHitboxes();

	bool debug;

	bool variables;
	bool hitboxes;
	bool debugCamera;
	bool godMode;
	bool limitFPS;

};

#endif // __DEBUG_H__





