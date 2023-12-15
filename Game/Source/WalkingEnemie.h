#ifndef __WALKENEM_H__
#define __WALKENEM_H__

#include "Entity.h"
#include "SDL/include/SDL.h"
#include "Anim.h"
#include "DynArray.h"


struct SDL_Texture;

class Wenem : public Entity
{
public:
	Wenem();
	virtual ~Wenem();

	void InitAnims();

	bool Awake();

	bool Start();

	void TeleportTo(iPoint pos);

	void SetSpawnPoint(iPoint pos);

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void IsDead();

public:

	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;
	int width = 34, height = 16;

	iPoint spawn;
	bool isKilled = false;
	bool killedPlayer = false;

private:
	Animation* currentAnim = nullptr;

	Animation wr_idle, wrigthmov;

	Animation wl_idle, wleftmov;

	float speedCap = 3.0f;
	DynArray<iPoint> pathToPlayer;
};
#endif